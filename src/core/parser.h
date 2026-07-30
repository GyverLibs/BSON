#pragma once
#include <string.h>

#include "./macro.h"

// parseFloat читает из Float и Int (т.к. JS пишет круглые float как int)

class BSParser {
   public:
    BSParser(const uint8_t* bson, size_t len) : _bson(bson), _end(bson + len) {}

    // ============ CHECK ============

    // получить тип блока
    BSType getType() const {
        return _type;
    }

    // совпадает тип блока
    bool is(BSType type) const {
        return _type == type;
    }

    // получить контейнер
    char getCont() const {
        switch (_data & (_BS_CONT_OBJ | _BS_CONT_OPEN)) {
            case _BS_ARR_OPEN: return '[';
            case _BS_ARR_CLOSE: return ']';
            case _BS_OBJ_OPEN: return '{';
            case _BS_OBJ_CLOSE: return '}';
        }
        return 0;
    }

    // контейнер-объект
    bool isObj() const {
        return _data & _BS_CONT_OBJ;
    }

    // контейнер открыт
    bool isOpen() const {
        return _data & _BS_CONT_OPEN;
    }

    // длина в байтах [String, Bin]
    size_t length() const {
        return _data;
    }

    // число отрицательное [Int]
    bool isNegative() const {
        return !_BS_IS_SMALL(_data) && _BS_IS_NEG(_data);
    }

    // ============ EXPORT ============

    // указатель на строку [String], длина length()
    const char* getStr() const {
        return (const char*)_p;
    }

    // в свой тип [Bin]
    void readBin(void* p, size_t size) const {
        if (_data == size) memcpy(p, _p, size);
    }
    template <typename T>
    void readBin(T* p) const {
        readBin(p, sizeof(T));
    }
    template <typename T>
    T getBin() const {
        T t;
        readBin(&t);
        return t;
    }

    // указатель на [Bin], длина length()
    const uint8_t* getBin() const {
        return _p;
    }

    // в uint16_t код [Code]
    void readCode(void* p) const {
        memcpy(p, &_data, 2);
    }
    uint16_t getCode() const {
        return _data;
    }
    template <typename T>
    T getCode() const {
        return T(_data);
    }

    // в bool [Bool]
    void readBool(void* p) const {
        *(bool*)p = getBool();
    }
    bool getBool() const {
        return _BS_GET_BOOLV(_data);
    }

    // в int [Int]
    void readInt(void* p, uint8_t size) const {
        uint8_t* dst = (uint8_t*)p;

        if (_BS_IS_SMALL(_data)) {
            memset(p, 0, size);
            *dst = _BS_GET_SMALLV(_data);
            return;
        }

        bool neg = _BS_IS_NEG(_data);
        uint8_t len = _BS_GET_INT_SIZE(_data);

        for (uint8_t i = 0, cr = 1; i < size; i++) {
            dst[i] = (i < len) ? _p[i] : 0;
            if (neg) {
                dst[i] = ~dst[i] + cr;
                cr = cr && !dst[i];
            }
        }
    }

    // в int [Int]
    int32_t getInt() const {
        int32_t v;
        readInt(&v, sizeof(v));
        return v;
    }

    // в uint [Int]
    uint32_t getUint() const {
        return getInt();
    }

    // в int [Int]
    int64_t getInt64() const {
        int64_t v;
        readInt(&v, sizeof(v));
        return v;
    }

    // в uint [Int]
    uint64_t getUint64() const {
        return getInt64();
    }

    // в float [Float]
    void readFloat(void* p) const {
        memcpy(p, _p, _BS_FLOAT_SIZE);
    }
    float getFloat() const {
        float f;
        readFloat(&f);
        return f;
    }

    // ============ READ ============

    // парсить и прочитать строку
    bool parseStr(char* s, uint16_t size, bool terminate = true) {
        if (!parse(BSType::String)) return false;

        uint16_t wrote = (size < _data) ? size : _data;
        memcpy(s, _p, wrote);
        if (terminate && size) s[wrote == size ? size - 1 : wrote] = 0;
        return true;
    }

