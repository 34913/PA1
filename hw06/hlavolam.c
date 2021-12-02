#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//

typedef struct record_struct
{
    char *str;
} record;

typedef struct matrix_struct
{
    long len;
    
    record *arr;
} save;

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
 */
void ClearAll( save matrix );

/**
 * @brief           Allocates enough memory to save whole matrix of characters needed from input
 * , (n*n) array, more precisely (n) array of records containing (n) characters
 * 
 * @param matrix    save structure instance
 * @return true     On success
 * @return false    On failure
 */
bool Allocate( save *matrix );

//

int main( void )
{
    save matrix;
    matrix.arr = NULL;
    matrix.len = 0;

    char *str = NULL;
    size_t len = 0;

    printf( "Hlavolam:\n" );
    long realLen = 0;
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
            ClearAll( matrix );
            ErrorMsg();
            return EXIT_FAILURE;
        }

    }

    if( getchar() != EOF ) {
        ClearAll( matrix );
        ErrorMsg();
        return EXIT_FAILURE;
    }

    //








    return EXIT_SUCCESS;
}

void ErrorMsg( void )
{
    printf( "Nespravny vstup.\n" );
}

void ClearAll( save matrix )
{
    for( long i = 0; i < matrix.len; i++ ) {
        free( matrix.arr[i].str );
    }
    free( matrix.arr );
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
