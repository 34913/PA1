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

    l = tmp;
    while( l ) {
        d = l->before;
        free( l );
        l = d;
    }

    return;
}

void RemoveDontFree( list *l ) 
{
    if( l->after != NULL )
        ( l->after )->before = l->before;
    if( l->before != NULL )
        ( l->before )->after = l->after;
}

void Remove( list *l )
{
    RemoveDontFree( l );
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
    list **sorted;

    measures size;

    list *start;
    list *need;
} save;

typedef struct possible_struct
{
    list** arr;
    type index;

    list *ptr;
    list *searching;
} possible;

typedef struct record_struct
{
    type index;
    type max;
    type occurs;

    possible **arr;
    measures size;

    possible **comp;
    measures compSize;
} record;

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
 * @param str       save struct, str saved on cycle
 */
void ClearAll   ( save wanted, save str, record rec );

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

/**
 * @brief           Function to find and save all possible outcomes, 
 *                  not really a recursion
 * 
 * @param pos       position pointer
 * @param rec       record pointer
 * @return true     on failure
 * @return false    on success
 */
bool Recursion( record *rec );

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
    rec->sorted     = NULL;
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

        measures *size = &rec->size;
        if( size->alloc == size->len ) {
            size->alloc += 10;
            unsigned long so = sizeof( list* );

            void *help1 = realloc( rec->arr, so * size->alloc );
            void *help2 = realloc( rec->sorted, so * size->alloc );
            
            if( help1 == NULL || help2 == NULL )
                return EXIT_FAILURE;

            rec->arr = ( list**) help1;
            rec->sorted = ( list** )help2;
        }

        rec->sorted[ size->len ] = n;
        rec->arr[ size->len ] = n;
        size->len ++; 
    }
    if ( ch == EOF )
        return EXIT_FAILURE;
    else if( quot ) {
        if ( ch == '\n' )
            return EXIT_FAILURE;
        else if( getchar() != '\n' )
            return EXIT_FAILURE;
    }

    rec->start = rec->sorted[ 0 ];

    qsort( rec->sorted, rec->size.len, sizeof( list* ), CmpByAlpha );

    return EXIT_SUCCESS;
}

/**
 * @brief           Prints out all possible outcomes on the end
 * 
 * @param rec       record with all possible outcomes
 * @param str       save str, with originaly loaded string
 * @param key       key from input, determines which output user wants
 */
void PrintOut( record *rec, save *str, char key )
{
    if( rec->compSize.len > 0 && key == all ) {
        for( type i = 0; i < rec->max; i++ )
            rec->comp[ 0 ]->arr[ i ]->data
                = toupper( rec->comp[ 0 ]->arr[ i ]->data );
        for( type y = 0; y < str->size.len; y++ )
            printf( "%c", str->arr[ y ]->data );
        printf( "\n" );
        
        for( type i = 1; i < rec->compSize.len; i++ ) {
            for( type y = 0; y < rec->max; y++ )
                rec->comp[ i - 1 ]->arr[ y ]->data
                    = tolower( rec->comp[ i - 1 ]->arr[ y ]->data );
            for( type y = 0; y < rec->max; y++ )
                rec->comp[ i ]->arr[ y ]->data
                    = toupper( rec->comp[ i ]->arr[ y ]->data );

            for( type y = 0; y < str->size.len; y++ )
                printf( "%c", str->arr[ y ]->data );
            printf( "\n" );
        }
    }

    printf( "%lld\n", rec->compSize.len );
}

//

