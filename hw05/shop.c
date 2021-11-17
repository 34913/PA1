// libraries

#include <stdio.h>      // standard I/O
#include <stdbool.h>    // boolean values
#include <stdlib.h>     // malloc
#include <string.h>

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
    int *counts;
    int nameLen;
    long length;
} record;

typedef struct pointRecord_struct
{
    record **pointers;
    int count;
} pointRecord;

void ErrorMessage();
void ClearArr( record *arr, pointRecord *sorted );
bool ExtendArr ( record *arr, pointRecord *sorted );

//

int main(void)
{
    // saying how many of tracked things are going to be displayed
    int tracked = 0;

    record arr[MAX_LEN];
    pointRecord sorted;
    sorted.count = 0;

    for( int i = 0; i < MAX_LEN; i++ ) {
        arr[i].length = 0;
        arr[i].nameLen = i + 1;
        
        arr[i].counts = NULL;
        arr[i].names = NULL;
    }

    if( scanf( "%d", &tracked ) != 1 || tracked <= 0 ) {
        ErrorMessage();
        ClearArr( arr, &sorted );
        return EXIT_FAILURE;
    }

    while( true ) {
        char c = def;
        if( scanf( "%c", &c ) != 1 ) {
            ErrorMessage();
            ClearArr( arr, &sorted );
            return EXIT_FAILURE;
        }

        switch ( c ) {
            case add:
                int strLen;
                char str[ MAX_LEN ];
                for ( strLen = 0; strLen < MAX_LEN; strLen++ ) {
                    str[ strLen ] = getchar();
                    if( str[ strLen ] == '\n' )
                        break;
                }

                if( strLen == MAX_LEN ) {
                    if( getchar() != '\n' ) {
                        ErrorMessage();
                        ClearArr( arr, &sorted );
                        return EXIT_FAILURE;
                    }
                }

                bool exist = false;
                record *r = &arr[ strLen - 1 ];

                for( int i = 0; i < r->length; i++ ) {
                    if( strcmp( str, r->names[ i ] ) == 0 ) {
                        exist = true;
                        r->counts++;
                        break;
                    }
                }

                if( !exist ) {
                    ExtendArr( r, &sorted );
                    strncpy( &r->names[ r->length - 1 ], &str, strLen);
                    r->counts[ r->length - 1 ] = 1;

                    sorted.pointers[ sorted.count - 1 ] = r->names[ r->length - 1 ];
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

    free( sorted->pointers );
}

bool ExtendArr ( record *arr, pointRecord *sorted )
{
    void** help;
    if( arr->length++ == 0 )
        help = malloc( sizeof( char* ) );
    else
        help = realloc( arr->names, sizeof( char * ) * arr->length );
    
    help[arr->length - 1] = malloc( sizeof( char ) * arr->nameLen );
    arr->names = (char **) help;

    if( sorted->count++ == 0 )
        help = malloc( sizeof( record* ) );
    else
        help = realloc( sorted->pointers, sizeof( record *) * sorted->count );
    sorted->pointers = (record **) help;

    return true;
}
