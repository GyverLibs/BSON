#pragma once
#include <stddef.h>
#include <string.h>

#include "./parser.h"

#ifdef ARDUINO

class BSStringify : public BSParser {
   public:
    BSStringify(const uint8_t* bson, size_t len, Print& p, bool pretty = false) : BSParser(bson, len), p(p), pretty(pretty) {}

    void stringify() {
        parse();
        _stringify();
        p.println();
    }

    void _stringify() {
        switch (getType()) {
            case BSType::Cont: {
                bool first = true;
                bool obj = isObj();

                if (isOpen()) {
                    p.print(obj ? '{' : '[');
                    if (pretty) p.println();

                    depth++;
                    while (parse()) {
                        if (is(BSType::Cont) && !isOpen()) break;

                        if (!first) {
                            p.print(',');
                            if (pretty) p.println();
                        }
                        first = false;

                        if (pretty) _indent();
                        _stringify();

                        if (obj) {
                            p.print(':');
                            if (pretty) p.print(' ');
                            parse();
                            _stringify();
                        }
                    }
                    depth--;

                    if (pretty) {
                        p.println();
                        _indent();
                    }
                    p.print(obj ? '}' : ']');
                }
            } break;

            case BSType::Null:
                p.print("null");
                break;

            case BSType::Bool:
                p.print(getBool() ? "true" : "false");
                break;

            case BSType::Float:
                p.print(getFloat());
                break;

            case BSType::Bin: {
                p.print("\"bin:");
                uint16_t len = length();
                uint8_t* b = (uint8_t*)getBin();
                while (len--) {
                    p.print(' ');
                    if (*b < 0x10) p.print('0');
                    p.print(*b++, HEX);
                }
                p.print("\"");
            } break;

            case BSType::Int:
                isNegative() ? p.print(getInt()) : p.print(getUint());
                break;

            case BSType::String:
                p.print('"');
                p.write(getStr(), length());
                p.print('"');
                break;

            case BSType::Code:
                p.print("\"#");
                p.print(getCode());
                p.print('"');
                break;

            default:
                break;
        }
    }

   private:
    Print& p;
    uint8_t depth = 0;
    bool pretty;

    void _indent() {
        for (uint8_t i = 0; i < depth * 2; i++) p.write(' ');
    }
};
#endif