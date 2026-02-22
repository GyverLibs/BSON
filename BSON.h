#pragma once
#include <inttypes.h>
#include <limits.h>
#include <string.h>

#ifdef BSON_USE_VECTOR
#include <vector>
#define BS_STACK std::vector<uint8_t>
#else
#include <GTL.h>
#define BS_STACK gtl::stack<uint8_t>
#endif

#if defined(ARDUINO) && !defined(BSON_NO_TEXT)
#include <StringUtils.h>
#endif

// ============== TYPES ==============
#define BS_STRING (0 << 5)
#define BS_BOOLEAN (1 << 5)
#define BS_INTEGER (2 << 5)
#define BS_FLOAT (3 << 5)
#define BS_CODE (4 << 5)
#define BS_BINARY (5 << 5)
#define BS_CONTAINER (6 << 5)
#define BS_NULL (7 << 5)

#define BS_CONT_OBJ (1 << 4)
#define BS_CONT_OPEN (1 << 3)
#define BS_CONT_ARR (1 << 2)
#define BS_CONT_CLOSE (1 << 1)

#define BS_OBJ_OPEN (BS_CONTAINER | BS_CONT_OBJ | BS_CONT_OPEN)
#define BS_OBJ_CLOSE (BS_CONTAINER | BS_CONT_OBJ | BS_CONT_CLOSE)
#define BS_ARR_OPEN (BS_CONTAINER | BS_CONT_ARR | BS_CONT_OPEN)
#define BS_ARR_CLOSE (BS_CONTAINER | BS_CONT_ARR | BS_CONT_CLOSE)

// ============== MACRO ==============
#define BS_MAX_LEN 0b0001111111111111u

#define BS_TYPE_MASK 0b11100000
#define BS_TYPE(x) ((x) & BS_TYPE_MASK)

#define BS_DATA_MASK 0b00011111
#define BS_DATA(x) ((x) & BS_DATA_MASK)

#define BS_BOOLV_MASK 0b1
#define BS_BOOLV(x) ((x) & BS_BOOLV_MASK)

#define BS_NEG_MASK 0b10000
#define BS_NEGATIVE(x) ((x) & BS_NEG_MASK)
#define BS_SIZE_MASK 0b01111
#define BS_SIZE(x) ((x) & BS_SIZE_MASK)

#define BS_FLOAT_SIZE 4
#define BS_DEC_MASK 0b1111
#define BS_DECIMAL(x) ((x) & BS_DEC_MASK)

#define BS_D16_MSB(x) ((uint16_t(x) >> 8) & BS_DATA_MASK)
#define BS_D16_LSB(x) (uint16_t(x) & 0xff)
#define BS_D16_MERGE(msb5, lsb) ((msb5 << 8) | lsb)

// ============== static macro ==============
#define _BSON_INTx(val, len) (BS_INTEGER | (val < 0 ? BS_NEG_MASK : 0) | len)
#define _BSON_BYTEx(val, n) (((val < 0 ? -val : val) >> (n * 8)) & 0xff)
inline uint8_t _BSON_FLOATx(float v, uint8_t n) { return ((uint8_t*)&v)[n]; }

// union _BS_FB {
//     float f;
//     uint8_t b[BS_FLOAT_SIZE];
// };
// inline constexpr uint8_t _BSON_FLOATx(float v, uint8_t n) {
//     return _BS_FB{v}.b[n];
// }

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

// =========== STATIC BUILD ==========
inline constexpr uint8_t BSON_CONT(char t) { return t == '{' ? BS_OBJ_OPEN : (t == '}' ? BS_OBJ_CLOSE : (t == '[' ? BS_ARR_OPEN : BS_ARR_CLOSE)); }
#define BSON_CODE(code) (BS_CODE | BS_D16_MSB(code)), BS_D16_LSB(code)
#define BSON_FLOAT(val) (BS_FLOAT | 4), _BSON_FLOATx(val, 0), _BSON_FLOATx(val, 1), _BSON_FLOATx(val, 2), _BSON_FLOATx(val, 3)
#define BSON_INT8(val) _BSON_INTx(val, 1), _BSON_BYTEx(val, 0)
#define BSON_INT16(val) _BSON_INTx(val, 2), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1)
#define BSON_INT24(val) _BSON_INTx(val, 3), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2)
#define BSON_INT32(val) _BSON_INTx(val, 4), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2), _BSON_BYTEx(val, 3)
#define BSON_INT64(val) _BSON_INTx(val, 8), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2), _BSON_BYTEx(val, 3), _BSON_BYTEx(val, 4), _BSON_BYTEx(val, 5), _BSON_BYTEx(val, 6), _BSON_BYTEx(val, 7)
#define BSON_BOOL(val) (BS_BOOLEAN | BS_BOOLV(val))
#define BSON_STR(str, len) BS_STRING | BS_D16_MSB(len), BS_D16_LSB(len), _BS_STR_N##len(str)
#define BSON_KEY(str, len) BSON_STR(str, len)
#define BSON_NULL() BS_NULL

