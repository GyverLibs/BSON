#include <Arduino.h>
#include <BSON.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "AlexMain"
#define WIFI_PASS "lolpass12345"

ESP8266WebServer server;

const char index_html[] PROGMEM = R"raw(
<!DOCTYPE html>

<body>
    <button id="req">REQUEST</button>
    <p id="out"></p>

    <script type="module">
        import decodeBson from 'https://gyverlibs.github.io/bson.js/bson.js';

        req.onclick = async () => {
            let res = await fetch(window.location.origin + '/bson');
            try {
                let arr = new Uint8Array(await res.arrayBuffer());
                let json = decodeBson(arr);
                console.log(arr);
                console.log(JSON.stringify(json), JSON.stringify(json).length);
                console.log(json);
                out.innerText = JSON.stringify(json, null, 2);
            } catch (e) {
                console.log(e);
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
        BSON b;
        b.beginObj();
        b.addStr("cstring", "text");
        b.addStr("fstring", F("text"));
        b.addStr("String", String("text"));
        
        b.addBool("true", true);
        b.addBool("false", false);

        b.addInt("nan", NAN);
        b.addInt("inf", INFINITY);
        b.addInt("int0", -0);
        b.addInt("int8", -123);
        b.addInt("int16", -12345);
        b.addInt("int24", -1234567);
        b.addInt("int32", -123456789);
        b.addInt("int40", -1000000000000);

        b.addInt("uint0", 0);
        b.addInt("uint8", 123);
        b.addInt("uint16", 12345);
        b.addInt("uint24", 1234567);
        b.addInt("uint32", 123456789);
        b.addInt("uint40", 1000000000000);

        b.addInt("uint0+", 0u);
        b.addInt("uint8+", 123u);
        b.addInt("uint16+", 12345u);
        b.addInt("uint24+", 1234567u);
        b.addInt("uint32+", 123456789u);
        b.addInt("uint40+", 1000000000000ull);

        b.addFloat("float1", 3.1415, 1);
        b.addFloat("float4", 3.1415, 4);
        b.addFloat("fnan", NAN, 4);
        b.addFloat("finf", INFINITY, 4);
        b.endObj();

        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.sendHeader(F("Access-Control-Allow-Private-Network"), F("true"));
        server.sendHeader(F("Access-Control-Allow-Methods"), F("*"));
        server.send(200, "text/html", b.buf(), b.length());
    });
}

void loop() {
    server.handleClient();
}