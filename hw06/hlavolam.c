#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean logic values
#include <string.h>     // strings functions
#include <ctype.h>      // isalpha

//

#define ADD 10

//

typedef struct record_struct
{
    char *str;
} record;

typedef struct save_struct
{
    long len;
    
    record *arr;
} save;

typedef struct sequence_struct
{
    record **arr;
    long len;
    long allocated;
} sequence;

//

/**
 * @brief           Displays common error message, called when the error in for example data loading occurs
 * 
 */
void ErrorMsg( void );

/**
 * @brief           Clears all given pointers with allocated memory
 * 
 * @param matrix    save struct with array to be freed
 * @param seq       sequence struct with array to be freed
 */
void ClearAll( save matrix, sequence seq );

/**
 * @brief           Allocates enough memory to save whole matrix of characters needed from input
 * , (n*n) array, more precisely (n) array of records containing (n) characters
 * 
 * @param matrix    save structure instance
 * @return true     On success
 * @return false    On failure
 */
bool Allocate( save *matrix );

/**
 * @brief           Comparing function for Qsort, compares two record, more precisely, strings in them
 * 
 * @param r1        record one
 * @param r2        record two
 * @return int      < 0 element1 less than element2 | 
 *                  = 0 element1 equal to element2 | 
 *                  > 0 element1 greater than element2 | 
 */
int RecordCompare( const void *r1, const void *r2 );

/**
 * @brief           Checks if the coordinates are in limits of maximum array based on len in save
 * 
 * @param cords     array with coordinates
 * @param matrix    save instance
 * @return true     if coordinates are in limits
 * @return false    if coordinates are not in limits
 */
bool InLimits( long cords[ 2 ], save matrix );

/**
 * @brief           Adds a specified record to a sequence
 * 
 * @param seq       sequence struct which holds the arrays
 * @param item      record item to be added in sequence
 * @return true     if the reallocation success
 * @return false    if the reallocation fail
 */
bool AddRecord( sequence *seq, record *item );

/**
 * @brief           Create item
 * 
 * @param item      item where to save the created struct
 * @param len       length of character to be saved within
 * @return true     on success
 * @return false    on failure
 */
bool CreateItem( record **item, long len );

//

int main( void )
{
    save matrix;
    matrix.arr = NULL;
    matrix.len = 0;

    //

    char *str = NULL;
    size_t len = 0;
    long realLen = 0;

    sequence seq;
    seq.len = 0;
    seq.allocated = 0;
    seq.arr = NULL;

    int dirs[ 8 ][ 2 ] =  { {  1,  0 },
                            {  1,  1 },
                            {  0,  1 },
                            { -1,  1 },
                            { -1,  0 },
                            { -1, -1 },
                            {  0, -1 },
                            {  1, -1 } };

    //

    printf( "Hlavolam:\n" );
    if( (realLen = getline( &str, &len, stdin ) ) == -1 ) {
        free( str );
        ErrorMsg();
        return EXIT_FAILURE;
    }

    // in case of array of size 0x0
    // one of epected errors
    if( realLen == 1 ) {
        free( str );
        ErrorMsg();
        return EXIT_FAILURE;
    }

    matrix.len = realLen - 1;
    Allocate( &matrix );
    strcpy( matrix.arr[ 0 ].str, str );

    free( str );
    len = realLen;
    
    for( long i = 1; i < matrix.len; i++ ) {
        realLen = getline( &matrix.arr[ i ].str, &len, stdin );
        if( realLen - 1 != matrix.len ) {
            ClearAll( matrix, seq );
            ErrorMsg();
            return EXIT_FAILURE;
        }

    }

    if( getchar() != EOF ) {
        ClearAll( matrix, seq );
        ErrorMsg();
        return EXIT_FAILURE;
    }

    //

    record *item = NULL;
    
    for( long y = 0; y < matrix.len; y++ ) {
        for( long x = 0; x < matrix.len; x++ ) {
            
            char ch = matrix.arr[ y ].str[ x ];
            CreateItem( &item, 1 );
            item->str[ 0 ] = ch;
            AddRecord( &seq, item );

            printf( "%c\n", ch );

            for( int i = 0; i < 8; i++ ) {
                len = 1;
                long cords[ 2 ] = { x + dirs[ i ][ 0 ],
                                    y + dirs[ i ][ 1 ] };

                while( InLimits( cords, matrix ) ) {

                    printf( "%c - %ld %ld\n", matrix.arr[ cords[ 1 ] ].str[ cords[ 0 ] ], 
                        cords[ 0 ], cords[ 1 ] );

                    CreateItem( &item, len + 1 );
                    if( len == 1 )
                        item->str[ 0 ] = ch;
                    else
                        for( long l = 0; l < len; l++ )
                            item->str[ l ] = seq.arr[ seq.len - 1 ]->str[ l ];
                    item->str[ len ] = matrix.arr[ cords[ 1 ] ].str[ cords[ 0 ] ];
                    len++;
                    AddRecord( &seq, item );

                    cords[ 0 ] += dirs[ i ][ 0 ];
                    cords[ 1 ] += dirs[ i ][ 1 ];

                }

                printf( "\n" );
            }

            printf( "...\n" );
        }
    }

    //

    printf( "---\n" );
    for( long i = 0; i < seq.len; i++ )
        printf( "%s\n", seq.arr[ i ]->str );

    //

    qsort( seq.arr, seq.len, sizeof( record* ), RecordCompare );

    //

    /*
    printf( "---\n" );
    for( long i = 0; i < seq.len; i++ )
        printf( "%s\n", seq.arr[ i ]->str );
    */

    //

    return EXIT_SUCCESS;
}

