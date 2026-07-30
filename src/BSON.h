#pragma once
#include <GTL.h>

#include "./core/parser.h"
#include "./core/static.h"
#include "./core/stringify.h"
#include "BSONV.h"

// ================= BSON =================
class BSON : public BSONV {
   public:
    using BSONV::add;
    using BSONV::operator=;
    using BSONV::operator+=;
    using BSONV::operator();
    using BSONV::operator[];

    // ============== add bson ==============
    // добавить BSON
    void addBSON(const BSON& bson) {
        write(bson.data(), bson.length());
    }

    void operator+=(const BSON& bson) { addBSON(bson); }
    void operator=(const BSON& bson) { addBSON(bson); }
    void add(const BSON& bson) { addBSON(bson); }

    // ============== stringify ==============
#ifdef ARDUINO
    // вывести в Print как JSON
    void stringify(Print& p, bool pretty = false) {
        stringify(*this, p, pretty);
    }

    // вывести в Print как JSON
    static void stringify(BSON& bson, Print& p, bool pretty = false) {
        stringify(bson, bson.length(), p, pretty);
    }

    static void stringify(const uint8_t* bson, size_t len, Print& p, bool pretty = false) {
        BSStringify(bson, len, p, pretty).stringify();
    }
#endif

    size_t write(const void* data, size_t len, bool pgm = false) override {
        return st.write(data, len, pgm);
    }

    void clear() { st.clear(); }
    bool reserve(size_t len) { return st.reserve(len); }
    size_t length() const { return st.length(); }
    void setOversize(uint8_t len) { st.setOversize(len); }

    operator const uint8_t*() const { return data(); }
    operator uint8_t*() { return data(); }
    const uint8_t* data() const { return st.data(); }
    uint8_t* data() { return st.data(); }
    const uint8_t* buf() const { return st.data(); }
    uint8_t* buf() { return st.data(); }

   private:
    gtl::stack<uint8_t> st;
};