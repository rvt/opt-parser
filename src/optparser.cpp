/**
* https://github.com/rvt/opt-parser
* Copyright (c) 2018 R. van Twisk
*/

#include "optparser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

OptValue::OptValue(const uint8_t p_pos, const char* p_key, const char* p_value) :
    m_pos(p_pos), m_key(p_key), m_value(p_value) {
}

const char* OptValue::key() const {
    return m_key;
}

OptValue::operator char* () const {
    return (char*)m_value;
}

OptValue::operator int16_t() const {
    return atoi(m_value);
}

OptValue::operator int32_t() const {
    return atol(m_value);
}

OptValue::operator float() const {
    return atof(m_value);
}

uint8_t OptValue::pos() const {
    return m_pos;
}

void OptParser::get(const char* p_options, TValueFunction f) {
    get(p_options, ' ', f);
}

void OptParser::get(const char* p_options, char m_sep, TValueFunction callBack) {
    if (p_options == nullptr) {
        return;
    }

    char* workMem = strdup(p_options);
    char* work = workMem;
    uint8_t pos = 0;

    char* keyValueToken;

    do {

        // Find m_sep
        keyValueToken = findToken(work, m_sep);
        char* key = work;
        char* value;
        // Check if the next char is a equals sign, if so we skip that
        keyValueToken = findToken(work, m_sep);

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

    free(workMem);
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

    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == 0) { // All spaces?
        return str;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;

    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // Write new null terminator character
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
