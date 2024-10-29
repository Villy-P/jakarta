#include "symbol.h"

#include <assert.h>

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
    return 0;
}