// libraries

#include <stdio.h>      // standard I/O
#include <stdbool.h>    // boolean values
#include <stdlib.h>     // malloc

#define MAX_LEN 99

enum keys {
    add = '+',
    all = '#',
    num = '?',
    def = ' ',
};

typedef struct record_struct
{
    char **names;
    int nameLen;
    long length;
} record;

typedef struct pointRecord_struct
{
    record *pointer;
    int count;
} pointRecord;

void ErrorMessage();

int main(void)
{
    // saying how many of tracked things are going to be displayed
    int tracked = 0;

    record arr[MAX_LEN];
    pointRecord *sorted;

    for( int i = 0; i < MAX_LEN; i++ ) {
        arr[i].length = 1;
        arr[i].nameLen = i + 1;
        arr[i].names = ( char** )malloc( sizeof( char* ));
        arr[i].names[0] = ( char* )malloc( sizeof( char ) * arr[i].nameLen );
    }

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
                int i;
                char str[MAX_LEN];
                for ( i = 0; i < MAX_LEN; i++ ) {
                    str[i] = getchar();
                    if( str[i] == '\n' ) 
                        break;
                }
                if(!( str[i] == '\n' || getchar() == '\n' )) {
                    ErrorMessage();
                    return EXIT_FAILURE;
                }

                

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

void ErrorMessage()
{
    printf( "Nespravny vstup.\n");
}

void ClearArr( record *arr, pointRecord *sorted )
{
    for( int i = 0; i < MAX_LEN; i++ ) {
        for( int y = 0; y < arr[i].length; y++ ) {
            free( arr[i].names[y] );
        }
        free( arr[i].names );
    }

    free( sorted );
}

bool ExtendArr ( record *arr )
{
    void** help;
    help = realloc( arr->names, sizeof( char * ) * ++arr->length );
    help[arr->length - 1] = malloc( sizeof( char ) * arr->nameLen );

    arr->names = help;

    return true;    
}
