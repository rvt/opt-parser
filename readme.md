[![Build Status](https://api.travis-ci.org/rvt/opt-parser.svg?branch=master)](https://www.travis-ci.org/rvt/opt-parser)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


# Small library to parse options for embedded systems like Arduino

This library provides a small class to parse option strings and allows you to get these into variables.
Options strings can be in the form of h=12 b=100.0 etc.
This is usefull if you want to send your device simple messages over MQTT and act on them accordingly.

## Example


```
float x;
float y;
float z;
OptParser::get("x=12 y=15.5 z=-23", [&x,&y,&z](OptValue f) {
        if (std::strcmp( f.key(), "x" ) == 0) {
            x = f.asInt();
        }
        if (std::strcmp( f.key(), "y" ) == 0) {
            y = f.asFloat();
        }
        if (std::strcmp( f.key(), "z" ) == 0) {
            z = f.asFloat();
        }
    });
```

More Examples of usage can be found in the ´tests/src/test_optparser.hpp´ directory and should be fairly straight forward.

## Idea

I want to implement a more convenicent method, something like this:

```OptParser::get("x=12 y=15.5 z=-23", { {"x", &x, intType}, {"y", &y, floatType}, {"z", &z, floatType} });```

But didn´t had time yet..

## License

This code is released under the MIT License.