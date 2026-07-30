#pragma once
#include "./macro.h"

// ============== static macro ==============
#define _BSON_INTx(val, len) (_BS_INT | (val < 0 ? _BS_INT_NEG : 0) | (len))
#define _BSON_BYTEx(val, n) (((val < 0 ? -val : val) >> (n * 8)) & 0xff)
#define _BSON_FLOATx(val, n) ((union { float f; uint8_t b[4]; }){val}.b[n])

// str
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

// chars
#define BS_NARG(...) BS_NARG_(__VA_ARGS__, BS_RSEQ_N())
#define BS_NARG_(...) BS_ARG_N(__VA_ARGS__)
#define BS_ARG_N(                                     \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,          \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
    _61, _62, _63, N, ...) N

#define BS_RSEQ_N()                             \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54,     \
        53, 52, 51, 50, 49, 48, 47, 46, 45, 44, \
        43, 42, 41, 40, 39, 38, 37, 36, 35, 34, \
        33, 32, 31, 30, 29, 28, 27, 26, 25, 24, \
        23, 22, 21, 20, 19, 18, 17, 16, 15, 14, \
        13, 12, 11, 10, 9, 8, 7, 6, 5, 4,       \
        3, 2, 1, 0

// =========== STATIC BUILD ==========
inline constexpr uint8_t BS_CONT(char t) { return _BS_SUBT | _BS_CONT | (t == '{' ? _BS_OBJ_OPEN : (t == '}' ? _BS_OBJ_CLOSE : (t == '[' ? _BS_ARR_OPEN : _BS_ARR_CLOSE))); }
#define BS_CODE(code) (_BS_CODE | _BS_EXT_FLAG | _BS_GET_LSB(code)), _BS_GET_MSB(code)
#define BS_FLOAT(val) (_BS_SUBT | _BS_FLOAT), _BSON_FLOATx(val, 0), _BSON_FLOATx(val, 1), _BSON_FLOATx(val, 2), _BSON_FLOATx(val, 3)
#define BS_ZERO() (_BS_INT | _BS_INT_SMALL)
#define BS_INT8(val) _BSON_INTx(val, 1), _BSON_BYTEx(val, 0)
#define BS_INT16(val) _BSON_INTx(val, 2), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1)
#define BS_INT24(val) _BSON_INTx(val, 3), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2)
#define BS_INT32(val) _BSON_INTx(val, 4), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2), _BSON_BYTEx(val, 3)
#define BS_INT64(val) _BSON_INTx(val, 8), _BSON_BYTEx(val, 0), _BSON_BYTEx(val, 1), _BSON_BYTEx(val, 2), _BSON_BYTEx(val, 3), _BSON_BYTEx(val, 4), _BSON_BYTEx(val, 5), _BSON_BYTEx(val, 6), _BSON_BYTEx(val, 7)
#define BS_BOOL(val) (_BS_SUBT | _BS_BOOL | _BS_GET_BOOLV(val))
#define BS_STR(str, len) (_BS_STR | _BS_EXT_FLAG | _BS_GET_LSB(len)), _BS_GET_MSB(len), _BS_STR_N##len(str)
#define BS_CHARS(...) (_BS_STR | _BS_EXT_FLAG | _BS_GET_LSB(BS_NARG(__VA_ARGS__))), _BS_GET_MSB(BS_NARG(__VA_ARGS__)), __VA_ARGS__
#define BS_NULL() (_BS_SUBT | _BS_NULL)