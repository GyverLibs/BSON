#pragma once
#include <Arduino.h>
#include <GTL.h>
#include <limits.h>

#ifndef BSON_NO_TEXT
#include <StringUtils.h>
#endif

// ============== const ==============
#define BS_MAX_LEN ((size_t)0b0001111111111111)

#define BS_STRING (0 << 5)
#define BS_BOOLEAN (1 << 5)
#define BS_INTEGER (2 << 5)
#define BS_FLOAT (3 << 5)
#define BS_CODE (4 << 5)
#define BS_BINARY (5 << 5)
#define BS_CONTAINER (6 << 5)

#define BS_CONT_OBJ (1 << 4)
#define BS_CONT_ARR (0 << 4)
#define BS_CONT_OPEN (1 << 3)
#define BS_CONT_CLOSE (0 << 3)

#define BS_NEGATIVE (1 << 4)

#define BS_LSB5(x) ((x) & 0b00011111)
#define BS_MSB5(x) BS_LSB5((x) >> 8)
#define BS_LSB(x) ((x) & 0xff)

// ============== class ==============
class BSON : private gtl::stack<uint8_t> {
    typedef gtl::stack<uint8_t> ST;

   public:
    using ST::addCapacity;
    using ST::buf;
    using ST::clear;
    using ST::concat;
    using ST::length;
    using ST::move;
    using ST::reserve;
    using ST::setOversize;
    using ST::write;

    // максимальная длина строк и бинарных данных
    static size_t maxDataLength() {
        return BS_MAX_LEN;
    }

    // размер числа в байтах
    static uint8_t uint32Size(uint8_t* p) {
        if (p[3]) return 4;
        if (p[2]) return 3;
        if (p[1]) return 2;
        if (p[0]) return 1;
        return 0;
    }

    // размер числа в байтах
    static uint8_t uint64Size(uint8_t* p) {
        if (p[7]) return 8;
        if (p[6]) return 7;
        if (p[5]) return 6;
        if (p[4]) return 5;
        return uint32Size(p);
    }

    // ============== add bson ==============
    BSON& add(const BSON& bson) {
        concat(bson);
        return *this;
    }
    void operator+=(const BSON& bson) { add(bson); }

    // ============ key ==============
    template <typename T>
    inline BSON& operator[](T key) { return add(key); }

    // ============== val code ==============
    template <typename T>
    BSON& add(T code) {
        push(BS_CODE | BS_MSB5((uint16_t)code));
        push(BS_LSB((uint16_t)code));
        return *this;
    }

    template <typename T>
    inline void operator=(T val) { add(val); }
    template <typename T>
    inline void operator+=(T val) { add(val); }

    // ============== val bool ==============
    BSON& add(bool b) {
        push(BS_BOOLEAN | b);
        return *this;
    }
    inline void operator=(bool val) { add(val); }
    inline void operator+=(bool val) { add(val); }

    // ============== val int ==============
    BSON& add(unsigned long val) {
        return _int32(&val);
    }
    BSON& add(unsigned long long val) {
        return _int64(&val);
    }

    inline BSON& add(unsigned char val) { return add((unsigned long)val); }
    inline BSON& add(unsigned short val) { return add((unsigned long)val); }
    inline BSON& add(unsigned int val) { return add((unsigned long)val); }

    BSON& add(long val) {
        if (val < 0) {
            val = -val;
            return _int32(&val, true);
        } else {
            return _int32(&val);
        }
    }
    BSON& add(long long val) {
        if (val < 0) {
            val = -val;
            return _int64(&val, true);
        } else {
            return _int64(&val);
        }
    }
    inline BSON& add(char val) { return add((long)val); }
    inline BSON& add(signed char val) { return add((long)val); }
    inline BSON& add(short val) { return add((long)val); }
    inline BSON& add(int val) { return add((long)val); }

#define BSON_MAKE_ADD_INT(T)                   \
    inline void operator=(T val) { add(val); } \
    inline void operator+=(T val) { add(val); }