// ============== BSON ==============
class BSON : private BS_STACK {
    typedef BS_STACK ST;

#define BSON_MAKE_ADD(T)                \
    void operator=(T val) { add(val); } \
    void operator+=(T val) { add(val); }

   public:
#ifdef BSON_USE_VECTOR
    using ST::clear;
    using ST::reserve;
    void push(uint8_t v) { ST::push_back(v); }
    uint8_t* buf() { return ST::data(); }
    uint16_t length() { return ST::size(); }
    bool concat(const ST& st) {
        ST::reserve(ST::size() + st.size());
        ST::insert(ST::end(), st.begin(), st.end());
        return true;
    }
    size_t write(const void* data, size_t len) {
        const uint8_t* p = (const uint8_t*)data;
        ST::reserve(ST::size() + len);
        ST::insert(ST::end(), p, p + len);
        return len;
    }
    size_t write(const void* data, size_t len, bool) {
        return write(data, len);
    }
    size_t write(const uint8_t* data, size_t len) {
        return write(data, len);
    }
    operator uint8_t*() {
        return ST::data();
    }
    void setOversize(uint8_t) {}
#else
    using ST::buf;
    using ST::clear;
    using ST::concat;
    using ST::length;
    using ST::move;
    using ST::reserve;
    using ST::setOversize;
    using ST::write;
    using ST::operator uint8_t*;
#endif

    class Parser;

    // ================ static ================
    // максимальная длина строк и бинарных данных
    static size_t maxDataLength() {
        return BS_MAX_LEN;
    }

    // размер числа в байтах
    static uint8_t uintSize(const uint8_t* p, uint8_t size) {
        switch (size) {
            case 8:
                if (p[7]) return 8;
                if (p[6]) return 7;
                if (p[5]) return 6;
                if (p[4]) return 5;

            case 4:
                if (p[3]) return 4;
                if (p[2]) return 3;

            case 2:
                if (p[1]) return 2;

            case 1:
                if (p[0]) return 1;
        }
        return 0;
    }

    // ============== add bson ==============
    BSON& add(const BSON& bson) {
        concat(bson);
        return *this;
    }
    void operator+=(const BSON& bson) { add(bson); }

    // ============== container ==============
    // [ ] { }, всегда вернёт true
    bool operator()(char type) {
        switch (type) {
            case '[': push(BS_ARR_OPEN); break;
            case ']': push(BS_ARR_CLOSE); break;
            case '{': push(BS_OBJ_OPEN); break;
            case '}': push(BS_OBJ_CLOSE); break;
        }
        return true;
    }

    // ================ key =================
    template <typename T>
    BSON& operator[](T key) { return add(key); }

    // ============== val code ==============
    template <typename T>
    BSON& add(T code) {
        push(BS_CODE | BS_D16_MSB(code));
        push(BS_D16_LSB(code));
        return *this;
    }

    template <typename T>
    void operator=(T val) { add(val); }
    template <typename T>
    void operator+=(T val) { add(val); }

    // ============== val bool ==============
    BSON& add(bool b) {
        push(BS_BOOLEAN | b);
        return *this;
    }

    BSON_MAKE_ADD(bool)

// ============== val int ==============
#define BSON_MAKE_UINT(T)                              \
    BSON& add(T val) { return _int(&val, sizeof(T)); } \
    BSON_MAKE_ADD(T)

#define BSON_MAKE_INT(T)                                                                                     \
    BSON& add(T val) { return val < 0 ? (val = -val, _int(&val, sizeof(T), true)) : _int(&val, sizeof(T)); } \
    BSON_MAKE_ADD(T)