    // парсить и прочитать bool
    bool parseBool(void* b) {
        return parse(BSType::Bool) ? (readBool(b), true) : false;
    }

    // парсить и прочитать int
    template <typename T>
    bool parseInt(T* i) {
        return parseInt(i, sizeof(T));
    }
    bool parseInt(void* i, uint8_t size) {
        return parse(BSType::Int) ? (readInt(i, size), true) : false;
    }

    // парсить и прочитать float
    bool parseFloat(void* f) {
        return parse(BSType::Float) ? (readFloat(f), true) : false;

        // todo float -> int
        // if (parse()) {
        //     switch (_type) {
        //         case BSType::Float:
        //             readFloat(f);
        //             return true;

        //         case BSType::Int:
        //             *(float*)f = getInt();
        //             return true;

        //         default:
        //             break;
        //     }
        // }
        // return false;
    }

    // парсить и прочитать uint16_t code
    template <typename T>
    bool parseCode(T* p) {
        return parse(BSType::Code) ? (readCode(p), true) : false;
    }

    // парсить и прочитать bin
    template <typename T>
    bool parseBin(T* b) {
        return parseBin(b, sizeof(T));
    }
    bool parseBin(void* b, uint16_t size) {
        return parse(BSType::Bin) ? (readBin(b, size), true) : false;
    }

    // ============= PARSE =============

    // true - парсинг окончен
    bool isDone() const {
        return _bson == _end;
    }

    // true - ошибка парсинга
    bool isError() const {
        return _type == BSType::Error;
    }

    // парсить следующий блок и проверить тип. Вернёт true при успехе
    bool parse(BSType type) {
        return parse() && is(type);
    }

    // парсить следующий блок. Вернёт true при успехе
    bool parse() {
        if (!_has(1)) return false;

        size_t len = 0;
        _data = *_bson++;
        uint8_t t = _BS_GET_TYPE(_data);
        _type = BSType(t == _BS_SUBT ? _BS_GET_SUBT(_data) : t);

        switch (_type) {
            case BSType::Float:
                len = _BS_FLOAT_SIZE;
                if (!_has(len)) return false;
                break;

            case BSType::Bin: {
                uint8_t lenb = _BS_GET_BIN_SIZE(_data);
                if (!_has(lenb)) return false;

                memcpy(&len, _bson, sizeof(len) < lenb ? sizeof(len) : lenb);
                _bson += lenb;
                if (!_has(len)) return false;

                _data = len;
            } break;

            case BSType::Int:
                if (!_BS_IS_SMALL(_data)) {
                    len = _BS_GET_INT_SIZE(_data);
                    if (!_has(len)) return false;
                }
                break;

            case BSType::String:
                if (!_readExt(&len, _data) || !_has(len)) return false;
                _data = len;
                break;

            case BSType::Code:
                if (!_readExt(&_data, _data)) return false;
                break;

            default:
                break;
        }

        _p = _bson;
        _bson += len;
        return true;
    }

   private:
    const uint8_t *_bson, *_end, *_p;
    size_t _data;
    BSType _type = BSType::Error;

    // Bool: _data = header
    // Cont: _data = header
    // Float: _data = header, _p
    // Bin: _data = len, _p
    // Int: _data = header, _p
    // String: _data = len, _p
    // Code: _data = val

    bool _has(size_t n) {
        if (size_t(_end - _bson) >= n) return true;

        _type = BSType::Error;
        _bson = _end;
        return false;
    }

    bool _readExt(size_t* v, uint8_t hdr) {
        *v = _BS_GET_LSB(hdr);
        if (_BS_IS_EXT(hdr)) {
            if (!_has(1)) return false;
            *v |= uint16_t(*_bson++) << _BS_EXT_BITS;
        }
        return true;
    }
};