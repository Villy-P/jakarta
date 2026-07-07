#include <assert.h>

#include "symbol.h"

int main() {
    assert(is_number_symbol("10"));
    assert(is_number_symbol("100"));
    assert(is_number_symbol("10.10"));
    assert(is_number_symbol("10.120"));
    assert(is_number_symbol("010"));
    assert(is_number_symbol("0b10100"));
    assert(is_number_symbol("0xAF3"));
    assert(is_number_symbol("07"));
    assert(is_number_symbol("10e2"));
    assert(is_number_symbol("10e3432"));
    assert(is_number_symbol(".123123"));
    assert(is_number_symbol("10."));
    assert(is_number_symbol("-10."));
    assert(is_number_symbol("-10"));
    assert(is_number_symbol("-10.1243"));
    assert(is_number_symbol("-010"));
    assert(is_number_symbol("-0b10"));
    assert(is_number_symbol("-0xAA"));
    assert(is_number_symbol("-076"));

    assert(!is_number_symbol("10e."));
    assert(!is_number_symbol("10e131.213"));
    assert(!is_number_symbol("e234"));
    assert(!is_number_symbol("1230.123.123"));
    assert(!is_number_symbol("0b99"));
    assert(!is_number_symbol("0b22"));
    assert(!is_number_symbol("099"));
    assert(!is_number_symbol("100.0.0"));
    assert(!is_number_symbol(".0.0"));
    assert(!is_number_symbol("asdfsadf"));
    return 0;
}