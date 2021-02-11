#include <optparser.hpp>
#include <catch2/catch.hpp>
using Catch::Matchers::Equals;


class TOptParser : OptParser {
public:
    char* findToken(char* str, int token) {
        return OptParser::findToken(str, token);
    }
};

TEST_CASE("Should parse empty strings", "[optparser]") {
    int calls = 0;
    OptParser::get<32>("first,second,", ',', [&calls](OptValue f) {

        switch (f.pos()) {
            case 0:
                calls++;
                REQUIRE_THAT((const char*)f, Equals("first"));
                break;

            case 1:
                REQUIRE_THAT((const char*)f, Equals("second"));
                calls++;
                break;

            case 2:
                REQUIRE_THAT((const char*)f, Equals(""));
                calls++;
                break;

            default:
                calls++;
        }

    });
    REQUIRE(calls == 3);
}

TEST_CASE("Should not parse single escaped token", "[optparser]") {
    int calls = 0;
    OptParser::get<12>("\\,", ',', [&calls](OptValue f) {

        switch (f.pos()) {
            case 0:
                calls++;
                REQUIRE_THAT((const char*)f, Equals(","));
                break;

            default:
                calls++;
        }

    });
    REQUIRE(calls == 1);
}

TEST_CASE("Should get floats and ints", "[optparser]") {
    int calls = 0;
    char testCase[] = "0.1,100.124,-100.678,-100,200,true";
    OptParser::get(testCase, ',', [&calls](OptValue f) {
        switch (f.pos()) {
            case 0:
                REQUIRE((float)f == Approx(0.1));
                calls++;
                break;

            case 1:
                REQUIRE((float)f == Approx(100.124));
                calls++;
                break;

            case 2:
                REQUIRE((float)f == Approx(-100.678));
                calls++;
                break;

            case 3:
                REQUIRE((int32_t)f == -100);
                calls++;
                break;

            case 4:
                REQUIRE((int)f == 200);
                calls++;
                break;

            case 5:
                REQUIRE((bool)f == true);
                calls++;
                break;

            default:
                calls++;
        }
    });
    REQUIRE(calls == 6);
}

TEST_CASE("Should get ints from floats", "[optparser]") {
    int calls = 0;
    char testCase[] = "abc ,  567.456,  -3567.8521  ";
    OptParser::get(testCase,  ',', [&calls](OptValue f) {
        switch (f.pos()) {
            case 1:
                REQUIRE((int32_t)f == 567);
                calls++;
                break;

            case 2:
                REQUIRE((int32_t)f == -3567);
                calls++;
                break;

            default:
                calls++;
        }
    });
    REQUIRE(calls == 3);
}

TEST_CASE("Should call with one item", "[optparser]") {
    int calls = 0;
    char testCase[] = "oneitem";
    OptParser::get(testCase,  ',', [&calls](OptValue f) {
        REQUIRE_THAT((const char*)f, Equals("oneitem"));
        calls++;
    });
    REQUIRE(calls == 1);
}

TEST_CASE("Should call once when empty", "[optparser]") {
    int calls = 0;
    char testCase[] = "";
    OptParser::get(testCase,  ',', [&calls](OptValue f) {
        calls++;
    });
    REQUIRE(calls == 1);
}

TEST_CASE("Should handle variable names with spaces", "[optparser]") {
    int calls = 0;
    char testCase[] = "abc=1,2,3     w =400.123 xyz= abc  w2 = 12.0  ";
    OptParser::get(testCase, [&calls](OptValue f) {

        switch (f.pos()) {
            case 0:
                REQUIRE_THAT(f.key(), Equals("abc"));
                REQUIRE_THAT((const char*)f, Equals("1,2,3"));
                calls += 1;

                break;

            case 1:
                REQUIRE_THAT(f.key(), Equals("w"));
                REQUIRE((float)f == Approx(400.123));
                calls += 2;
                break;

            case 2:
                REQUIRE_THAT(f.key(), Equals("xyz"));
                REQUIRE_THAT((const char*)f, Equals("abc"));
                calls += 4;
                break;

            case 3:
                REQUIRE_THAT(f.key(), Equals("w2"));
                REQUIRE((float)f == Approx(12.0));
                calls += 8;
                break;

            default:
                calls++;
        }

    });
    REQUIRE(calls == 15);
}

TEST_CASE("Should handle variabel strings with spaces", "[optparser]") {
    int calls = 0;
    char testCase[] = "str1= Hello \\, there ,str2 = Some other = string \\=    ";
    OptParser::get(testCase, ',', [&calls](OptValue f) {

        switch (f.pos()) {
            case 0:
                REQUIRE_THAT(f.key(), Equals("str1"));
                REQUIRE_THAT((const char*)f, Equals("Hello , there"));
                calls++;
                break;

            case 1:
                REQUIRE_THAT(f.key(), Equals("str2"));
                REQUIRE_THAT((const char*)f, Equals("Some other = string ="));
                calls++;
                break;

            default:
                calls++;
        }

    });
    REQUIRE(calls == 2);
}

TEST_CASE("Should handle newlines", "[optparser]") {
    int calls = 0;
    char testCase[] = "str1=foo;\n\n\n\nstr2\n\n=\n\nbar";
    OptParser::get(testCase, ';', [&calls](OptValue f) {


        switch (f.pos()) {
            case 0:
                REQUIRE_THAT(f.key(), Equals("str1"));
                REQUIRE_THAT((const char*)f, Equals("foo"));
                calls++;

                break;

            case 1:
                REQUIRE_THAT(f.key(), Equals("str2"));
                REQUIRE_THAT((const char*)f, Equals("bar"));
                calls++;
                break;

            default:
                calls++;
        }

    });
    REQUIRE(calls == 2);
}


TEST_CASE("Stack version should not corrupt buffer", "[optparser]") {
    int calls = 0;
    const char testCase[] = "0.1,100.124,-100.678,-100,200,true";
    OptParser::get<64>(testCase, ',', [&calls](OptValue f) {
        calls++;
    });
    REQUIRE_THAT((const char*)testCase, Equals("0.1,100.124,-100.678,-100,200,true"));
    REQUIRE(calls == 6);
}

TEST_CASE("Stack version should not crash if to small", "[optparser]") {
    int calls = 0;
    const char testCase[] = "0.1,100.124,-100.678,-100,200,true";
    OptParser::get<12>(testCase, ',', [&calls](OptValue f) {
        calls++;
    });
    REQUIRE_THAT((const char*)testCase, Equals("0.1,100.124,-100.678,-100,200,true"));
    REQUIRE(calls == 2);
}

