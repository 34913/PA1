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
bool Allocate( save matrix );

//

int main( void )
{
    save matrix;
    matrix.arr = NULL;
    matrix.len = 0;

    char *str = NULL;
    long len = 0;

    printf( "Hlavolam:\n" );
    if( getline( &str, &len, stdin ) == -1 ) {
        free( str );
        return EXIT_FAILURE;
    }

    Allocate( matrix );








    return EXIT_SUCCESS;
}



void ClearAll( save matrix )
{
    for( long i = 0; i < matrix.len; i++ ) {
        free( matrix.arr[i].str );
    }
    free( matrix.arr );
}

bool Allocate( save matrix )
{
    matrix.arr = ( record* )malloc( sizeof( record ) * matrix.len );
    if( !matrix.arr )
        return false;

    for( long i = 0; i < matrix.len; i++ ) {
        matrix.arr[ i ].str = ( char* )malloc( sizeof(char) * matrix.len );
        if( !matrix.arr[ i ].str) {
            for( long y = 0; y < i; y++ )
                free( matrix.arr[ y ].str );    
        
            free( matrix.arr );
            return false;
        }
    }
    
    return true;
}
