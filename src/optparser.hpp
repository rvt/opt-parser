/**
* https://github.com/rvt/opt-parser
* Copyright (c) 2018 R. van Twisk
*/
#pragma once

#include <cstdint>
#include <functional>
#include <cstring>

class OptValue {
private:
    const uint16_t m_pos;
    const char* m_key;
    const char* m_value;
public:
    OptValue(const uint16_t m_pos, const char* p_key, const char* p_value);
    OptValue(const OptValue &other);
    const char* key() const;
    bool isKey(const char* other) const;
    operator char* () const;
    operator float() const;
    operator long() const;
    operator char() const;
    operator int8_t() const;
    operator int16_t() const;
    operator int32_t() const;
    operator bool() const;
    uint16_t pos() const;
};

/**
 * Simple parser to parse a string into key/value pairs
 * example: abc=1,2,3     w = 400.123 xyz=abc
 * will call back with key of abc, w and xyz with it´s values
 * Note: Will not handle constituate spaces so keep it down to single space betwene characters
 */
class OptParser {
public:
    typedef std::function<void(const OptValue&)> OptParserFunction;
    /**
     * Start the parser with space as a seperator
     * p_options : Options that needs to be parsed, might get modified!
     * callBack  : callback function for each option found
     */
    static void get(char* p_options, OptParserFunction callBack);
    /**
     * Start the parser with a given seperator
     * p_options : Options that needs to be parsed
     * m_sep     : Seperator to be used
     * callBack  : callback function for each option found
     */
    static void get(char* p_options, char m_sep, OptParserFunction callBack);

    template<std::size_t desiredCapacity>
    static void get(const char* p_options, char m_sep, OptParserFunction callBack) {
        static_assert(desiredCapacity > 0, "Must be > 0");
        char buffer[desiredCapacity];
        size_t sizeOptions = strlen(p_options);
        memcpy(buffer, p_options, desiredCapacity<sizeOptions?desiredCapacity:sizeOptions);
        buffer[desiredCapacity-1] = '\0';
        OptParser::get(buffer, m_sep, callBack);
    }

    static char* trimwhitespace(char* str);
private:
protected:
    static char* findToken(char* str, const char token);
    static char* charToSkip(char* str, const char charToSkip = ' ');
    static char* deEscape(char* str);
};
