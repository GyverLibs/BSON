This is an automatic translation, may be incorrect in some places. See sources and examples!

# Bson
A simple "binary" version of the Json package, gathers linearly:
- On average, 2-3 times lighter than a regular JSON, it is going much faster
- B ~ 4 times faster string lines in the assembly
- supports "codes": the number that can be a key or value, and when unpacking will be replaced with a line from the list on the index
- Lines do not need to be shielded
- support for integers 0..8 bytes
- Float support indicating the number of accuracy signs
- Support for JSON of MASSES and Objects Key: Value
- Support for packaging of arbitrary binary data
- does not contain commas, they are added when unpacking
- Limit of the length of `8192 back for everything: codes, lines length, binary data length

## compatibility
Compatible with all arduino platforms (used arduino functions)

### Dependencies
- [Stringutils] (https://github.com/gyverlibs/stringutils)
- [gtl] (https://github.com/gyverlibs/gtl)

## Content
- [use] (#usage)
- [versions] (#varsions)
- [installation] (# Install)
- [bugs and feedback] (#fedback)

<a id = "USAGE"> </A>

## Usage
### Package structure
! [bson] (/doc/bson.png)

### description of the class
`` `CPP
// add any type of data
BSON & Add (T Data);
VOID Operator = (T Data);
VOID Operator+= (T Data);

// Float
BSON & Add (Float Data, Int dec);
BSON & Add (Double Data, Int dec);

// key
BSON & Operator [] (T Key);

// container, always return True.type: '{', '[', '}', ']'
Bool Operator () (Char Type);

// binary data
Bool Beginbin (Uint16_T Size);
BSON & Add (Consta* Data, Size_t Size, Bool PGM = FALSE);

// Lines
Bson & beginstr (size_t leen);

// Reserve size
Bool Reserve (Size_T SIZE);

// reserve, elements (add to the current size of the buffer)
Bool Addcapacy (Size_t Size);

// Install an increase in size to reduce the number of small reallocks.Silent.8
VOID setOVERSIZE (Uint16_T Oversize);

// size in bytes
Size_t Length ();

// access to the buffer
uint8_t* buf ();

// Clean
Void Clear ();

// move to another object
VOID Move (BSON & BSON);

// Static

// maximum length of lines and binary data
static size_t maxdatalength ();

// Bring to Print as json
Static Vood Stringifi (BSON & BSON, Print & P, Bool Pretty = False);

// Bring to Print as json
Static Void Stringifi (Consta Uint8_t* Bson, Size_t Len, Print & P, Bool Pretty = False);
`` `

### Static assembly
`` `CPP
Bson_cont (char t) // container '{' ','} ',' [','] '
Bson_code (code) // Code
Bson_float (val) // Float
BSON_INT8 (VAL) // Int8
BSON_INT16 (VAL) // Int16
BSON_INT24 (VAL) // Int24
Bson_int32 (val) // Int32
BSON_INT64 (VAL) // Int64
BSON_BOOL (VAL) // Bool
Bson_str (str, len) // "string" + length
Bson_key (str, len) // "string" + length
`` `

## Example
### Dynamic assembly
`` `CPP
enum class const {
SOME,
String,
Constants,
};

Bson b;
b ('{');

if (b ["str"] ('{')) {
b ["cstring"] = "text";
b ["fstring"] = f ("text");
b ["string"] = string ("text");
b ('}');
}

if (b [const :: constants] ('{')) {
B [const :: some] = const :: string;
B [const :: string] = "Cstring";
B [const :: constants] = 123;
b ('}');
}

if (b ["num"] ('{')) {
b ["int8"] = 123;
b ["int16"] = 12345;
b ["int32"] = -123456789;
b ('}');
}

if (b ["arr"] ('')) {
b += "str";
b += 123;
b += 3.14;
b += const :: string;
b (']');
}

b ('}');
`` `

### Static assembly
`` `CPP
uint8_t bson [] = {{
Bson_cont ('{'),
Bson_key ("str", 3),
Bson_str ("Hello", 5),

Bson_key ("int", 3),
Bson_int16 (12345),

Bson_key ("Arr", 3),
Bson_cont ('),
Bson_str ("String", 6),
Bson_code (12),
Bson_int8 (123),
Bson_int8 (-123),
Bson_int16 (12345),
Bson_int16 (-12345),
Bson_int32 (12345678),
Bson_int32 (-12345678),
// bson_float (3.1415),
BSON_BOOL (True),
Bson_cont (']'),

Bson_cont ('}'),
};
`` `

### unpacking
There is [the finished library] (https://github.com/gyverlibs/bson.js) for JavaScript

> npm i @alexgyver/bson

<a ID = "Versions"> </a>

## versions
- V2.0.0

<a id = "Install"> </a>
## Installation
- The library can be found by the name ** bson ** and installed through the library manager in:
- Arduino ide
- Arduino ide v2
- Platformio
- [download the library] (https://github.com/gyverlibs/bson/archive/refs/heads/main.zip) .Zip archive for manual installation:
- unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
- unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
- unpack and put in *documents/arduino/libraries/ *
- (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries[here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id = "Feedback"> </a>

## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code