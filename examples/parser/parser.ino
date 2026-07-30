#include <Arduino.h>
#include <BSON.h>

uint8_t bson_st[] = {
    BS_CONT('{'),
    BS_STR("str", 3),
    BS_STR("hello", 5),

    BS_STR("int", 3),
    BS_INT16(12345),

    BS_STR("arr", 3),
    BS_CONT('['),
    // BS_STR("string", 6),
    BS_CHARS('s', 't', 'r', 'i', 'n', 'g'),
    BS_CODE(12),
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
    Serial.println("start");

    BSParser p(bson_st, sizeof(bson_st));

    while (p.parse()) {
        switch (p.getType()) {
            case BSType::String:
                Serial.print("String: ");
                Serial.write(p.getStr(), p.length());
                Serial.println();
                break;

            case BSType::Bool:
                Serial.print("Boolean: ");
                Serial.println(p.getBool());
                break;

            case BSType::Int:
                Serial.print("Integer: ");
                Serial.println(p.getInt());
                break;

            case BSType::Float:
                Serial.print("Float: ");
                Serial.println(p.getFloat());
                break;

            case BSType::Code:
                Serial.print("Code: ");
                Serial.println(p.getCode());
                break;

            case BSType::Bin:
                Serial.println("Binary");
                break;

            case BSType::Null:
                Serial.println("Null");
                break;

            case BSType::Cont:
                Serial.println(p.getCont());
                break;

            default:
                break;
        }
    }

    Serial.println("end");
}

void loop() {
}