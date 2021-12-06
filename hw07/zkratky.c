#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean true false logic values
#include <string.h>     // string functions
#include <ctype.h>

// using numeric type on all numbers
#define type long long
#define ADD 10

//

typedef struct record_struct
{
    char* str;
    type len;
} record;

typedef struct
{
    type *index;    
} possible;

enum keys
{
    all = '?',
    counts = '#',
    def = ' ',
};

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

    for( type i = 0; i < wanted.len; i++ )
        wanted.str[ i ] = tolower( wanted.str[ i ] );

    printf( "Problemy:\n" );

    int check;
    char key;
    type occurs;
    while( ( check = scanf( "%c %d ", &key, &occurs ) ) == 2 ) {

        if( ( key != all && key != counts ) || occurs <= 0 ) {
            PrintError();
            ClearAll( wanted, str );
            return EXIT_FAILURE;
        }

        if( Init( &str ) ) {
            PrintError();
            ClearAll( wanted, str );
            return EXIT_FAILURE;
        }   

        if( str.str[ 0 ] != '"' || str.str[ str.len - 1 ] != '"' ) {
            PrintError();
            ClearAll( wanted, str );
            return EXIT_FAILURE;
        }

        for( type i = 0; i < str.len; i++ )
            str.str[ i ] = tolower( str.str[ i ] );

        // possible outcomes
        

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

/*
void MoveRecord( record *to, record *tmp )
{
    to->str = ( char* )malloc( sizeof( char ) * ( count + 1 ) );
    if( wanted.str == NULL ) {
        PrintError();
        ClearAll( wanted );
        return EXIT_FAILURE;
    }
    strcpy( wanted.str, tmp.str );
    wanted.len = count;
}
*/