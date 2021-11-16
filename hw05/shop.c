// libraries

#include <stdio.h>      // standard I/O
#include <stdbool.h>    // boolean values
#include <stdlib.h>     // malloc

enum keys {
    add = '+',
    all = '#',
    num = '?',
    def = ' ',
};

void ErrorMessage();

int main(void)
{
    // saying how many of tracked things are going to be displayed
    int tracked = 0;

    if( scanf( "%d", &tracked ) != 1 || tracked <= 0 ) {
        ErrorMessage();
        return EXIT_FAILURE;
    }

    while( true ) {
        char c = def;
        if( scanf( "%c", &c ) != 1 ) {
            ErrorMessage();
            return EXIT_FAILURE;
        }

        switch ( c ) {
            case add:

                break;
            case all:

                break;
            case num:

                break;
            default:
                ErrorMessage();
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

void ErrorMessage() {
    printf( "Nespravny vstup.\n");
}