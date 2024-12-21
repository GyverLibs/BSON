#pragma once
#include <Arduino.h>
#include <StringUtils.h>
#include <limits.h>
#include <stack/stack.h>

// ============== const ==============
#define BS_MAX_LEN ((size_t)0b0001111111111111)

#define BS_KEY_CODE (0 << 5)
#define BS_KEY_STR (1 << 5)
#define BS_VAL_CODE (2 << 5)
#define BS_VAL_STR (3 << 5)
#define BS_VAL_INT (4 << 5)
#define BS_VAL_FLOAT (5 << 5)
#define BS_CONTAINER (6 << 5)
#define BS_BINARY (7 << 5)

#define BS_CONT_OBJ (1 << 4)
#define BS_CONT_ARR (0 << 4)
#define BS_CONT_OPEN (1 << 3)
#define BS_CONT_CLOSE (0 << 3)

#define BS_NEGATIVE (1 << 4)
#define BS_BOOLEAN (1 << 3)

#define BS_LSB5(x) ((x) & 0b00011111)
#define BS_MSB5(x) BS_LSB5((x) >> 8)
#define BS_LSB(x) ((x) & 0xff)

// ============== class ==============
class BSON : private gtl::stack_uniq<uint8_t> {
   public:
    using gtl::stack_uniq<uint8_t>::write;
    using gtl::stack_uniq<uint8_t>::concat;
    using gtl::stack_uniq<uint8_t>::reserve;
    using gtl::stack_uniq<uint8_t>::length;
    using gtl::stack_uniq<uint8_t>::buf;
    using gtl::stack_uniq<uint8_t>::clear;
    using gtl::stack_uniq<uint8_t>::move;

    BSON() {}
    BSON(BSON& b) {
        move(b);
    }
    BSON(BSON&& b) {
        move(b);
    }
    BSON& operator=(BSON& b) {
        move(b);
        return *this;
    }
    BSON& operator=(BSON&& b) {
        move(b);
        return *this;
    }

    operator Text() {
        return toText();
    }

    Text toText() {
        return Text(buf(), length());
    }

    // ============== concat bson ==============
    BSON& add(const BSON& bson) {
        concat(bson);
        return *this;
    }
    BSON& operator+=(const BSON& bson) {
        concat(bson);
        return *this;
    }

    // ============ key str ==============
    BSON& addKey(const Text& key) {
        return _str(key, BS_KEY_STR);
    }

    BSON& operator[](const Text& key) { return addKey(key); }
    BSON& operator[](const String& key) { return addKey(key); }
    BSON& operator[](const char* key) { return addKey(key); }
    BSON& operator[](const __FlashStringHelper* key) { return addKey(key); }

    // ============== key code ==============
    BSON& addKey(uint16_t key) {
        push(BS_KEY_CODE | BS_MSB5(key));
        push(BS_LSB(key));
        return *this;
    }

    template <typename T>
    BSON& operator[](T key) { return addKey((uint16_t)key); }

    // ============== val code ==============
    BSON& addCode(uint16_t key, uint16_t val) {
        reserve(length() + 5);
        addKey(key);
        return addCode(val);
    }
    BSON& addCode(const Text& key, uint16_t val) {
        addKey(key);
        return addCode(val);
    }
    BSON& addCode(uint16_t val) {
        push(BS_VAL_CODE | BS_MSB5(val));
        push(BS_LSB(val));
        return *this;
    }

    template <typename T>
    void operator=(T val) { addCode((uint16_t)val); }
    template <typename T>
    void operator+=(T val) { addCode((uint16_t)val); }

    // ============== val bool ==============
    BSON& addBool(bool b) {
        push(BS_VAL_INT | BS_BOOLEAN | b);
        return *this;
    }
    BSON& addBool(uint16_t key, bool b) {
        addKey(key);
        return addBool(b);
    }
    BSON& addBool(const Text& key, bool b) {
        addKey(key);
        return addBool(b);
    }
    void operator=(bool val) { addBool(val); }
    void operator+=(bool val) { addBool(val); }

    // ============== val int ==============
    template <typename T>
    BSON& addInt(T val) {
        return _int32(val, false);
    }
    BSON& addInt(unsigned long long val) {
        return _int64(val, false);
    }
    BSON& addInt(char val) {
        return addInt((long)val);
    }
    BSON& addInt(signed char val) {
        return addInt((long)val);
    }
    BSON& addInt(short val) {
        return addInt((long)val);
    }
    BSON& addInt(int val) {
        return addInt((long)val);
    }
    BSON& addInt(long val) {
        return _int32(val < 0 ? -val : val, val < 0);
    }
    BSON& addInt(long long val) {
        return _int64(val < 0 ? -val : val, val < 0);
    }

    template <typename T>
    BSON& addInt(uint16_t key, T val) {
        addKey(key);
        return addInt(val);
    }
    template <typename T>
    BSON& addInt(const Text& key, T val) {
        addKey(key);
        return addInt(val);
    }

#define BSON_MAKE_ADD_INT(T)               \
    void operator=(T val) { addInt(val); } \
    void operator+=(T val) { addInt(val); }

    BSON_MAKE_ADD_INT(char)
    BSON_MAKE_ADD_INT(unsigned char)
    BSON_MAKE_ADD_INT(unsigned short)
    BSON_MAKE_ADD_INT(unsigned int)
    BSON_MAKE_ADD_INT(unsigned long)
    BSON_MAKE_ADD_INT(unsigned long long)

    BSON_MAKE_ADD_INT(signed char)
    BSON_MAKE_ADD_INT(short)
    BSON_MAKE_ADD_INT(int)
    BSON_MAKE_ADD_INT(long)
    BSON_MAKE_ADD_INT(long long)