    BSON_MAKE_UINT(unsigned char)
    BSON_MAKE_UINT(unsigned short)
    BSON_MAKE_UINT(unsigned int)
    BSON_MAKE_UINT(unsigned long)
    BSON_MAKE_UINT(unsigned long long)

#if (CHAR_MIN < 0)
    BSON_MAKE_INT(char)
#else
    BSON_MAKE_UINT(char)
#endif

    BSON_MAKE_INT(signed char)
    BSON_MAKE_INT(short)
    BSON_MAKE_INT(int)
    BSON_MAKE_INT(long)
    BSON_MAKE_INT(long long)

    // ============== val float ==============
    BSON& add(float value, int dec) {
        push(BS_FLOAT | BS_DECIMAL(dec));
        write(&value, BS_FLOAT_SIZE);
        return *this;
    }
    BSON& add(double value, int dec) { return add((float)value, dec); }

    void operator+=(float val) { add(val, 4); }
    void operator=(float val) { add(val, 4); }
    void operator+=(double val) { add(val, 4); }
    void operator=(double val) { add(val, 4); }

    // ============== null ==============
    BSON& addNull() {
        push(BS_NULL);
        return *this;
    }

    // ============== val bin ==============
    // затем вручную write(data, size, pgm)
    bool beginBin(uint16_t size) {
        if (size > BS_MAX_LEN) {
            addNull();
            return false;
        }
        push(BS_BINARY | BS_D16_MSB(size));
        push(BS_D16_LSB(size));
        return true;
    }
    template <typename T>
    BSON& addBin(const T& data) {
        return addBin(&data, sizeof(T));
    }
    BSON& addBin(const void* data, size_t size, bool pgm = false) {
        if (beginBin(size)) write(data, size, pgm);
        return *this;
    }
    BSON& add(const void* data, size_t size, bool pgm = false) {
        return addBin(data, size, pgm);
    }

    // ============== val string ==============
    // затем вручную write(str, len, pgm)
    BSON& beginStr(size_t len) {
        push(BS_STRING | BS_D16_MSB(len));
        push(BS_D16_LSB(len));
        return *this;
    }
    BSON& addStr(const char* str, size_t len, bool pgm = false) {
        if (len > BS_MAX_LEN) len = BS_MAX_LEN;
        beginStr(len);
        write(str, len, pgm);
        return *this;
    }
    BSON& add(const char* str, size_t len, bool pgm = false) {
        return addStr(str, len, pgm);
    }

    BSON& add(char* str) { return add((const char*)str); }
    BSON& add(const char* str) { return add(str, strlen(str), false); }

    BSON_MAKE_ADD(char*)
    BSON_MAKE_ADD(const char*)

#ifdef ARDUINO
    BSON& add(const String& str) { return add(str.c_str(), str.length(), false); }
    BSON& add(const __FlashStringHelper* str) { return add((const char*)str, strlen_P((PGM_P)str), true); }

    BSON_MAKE_ADD(const String&)
    BSON_MAKE_ADD(const __FlashStringHelper*)

    BSON& add(const StringSumHelper&) = delete;
    void operator=(const StringSumHelper&) = delete;
    void operator+=(const StringSumHelper&) = delete;

#ifndef BSON_NO_TEXT
    BSON& add(const Text& str) { return add(str.str(), str.length(), str.pgm()); }
    BSON& add(const Value& str) { return add((Text)str); }

