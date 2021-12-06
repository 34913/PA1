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
    long len;
} record;

typedef struct save_struct
{
    long len;
    
    char **arr;
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
 * @param seq       sequence struct seq with array to be freed
 * @param most      sequence struct most with array to be freed
 */
void ClearAll( save matrix, sequence seq, sequence most );

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

bool Reading( sequence *seq, save *matrix )
{
    char *str = NULL;
    long realLen = 0;
    size_t len = 0;

    printf( "Hlavolam:\n" );
    if( (realLen = getline( &str, &len, stdin ) ) == -1 ) {
        free( str );
        return EXIT_FAILURE;
    }

    // in case of array of size 0x0
    // one of epected errors
    if( realLen == 1 ) {
        free( str );
        return EXIT_FAILURE;
    }

    matrix->len = realLen - 1;
    Allocate( matrix );
    strcpy( matrix->arr[ 0 ], str);

    free( str );
    len = realLen;
    
    for( long i = 1; i < matrix->len; i++ ) {
        realLen = getline( &matrix->arr[ i ], &len, stdin );
        if( realLen - 1 != matrix->len )
            return EXIT_FAILURE;
    }

    if( getchar() != EOF )
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void AllPermutations( sequence *seq, save *matrix )
{
    const int dirs[ 8 ][ 2 ] =  { {  1,  0 },
                                  {  1,  1 },
                                  {  0,  1 },
                                  { -1,  1 },
                                  { -1,  0 },
                                  { -1, -1 },
                                  {  0, -1 },
                                  {  1, -1 } };

    long len = 0;
    record *item = NULL;
    
    for( long y = 0; y < matrix->len; y++ ) {
        for( long x = 0; x < matrix->len; x++ ) {
            
            char ch = matrix->arr[ y ][ x ];
            CreateItem( &item, 1 );
            item->str[ 0 ] = ch;
            item->len = 1;
            AddRecord( seq, item );

            for( int i = 0; i < 8; i++ ) {
                len = 1;
                long cords[ 2 ] = { x + dirs[ i ][ 0 ],
                                    y + dirs[ i ][ 1 ] };

                while( InLimits( cords, *matrix ) ) {

                    CreateItem( &item, len + 1 );
                    if( len == 1 )
                        item->str[ 0 ] = ch;
                    else
                        for( long l = 0; l < len; l++ )
                            item->str[ l ] = seq->arr[ seq->len - 1 ]->str[ l ];
                    item->str[ len ] = matrix->arr[ cords[ 1 ] ][ cords[ 0 ] ];
                    len++;
                    item->len = len;
                    AddRecord( seq, item );

                    cords[ 0 ] += dirs[ i ][ 0 ];
                    cords[ 1 ] += dirs[ i ][ 1 ];
                }
            }
        }
    }
}

bool Sieve( sequence *seq, sequence *most, save *matrix )
{
    qsort( seq->arr, seq->len, sizeof( record* ), RecordCompare );

    //

    record *item = seq->arr[ 0 ];
    for( long i = 1; i < seq->len; i++ ) {
        record *tmp = seq->arr[ i ];

        if( tmp->len != item->len ) {
            if( most->len > 0 )
                break;
            else
                item = seq->arr[ i ];
        }
        else if( strncmp( item->str, tmp->str, item->len ) == 0 ) {
            if( !AddRecord( most, tmp ) )
                return EXIT_FAILURE;

            tmp = seq->arr[ ++i ];
            while( i < seq->len ) {
                if( item->len != tmp->len || strncmp( item->str, tmp->str, item->len ) != 0 )
                    break;
                tmp = seq->arr[ ++i ];
            }
            i--;
        }
        else
            item = seq->arr[ i ];
    }
    return EXIT_SUCCESS;
}

void Print( sequence *most )
{
    if( most->len == 0 )
        printf( "Zadne opakujici se slovo.\n" );
    else {
        printf( "Nejdelsi opakujici se slova:\n" );
        for( long i = 0; i < most->len; i++ ) {
            for( long l = 0; l < most->arr[ i ]->len; l++ )
                printf( "%c", most->arr[ i ]->str[ l ] );
            printf( "\n" );
        }
    }
}

//

int main( void )
{
    save matrix;
    matrix.arr = NULL;
    matrix.len = 0;

    //

    sequence seq;
    seq.len = 0;
    seq.allocated = 0;
    seq.arr = NULL;

    //

    sequence most;
    most.arr = NULL;
    most.allocated = 0;
    most.len = 0;

    //

    if( Reading( &seq, &matrix ) == EXIT_FAILURE ) {
        ClearAll( matrix, seq, most );
        ErrorMsg();
        return EXIT_FAILURE;
    }
    AllPermutations( &seq, &matrix );
    if( Sieve( &seq, &most, &matrix ) == EXIT_FAILURE ) {
        ClearAll( matrix, seq, most );
        ErrorMsg();
        return EXIT_FAILURE;
    }
    Print( &most );

    //

    ClearAll( matrix, seq, most );
    return EXIT_SUCCESS;
}

void ErrorMsg( void )
{
    printf( "Nespravny vstup.\n" );
}

void ClearAll( save matrix, sequence seq, sequence most )
{
    if( matrix.arr != NULL) {
        for( long i = 0; i < matrix.len; i++ )
            free( matrix.arr[ i ] );
    }
    free( matrix.arr );

    for( long i = 0; i < seq.len; i++ ) {
        free( seq.arr[ i ]->str );
        free( seq.arr[ i ] );
    }
    free( seq.arr );

    /*
    for( long i = 0; i < most.len; i++ ) {
        free( most.arr[ i ]->str );
        free( most.arr[ i ] );
    }
    */
    free( most.arr );
}

bool Allocate( save *matrix )
{
    matrix->arr = ( char** )malloc( sizeof( char* ) * matrix->len );
    if( matrix->arr == NULL)
        return false;

    for( long i = 0; i < matrix->len; i++ ) {
        matrix->arr[ i ] = ( char* )malloc( sizeof(char) * ( matrix->len + 2 ) );
        if( matrix->arr[ i ] == NULL )
            return false;
    }

    return true;
}

int RecordCompare( const void *r1, const void *r2 )
{
    record *record1 = *( record** )r1;
    record *record2 = *( record** )r2;

    if( record1->len < record2->len )
        return 1;
    else if( record1->len > record2->len )
        return -1;
    else
        return strncmp( record1->str, record2->str, record1->len );
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