int main( void )
{
    save wanted, str;
    str.sorted      = NULL;
    str.arr         = NULL;
    str.start       = NULL;

    record rec;
    rec.arr         = NULL;
    rec.comp        = NULL;

    //

    printf( "Zkratka:\n" );

    if( Init( &wanted, false ) ) {
        PrintError();
        ClearAll( wanted, str, rec );
        return EXIT_FAILURE;
    }

    printf( "Problemy:\n" );

    int check;
    char key;
    type occurs;
    while( ( check = scanf( "%c %lld ", &key, &occurs ) ) == 2 ) {

        if( ( key != all && key != counts ) || occurs <= 0 ) {
            PrintError();
            ClearAll( wanted, str, rec );
            return EXIT_FAILURE;
        }

        if( getchar() != '"' ) {
            PrintError();
            ClearAll( wanted, str, rec );
            return EXIT_FAILURE;
        }

        if( Init( &str, true ) ) {
            PrintError();
            ClearAll( wanted, str, rec );
            return EXIT_FAILURE;
        }

        Eliminate( wanted, &str );

        //

        possible *pos       = ( possible* )malloc( sizeof( possible ) );
        if( pos == NULL ) {
            ClearAll( wanted, str, rec );
            PrintError();
            return EXIT_FAILURE;
        }
        pos->arr            = ( list** )malloc( sizeof( list* ) * wanted.size.len );
        if( pos->arr == NULL ) {
            ClearAll( wanted, str, rec );
            PrintError();
            return EXIT_FAILURE;
        }
        pos->index          = 0;
        pos->ptr            = str.arr[ 0 ];
        pos->searching      = wanted.start;

        rec.size.alloc      = 1;
        rec.size.len        = 1;
        rec.arr             = ( possible** )malloc( sizeof( possible* ) );
        if( rec.arr == NULL ) {
            ClearAll( wanted, str, rec );
            PrintError();
            return EXIT_FAILURE;
        }
        rec.arr[ 0 ]        = pos;
        rec.compSize.alloc  = 0;
        rec.compSize.len    = 0;
        rec.comp            = NULL;
        rec.max             = wanted.size.len;
        rec.index           = 0;
        rec.occurs          = occurs;

        //

        while( rec.index != rec.size.len ) {
            if( Recursion( &rec ) ) {
                PrintError();
                ClearAll( wanted, str, rec );
                return EXIT_FAILURE;
            }
        }

        //



        //

        free( str.sorted );
        for( type i = 0; i < str.size.len; i++ )
            free( str.arr[ i ] );
        free( str.arr );

        for( type i = 0; i < rec.size.len; i++ ) {
            free( rec.arr[ i ]->arr );
            free( rec.arr[ i ] );
        }
        free( rec.arr );

        rec.arr         = NULL;
        str.sorted      = NULL;
        str.arr         = NULL;
    }

    //

    ClearAll( wanted, str, rec );

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

void ClearAll( save wanted, save str, record rec )
{
    free( wanted.sorted );
    free( wanted.arr );
    Clear( wanted.start );

    free( str.sorted );
    if( str.arr != NULL ) {
        for( type i = 0; i < str.size.len; i++ )
            free( str.arr[ i ] );
    }
    free( str.arr );

    if( rec.arr != NULL ) {
        for( type i = 0; i < rec.size.len; i++ ) {
            free( rec.arr[ i ]->arr );
            free( rec.arr[ i ] );
        }
    }
    free( rec.arr );
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
    list* l1 = *( list** )r1;
    list* l2 = *( list** )r2;

    if( l1->data < l2->data )
        return -1;
    return 1;
}

bool Eliminate( save wanted, save *str )
{
    type wIndex = 0;

    type count = 0;
    type i = 0;
    for( ; i < str->size.len; i++ )
        if( str->sorted[ i ]->data != ' ' )
            break;
    for( ; i < str->size.len; i++ ) {
        list *record = str->sorted[ i ];

        if( wanted.sorted[ wIndex ]->data < record->data ) {
            if( count == 0 )
                return EXIT_FAILURE;

            if( ++wIndex == wanted.size.len )
                break;
            count = 0;
        }

        if( record->data != wanted.sorted[ wIndex ]->data ) {
            if( str->start == record )
                str->start = str->start->after;
            RemoveDontFree( record );
        }
        else {
            count++;
        }
    }
    for( ; i < str->size.len; i++ )
        RemoveDontFree( str->sorted[ i ] );

    //

    return true;
}

bool Recursion( record *rec )
{
    possible *pos = rec->arr[ rec->index ];
    
    if( pos->searching == NULL ) {
        rec->comp = ( possible** )Extend( rec->comp, sizeof( possible* ), &rec->compSize );
        if( rec->comp == NULL )
            return EXIT_FAILURE;
        rec->comp[ rec->compSize.len ++ ] = pos;
        rec->index ++;
        return EXIT_SUCCESS;
    }
    if( pos->ptr == NULL ) {
        rec->index ++;
        return EXIT_SUCCESS;
    }

    if( pos->ptr->data == ' ' ) {
        pos->ptr = pos->ptr->before;
        return EXIT_SUCCESS;
    }

    if( pos->searching->data == pos->ptr->data ) {
        possible *newPos = ( possible* )malloc( sizeof( possible ) );
        if( newPos == NULL )
            return EXIT_FAILURE;

        newPos->arr = ( list** )malloc( sizeof( list* ) * rec->max );
        if( newPos->arr == NULL )
            return EXIT_FAILURE;

        for( type i = 0; i < pos->index; i++ )
            newPos->arr[ i ] = pos->arr[ i ];
        newPos->arr[ pos->index ] = pos->ptr;

        newPos->ptr = pos->ptr->before;
        newPos->searching = pos->searching->before;
        newPos->index = pos->index + 1;

        rec->arr = ( possible** )Extend( rec->arr, sizeof( possible* ), &rec->size );
        if( rec->arr == NULL )
            return EXIT_FAILURE;

        rec->arr[ rec->size.len ++ ] = newPos;
    }
    pos->ptr = pos->ptr->before;

    return EXIT_SUCCESS;
}
