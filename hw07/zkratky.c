#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean true false logic values
#include <string.h>     // string functions
#include <ctype.h>      // isalpha, isupper, islower

// using numeric type on all numbers
#define type long long
#define ADD 10
#define DELIMETER ' '

//

typedef struct list_struct
{
    char data;
    struct list_struct *after;
    struct list_struct *before;
} list;

void Add            ( list *n, list **after )
{
    n->after = *after;
    if( *after != NULL )
        ( *after )->before = n;
    *after = n;
    n->before = NULL;
}

list *Create        ( char data )
{
    list *n = ( list* )malloc( sizeof( list ) );
    n->after = NULL;
    n->before = NULL;
    n->data = data;
    return n;
}

void Clear          ( list *l )
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

void RemoveDontFree ( list *l ) 
{
    if( l->after != NULL )
        ( l->after )->before = l->before;
    if( l->before != NULL )
        ( l->before )->after = l->after;
}

void Remove         ( list *l )
{
    RemoveDontFree( l );
    free( l );
}

void Insert         ( list* where, list* item )
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
} save;

typedef struct possible_struct
{
    struct possible_struct *last;
    list *change;

    // list** arr;
    // type index;

    type occurs;
    type maxOccurs;

    list *ptr;
    list *searching;
} possible;

