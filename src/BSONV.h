#pragma once
#include <stddef.h>

#include "./core/macro.h"

#define BSON_MAKE_ADD(T, func)            \
    void operator=(T val) { func(val); }  \
    void operator+=(T val) { func(val); } \
    void add(T val) { func(val); }

class BSONV {
   public:
    // ================ static ================
    // максимальная длина строк и величина code
    static size_t maxDataLength() {
        return _BS_MAX_EXT_LEN;
    }

    // ============== container ==============
    // открыть/закрыть контейнер [ ] { }, всегда вернёт true
    bool cont(const char type) {
        switch (type) {
            case '[': _push(_BS_SUBT | _BS_CONT | _BS_ARR_OPEN); break;
            case ']': _push(_BS_SUBT | _BS_CONT | _BS_ARR_CLOSE); break;
            case '{': _push(_BS_SUBT | _BS_CONT | _BS_OBJ_OPEN); break;
            case '}': _push(_BS_SUBT | _BS_CONT | _BS_OBJ_CLOSE); break;
            default: return false;
        }
        return true;
    }

    bool operator()(char type) { return cont(type); }

    // ================ key =================
    // ключ для объектов
    template <typename T>
    BSONV& key(T k) {
        add(k);
        return *this;
    }

    template <typename T>
    BSONV& operator[](T k) { return key(k); }

    template <typename T>
    BSONV& keyCode(T k) {
        addCode(k);
        return *this;
    }

    // ============== val code ==============
    // добавить код
    void addCode(uint16_t code) {
        _addExt(_BS_CODE, code);
    }
    template <typename T>
    void addCode(T code) { addCode(uint16_t(code)); }

    void addCode(BSCode_t code) { addCode(code.value); }

    BSON_MAKE_ADD(BSCode_t, addCode)

    // ============== val bool ==============
    // добавить bool
    void addBool(bool b) {
        _push(_BS_SUBT | _BS_BOOL | b);
    }
    void addBool(const void* b) {
        addBool(*(const bool*)b);
    }

    BSON_MAKE_ADD(bool, addBool)

    // ============== val int ==============
    // добавить int=0
    void addZero() {
        _push(_BS_INT | _BS_INT_SMALL);
    }

    // добавить int
    void addInt(const void* p, uint8_t size) {
        if (!size || size > 8) {
            addZero();
            return;
        }

        const uint8_t* b = (const uint8_t*)p;

        if (b[size - 1] & 0x80) {
            uint8_t v[8];
            for (uint8_t i = 0, cr = 1; i < size; i++) {
                v[i] = ~b[i] + cr;
                cr = cr && !v[i];
            }
            addUint(v, size, true);
        } else {
            addUint(p, size);
        }
    }

    // добавить uint
    void addUint(const void* p, uint8_t size, bool negative = false) {
        const uint8_t* b = (const uint8_t*)p;

        while (size && !b[size - 1]) --size;

        if (!size) {
            addZero();
        } else if (!negative && size == 1 && *b < _BS_INT_SMALL) {
            _push(_BS_INT | _BS_INT_SMALL | *b);
        } else {
            _push(_BS_INT | (negative ? _BS_INT_NEG : 0) | size);
            write(p, size);
        }
    }

    // добавить int
    template <typename T>
    void addInt(T val) {
        if (T(-1) < T(0) && val < 0) {
            addInt(&val, sizeof(T));
            // todo why not val = -val, addUint(val, sizeof(T), true) ?
        } else {
            addUint(&val, sizeof(T));
        }
    }

    BSON_MAKE_ADD(signed char, addInt)
    BSON_MAKE_ADD(unsigned char, addInt)
    BSON_MAKE_ADD(short, addInt)
    BSON_MAKE_ADD(unsigned short, addInt)
    BSON_MAKE_ADD(int, addInt)
    BSON_MAKE_ADD(unsigned int, addInt)
    BSON_MAKE_ADD(long, addInt)
    BSON_MAKE_ADD(unsigned long, addInt)
    BSON_MAKE_ADD(long long, addInt)
    BSON_MAKE_ADD(unsigned long long, addInt)

