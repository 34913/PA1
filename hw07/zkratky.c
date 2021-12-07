#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean true false logic values
#include <string.h>     // string functions
#include <ctype.h>

// using numeric type on all numbers
#define type long long
#define ADD 10

//

typedef struct measures_struct
{
    type alloc;
    type len;
} measures;

typedef struct record_struct
{
    char ch;
    type index;
} record;

typedef struct save_struct
{
    record *arr;
    measures size;
} save;

typedef struct array_struct
{
    type **arr;
    type index;
    measures size;
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
void PrintError ( void );

/**
 * @brief           clears memory of all allocated memory on given pointers
 * 
 * @param wanted    save struct, wanted char on start
 */
void ClearAll   ( save wanted, save str );

/**
 * @brief           Extends given pointer when needed based on size data
 * 
 * @param ptr       ptr to be expanded
 * @param so        sizeof object
 * @param size      measures struct, given sizes
 * @return void*    returns the same or extended (when needed) ptr, NULL on failure
 */
void *Extend    ( void *ptr, unsigned long so, measures *size );

/**
 * @brief           Comparing function for qsort, sorts by numeric index
 * 
 * @param r1        record one
 * @param r2        record two
 * @return int      -1 on record one index smaller, otherwise 1
 */
int CmpByNum    ( const void *r1, const void *r2 );

/**
 * @brief           Comparing function for qsort, sorts by char ch
 * 
 * @param r1        record one
 * @param r2        record two
 * @return int      -1 on record one ch smaller, 0 on even, otherwise 1
 */
int CmpByAlpha  ( const void *r1, const void *r2 );

//

/**
 * @brief           Initializes work, reading from input
 * 
 * @param rec       save struct to save read string in array
 * @return true     on failure
 * @return false    on success
 */
bool Init( save *rec )
{
    rec->arr        = NULL;
    rec->size.alloc = 0;
    rec->size.len   = 0;

    type count;
    char ch;
    while( ( ch = getchar() ) != '\n' && ch != EOF ) {
        if( ch == '"' )
            break;
        rec->arr = ( record* )Extend( (void*)rec->arr, sizeof( record ), &rec->size );
        if( rec->arr == NULL )
            return EXIT_FAILURE;

        rec->arr[ rec->size.len - 1 ].ch = ch;
        rec->arr[ rec->size.len - 1 ].index = rec->size.len - 1;
    }
    if ( ch == '\n' || ch == EOF )
        return EXIT_FAILURE;
    else if( getchar() != '\n' )
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

//

int main( void )
{
    save wanted, str;
    save alpha;

    //

    printf( "Zkratka:\n" );

    if( Init( &wanted ) ) {
        PrintError();
        ClearAll( wanted, str );
        return EXIT_FAILURE;
    }   

    alpha.arr = ( record* )malloc( sizeof( record ) * wanted.size.len );
    if( alpha.arr == NULL ) {
        PrintError();
        ClearAll( wanted, str );
        return EXIT_FAILURE;
    }
    alpha.size.len = wanted.size.len;
    alpha.size.alloc = alpha.size.len;

    for( type i = 0; i < wanted.size.len; i++ ) {
        wanted.arr[ i ].ch = tolower( wanted.arr[ i ].ch );
        alpha.arr[ i ].ch = wanted.arr[ i ].ch;
    }
    qsort( alpha.arr, alpha.size.len, sizeof( record ), CmpByAlpha );

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

        if( getchar() != '"' ) {
            PrintError();
            ClearAll( wanted, str );
            return EXIT_FAILURE;
        }

        if( Init( &str ) ) {
            PrintError();
            ClearAll( wanted, str );
            return EXIT_FAILURE;
        }

        for( type i = 0; i < str.size.len; i++ )
            str.arr[ i ].ch = tolower( str.arr[ i ].ch );

        // possible outcomes
        possible pos;
        pos.arrIndex = 1;
        
        pos.occ.arr         = NULL;
        pos.occ.index       = 0;
        pos.occ.size.alloc  = 0;
        pos.occ.size.len    = 0;

        pos.skip            = NULL;
        pos.which           = 0;

        pos.occ.arr = ( type* )malloc( sizeof( type ) * wanted.size.len );
        if( pos.occ.arr == NULL ) {
            PrintError();
            ClearAll( wanted, str );
            return EXIT_FAILURE;
        }


        free( str.arr );
        free( pos.occ.arr );
        free( pos.skip );
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

void ClearAll( save wanted, save str )
{
    free( wanted.arr );
    free( str.arr    );
}

void *Extend( void *ptr, unsigned long so, measures *size )
{
    if( size->alloc == size->len ) {
        size->alloc += 10;
        void *help = realloc( ptr, so * size->alloc );

        if( help == NULL )
            free( ptr );
        return help;
    }
    return ptr;
}

int CmpByNum( const void *r1, const void *r2 )
{
    record *record1 = *( record** )r1;
    record *record2 = *( record** )r2;

    if( record1->index < record2->index )
        return -1;
    return 1;
}

int CmpByAlpha( const void *r1, const void *r2 )
{
    record *record1 = *( record** )r1;
    record *record2 = *( record** )r2;

    if( record1->ch < record2->ch )
        return -1;
    else if( record1->ch > record2->ch )
        return 1;
    return 0;
}

bool Eliminate( save wanted, save *str )
{
    qsort( str->arr, str->size.len, sizeof( record ), CmpByAlpha );

    //



    //
    
}

