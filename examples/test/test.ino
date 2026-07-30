#include <Arduino.h>
#include <BSON.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID ""
#define WIFI_PASS ""

ESP8266WebServer server;

const char index_html[] PROGMEM = R"raw(
<!DOCTYPE html>

<body>
    <button id="req">REQUEST</button>
    <p id="out" style="white-space: break-spaces;"></p>

    <script type="module">
        import {decodeBson} from 'https://gyverlibs.github.io/bson.js/bson.min.js';

        const codes = [
            'some',
            'string',
            'constants',
        ];

        req.onclick = async () => {
            let res = await fetch(window.location.origin + '/bson');
            try {
                let arr = new Uint8Array(await res.arrayBuffer());
                let json = decodeBson(arr, codes);
                console.log(json);
                out.innerText = JSON.stringify(json, null, 2);
            } catch (e) {
                console.log(e);
                out.innerText = e;
            }
        }
    </script>
</body>

</html>
)raw";

void setup() {
    Serial.begin(115200);
    Serial.println();
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());

    server.begin();

    server.on("/", []() {
        server.sendHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
        server.sendHeader(F("Pragma"), F("no-cache"));
        server.sendHeader(F("Expires"), F("0"));
        server.send_P(200, "text/html", (PGM_P)index_html, strlen_P(index_html));
    });
    server.on("/bson", []() {
        enum class Const {
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
            b["f0"] = 0.0;
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
            b["someval"] = BSCode(Const::some);
            b[BSCode(Const::string)] = "string";
            b.keyCode(Const::some) = BSCode(Const::constants);
            b('}');
        }

        if (b["arr"]('[')) {
            b += 123;
            b += 3.1415;
            b += "str";
            b += false;
            b += nullptr;
            b += BSCode(Const::some);

            b('{');
            b[BSCode(Const::some)] = 123;
            b('}');

            b(']');
        }

        b["bin"].addBin("test", 4);
        b('}');

        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.sendHeader(F("Access-Control-Allow-Private-Network"), F("true"));
        server.sendHeader(F("Access-Control-Allow-Methods"), F("*"));
        server.send(200, "text/html", b.buf(), b.length());
    });
}

void loop() {
    server.handleClient();
}