    // ============== val float ==============
    // добавить float
    void addFloat(const void* p) {
        _push(_BS_SUBT | _BS_FLOAT);
        write(p, _BS_FLOAT_SIZE);

        // todo
        // float 0 как int 0
        // uint32_t rawf;
        // memcpy(&rawf, p, _BS_FLOAT_SIZE);

        // if (rawf & 0x7fffffff) {
        //     _push(_BS_SUBT | _BS_FLOAT);
        //     write(p, _BS_FLOAT_SIZE);
        // } else {
        //     addZero();
        // }
    }
    void addFloat(float value) {
        addFloat(&value);
    }
    void addFloat(double value) {
        float f = value;
        addFloat(&f);
    }

    BSON_MAKE_ADD(float, addFloat)
    BSON_MAKE_ADD(double, addFloat)

    // ============== null ==============
    // добавить null
    void addNull() {
        _push(_BS_SUBT | _BS_NULL);
    }

    void operator=(nullptr_t) { addNull(); }
    void operator+=(nullptr_t) { addNull(); }
    void add(nullptr_t) { return addNull(); }

    // ============== val bin ==============
    // начать бинарные данные, затем вручную write(data, size, pgm)
    void beginBin(uint32_t size) {
        uint8_t len = size ? (size > 0xff ? (size > 0xffff ? 4 : 2) : 1) : 0;
        _push(_BS_SUBT | _BS_BIN | len);
        write(&size, len);
    }

    // добавить бинарные данные
    void addBin(const void* data, uint32_t size, bool pgm = false) {
        beginBin(size);
        write(data, size, pgm);
    }
    template <typename T>
    void addBin(const T& data) {
        addBin(&data, sizeof(T));
    }

    // ============== val string ==============
    // добавить символ как строку
    void addStr(char sym) {
        _push(_BS_STR | 1);
        _push(sym);
    }

    BSON_MAKE_ADD(char, addStr)

    // начать строку, затем вручную write(str, len, pgm). Если размер превышает - добавится пустая строка
    bool beginStr(size_t len) {
        if (len > _BS_MAX_EXT_LEN) len = 0;
        _addExt(_BS_STR, len);
        return len != 0;
    }

    // добавить строку. Обрежется по макс. размеру
    void addStr(const void* str, size_t len, bool pgm = false) {
        if (len > _BS_MAX_EXT_LEN) len = _BS_MAX_EXT_LEN;
        _addExt(_BS_STR, len);
        write(str, len, pgm);
    }
    void addStr(char* str) { addStr((const char*)str); }
    void addStr(const char* str) { addStr(str, strlen(str)); }

    BSON_MAKE_ADD(char*, addStr)
    BSON_MAKE_ADD(const char*, addStr)

#ifdef ARDUINO
    void addStr(const String& str) { addStr(str.c_str(), str.length()); }
    void addStr(const __FlashStringHelper* str) { addStr((const char*)str, strlen_P((PGM_P)str), true); }

    BSON_MAKE_ADD(const String&, addStr)
    BSON_MAKE_ADD(const __FlashStringHelper*, addStr)
#endif

    // добавить по типу
    void add(BSType type, const void* p, uint16_t size) {
        switch (type) {
            case BSType::Null: addNull(); break;
            case BSType::Bool: addBool(p); break;
            case BSType::Int: addInt(p, size); break;
            case BSType::Uint: addUint(p, size); break;
            case BSType::Float: addFloat(p); break;
            case BSType::String: addStr(p, size, false); break;
            case BSType::FString: addStr(p, size, true); break;
            case BSType::Bin: addBin(p, size, false); break;
            default: break;
        }
    }

    // ============== private ==============
   protected:
    // void write(const void* data, size_t len, bool pgm = false) {
    //     static_cast<Stack*>(this)->write((const uint8_t*)data, len, pgm);
    // }
    virtual size_t write(const void* data, size_t len, bool pgm = false) = 0;

   private:
    void _push(uint8_t b) {
        write(&b, 1);
    }
    void _addExt(uint8_t t, uint16_t v) {
        if (v < _BS_EXT_FLAG) {
            _push(t | v);
        } else {
            _push(t | _BS_EXT_FLAG | _BS_GET_LSB(v));
            _push(_BS_GET_MSB(v));
        }
    }
};