#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean true false logic values
#include <string.h>     // string functions
#include <ctype.h>

// using numeric type on all numbers
#define type long long
#define ADD 10

//

typedef struct list_struct
{
    char data;
    struct list_struct *after;
    struct list_struct *before;
} list;

void Add( list *n, list **after )
{
    n->after = *after;
    if( *after != NULL )
        ( *after )->before = n;
    *after = n;
    n->before = NULL;
}

list *Create( char data )
{
    list *n = ( list* )malloc( sizeof( list ) );
    n->after = NULL;
    n->before = NULL;
    n->data = data;
    return n;
}

void Clear( list *l )
{
    list *d;
    list *tmp = l->before;
    while( l ) {
        d = l->after;
        free( l );
        l = d;
    }

    l= tmp;
    while( l ) {
        d = l->before;
        free( l );
        l = d;
    }

    return;
}

void Remove( list *l )
{
    if( l->after != NULL )
        ( l->after )->before = l->before;
    if( l->before != NULL )
        ( l->before )->after = l->after;
    free( l );
}

void Insert( list* where, list* item )
{
    item->before = where->before;
    ( where->before )->after = item;
    where->before = item;
    item->after = where;

    return;
}

//

typedef struct measures_struct
{
    type alloc;
    type len;
} measures;

typedef struct save_struct
{
    list **arr;
    measures size;

    list *start;
} save;

typedef struct possible_struct
{
    // array with all upper letters to be located
    list ***arr;
    measures size;
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
 * @brief           Comparing function for qsort, sorts by char ch
 * 
 * @param r1        record one
 * @param r2        record two
 * @return int      -1 on record one ch smaller, 0 on even, otherwise 1
 */
int CmpByAlpha  ( const void *r1, const void *r2 );

/**
 * @brief           Removes not needed characters from list in str save
 * 
 * @param wanted    wanted save with characters needed
 * @param str       str save with characters from input
 * @return true     on failure
 * @return false    on success
 */
bool Eliminate( save wanted, save *str );

//

/**
 * @brief           Initializes work, reading from input
 * 
 * @param rec       save struct to save read string in array
 * @return true     on failure
 * @return false    on success
 */
bool Init( save *rec, bool quot )
{
    rec->arr        = NULL;
    rec->size.alloc = 0;
    rec->size.len   = 0;

    rec->start      = NULL;

    char ch;
    while( ( ch = getchar() ) != '\n' && ch != EOF ) {
        if( ch == '"' )
            break;
        ch = tolower( ch );

        list *n = Create( ch );
        Add( n, &rec->start );
        rec->arr = ( list** )Extend( (void*)rec->arr, sizeof( list* ), &rec->size );
        if( rec->arr == NULL )
            return EXIT_FAILURE;

        rec->arr[ ++rec->size.len - 1 ] = n;
    }
    if ( ch == EOF )
        return EXIT_FAILURE;
    else if( quot ) {
        if ( ch == '\n' )
            return EXIT_FAILURE;
        else if( getchar() != '\n' )
            return EXIT_FAILURE;
    }

    rec->start = rec->arr[ 0 ];

    qsort( rec->arr, rec->size.len, sizeof( list* ), CmpByAlpha );

    return EXIT_SUCCESS;
}

//

int main( void )
{
    save wanted, str;

    // possible outcomes
    possible pos;
    pos.arr         = NULL;
    pos.size.alloc  = 0;
    pos.size.len    = 0;

    //

    printf( "Zkratka:\n" );

    if( Init( &wanted, false ) ) {
        PrintError();
        ClearAll( wanted, str );
        return EXIT_FAILURE;
    }   
  
    printf( "Problemy:\n" );

    int check;
    char key;
    type occurs;
    while( ( check = scanf( "%c %lld ", &key, &occurs ) ) == 2 ) {

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

        if( Init( &str, true ) ) {
            PrintError();
            ClearAll( wanted, str );
            return EXIT_FAILURE;
        }

        pos.arr = ( list*** )malloc( sizeof( list** ) * wanted.size.len );

        Eliminate( wanted, &str );
        


        free( pos.arr );
        Clear( str.start );
        free( str.arr);
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
    Clear( wanted.start );
    //free( str.arr );
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

int CmpByAlpha( const void *r1, const void *r2 )
{
    list *record1 = *( list** )r1;
    list *record2 = *( list** )r2;

    if( record1->data < record2->data )
        return -1;
    return 1;
}

bool Eliminate( save wanted, save *str )
{
    type wIndex = 0;

    type count = 0;
    type i = 0;
    for( ; i < str->size.len; i++ )
        if( str->arr[ i ]->data != ' ' )
            break;
    for( ; i < str->size.len; i++ ) {
        list *record = str->arr[ i ];

        if( wanted.arr[ wIndex ]->data < record->data ) {
            if( count == 0 ) {
                printf("\nasasd\n");
                return EXIT_FAILURE;
            }

            if( ++wIndex == wanted.size.len )
                break;
            count = 0;
        }

        if( record->data != wanted.arr[ wIndex ]->data ) {
            if( str->start == record )
                str->start = str->start->after;
            Remove( record );
        }
        else
            count++;
    }
    for( ; i < str->size.len; i++ )
        Remove( str->arr[ i ] );

    //

    return true;
}
