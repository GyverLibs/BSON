#pragma once
#include <GTL.h>
#include <limits.h>

#if defined(ARDUINO) && !defined(BSON_NO_TEXT)
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
#define BS_CONT_OPEN (1 << 3)
#define BS_CONT_ARR (1 << 2)
#define BS_CONT_CLOSE (1 << 1)

#define BS_NEGATIVE (1 << 4)

// ============== helper ==============
#define BS_LSB5(x) ((x) & 0b00011111)
#define BS_MSB5(x) BS_LSB5((x) >> 8)
#define BS_LSB(x) ((x) & 0xff)
#define BS_UNPACK5(msb5, lsb) ((msb5 << 8) | lsb)

#define _BSON_INTx(val, len) (BS_INTEGER | (val < 0 ? BS_NEGATIVE : 0) | len)
#define _BSON_BYTEx(val, n) ((val < 0 ? -val : val) >> (n * 8)) & 0xff
inline uint8_t _BSON_FLOATx(float v, uint8_t n) { return ((uint8_t*)&v)[n]; }

#define _BS_STR_N1(str) str[0]
#define _BS_STR_N2(str) _BS_STR_N1(str), str[1]
#define _BS_STR_N3(str) _BS_STR_N2(str), str[2]
#define _BS_STR_N4(str) _BS_STR_N3(str), str[3]
#define _BS_STR_N5(str) _BS_STR_N4(str), str[4]
#define _BS_STR_N6(str) _BS_STR_N5(str), str[5]
#define _BS_STR_N7(str) _BS_STR_N6(str), str[6]
#define _BS_STR_N8(str) _BS_STR_N7(str), str[7]
#define _BS_STR_N9(str) _BS_STR_N8(str), str[8]
#define _BS_STR_N10(str) _BS_STR_N9(str), str[9]
#define _BS_STR_N11(str) _BS_STR_N10(str), str[10]
#define _BS_STR_N12(str) _BS_STR_N11(str), str[11]
#define _BS_STR_N13(str) _BS_STR_N12(str), str[12]
#define _BS_STR_N14(str) _BS_STR_N13(str), str[13]
#define _BS_STR_N15(str) _BS_STR_N14(str), str[14]
#define _BS_STR_N16(str) _BS_STR_N15(str), str[15]
#define _BS_STR_N17(str) _BS_STR_N16(str), str[16]
#define _BS_STR_N18(str) _BS_STR_N17(str), str[17]
#define _BS_STR_N19(str) _BS_STR_N18(str), str[18]
#define _BS_STR_N20(str) _BS_STR_N19(str), str[19]
#define _BS_STR_N21(str) _BS_STR_N20(str), str[20]
#define _BS_STR_N22(str) _BS_STR_N21(str), str[21]
#define _BS_STR_N23(str) _BS_STR_N22(str), str[22]
#define _BS_STR_N24(str) _BS_STR_N23(str), str[23]
#define _BS_STR_N25(str) _BS_STR_N24(str), str[24]
#define _BS_STR_N26(str) _BS_STR_N25(str), str[25]
#define _BS_STR_N27(str) _BS_STR_N26(str), str[26]
#define _BS_STR_N28(str) _BS_STR_N27(str), str[27]
#define _BS_STR_N29(str) _BS_STR_N28(str), str[28]
#define _BS_STR_N30(str) _BS_STR_N29(str), str[29]
#define _BS_STR_N31(str) _BS_STR_N30(str), str[30]
#define _BS_STR_N32(str) _BS_STR_N31(str), str[31]
#define _BS_STR_N33(str) _BS_STR_N32(str), str[32]

// ============== STATIC ==============
inline constexpr uint8_t BSON_CONT(char t) { return t == '{' ? (BS_CONTAINER | BS_CONT_OBJ | BS_CONT_OPEN) : (t == '}' ? (BS_CONTAINER | BS_CONT_OBJ | BS_CONT_CLOSE) : (t == '[' ? (BS_CONTAINER | BS_CONT_ARR | BS_CONT_OPEN) : (BS_CONTAINER | BS_CONT_ARR | BS_CONT_CLOSE))); }
#define BSON_CODE(code) (BS_CODE | BS_MSB5((uint16_t)code)), BS_LSB((uint16_t)code)
#define BSON_FLOAT(val) (BS_FLOAT | 4), _BSON_FLOATx(val, 0), _BSON_FLOATx(val, 1), _BSON_FLOATx(val, 2), _BSON_FLOATx(val, 3)
#define BSON_INT8(val) _BSON_INTx(val, 1), _BSON_BYTEx(val, 0)
#define BSON_INT16(val) _BSON_INTx(val, 2), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1)
#define BSON_INT24(val) _BSON_INTx(val, 3), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2)
#define BSON_INT32(val) _BSON_INTx(val, 4), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2), _BSON_BYTEx(val, 3)
#define BSON_INT64(val) _BSON_INTx(val, 8), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2), _BSON_BYTEx(val, 3), _BSON_BYTEx(val, 4), _BSON_BYTEx(val, 5), _BSON_BYTEx(val, 6), _BSON_BYTEx(val, 7)
#define BSON_BOOL(val) (BS_BOOLEAN | val)
#define BSON_STR(str, len) BS_STRING | BS_MSB5(len), BS_LSB(len), _BS_STR_N##len(str)
#define BSON_KEY(str, len) BSON_STR(str, len)

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
    using ST::operator uint8_t*;

    // максимальная длина строк и бинарных данных
    static size_t maxDataLength() {
        return BS_MAX_LEN;
    }