void ErrorMsg( void )
{
    printf( "Nespravny vstup.\n" );
}

void ClearAll( save matrix, sequence seq )
{
    for( long i = 0; i < matrix.len; i++ ) {
        free( matrix.arr[i].str );
    }
    for( long i = 0; i < seq.len; i++ ) {
        free( seq.arr[ i ]->str );
        free( seq.arr[ i ] );
    }
    free( matrix.arr );
    free( seq.arr );
}

bool Allocate( save *matrix )
{
    matrix->arr = ( record* )malloc( sizeof( record ) * matrix->len );
    if( matrix->arr == NULL)
        return false;

    for( long i = 0; i < matrix->len; i++ ) {
        matrix->arr[ i ].str = ( char* )malloc( sizeof(char) * ( matrix->len + 1 ) );
        if( !matrix->arr[ i ].str) {
            for( long y = 0; y < i; y++ )
                free( matrix->arr[ y ].str );    
        
            free( matrix->arr );
            return false;
        }
    }

    return true;
}

int RecordCompare( const void *r1, const void *r2 )
{
    char *str1 = (*( record** )r1)->str;
    char *str2 = (*( record** )r2)->str;

    if( strlen( str1 ) < strlen( str2 ) )
        return 1;
    else if( strlen( str1 ) > strlen( str2 ) )
        return -1;
    else
        return strcmp( str1, str2 );
}

bool InLimits( long cords[ 2 ], save matrix )
{
    return ( cords[ 0 ] >= 0
          && cords[ 0 ] <  matrix.len
          && cords[ 1 ] >= 0
          && cords[ 1 ] <  matrix.len );
}

bool AddRecord( sequence *seq, record *item )
{
    if( seq->allocated == seq->len ) {
        seq->allocated += ADD;
        void *help = realloc( seq->arr, sizeof( record* ) * seq->allocated );

        if( help == NULL )
            return false;
        seq->arr = ( record** )help;
    }

    seq->arr[ seq->len++ ] = item;
    return true;
}

bool CreateItem( record **item, long len )
{
    *item = ( record* )malloc( sizeof( record ) );
    if( item == NULL )
        return false;
    (*item)->str = ( char* )malloc( sizeof( char ) * len );
    if( *item == NULL ) {
        free( *item );
        return false;
    }

    return true;
}