[![latest](https://img.shields.io/github/v/release/GyverLibs/BSON.svg?color=brightgreen)](https://github.com/GyverLibs/BSON/releases/latest/download/BSON.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/BSON.svg)](https://registry.platformio.org/libraries/gyverlibs/BSON)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/BSON?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# BSON
Простой "бинарный" вариант JSON пакета, собирается линейно:
- В среднем в 2-3 раза легче обычного JSON, собирается сильно быстрее
- Поддерживает "коды": число, которое может быть ключом или значением, а при распаковке заменится на строку из списка по индексу
- Строки не нужно экранировать
- Поддержка целых чисел 1..8 байт
- Поддержка float с указанием количества знаков точности
- Поддержка JSON массивов и объектов ключ:значение
- Поддержка упаковки произвольных бинарных данных
- Не содержит запятых, они добавляются при распаковке
- Лимит длины `8192` байт для всего: коды, ключи, строки, бинарные данные

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
```
0 key code: [code msb:5] + [code:8]
1 key str: [len msb:5] + [len:8] + [...]
2 val code: [code msb:5] + [code:8]
3 val str: [len msb:5] + [len:8] + [...]
4 val int: [sign:1 + len:4]
5 val float: [dec:5]
6 cont: [obj:1 / arr:0] + [open:1 / close:0]
7 bin: [len msb:5] + [len:8] + [...]
```

![bson](/docs/bson.png)

### Описание класса
```cpp
// прибавить бинарные данные
size_t write(const uint8_t* buf, size_t len);

// зарезервировать размер
bool reserve(size_t size);

// размер в байтах
size_t length();

// доступ к буферу
uint8_t* buf();

// очистить
void clear();

// переместить в другой объект
void move(BSON& bson);

// данные как Text
Text toText();
operator Text();

// bson
void add(const BSON& bson);
void operator+=(const BSON& bson);

// key
void addKey(uint16_t key);
void addKey(const Text& key);

BSON& operator[](uint16_t key);
BSON& operator[](Text key);

// code
void addCode(uint16_t key, uint16_t val);
void addCode(const Text& key, uint16_t val);
void addCode(uint16_t val);

// bool
void addBool(bool b);
void addBool(uint16_t key, bool b);
void addBool(const Text& key, bool b);

// uint
void addUint(T val);
void addUint(unsigned long long val);
void addUint(uint16_t key, T val);
void addUint(const Text& key, T val);

// int
void addInt(T val);
void addInt(long long val);
void addInt(uint16_t key, T val);
void addInt(const Text& key, T val);

// float
void addFloat(T value, uint8_t dec);
void addFloat(uint16_t key, T value, uint8_t dec);
void addFloat(const Text& key, T value, uint8_t dec);

// text
void addText(const Text& text);
void addText(uint16_t key, const Text& text);
void addText(const Text& key, const Text& text);

// bin
void addBin(const void* data, size_t size);
void addBin(const Text& key, const void* data, size_t size);
void addBin(uint16_t key, const void* data, size_t size);
bool beginBin(uint16_t size);

// object
void beginObj();
void beginObj(uint16_t key);
void beginObj(const Text& key);
void endObj();

// array
void beginArr();
void beginArr(uint16_t key);
void beginArr(const Text& key);
void endArr();
```

### Примеры
```cpp
BSON bs;
bs.beginObj();
bs.addInt("key", 123);
bs["key2"] = 456;

bs.beginArr("arr");
bs.addText("str1");
bs += F("str2");
bs += "str3";
bs.endArr();

bs.endObj();
```

### Распаковка
Есть [готовая библиотека](https://github.com/GyverLibs/bson.js) для JavaScript

> npm i @alexgyver/bson

<a id="versions"></a>

## Версии
- v1.0

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