typedef struct record_struct
{
    type index;
    type max;

    type occurs;
    bool once;

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
void PrintError     ( void );

/**
 * @brief           clears memory of all allocated memory on given pointers
 * 
 * @param wanted    save struct, wanted char on start
 * @param str       save struct, str saved on cycle
 * @param rec       record struct, to clear the outomes
 */
void ClearAll       ( save wanted, save str, record rec );

/**
 * @brief           Clears allocated memory of save
 * 
 * @param rec       save struct to be freed
 */
void SaveClear       ( save *rec );

/**
 * @brief           Extends given pointer when needed based on size data
 * 
 * @param ptr       ptr to be expanded
 * @param so        sizeof object
 * @param size      measures struct, given sizes
 * @return void*    returns the same or extended (when needed) ptr, NULL on failure
 */
void *Extend        ( void *ptr, unsigned long so, measures *size );

/**
 * @brief           Comparing function for qsort, sorts by char ch
 * 
 * @param r1        record one
 * @param r2        record two
 * @return int      -1 on record one ch smaller, 0 on even, otherwise 1
 */
int CmpByAlpha      ( const void *r1, const void *r2 );

/**
 * @brief           Removes not needed characters from list in str save
 * 
 * @param wanted    wanted save with characters needed
 * @param str       str save with characters from input
 * @return true     when it is not possible to match the pattern
 * @return false    when it is possible to match
 */
bool Eliminate      ( save wanted, save *str );

/**
 * @brief           Function to find and save all possible outcomes, 
 *                  not really a recursion
 * 
 * @param rec       record pointer, saving all possible outcomes
 * @return true     on failure
 * @return false    on success
 */
bool Recursion      ( record *rec );

//

/**
 * @brief           Initializes work, reading from input
 * 
 * @param rec       save struct to save read string in array
 * @return true     on failure
 * @return false    on success
 */
bool Init           ( save *rec, bool quot )
{
    rec->sorted     = NULL;
    rec->arr        = NULL;
    rec->size.alloc = 0;
    rec->size.len   = 0;
    rec->start      = NULL;

    char ch = getchar();
    if( quot ) {
        while( ch == ' ' )
            ch = getchar();
        if( ch != '"' )
            return EXIT_FAILURE;
        ch = getchar();
    }

    do {
        if( ch == '"' )
            break;
        if( !quot && !isupper( ch ) )
            return EXIT_FAILURE;
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
    } while( ( ch = getchar() ) != '\n' && ch != EOF );

    if ( ch == EOF )
        return EXIT_FAILURE;
    else if( quot ) {
        if ( ch == '\n' )
            return EXIT_FAILURE;
    }

    while( ch != '\n' )
        ch = getchar();
    if( rec->size.len > 0 ) {
        rec->start = rec->sorted[ 0 ];

        qsort( rec->sorted, rec->size.len, sizeof( list* ), CmpByAlpha );
    }
    return EXIT_SUCCESS;
}

/**
 * @brief           Prints out all possible outcomes on the end
 * 
 * @param rec       record with all possible outcomes
 * @param str       save str, with originaly loaded string
 * @param key       key from input, determines which output user wants
 */
void PrintOut       ( record *rec, save *str, char key )
{
    if( key == counts ) {
        type *occursCounts = ( type* )malloc( sizeof( type ) * rec->occurs );
        for( type i = 0; i < rec->occurs; i++ )
            occursCounts[ i ] = 0;
        for( type i = 0; i < rec->compSize.len; i++ ) {
            occursCounts[ rec->comp[ i ]->maxOccurs - 1 ] ++;
        }
        for( type i = 1; i < rec->occurs; i++ )
            occursCounts[ i ] += occursCounts[ i - 1 ];
        for( type i = 0; i < rec->occurs; i++ )
            printf( "> limit %lld: %lld\n", i + 1, occursCounts[ i ] );
        free( occursCounts );
        return;
    }
    else {
        if( rec->compSize.len > 0 ) {
            possible *help = rec->comp[ 0 ];
            while( help->last != NULL ) {
                char ch = help->change->data;
                help->change->data = toupper( ch );
                help = help->last;
            }
            printf( "\"" );
            for( type y = 0; y < str->size.len; y++ )
                printf( "%c", str->arr[ y ]->data );
            printf( "\"\n" );
            
            for( type i = 1; i < rec->compSize.len; i++ ) {
                help = rec->comp[ i - 1 ];
                while( help->change != NULL ) {
                    char *ch = &help->change->data;
                    *ch = tolower( *ch );
                    help = help->last;
                }
                help = rec->comp[ i ];
                while( help->change != NULL ) {
                    char *ch = &help->change->data;
                    *ch = toupper( *ch );
                    help = help->last;
                }
                printf( "\"" );
                for( type y = 0; y < str->size.len; y++ )
                    printf( "%c", str->arr[ y ]->data );
                printf( "\"\n" );
            }
        }
        printf( "> %lld\n", rec->compSize.len );
        return;
    }
}

/**
 * @brief           Solving, deciding and printing all the possible outcomes
 * 
 * @param wanted    save struct, wanted string
 * @param str       save struct, str to save loaded string to modify
 * @param rec       record struct, saving all possible outcomes
 * @return true     on success
 * @return false    on failure
 */
bool InCycle        ( save *wanted, save *str, record *rec, char key )
{
    possible *pos       = ( possible* )malloc( sizeof( possible ) );
    if( pos == NULL )
        return EXIT_FAILURE;

    pos->last           = NULL;
    pos->change         = NULL;
    pos->ptr            = str->arr[ 0 ];
    pos->searching      = wanted->start;
    pos->occurs         = 0;
    pos->maxOccurs      = 0;

    rec->size.alloc     = 1;
    rec->size.len       = 1;
    rec->arr            = ( possible** )malloc( sizeof( possible* ) );
    if( rec->arr == NULL )
        return EXIT_FAILURE;
    rec->arr[ 0 ]       = pos;
    
    rec->compSize.alloc = 0;
    rec->compSize.len   = 0;
    rec->comp           = NULL;
    
    rec->max            = wanted->size.len;
    rec->index          = 0;

    //

    while( rec->index != rec->size.len ) {
        if( Recursion( rec ) )
            return EXIT_FAILURE;
    }
    PrintOut( rec, str, key );

    //

    for( type i = 0; i < rec->size.len; i++ )
        free( rec->arr[ i ] );
    free( rec->arr );
    free( rec->comp );

    rec->arr            = NULL;
    rec->comp           = NULL;

    return EXIT_SUCCESS;
}

//

int main            ( void )
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
    rec.once = false;
    while( ( check = scanf( "%c %lld ", &key, &occurs ) ) == 2 ) {
        rec.once = true;
        rec.occurs = occurs;
        
        if( ( key != all && key != counts ) || occurs <= 0 ) {
            PrintError();
            ClearAll( wanted, str, rec );
            return EXIT_FAILURE;
        }

        if( Init( &str, true ) ) {
            PrintError();
            ClearAll( wanted, str, rec );
            return EXIT_FAILURE;
        }

        if( str.size.len == 0 ) {
            rec.compSize.len = 0;
            PrintOut( &rec, &str, key );

            SaveClear( &str );
            continue;
        }

        if( Eliminate( wanted, &str ) ) {
            rec.compSize.len = 0;
            PrintOut( &rec, &str, key );

            SaveClear( &str );
            continue;
        }

        if( InCycle( &wanted, &str, &rec, key ) ) {
            PrintError();
            ClearAll( wanted, str, rec );
            return EXIT_FAILURE;
        }

        SaveClear( &str );
    }

    ClearAll( wanted, str, rec );

    if( check != EOF || !rec.once ) {
        PrintError();
        return EXIT_FAILURE;
    }

    //

    return EXIT_SUCCESS;
}

