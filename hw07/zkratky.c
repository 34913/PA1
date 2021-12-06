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

typedef struct array_struct
{
    type **arr;
    type index;
    type len;
    type allocated;
} array;

typedef struct possible_struct
{
    type arrIndex;

    array occ;

    type *skip;

    type which;

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

/**
 * @brief           Extends arr in given array structure by specific size given by macro ADD
 * 
 * @param occ       array structure to extend by realloc
 */
void Extend( array *occ, type len );

//

/**
 * @brief 
 * 
 * @param rec 
 * @return true 
 * @return false 
 */
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
        possible pos;
        pos.arrIndex = 1;
        
        pos.occ.arr         = NULL;
        pos.occ.index       = 0;
        pos.occ.len         = 0;
        pos.occ.allocated   = 0;

        pos.skip            = NULL;
        pos.which           = 0;

        pos.occ.arr = ( type* )malloc( sizeof( type ) * ( str.len - 2 ) );
        for( type i = 0; i < str.len - 2; i++ )
            pos.occ.arr[ i ] = -1;

        


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

void Extend( array *occ, type len )
{
    if( occ->allocated == occ->len ) {
        occ->allocated += 10;
        void *help = realloc( occ->arr, sizeof( type ) * occ->allocated );

        if( help == NULL )
            return EXIT_FAILURE;
        occ->arr = ( type* )help;
    }
}
