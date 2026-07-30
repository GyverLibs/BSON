#include <Arduino.h>
#include <BSON.h>

void setup() {
    Serial.begin(115200);
    Serial.println("start");

    enum class Codes {
        some,
        string,
        constants,
    };

    BSON b;

    b('{');

    if (b["str"]('{')) {
        b["cstring"] = "text";
        b["fstring"] = F("ftext");
        b["String"] = String("stext");
        b['s'] = 'a';
        b('}');
    }

    if (b["int"]('{')) {
        b["int0"] = (int8_t)-0;
        b["int4"] = (int8_t)-16;
        b["int8"] = (int8_t)-123;
        b["int16"] = (int16_t)-12345;
        b["int24"] = (int32_t)-1234567;
        b["int32"] = -123456789;
        b["int40"] = -1234567898765;

        b["uint0"] = (uint8_t)0;
        b["uint4"] = (uint8_t)16;
        b["uint8"] = (uint8_t)123;
        b["uint16"] = (uint16_t)12345;
        b["uint24"] = (uint32_t)1234567;
        b["uint32"] = 123456789;
        b["uint40"] = 1234567898765;
        b('}');
    }

    if (b["float"]('{')) {
        b["zero"] = 0.0;
        b["float"] = 3.1415;
        b["fnan"] = NAN;
        b["finf"] = INFINITY;
        b('}');
    }

    if (b["other"]('{')) {
        b["true"] = true;
        b["false"] = false;
        b["null"] = nullptr;
        b('}');
    }

    if (b["codes"]('{')) {
        b["some"] = BSCode(Codes::some);
        b[BSCode(Codes::string)] = "string";
        b[BSCode(Codes::some)] = BSCode(Codes::string);
        b('}');
    }

    if (b["arr"]('[')) {
        b += 123;
        b += 3.1415;
        b += "str";
        b += false;
        b += nullptr;
        b += BSCode(Codes::some);

        b('{');
        b[BSCode(Codes::some)] = 123;
        b('}');

        b(']');
    }

    b["bin"].addBin("test", 4);
    b('}');

    b.stringify(Serial, true);
}

void loop() {
}