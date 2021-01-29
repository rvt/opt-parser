[![Build Status](https://api.travis-ci.org/rvt/opt-parser.svg?branch=master)](https://www.travis-ci.org/rvt/opt-parser)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


# Small library to parse options for embedded systems like Arduino

This library provides a small class to parse option strings and allows you to get these into variables.
Options strings can be in the form of h=12 b=100.0 etc.
This is usefull if you want to send your device simple messages over MQTT and act on them accordingly instead of using sometimes inconvenient JSON messages.

## Example


```
uint32_t x;
float y;
float z;
OptParser::get("x=12 y=15.5 z=-23", [&x,&y,&z](OptValue f) {
        if (std::strcmp( f.key(), "x" ) == 0) {
            x = f;
        }
        if (std::strcmp( f.key(), "y" ) == 0) {
            y = f;
        }
        if (std::strcmp( f.key(), "z" ) == 0) {
            z = f;
        }
    });
```

More Examples of usage can be found in the ´tests/src/test_optparser.hpp´ directory and should be fairly straight forward.

Notice that this string is modified by being broken into smaller chunks if passed as a char array.

You can also use the stack version that does not corrupt the buffer.

```
    int calls = 0;
    char testCase[]="0.1,100.124,-100.678,-100,200,true";
    OptParser::get<64>(testCase, ',', [&calls](OptValue f) {        
            calls++;
    });
    REQUIRE_THAT((const char*)testCase, Equals("0.1,100.124,-100.678,-100,200,true"));
    REQUIRE(calls == 6);
```

## Explicit return types
Since v2.0.0 we do support implicit return types and the following types are supported:

* char*
* float
* long
* char
* int16_t
* int32_t
* bool

## Idea 1

I want to implement a more convenicent method, something like this:

```OptParser::get("x=12 y=15.5 z=-23", { {"x", &x, intType}, {"y", &y, floatType}, {"z", &z, floatType} });```

But didn´t had time yet..

## License

This code is released under the MIT License.
