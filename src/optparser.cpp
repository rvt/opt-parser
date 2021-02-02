/**
* https://github.com/rvt/opt-parser
* Copyright (c) 2018 R. van Twisk
*/

#include "optparser.hpp"
#include <cstdlib>

OptValue::OptValue(const uint16_t p_pos, const char* p_key, const char* p_value) :
    m_pos{p_pos}, m_key{p_key}, m_value{p_value} {
}

OptValue::OptValue(const OptValue &other) :
   m_pos{other.m_pos}, m_key{other.m_key}, m_value{other.m_value} {  
}

const char* OptValue::key() const {
    return m_key;
}
bool OptValue::isKey(const char *other) const {
    return strcmp(m_key, other) == 0;
}

OptValue::operator char* () const {
    return (char*)m_value;
}

OptValue::operator int16_t() const {
    return atoi(m_value);
}

OptValue::operator int8_t() const {
    return atoi(m_value);
}

OptValue::operator int32_t() const {
    return atol(m_value);
}

OptValue::operator bool() const {
    return atoi(m_value) || strcmp((char*)m_value, "true") == 0;
}

OptValue::operator float() const {
    return atof(m_value);
}

uint16_t OptValue::pos() const {
    return m_pos;
}

void OptParser::get(char* p_options, OptParserFunction f) {
    get(p_options, ' ', f);
}

void OptParser::get(char* p_options, char m_sep, OptParserFunction callBack) {
    if (p_options == nullptr) {
        return;
    }

    char* workMem = p_options; //strdup(p_options);
    char* work = workMem;
    uint16_t pos = 0;

    char* keyValueToken;

    do {

        // Find m_sep
        keyValueToken = findToken(work, m_sep);
        char* key = work;
        char* value;

        if (keyValueToken != nullptr) {
            if (keyValueToken[1] == '=') {
                keyValueToken++;
                keyValueToken = charToSkip(keyValueToken, m_sep);
                keyValueToken = findToken(keyValueToken, m_sep);
            }

            if (keyValueToken[-1] == '=' && keyValueToken[-2] != '\\') {
                keyValueToken++;
                keyValueToken = charToSkip(keyValueToken, m_sep);
                keyValueToken = findToken(keyValueToken, m_sep);
            }

            keyValueToken[0] = '\0';
            work = keyValueToken + 1;
        } else {
            work = work + strlen(work);
        }

        // See if we can split the string
        value = findToken(key, '=');

        if (value != nullptr) {
            value[0] = '\0';
            value++;
        } else {
            value = key;
        }

        callBack(OptValue(pos++, trimwhitespace(key), deEscape(trimwhitespace(value))));
    } while (work[0] != '\0');

    //free(workMem);
}

char* OptParser::findToken(char* str, const char token) {
    if (str == nullptr) {
        return nullptr;
    }

    char* pos = str;

    while ((pos = strchr(pos, token)) != nullptr) {

        // If the token is escaped we continue
        if (pos[-1] == '\\') {
            pos++;
            continue;
        }

        // find the last position of the token we are looking for
        char* lastTokenPos = charToSkip(pos, token);

        // If more tokens where found, we go back to the last one and return
        if (lastTokenPos > pos) {
            lastTokenPos--;
        }

        return lastTokenPos;
    }

    return nullptr;
}

/**
 * Skip´s until we find a char that is not charToSkip
 * Pointer will be at the char that did not match
 */
char* OptParser::charToSkip(char* str, const char charToSkip) {
    if (str == nullptr) {
        return nullptr;
    }
    while (str[0] == charToSkip)  {
        str++;
    };

    return str;
}

char* OptParser::trimwhitespace(char* str) {
    if (str == nullptr) {
        return nullptr;
    }

    // Trim leading space
    if (isspace((unsigned char)*str)) {
        char *startNonWhiteSpace = str;
        do  {
            startNonWhiteSpace++;
        } while (isspace((unsigned char)*startNonWhiteSpace));
        auto strLen = strlen(startNonWhiteSpace);
        strncpy(str, startNonWhiteSpace, strlen(startNonWhiteSpace));
        str[strLen] = 0;
    }

    // Trim trailing space
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';
    return str;
}

char* OptParser::deEscape(char* str) {
    if (str == nullptr) {
        return nullptr;
    }

    char* work = str;

    while ((work = strchr(work, '\\')) != nullptr) {
        strcpy(work, work + 1);
    }

    return str;
}
