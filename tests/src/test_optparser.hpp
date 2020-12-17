#include <optparser.h>
#include <catch2/catch.hpp>
using Catch::Matchers::Equals;


class TOptParser : OptParser {
public:
    char* findToken(char* str, int token) {
        return OptParser::findToken(str, token);
    }
};

TEST_CASE("Should get floats and ints", "[optparser]") {
    int calls = 0;
    char testCase[]="0.1,100.124,-100.678,-100,200";
    OptParser::get(testCase, ',', [&calls](OptValue f) {
        if (f.pos() == 0) {
            REQUIRE((float)f == Approx(0.1));
            calls++;
        }

        if (f.pos() == 1) {
            REQUIRE((float)f == Approx(100.124));
            calls++;
        }

        if (f.pos() == 2) {
            REQUIRE((float)f == Approx(-100.678));
            calls++;
        }

        if (f.pos() == 3) {
            REQUIRE((int32_t)f == -100);
            calls++;
        }

        if (f.pos() == 4) {
            REQUIRE((int)f == 200);
            calls++;
        }
    });
    REQUIRE(calls == 5);
}

TEST_CASE("Should get ints from floats", "[optparser]") {
    int calls = 0;
    char testCase[]="abc ,  567.456,  -3567.8521  ";
    OptParser::get(testCase,  ',', [&calls](OptValue f) {
        if (f.pos() == 1) {
            REQUIRE((int32_t)f == 567);
            calls++;
        }

        if (f.pos() == 2) {
            REQUIRE((int32_t)f == -3567);
            calls++;
        }
    });
    REQUIRE(calls == 2);
}

TEST_CASE("Should call with one item", "[optparser]") {
    int calls = 0;
    char testCase[]="oneitem";
    OptParser::get(testCase,  ',', [&calls](OptValue f) {
        REQUIRE_THAT((const char*)f, Equals("oneitem"));
        calls++;
    });
    REQUIRE(calls == 1);
}

TEST_CASE("Should not call when empty", "[optparser]") {
    int calls = 0;
    char testCase[]="oneitem";
    OptParser::get(testCase,  ',', [&calls](OptValue f) {
        calls++;
    });
    REQUIRE(calls == 1);
}

TEST_CASE("Should handle variable names with spaces", "[optparser]") {
    int calls = 0;
    char testCase[]="abc=1,2,3     w =400.123 xyz= abc  w2 = 12.0  ";
    OptParser::get(testCase, [&calls](OptValue f) {
        if (f.pos() == 0) {
            REQUIRE_THAT(f.key(), Equals("abc"));
            REQUIRE_THAT((const char*)f, Equals("1,2,3"));
            calls += 1;
        }

        if (f.pos() == 1) {
            REQUIRE_THAT(f.key(), Equals("w"));
            REQUIRE((float)f == Approx(400.123));
            calls += 2;
        }

        if (f.pos() == 2) {
            REQUIRE_THAT(f.key(), Equals("xyz"));
            REQUIRE_THAT((const char*)f, Equals("abc"));
            calls += 4;
        }

        if (f.pos() == 3) {
            REQUIRE_THAT(f.key(), Equals("w2"));
            REQUIRE((float)f == Approx(12.0));
            calls += 8;
        }
    });
    REQUIRE(calls == 15);
}

TEST_CASE("Should handle variabel strings with spaces", "[optparser]") {
    int calls = 0;
    char testCase[]="str1= Hello \\, there ,str2 = Some other = string \\=    ";
    OptParser::get(testCase, ',', [&calls](OptValue f) {
        if (f.pos() == 0) {
            REQUIRE_THAT(f.key(), Equals("str1"));
            REQUIRE_THAT((const char*)f, Equals("Hello , there"));
            calls++;
        }

        if (f.pos() == 1) {
            REQUIRE_THAT(f.key(), Equals("str2"));
            REQUIRE_THAT((const char*)f, Equals("Some other = string ="));
            calls++;
        }

    });
    REQUIRE(calls == 2);
}
