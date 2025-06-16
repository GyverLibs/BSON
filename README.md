[![latest](https://img.shields.io/github/v/release/GyverLibs/BSON.svg?color=brightgreen)](https://github.com/GyverLibs/BSON/releases/latest/download/BSON.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/BSON.svg)](https://registry.platformio.org/libraries/gyverlibs/BSON)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/BSON?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# BSON
Простой "бинарный" вариант JSON пакета, собирается линейно:
- В среднем в 2-3 раза легче обычного JSON, собирается сильно быстрее
- В ~4 раза быстрее String строки в сборке
- Поддерживает "коды": число, которое может быть ключом или значением, а при распаковке заменится на строку из списка по индексу
- Строки не нужно экранировать
- Поддержка целых чисел 0..8 байт
- Поддержка float с указанием количества знаков точности
- Поддержка JSON массивов и объектов ключ:значение
- Поддержка упаковки произвольных бинарных данных
- Не содержит запятых, они добавляются при распаковке
- Лимит длины `8192` байт для всего: значение кодов, длина строк, длина бинарных данных

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

### Зависимости
- [StringUtils](https://github.com/GyverLibs/StringUtils)
- [GTL](https://github.com/GyverLibs/GTL)

## Содержание
- [Использование](#usage)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="usage"></a>

## Использование
### Структура пакета
![bson](/docs/bson.png)

### Описание класса
```cpp
// прибавить данные любого типа
BSON& add(T data);
void operator=(T data);
void operator+=(T data);

// float
BSON& add(float data, int dec);
BSON& add(double data, int dec);

// ключ
BSON& operator[](T key);

// контейнер, всегда вернёт true. type: '{', '[', '}', ']'
bool operator()(char type);

// бинарные данные
bool beginBin(uint16_t size);
BSON& add(const void* data, size_t size, bool pgm = false);

// строки
BSON& beginStr(size_t len);

// зарезервировать размер
bool reserve(size_t size);

// зарезервировать, элементов (добавить к текущему размеру буфера)
bool addCapacity(size_t size);

// установить увеличение размера для уменьшения количества мелких реаллокаций. Умолч. 8
void setOversize(uint16_t oversize);

// размер в байтах
size_t length();

// доступ к буферу
uint8_t* buf();

// очистить
void clear();

// переместить в другой объект
void move(BSON& bson);

// STATIC

// максимальная длина строк и бинарных данных
static size_t maxDataLength();

// вывести в Print как JSON
static void stringify(BSON& bson, Print& p, bool pretty = false);

// вывести в Print как JSON
static void stringify(const uint8_t* bson, size_t len, Print& p, bool pretty = false);
```

### Статическая сборка
```cpp
BSON_CONT(char t)   // контейнер '{', '}', '[', ']'
BSON_CODE(code)     // код
BSON_FLOAT(val)     // float
BSON_INT8(val)      // int8
BSON_INT16(val)     // int16
BSON_INT24(val)     // int24
BSON_INT32(val)     // int32
BSON_INT64(val)     // int64
BSON_BOOL(val)      // bool
BSON_STR(str, len)  // "string" + длина
BSON_KEY(str, len)  // "string" + длина
```

## Пример
### Динамическая сборка
```cpp
enum class Const {
    some,
    string,
    constants,
};

BSON b;
b('{');

if (b["str"]('{')) {
    b["cstring"] = "text";
    b["fstring"] = F("text");
    b["String"] = String("text");
    b('}');
}

if (b[Const::constants]('{')) {
    b[Const::some] = Const::string;
    b[Const::string] = "cstring";
    b[Const::constants] = 123;
    b('}');
}

if (b["num"]('{')) {
    b["int8"] = 123;
    b["int16"] = 12345;
    b["int32"] = -123456789;
    b('}');
}

if (b["arr"]('[')) {
    b += "str";
    b += 123;
    b += 3.14;
    b += Const::string;
    b(']');
}

b('}');
```

### Статическая сборка
```cpp
uint8_t bson[] = {
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
    // BSON_FLOAT(3.1415),
    BSON_BOOL(true),
    BSON_CONT(']'),

    BSON_CONT('}'),
};
```

### Распаковка
Есть [готовая библиотека](https://github.com/GyverLibs/bson.js) для JavaScript

> npm i @alexgyver/bson

<a id="versions"></a>

## Версии
- v2.0.0

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