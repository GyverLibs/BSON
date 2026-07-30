#include <Arduino.h>
#include <BSON.h>

uint8_t bson_st[] = {
    BS_CONT('{'),
    BS_STR("str1", 3),
    BS_CHARS('s', 't', 'r', 'i', 'n', 'g'),

    BS_STR("str2", 3),
    BS_STR("string", 6),

    BS_STR("int", 3),
    BS_INT16(12345),

    BS_STR("arr", 3),
    BS_CONT('['),
    BS_CODE(12),
    BS_ZERO(),
    BS_INT8(123),
    BS_INT8(-123),
    BS_INT16(12345),
    BS_INT16(-12345),
    BS_INT32(12345678),
    BS_INT32(-12345678),
    BS_FLOAT(3.1415),
    BS_BOOL(true),
    BS_NULL(),
    BS_CONT(']'),

    BS_CONT('}'),
};

void setup() {
    Serial.begin(115200);

    BSON::stringify(bson_st, sizeof(bson_st), Serial, true);
}

void loop() {
}