//

void PrintError( void )
{
    printf( "Nespravny vstup.\n" );    
}

void ClearAll( save wanted, save str, record rec )
{
    SaveClear( &wanted );

    SaveClear( &str );

    if( rec.arr != NULL ) {
        for( type i = 0; i < rec.size.len; i++ )
            free( rec.arr[ i ] );
    }
    free( rec.arr );
}

void SaveClear( save *rec )
{
    free( rec->sorted );
    if( rec->arr != NULL ) {
        for( type i = 0; i < rec->size.len; i++ )
            free( rec->arr[ i ] );
    }
    free( rec->arr );

    rec->sorted = NULL;
    rec->arr = NULL;
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
        if( isalpha( str->sorted[ i ]->data ) )
            break;
        else
            RemoveDontFree( str->sorted[ i ] );
    }
    for( ; wIndex < wanted.size.len; wIndex++ ) {
        if( wIndex + 1 >= wanted.size.len )
            break;
        else if( wanted.sorted[ wIndex + 1 ]->data
            != wanted.sorted[ wIndex ]->data )
            break;
    }

    for( ; i < str->size.len; i++ ) {
        list *record = str->sorted[ i ];

        if( wanted.sorted[ wIndex ]->data < record->data ) {
            if( count == 0 )
                return EXIT_FAILURE;
            wIndex ++;
            for( ; wIndex < wanted.size.len; wIndex++ ) {
                if( wIndex + 1 >= wanted.size.len )
                    break;
                else if( wanted.sorted[ wIndex + 1 ]->data
                 != wanted.sorted[ wIndex ]->data )
                    break;
            }

            if( wIndex >= wanted.size.len )
                break;
            count = 0;
        }

        if( record->data != wanted.sorted[ wIndex ]->data ) {
            if( str->start == record )
                str->start = record->after;
            RemoveDontFree( record );
        }
        else {
            count++;
        }
    }
    for( ; i < str->size.len; i++ )
        RemoveDontFree( str->sorted[ i ] );

    //

    return EXIT_SUCCESS;
}

bool Recursion( record *rec )
{
    possible *pos = rec->arr[ rec->index ];
    
    if( pos->occurs == rec->occurs + 1 ) {
        rec->index ++;
        return EXIT_SUCCESS;
    }
    else if( pos->searching == NULL ) {
        rec->comp = ( possible** )Extend( rec->comp, sizeof( possible* ), &rec->compSize );
        if( rec->comp == NULL )
            return EXIT_FAILURE;
        rec->comp[ rec->compSize.len ++ ] = pos;
        rec->index ++;
        return EXIT_SUCCESS;
    }
    else if( pos->ptr == NULL ) {
        rec->index ++;
        return EXIT_SUCCESS;
    }
    else if( pos->ptr->data == DELIMETER ) {
        pos->ptr = pos->ptr->before;
        pos->occurs = 0;
        return EXIT_SUCCESS;
    }

    if( pos->searching->data == pos->ptr->data ) {
        possible *newPos = ( possible* )malloc( sizeof( possible ) );
        if( newPos == NULL )
            return EXIT_FAILURE;

        newPos->ptr         = pos->ptr->before;
        newPos->searching   = pos->searching->before;
        newPos->occurs      = pos->occurs;
        newPos->maxOccurs   = pos->maxOccurs;
        newPos->last        = pos;
        newPos->change      = pos->ptr;

        if( ++ newPos->occurs > newPos->maxOccurs )
            newPos->maxOccurs = newPos->occurs;

        rec->arr = ( possible** )Extend( rec->arr, sizeof( possible* ), &rec->size );
        if( rec->arr == NULL )
            return EXIT_FAILURE;

        rec->arr[ rec->size.len ++ ] = newPos;
    }
    pos->ptr = pos->ptr->before;

    return EXIT_SUCCESS;
}
