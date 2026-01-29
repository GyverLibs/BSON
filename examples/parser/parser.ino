#include <Arduino.h>
#include <BSON.h>

uint8_t bson_st[] = {
    BSON_CONT('{'),
    BSON_KEY("str", 3),
    BSON_STR("hello", 5),

    BSON_KEY("int", 3),
    BSON_INT16(12345),

    BSON_KEY("arr", 3),
    BSON_CONT('['),
    BSON_STR("string", 6),
    BSON_CODE(12),
    BSON_INT8(123),
    BSON_INT8(-123),
    BSON_INT16(12345),
    BSON_INT16(-12345),
    BSON_INT32(12345678),
    BSON_INT32(-12345678),
    BSON_FLOAT(3.1415),
    BSON_BOOL(true),
    BSON_CONT(']'),

    BSON_CONT('}'),
};

void setup() {
    Serial.begin(115200);
    Serial.println("start");

    // BSON b;
    // b.concat(bson_st, sizeof(bson_st));
    // b.stringify(Serial);

    BSON::Parser p(bson_st, sizeof(bson_st));

    while (p.next()) {
        switch (p.getType()) {
            case BSType::String:
                Serial.print("String: ");
                Serial.write(p.toStr(), p.length());
                Serial.println();
                break;

            case BSType::Boolean:
                Serial.print("Boolean: ");
                Serial.println(p.toBool());
                break;

            case BSType::Integer:
                Serial.print("Integer: ");
                Serial.println(p.toInt());
                break;

            case BSType::Float:
                Serial.print("Float: ");
                Serial.println(p.toFloat());
                break;

            case BSType::Code:
                Serial.print("Code: ");
                Serial.println(p.toCode<int>());
                break;

            case BSType::Binary:
                Serial.println("Binary");
                break;

            case BSType::Container:
                Serial.print(p.isObject() ? "Object" : "Array");
                Serial.println(p.isOpen() ? "Open" : "Close");
                break;
        }
    }

    Serial.println("end");
}

void loop() {
}