    BSON_MAKE_ADD_INT(char)
    BSON_MAKE_ADD_INT(signed char)
    BSON_MAKE_ADD_INT(short)
    BSON_MAKE_ADD_INT(int)
    BSON_MAKE_ADD_INT(long)
    BSON_MAKE_ADD_INT(long long)

    BSON_MAKE_ADD_INT(unsigned char)
    BSON_MAKE_ADD_INT(unsigned short)
    BSON_MAKE_ADD_INT(unsigned int)
    BSON_MAKE_ADD_INT(unsigned long)
    BSON_MAKE_ADD_INT(unsigned long long)

    // ============== val float ==============
    BSON& add(float value, int dec) {
        push(BS_FLOAT | BS_LSB5(dec));
        write(&value, 4);
        return *this;
    }
    BSON& add(double value, int dec) {
        return add((float)value, dec);
    }

    inline void operator+=(float val) { add(val, 4); }
    inline void operator=(float val) { add(val, 4); }
    inline void operator+=(double val) { add(val, 4); }
    inline void operator=(double val) { add(val, 4); }

    // ============== val string ==============
    BSON& beginStr(size_t len) {
        push(BS_STRING | BS_MSB5(len));
        push(BS_LSB(len));
        return *this;
    }
    BSON& add(const char* str, size_t len, bool pgm = false) {
        if (len > BS_MAX_LEN) len = BS_MAX_LEN;
        beginStr(len);
        write(str, len, pgm);
        return *this;
    }

    inline BSON& add(char* str) {
        return add((const char*)str);
    }
    inline void operator=(char* str) { add(str); }
    inline void operator+=(char* str) { add(str); }

    BSON& add(const char* str) {
        return add(str, strlen(str), false);
    }
    inline void operator=(const char* str) { add(str); }
    inline void operator+=(const char* str) { add(str); }

    BSON& add(const String& str) {
        return add(str.c_str(), str.length(), false);
    }
    inline void operator=(const String& str) { add(str); }
    inline void operator+=(const String& str) { add(str); }

    BSON& add(const __FlashStringHelper* str) {
        return add((const char*)str, strlen_P((PGM_P)str), true);
    }
    inline void operator=(const __FlashStringHelper* str) { add(str); }
    inline void operator+=(const __FlashStringHelper* str) { add(str); }

#ifndef BSON_NO_TEXT
    BSON& add(const Text& str) {
        return add(str.str(), str.length(), str.pgm());
    }
    inline void operator=(const Text& str) { add(str); }
    inline void operator+=(const Text& str) { add(str); }
#endif

    BSON& add(const StringSumHelper&) = delete;
    inline void operator=(const StringSumHelper&) = delete;
    inline void operator+=(const StringSumHelper&) = delete;

    // ============== val bin ==============
    bool beginBin(uint16_t size) {
        if (size > BS_MAX_LEN) return false;
        push(BS_BINARY | BS_MSB5(size));
        push(BS_LSB(size));
        return true;
    }
    BSON& add(const void* data, size_t size, bool pgm = false) {
        if (beginBin(size)) write(data, size, pgm);
        return *this;
    }

    // ============== container [ ] { } ==============
    bool operator()(char type) {
        switch (type) {
            case '[': push(BS_CONTAINER | BS_CONT_ARR | BS_CONT_OPEN); break;
            case ']': push(BS_CONTAINER | BS_CONT_ARR | BS_CONT_CLOSE); break;
            case '{': push(BS_CONTAINER | BS_CONT_OBJ | BS_CONT_OPEN); break;
            case '}': push(BS_CONTAINER | BS_CONT_OBJ | BS_CONT_CLOSE); break;
        }
        return true;
    }

    // ============== private ==============
   private:
    BSON& _int32(void* p, bool neg = false) {
        uint8_t len = uint32Size((uint8_t*)p);
        push(BS_INTEGER | (neg ? BS_NEGATIVE : 0) | len);
        write(p, len);
        return *this;
    }
    BSON& _int64(void* p, bool neg = false) {
        uint8_t len = uint64Size((uint8_t*)p);
        push(BS_INTEGER | (neg ? BS_NEGATIVE : 0) | len);
        write(p, len);
        return *this;
    }
};