    // ============== val float ==============
    template <typename T>
    BSON& addFloat(T value, uint8_t dec) {
        push(BS_VAL_FLOAT | BS_LSB5(dec));
        float f = value;
        concat((uint8_t*)&f, 4);
        return *this;
    }
    template <typename T>
    BSON& addFloat(uint16_t key, T value, uint8_t dec) {
        addKey(key);
        addFloat(value, dec);
        return *this;
    }
    template <typename T>
    BSON& addFloat(const Text& key, T value, uint8_t dec) {
        addKey(key);
        addFloat(value, dec);
        return *this;
    }

#define BSON_MAKE_ADD_FLOAT(T)                  \
    void operator=(T val) { addFloat(val, 4); } \
    void operator+=(T val) { addFloat(val, 4); }

    BSON_MAKE_ADD_FLOAT(float)
    BSON_MAKE_ADD_FLOAT(double)

    // ============== val string ==============
    BSON& addStr(const Text& text) {
        return _str(text, BS_VAL_STR);
    }
    BSON& addStr(uint16_t key, const Text& text) {
        reserve(length() + text.length() + 5);
        addKey(key);
        return _str(text, BS_VAL_STR);
    }
    BSON& addStr(const Text& key, const Text& text) {
        addKey(key);
        return _str(text, BS_VAL_STR);
    }
    BSON& beginStr(size_t len) {
        push(BS_VAL_STR | BS_MSB5(len));
        push(BS_LSB(len));
        return *this;
    }
#define BSON_MAKE_ADD_STR(T)               \
    void operator=(T val) { addStr(val); } \
    void operator+=(T val) { addStr(val); }

    BSON_MAKE_ADD_STR(const char*)
    BSON_MAKE_ADD_STR(const __FlashStringHelper*)
    BSON_MAKE_ADD_STR(const String&)
    BSON_MAKE_ADD_STR(const Text&)

    // ============== val bin ==============
    BSON& addBin(const void* data, size_t size, bool pgm = false) {
        if (beginBin(size)) concat((const uint8_t*)data, size, pgm);
        return *this;
    }
    BSON& addBin(const Text& key, const void* data, size_t size, bool pgm = false) {
        if (size > BS_MAX_LEN) return *this;
        addKey(key);
        return addBin(data, size, pgm);
    }
    BSON& addBin(uint16_t key, const void* data, size_t size, bool pgm = false) {
        if (size > BS_MAX_LEN) return *this;
        addKey(key);
        return addBin(data, size, pgm);
    }
    bool beginBin(uint16_t size) {
        if (size > BS_MAX_LEN) return false;
        push(BS_BINARY | BS_MSB5(size));
        push(BS_LSB(size));
        return true;
    }

    // ============== container ==============
    BSON& operator()(char cont) {
        switch (cont) {
            case '[': return beginArr();
            case ']': return endArr();
            case '{': return beginObj();
            case '}': return endObj();
        }
        return *this;
    }
    BSON& operator()(uint16_t key, char cont) {
        addKey(key);
        return (*this)(cont);
    }
    BSON& operator()(const Text& key, char cont) {
        addKey(key);
        return (*this)(cont);
    }

    // ============== object ==============
    BSON& beginObj() {
        push(BS_CONTAINER | BS_CONT_OBJ | BS_CONT_OPEN);
        return *this;
    }
    BSON& beginObj(uint16_t key) {
        reserve(length() + 4);
        addKey(key);
        return beginObj();
    }
    BSON& beginObj(const Text& key) {
        addKey(key);
        return beginObj();
    }
    BSON& endObj() {
        push(BS_CONTAINER | BS_CONT_OBJ | BS_CONT_CLOSE);
        return *this;
    }

    // ============== array ==============
    BSON& beginArr() {
        push(BS_CONTAINER | BS_CONT_ARR | BS_CONT_OPEN);
        return *this;
    }
    BSON& beginArr(uint16_t key) {
        reserve(length() + 4);
        addKey(key);
        return beginArr();
    }
    BSON& beginArr(const Text& key) {
        addKey(key);
        return beginArr();
    }
    BSON& endArr() {
        push(BS_CONTAINER | BS_CONT_ARR | BS_CONT_CLOSE);
        return *this;
    }

    // ============== private ==============
   private:
    BSON& _str(const Text& text, uint8_t type) {
        uint16_t len = min((size_t)text.length(), BS_MAX_LEN);
        reserve(length() + len + 3);
        push(type | BS_MSB5(len));
        push(BS_LSB(len));
        concat((const uint8_t*)text.str(), len, text.pgm());
        return *this;
    }
    BSON& _int32(uint32_t val, bool neg) {
        return _int(&val, neg, _uint32Size((uint8_t*)&val));
    }
    BSON& _int64(uint64_t val, bool neg) {
        return _int(&val, neg, _uint64Size((uint8_t*)&val));
    }
    BSON& _int(void* val, bool neg, uint8_t len) {
        push(BS_VAL_INT | (neg ? BS_NEGATIVE : 0) | len);
        concat((uint8_t*)val, len);
        return *this;
    }

    uint8_t _uint32Size(uint8_t* val) {
        if (val[3]) return 4;
        if (val[2]) return 3;
        if (val[1]) return 2;
        if (val[0]) return 1;
        return 0;
    }
    uint8_t _uint64Size(uint8_t* val) {
        if (val[7]) return 7;  // 7 byte max
        if (val[6]) return 7;
        if (val[5]) return 6;
        if (val[4]) return 5;
        return _uint32Size(val);
    }
};