/**
* https://github.com/rvt/opt-parser
* Copyright (c) 2018 R. van Twisk
*/
#pragma once

#include <stdint.h>
#include <functional>

class OptValue {
private:
    const uint8_t m_pos;
    const char* m_key;
    const char* m_value;
public:
    OptValue(const uint8_t m_pos, const char* p_key, const char* p_value);
    const char* key() const;
    int16_t asInt() const;
    int32_t asLong() const;
    float asFloat() const;
    const char* asChar() const;
    uint8_t pos() const;
};

/**
 * Simple parser to parse a string into key/value pairs
 * example: abc=1,2,3     w = 400.123 xyz=abc   
 * will call back with key of abc, w and xyz with it´s values
 * Note: Will not handle constituate spaces so keep it down to single space betwene characters
 */
class OptParser {
public:
    typedef std::function<void(const OptValue&)> TValueFunction;
    /**
     * Start the parser with space as a seperator
     * p_options : Options that needs to be parsed
     * callBack  : callback function for each option found
     */
    static void get(const char* p_options, TValueFunction callBack);
    /**
     * Start the parser with a given seperator
     * p_options : Options that needs to be parsed
     * m_sep     : Seperator to be used
     * callBack  : callback function for each option found
     */
    static void get(const char* p_options, const char* m_sep, TValueFunction callBack);
private:
    // Very simply cleanup routine to remove some whitespace from the string
    // while respecting space between variables
    static bool cleanUp(bool didCleanup, const char m_sep, char* str,  size_t length, size_t pos);
    static char *trimwhitespace(char* str);
    static bool isNext(char *str, char find);
};

