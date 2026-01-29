#include <Arduino.h>
#include <BSON.h>

void setup() {
    Serial.begin(115200);
    Serial.println("start");

    struct Str {
        int v;
        float f;
    };

    BSON bs;
    bs += 1234;
    bs += "keks";
    bs += true;
    bs += 3.14;
    bs.addBin(Str{321, 33.44});

    BSON::Parser p(&bs);

    int v;
    char s[5];
    bool b;
    float f;
    Str str;

    if (p.readInt(&v) &&
        p.readStr(s, sizeof(s)) &&
        p.readBool(&b) &&
        p.readFloat(&f) &&
        p.readBin(&str)) {
        //
        Serial.println("done");
        Serial.println(v);
        Serial.println(s);
        Serial.println(b);
        Serial.println(f);
        Serial.println(str.v);
        Serial.println(str.f);
    } else {
        Serial.println("error");
    }
}

void loop() {
}