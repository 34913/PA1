#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean true false logic values

// using numeric type on all numbers
#define type long long

//

typedef struct record_struct
{
    char* str;
    type len;
} record;

//

/**
 * @brief           Prints common error message on stdout
 * 
 */
void PrintError( void );

/**
 * @brief           clears memory of all allocated memory on given pointers
 * 
 * @param wanted    record struct, wanted char on start
 */
void ClearAll( record wanted );

//

int main( void )
{
    record wanted;
    wanted.str      = NULL;
    wanted.len      = 0;

    //

    printf( "Zkratka:\n" );
    type count = 0;

    if( ( count = getline( &wanted.str, &wanted.len, stdin ) ) == -1 ) {
        PrintError();
        ClearAll( wanted );
        return EXIT_FAILURE;
    }
    wanted.str[ count - 1 ] = '\0';
    wanted.len = count - 1;

    printf( "Problemy:\n" );

    int check;
    char key;
    while( ( check = scanf( "%c %d ", &key, &count ) ) == 2 ) {





    }

    //

    if( check != EOF ) {
        PrintError();
        ClearAll( wanted );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//

void PrintError( void )
{
    printf( "Nespravny vstup.\n" );    
}

void ClearAll( record wanted )
{
    
    free( wanted.str );
}