    BSON_MAKE_ADD(const Text&)
    BSON_MAKE_ADD(const Value&)
#endif
#endif

// ============== stringify ==============
#if defined(ARDUINO) && !defined(BSON_USE_VECTOR)
    // вывести в Print как JSON
    void stringify(Print& p, bool pretty = false) {
        stringify(*this, p, pretty);
    }

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
            return (bson != end) && (*bson & (BS_CONTAINER | BS_CONT_CLOSE)) == (BS_CONTAINER | BS_CONT_CLOSE);
        };

        while (bson != end) {
            uint8_t type = BS_TYPE(*bson);
            uint8_t data = BS_DATA(*bson);

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
                        if ((bson != end) && !(*bson & (BS_CONTAINER | BS_CONT_CLOSE))) pretty ? p.print(",\r\n") : p.print(',');
                        stack.pop();
                    }
                    keyf = true;
                    continue;

                case BS_STRING: {
                    uint16_t len = BS_D16_MERGE(data, *bson++);
                    p.print('"');
                    while (len--) p.write(*bson++);
                    p.print('"');
                } break;

                case BS_BOOLEAN:
                    p.print(BS_BOOLV(data) ? "true" : "false");
                    break;

                case BS_NULL:
                    p.print("null");
                    break;

                case BS_INTEGER: {
                    if (BS_NEGATIVE(data)) p.print('-');
                    uint32_t v = 0;
                    uint8_t size = BS_SIZE(data);
                    memcpy(&v, bson, size > 4 ? 4 : size);  // todo 64
                    bson += size;
                    p.print(v);
                } break;

                case BS_FLOAT: {
                    float v;
                    memcpy(&v, bson, BS_FLOAT_SIZE);
                    bson += BS_FLOAT_SIZE;
                    p.print(v, BS_DECIMAL(data));
                } break;

                case BS_CODE:
                    p.print("\"#");
                    p.print(BS_D16_MERGE(data, *bson++));
                    p.print('"');
                    break;

                case BS_BINARY: {
                    uint16_t len = BS_D16_MERGE(data, *bson++);
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

    // ============== private ==============
   private:
    BSON& _int(const void* p, uint8_t size, bool neg = false) {
        uint8_t len = uintSize((uint8_t*)p, size);
        push(BS_INTEGER | (neg ? BS_NEG_MASK : 0) | len);
        write(p, len);
        return *this;
    }
};

// типы BSON для парсера
enum class BSType : uint8_t {
    String = BS_STRING,
    Boolean = BS_BOOLEAN,
    Integer = BS_INTEGER,
    Float = BS_FLOAT,
    Code = BS_CODE,
    Binary = BS_BINARY,
    Container = BS_CONTAINER,
    Null = BS_NULL,
    Error = 0xff,
};

// ============== PARSER ==============
// линейный парсер BSON
class BSON::Parser {
   public:
    Parser(uint8_t* bson, uint16_t len) : _bson(bson), _bend(bson + len) {}
    Parser(BSON* b) : Parser(b->buf(), b->length()) {}
    Parser(BSON& b) : Parser(&b) {}

    // ============ CHECK ============

    // получить тип блока
    BSType getType() const {
        return _type;
    }

    // контейнер - объект [Container]
    bool isObject() const {
        return (_type == BSType::Container) ? (_data & BS_CONT_OBJ) : false;
    }

    // контейнер - массив [Container]
    bool isArray() const {
        return (_type == BSType::Container) ? (_data & BS_CONT_ARR) : false;
    }

    // контейнер открыт [Container]
    bool isOpen() const {
        return (_type == BSType::Container) ? (_data & BS_CONT_OPEN) : false;
    }

    // контейнер закрыт [Container]
    bool isClose() const {
        return (_type == BSType::Container) ? (_data & BS_CONT_CLOSE) : false;
    }

    // длина в байтах [String, Binary, Integer]
    uint16_t length() const {
        switch (_type) {
            case BSType::String:
            case BSType::Binary:
                return _data;

            case BSType::Integer:
                return BS_SIZE(_data);

            default:
                return 0;
        }
    }

    // число отрицательное [Integer]
    bool isNegative() const {
        return (_type == BSType::Integer) ? BS_NEGATIVE(_data) : false;
    }

    // ============ EXPORT ============

    // указатель на строку [String], длина length()
    const char* toStr() const {
        return (_type == BSType::String) ? (const char*)_dataP() : "";
    }

#ifndef BSON_NO_TEXT
    // в текст [String]
    Text toText() const {
        return (_type == BSType::String) ? Text((const char*)_dataP(), _data) : Text();
    }
#endif

    // в свой тип [Binary]
    template <typename T>
    T toBin() const {
        T t{};
        if (_type == BSType::Binary && _data == sizeof(T)) memcpy(&t, _dataP(), _data);
        return t;
    }

    // в код [Code]
    template <typename T>
    T toCode() const {
        return (_type == BSType::Code) ? T(_data) : T(0);
    }

    // в bool [Boolean]
    bool toBool() const {
        return (_type == BSType::Boolean) ? _data : false;
    }

    // в int [Integer]
    int32_t toInt() const {
        return _toInt<int32_t>();
    }

    // в uint [Integer]
    uint32_t toUint() const {
        return toInt();
    }

    // в int [Integer]
    int64_t toInt64() const {
        return _toInt<int64_t>();
    }

    // в uint [Integer]
    uint64_t toUint64() const {
        return toInt64();
    }

