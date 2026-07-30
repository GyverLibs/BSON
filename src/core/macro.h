#pragma once
#include <inttypes.h>

/*
[TTxxxxxx]

0. Subtype  [xxSSSxxx]
  0. null
  1. bool   [xxxxxxxV]
  2. cont   [xxxxxx<opn><obj>]
  3. float
  4. bin    [xxxxxLLL] + len + bytes
  5. -
  6. -
  7. -
1. Integer  [xxSVVVVV / xxSNLLLL]: small_uint:1 | (val:5 / neg:1 | len:4) + bytes
2. String   [xxELLLLL]: ext:1 | len:5 + 1 msb ext + bytes
3. Code     [xxEVVVVV]: ext:1 | val:5 + 1 msb ext
*/

// type
#define _BS_TYPE_MASK 0b11000000
#define _BS_GET_TYPE(x) ((x) & _BS_TYPE_MASK)
#define _BS_SUBT (0u << 6)
#define _BS_INT (1u << 6)
#define _BS_STR (2u << 6)
#define _BS_CODE (3u << 6)

// subtype
#define _BS_SUBT_MASK 0b00111000
#define _BS_GET_SUBT(x) ((x) & _BS_SUBT_MASK)
#define _BS_NULL (0u << 3)
#define _BS_BOOL (1u << 3)
#define _BS_CONT (2u << 3)
#define _BS_FLOAT (3u << 3)
#define _BS_BIN (4u << 3)

// type int
#define _BS_INT_NEG (1u << 4)
#define _BS_INT_SMALL (1u << 5)
#define _BS_GET_SMALLV(x) ((x) & 0b11111)
#define _BS_GET_INT_SIZE(x) ((x) & 0b1111)
#define _BS_IS_NEG(x) ((x) & _BS_INT_NEG)
#define _BS_IS_SMALL(x) ((x) & _BS_INT_SMALL)

// type string
// type code
#define _BS_EXT_BITS 5
#define _BS_EXT_FLAG (1u << _BS_EXT_BITS)
#define _BS_EXT_MASK (_BS_EXT_FLAG - 1)
#define _BS_MAX_EXT_LEN ((1u << (_BS_EXT_BITS + 8)) - 1)
#define _BS_IS_EXT(x) ((x) & _BS_EXT_FLAG)
#define _BS_GET_LSB(x) ((x) & _BS_EXT_MASK)
#define _BS_GET_MSB(x) ((x) >> _BS_EXT_BITS)

// subtype bool
#define _BS_GET_BOOLV(x) ((x) & 1)

// subtype cont
#define _BS_CONT_ARR (0u << 0)
#define _BS_CONT_OBJ (1u << 0)
#define _BS_CONT_CLOSE (0u << 1)
#define _BS_CONT_OPEN (1u << 1)
#define _BS_ARR_OPEN (_BS_CONT_ARR | _BS_CONT_OPEN)
#define _BS_OBJ_OPEN (_BS_CONT_OBJ | _BS_CONT_OPEN)
#define _BS_ARR_CLOSE (_BS_CONT_ARR | _BS_CONT_CLOSE)
#define _BS_OBJ_CLOSE (_BS_CONT_OBJ | _BS_CONT_CLOSE)
#define _BS_IS_OBJ(x) ((x) & _BS_CONT_OBJ)
#define _BS_IS_OPEN(x) ((x) & _BS_CONT_OPEN)

// subtype float
#define _BS_FLOAT_SIZE sizeof(float)

// subtype bin
#define _BS_GET_BIN_SIZE(x) ((x) & 0b111)

// =========== Code ===========
struct BSCode_t {
    uint16_t value;
};

template <typename T>
inline constexpr BSCode_t BSCode(T code) {
    return BSCode_t{uint16_t(code)};
}

// =========== Type ===========
enum class BSType : uint8_t {
    Null = _BS_NULL,
    Bool = _BS_BOOL,
    Int = _BS_INT,
    Uint,
    Float = _BS_FLOAT,
    String = _BS_STR,
    FString,
    Bin = _BS_BIN,
    Code = _BS_CODE,
    Cont = _BS_CONT,
    Error = 0xff,
};