#ifdef ARDUINO
    // вывести в Print как JSON
    static void stringify(BSON& bson, Print& p, bool pretty = false) {
        stringify(bson, bson.length(), p, pretty);
    }

    // вывести в Print как JSON
    static void stringify(const uint8_t* bson, size_t len, Print& p, bool pretty = false) {
        const uint8_t* end = bson + len;
        gtl::stack<uint8_t> stack;
        bool keyf = true;

        auto isClose = [](const uint8_t* bson, const uint8_t* end) -> bool {
            return bson != end && (*bson & (BS_CONTAINER | BS_CONT_CLOSE)) == (BS_CONTAINER | BS_CONT_CLOSE);
        };

        while (bson != end) {
            uint8_t type = *bson & 0b11100000;
            uint8_t data = *bson & 0b00011111;

            if (pretty && keyf) {
                bool close = isClose(bson, end);
                if (close) p.println();
                for (int i = 0; i < (int)stack.length() - close; i++) p.print("   ");
            }

            ++bson;

            switch (type) {
                case BS_CONTAINER:
                    if (data & BS_CONT_OPEN) {
                        char t = (data & BS_CONT_OBJ) ? '{' : '[';
                        stack.push(t);
                        p.print(t);
                        if (pretty) p.println();
                    } else {
                        char t = (data & BS_CONT_OBJ) ? '}' : ']';
                        p.print(t);
                        if (bson != end && !(*bson & (BS_CONTAINER | BS_CONT_CLOSE))) pretty ? p.print(",\r\n") : p.print(',');
                        stack.pop();
                    }
                    keyf = true;
                    continue;

                case BS_STRING: {
                    uint16_t len = BS_UNPACK5(data, *bson++);
                    p.print('"');
                    while (len--) p.write(*bson++);
                    p.print('"');
                } break;

                case BS_BOOLEAN:
                    p.print((data & 0b1) ? "true" : "false");
                    break;

                case BS_INTEGER: {
                    if (data & BS_NEGATIVE) p.print('-');
                    uint16_t len = data & 0b1111;
                    uint32_t v = 0;
                    for (uint8_t i = 0; i < len; i++) {
                        ((uint8_t*)&v)[i] = *bson++;
                    }
                    p.print(v);
                } break;

                case BS_FLOAT: {
                    float v;
                    memcpy(&v, bson, 4);
                    bson += 4;
                    p.print(v, data);
                } break;

                case BS_CODE:
                    p.print("\"#");
                    p.print(BS_UNPACK5(data, *bson++));
                    p.print('"');
                    break;

                case BS_BINARY: {
                    uint16_t len = BS_UNPACK5(data, *bson++);
                    bson += len;
                    p.print("\"<bin:");
                    p.print(len);
                    p.print(">\"");
                } break;
            }

            if (stack.last() == '{') {
                if (keyf) p.print(':');
                else if (!isClose(bson, end)) pretty ? p.print(",\r\n") : p.print(',');
                keyf ^= 1;
            } else {
                if (!isClose(bson, end)) pretty ? p.print(",\r\n") : p.print(',');
            }
        }
        p.println();
    }
#endif

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
    BSON& add(const char* str) {
        return add(str, strlen(str), false);
    }

#ifdef ARDUINO
    BSON& add(const String& str) {
        return add(str.c_str(), str.length(), false);
    }
    BSON& add(const __FlashStringHelper* str) {
        return add((const char*)str, strlen_P((PGM_P)str), true);
    }
#endif

#define BSON_MAKE_ADD_STR(T)                   \
    inline void operator=(T val) { add(val); } \
    inline void operator+=(T val) { add(val); }

    BSON_MAKE_ADD_STR(char*)
    BSON_MAKE_ADD_STR(const char*)
#ifdef ARDUINO
    BSON_MAKE_ADD_STR(const String&)
    BSON_MAKE_ADD_STR(const __FlashStringHelper*)
#endif

#if defined(ARDUINO) && !defined(BSON_NO_TEXT)
    BSON& add(const Text& str) {
        return add(str.str(), str.length(), str.pgm());
    }
    inline void operator=(const Text& str) { add(str); }
    inline void operator+=(const Text& str) { add(str); }
#endif

#ifdef ARDUINO
    BSON& add(const StringSumHelper&) = delete;
    inline void operator=(const StringSumHelper&) = delete;
    inline void operator+=(const StringSumHelper&) = delete;
#endif
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