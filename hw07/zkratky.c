#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean true false logic values
#include <string.h>     // string functions

// using numeric type on all numbers
#define type long long
#define ADD 10

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
void ClearAll( record wanted, record str );

//


bool Init( record *rec )
{
    rec->str      = NULL;
    rec->len      = 0;

    type count;
    if( ( count = getline( &rec->str, &rec->len, stdin ) ) == -1 )
        return EXIT_FAILURE;
    rec->str[ --count ] = '\0';
    
    void *help = realloc( rec->str, sizeof( char ) * ( count + 1 ) );
    if( help == NULL )
        return EXIT_FAILURE;
    rec->str = ( char* )help;
    rec->len = count;

    return EXIT_SUCCESS;
}

//

int main( void )
{
    record wanted, str;

    //

    printf( "Zkratka:\n" );

    if( Init( &wanted ) ) {
        PrintError();
        ClearAll( wanted, str );
        return EXIT_FAILURE;
    }   

    printf( "Problemy:\n" );

    int check;
    char key;
    type occurs;
    while( ( check = scanf( "%c %d ", &key, &occurs ) ) == 2 ) {

        if( Init( &str ) ) {
            PrintError();
            ClearAll( wanted, str );
            return EXIT_FAILURE;
        }   



        free( str.str );
    }

    //

    ClearAll( wanted, str );

    if( check != EOF ) {
        PrintError();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//

void PrintError( void )
{
    printf( "Nespravny vstup.\n" );    
}

void ClearAll( record wanted, record str )
{
    free( wanted.str );
    free( str.str );
}
