[![latest](https://img.shields.io/github/v/release/GyverLibs/BSON.svg?color=brightgreen)](https://github.com/GyverLibs/BSON/releases/latest/download/BSON.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/BSON.svg)](https://registry.platformio.org/libraries/gyverlibs/BSON)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/BSON?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# BSON
Простой "бинарный" вариант JSON пакета:
- В среднем в 2-3 раза легче обычного JSON, собирается сильно быстрее
- В ~4 раза быстрее String строки в сборке
- Поддержка целых чисел 0..8 байт, float, строк (длина до 8192), bool, произвольных бинарных данных
- Поддерживает "коды": число до 8192, которое может быть ключом или значением, а при распаковке заменится на строку из списка по индексу
- Статическая и динамическая сборка
- Встроенный парсер и JSON-стрингификатор

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

### Зависимости
- [GTL](https://github.com/GyverLibs/GTL)

## Содержание
- [Использование](#usage)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="usage"></a>

## Использование
### Структура пакета
```
|-------------------------------------------------|---------------------------|
| 0                                               | n                         |
|-------------------------------------------------|---------------------------|
| 0  | 1  | 2      | 3   | 4 | 5    | 6    | 7    |                           |
|----|----|--------|-----|---|------|------|------|---------------------------|
| 0. subt |     0. null      |      |      |      |                           |
| 0. subt |     1. bool      |      |      |  v   |                           |
| 0. subt |     2. cont      |      | open | obj  |                           |
| 0. subt |     3. float     |      |      |      |                           |
| 0. subt |     4. bin       |       len len      | len bytes + data bytes    |
| 1. int  | smallu |            val               |                           |
| 1. int  | smallu | neg |           len          | uint bytes                |
| 2. str  | ext    |          len lsb             | ext ? len msb + str bytes |
| 3. code | ext    |          val lsb             | ext ? val msb             |
|-----------------------------------------------------------------------------|
```

### Динамическая сборка
```cpp
// добавить любой поддерживаемый тип
void operator=(T val);
void operator+=(T val);
void add(T val);

// максимальная длина строк и величина code
size_t maxDataLength();

// открыть/закрыть контейнер [ ] { }, всегда вернёт true
bool cont(const char type);
bool operator()(char type);

// ключ для объектов
BSON& key(T k);
BSON& operator[](T k);
BSON& keyCode(T k);

// добавить код
void addCode(T code);
void addCode(BSCode_t code);    // addCode(BSCode(T))

// добавить bool
void addBool(bool b);
void addBool(const void* b);

// добавить int
void addInt(const void* p, uint8_t size);

// добавить uint
void addUint(const void* p, uint8_t size, bool negative = false);

// добавить int
void addInt(T val);

// добавить float
void addFloat(const void* p);
void addFloat(float value);
void addFloat(double value);

// добавить null
void addNull();

void operator=(nullptr_t);
void operator+=(nullptr_t);
void add(nullptr_t);

// начать бинарные данные, затем вручную write(data, size, pgm)
void beginBin(uint32_t size);

// добавить бинарные данные
void addBin(const void* data, uint32_t size, bool pgm = false);
void addBin(const T& data);

// добавить символ как строку
void addStr(char sym);

// начать строку, затем вручную write(str, len, pgm). Если размер превышает - добавится пустая строка
bool beginStr(size_t len);

// добавить строку. Обрежется по макс. размеру
void addStr(const char* str, size_t len, bool pgm = false);
void addStr(char* str);
void addStr(const char* str);
void addStr(const String& str);
void addStr(const __FlashStringHelper* str);

// вывести в Print как JSON
void stringify(Print& p, bool pretty = false);

// вывести в Print как JSON
static void stringify(BSON& bson, Print& p, bool pretty = false);
static void stringify(const uint8_t* bson, size_t len, Print& p, bool pretty = false);
```

### Статическая сборка
```cpp
BN_CONT(char t)   // контейнер '{', '}', '[', ']'
BS_CODE(code)     // код
BS_FLOAT(val)     // float
BS_ZERO()         // 0
BS_INT8(val)      // int8
BS_INT16(val)     // int16
BS_INT24(val)     // int24
BS_INT32(val)     // int32
BS_INT64(val)     // int64
BS_BOOL(val)      // bool
BS_NULL()         // null
BS_STR(str, len)  // "string" + длина
BS_CHARS(...)     // 's', 't', 'r', 'i', 'n', 'g'
```

### Линейный парсер BSON::Parser
```cpp
enum class BSType {
    Null,
    Bool,
    Cont,
    Float,
    Bin,
    Int,
    String,
    Code,
    Error,
};
```
```cpp
BSParser(const uint8_t* bson, size_t len);

// получить тип блока
BSType getType();

// совпадает тип блока
bool is(BSType type);

// получить контейнер
char getCont();

// контейнер-объект
bool isObj();

// контейнер открыт
bool isOpen();

// длина в байтах [String, Bin]
size_t length();

// число отрицательное [Int]
bool isNegative();

// ============ GET ============

// указатель на строку [String], длина length()
const char* getStr();

// в свой тип [Bin]
void readBin(void* p, size_t size);
void readBin(T* p);
T getBin();

// указатель на [Bin], длина length()
const uint8_t* getBin();

// в uint16_t код [Code]
void readCode(void* p);
uint16_t getCode();
T getCode();

// в bool [Bool]
void readBool(void* p);
bool getBool();

// в int [Int]
void readInt(void* p, uint8_t size);

// в int [Int]
int32_t getInt();

// в uint [Int]
uint32_t getUint();

// в int [Int]
int64_t getInt64();

// в uint [Int]
uint64_t getUint64();

// в float [Float]
void readFloat(void* p);
float getFloat();

// ============ PARSE ============

// парсить и прочитать строку
bool parseStr(char* s, uint16_t size, bool terminate = true);

// парсить и прочитать bool
bool parseBool(void* b);

// парсить и прочитать int
bool parseInt(T* i);
bool parseInt(void* i, uint8_t size);

// парсить и прочитать float
bool parseFloat(void* f);

// парсить и прочитать uint16_t code
bool parseCode(T* p);

// парсить и прочитать bin
bool parseBin(T* b);
bool parseBin(void* b, uint16_t size);

// ============= PARSE =============

// true - парсинг окончен
bool isDone();

// true - ошибка парсинга
bool isError();

// парсить следующий блок и проверить тип. Вернёт true при успехе
bool parse(BSType type);

// парсить следующий блок. Вернёт true при успехе
bool parse();
```

## Примеры
### Динамическая сборка
```cpp
BSON b;

b('{');

if (b["str"]('{')) {
    b["cstring"] = "text";
    b["fstring"] = F("ftext");
    b('}');
}

if (b["int"]('{')) {
    b["int"] = -1234567;
    b["uint"] = (uint16_t)12345;
    b('}');
}

if (b["float"]('{')) {
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

enum class Codes {
    some,
    string,
    constants,
};

if (b["codes"]('{')) {
    b["some"] = BSCode(Codes::constants);
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
    b(']');
}

b('}');

b.stringify(Serial, true);
```

### Статическая сборка
```cpp
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
    BS_FLOAT(3.1415),
    BS_BOOL(true),
    BS_NULL(),
    BS_CONT(']'),

    BS_CONT('}'),
};
```

### Парсинг
```cpp
struct Str {
    int v;
    float f;
};

int v;
char s[5];
bool b;
float f;
Str str;

BSParser p(bs, sizeof(bs));

if (p.parseInt(&v) &&
    p.parseStr(s, sizeof(s)) &&
    p.parseBool(&b) &&
    p.parseFloat(&f) &&
    p.parseBin(&str)) {
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
```

### Другие платформы
Есть [готовая библиотека](https://github.com/GyverLibs/bson.js) для JavaScript

> npm i @alexgyver/bson

<a id="versions"></a>

## Версии
- v3.0.0

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **BSON** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/BSON/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>

## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код