    // в float [Float]
    float toFloat() const {
        float f;
        if (_type == BSType::Float) memcpy(&f, _dataP(), _data);
        return f;
    }

    // ============ PARSING ============

#ifndef BSON_NO_TEXT
    bool readStr(Text* t) {
        return next(BSType::String) ? (*t = Text((const char*)_dataP(), _data), true) : false;
    }
#endif

    bool readStr(char* s, uint16_t size, bool terminate = true) {
        if (!next(BSType::String)) return false;
        uint16_t wrote = size < _data ? size : _data;
        memcpy(s, _dataP(), wrote);
        if (terminate && size > wrote) s[wrote] = 0;
        return true;
    }

    bool readBool(bool* b) {
        return next(BSType::Boolean) ? (*b = _data, true) : false;
    }

    template <typename T>
    bool readInt(T* i) {
        return next(BSType::Integer) ? (*i = toInt(), true) : false;
    }

    template <typename T>
    bool readInt64(T* i) {
        return next(BSType::Integer) ? (*i = toInt64(), true) : false;
    }

    bool readFloat(float* f) {
        return next(BSType::Float) ? (memcpy(f, _dataP(), _data), true) : false;
    }

    template <typename T>
    bool readCode(T* c) {
        return next(BSType::Code) ? (*c = T(_data), true) : false;
    }

    template <typename T>
    bool readBin(T* b) {
        return readBin(b, sizeof(T));
    }
    template <typename T>
    bool readBin(T* b, uint16_t size) {
        if (!next(BSType::Binary) || _data != size) return false;
        memcpy(b, _dataP(), _data);
        return true;
    }

    // ============= NEXT =============

    // true - парсинг окончен корректно
    bool isDone() const {
        return _done;
    }

    // парсить следующий блок и проверить тип. Вернёт true при успехе
    bool next(BSType type) {
        return next() && getType() == type;
    }

    // парсить следующий блок и проверить контейнер [ ] { }. Вернёт true при успехе
    bool next(char cont) {
        if (!next(BSType::Container)) return false;
        switch (cont) {
            case '[': return _data == (BS_CONT_ARR | BS_CONT_OPEN);
            case ']': return _data == (BS_CONT_ARR | BS_CONT_CLOSE);
            case '{': return _data == (BS_CONT_OBJ | BS_CONT_OPEN);
            case '}': return _data == (BS_CONT_OBJ | BS_CONT_CLOSE);
            default: return false;
        }
    }

    // парсить следующий блок. Вернёт true при успехе
    bool next() {
        if (_ovf()) return false;

        uint8_t data = BS_DATA(*_bson);
        _type = (BSType)BS_TYPE(*_bson);
        ++_bson;

        switch (_type) {
            case BSType::Container:
                _data = data;
                break;

            case BSType::Boolean:
                _data = BS_BOOLV(data);
                break;

            case BSType::Code:
                if (_ovf()) return _abort();
                _data = BS_D16_MERGE(data, *_bson++);
                break;

            case BSType::String:
            case BSType::Binary:
                if (_ovf()) return _abort();
                _data = BS_D16_MERGE(data, *_bson++);
                if (_ovf(_data)) return _abort();
                _bson += _data;
                break;

            case BSType::Integer:
                _data = BS_SIZE(data);
                if (_ovf(_data)) return _abort();
                _bson += _data;
                _data = data;  // + neg
                break;

            case BSType::Float:
                _data = BS_FLOAT_SIZE;
                if (_ovf(_data)) return _abort();
                _bson += _data;
                break;

            default: break;
        }

        if (_bson == _bend) _done = true;
        return _bson <= _bend;
    }

   private:
    void* _dataP() const {
        return _bson - _data;
    }
    bool _ovf(uint16_t len) const {
        return _bson + len > _bend;
    }
    bool _ovf() const {
        return _bson >= _bend;
    }
    bool _abort() {
        _type = BSType::Error;
        _bson = _bend;
        return false;
    }

    template <typename T>
    T _toInt() const {
        if (_type != BSType::Integer) return 0;

        T v = 0;
        uint8_t size = BS_SIZE(_data);
        memcpy(&v, _bson - size, size > sizeof(T) ? sizeof(T) : size);
        return BS_NEGATIVE(_data) ? -v : v;
    }

    uint8_t* _bson;
    uint8_t* _bend;
    uint16_t _data;
    bool _done = false;
    BSType _type = BSType::Error;
};
