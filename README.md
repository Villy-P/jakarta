# Jakarta

Jakarta is a statically-typed programming language in development.
Jakarta focuses on readability and simplicity, while still allowing for complex applications and speed.

## Example code

``` jakarta
import "lib/stdio.jk";

func int main() {
    int f10 = factorial(10);

    print(f10);

    ret 0;
}

func int factorial(int n) {
    if (n == 0) {
        ret 1;
    }
    ret n * factorial(n - 1);
}
```

## Progress

To streamline development, only keywords required for a factorial program to run are being implemented, thus the inclusion of "Semi-Completed".

| Section                     | Status         |
| --------------------------- | -------------- |
| Lexer                       | Completed      |
| Parser                      | Semi-Completed |
| Semantic Analysis           | In Progress    |
| Intermediate Representation | Not Started    |
| Assembler                   | Not Started    |

## Running Jakarta

Jakarta runs best on Linux based systems.

Before running jakarta, ensure you have `make`, `clang`, `ninja`, and `cmake` installed.
Then run:

``` bash
make setup
make run
```

Logs can be found in `build/bin/logs/`, which contain debug logs as well as a text representation of the AST.
