// libraries

#include <stdio.h>      /* standard I/O */
#include <stdbool.h>    /* boolean logic values */
#include <stdlib.h>     /* malloc, realloc, free */
#include <string.h>     /* using string relative functions */
#include <time.h>       /* using time to set srand */

#define MAX_LEN 99      /* max length of words on input */

// typedefs and enums

/**
 * @brief keys of input, indicating what does the user want the program to do at the moment
 */
enum keys {
    add = '+',
    all = '#',
    num = '?',
    def = ' ',
};

/**
 * @brief one record, saving one thing and its
 * name, count, index in sorted array
 * 
 */
typedef struct record_struct
{
    char *name;
    long count;
    long index;
} record;

/**
 * @brief saving array records all with specified name length
 * , length saves number of these records in array 
 * 
 */
typedef struct list_struct
{
    record **names;
    int nameLen;
    long length;
} list;

/**
 * @brief saving everything for keeping sorted data together
 * 
 */
typedef struct pointRecord_struct
{
    record **pointers;
    long count;

    long sorted;
    long *indexes;
    bool newData;
} pointRecord;

// prototyping

/**
 * @brief Prints out common error message in stdout
 */
void ErrorMessage   ( void );

/**
 * @brief            Clears allocated memory in given pointers
 * 
 * @param[in] arr    struct list to free all allocated memory
 * @param[in] sorted sequence of all record pointers to free allocated memory
 */
void ClearArr       ( list *arr, pointRecord *sorted );

/**
 * @brief             Extending function for allocating more space for saving
 * 
 * @param[out] arr    struct list to expand array of record pointers
 * @param[out] sorted sequence of all record pointers to be expanded
 */
void ExtendArr      ( list *arr, pointRecord *sorted );

/**
 * @brief            Auxiliary function for quicksort, dont use this on your own, separates array by pivot
 * 
 * @param[out] arr   array to be sorted
 * @param[in]  left  left index inclusive
 * @param[in] right right index inclusive
 * @return int position of pivot in array
 */
int Partition       ( record **arr, int left, int right );

/**
 * @brief            Sorting by quicksort algorithm, divide and conquer, using random pivot 
 * 
 * @param[out] arr   array of record pointers to be sorted by the count
 * @param[in]  left  left index inclusive
 * @param[in]  right right index inclusive
 */
void Quicksort      ( record **arr, int left, int right );

/**
 * @brief       Used to swap two pointers of record on given pointers
 * 
 * @param[in] a first pointer to swap with second
 * @param[in] b second pointer to swap with first
 */
void Swap           ( record **a, record **b );

/**
 * @brief             Merges sorted and freshly sorted parts of sequence of record pointers by count
 * 
 * @param[out] sorted struct pointer to sort array of record pointers
 */
void Merge          ( pointRecord *sorted );

/**
 * @brief             Combines all sorting parts all together and from partially sorted array produces sorted array of record pointers
 *                    also resets indexes for fast sorting on adding already added data 
 * 
 * @param[out] sorted struct pointer to sort array of record pointers
 */
void AllTogether    ( pointRecord *sorted );

//

int main( void )
{
    list arr[ MAX_LEN ];
    pointRecord sorted;

    int tracked     = 0;

    sorted.count    = 0;
    sorted.sorted   = 0;
    sorted.indexes  = NULL;
    sorted.pointers = NULL;
    sorted.newData  = false;

    for( int i = 0; i < MAX_LEN; i++ ) {
        arr[i].length = 0;
        arr[i].nameLen = i + 1;
        
        arr[i].names = NULL;
    }

    printf( "Pocet sledovanych:\n" );
    if( scanf( " %d", &tracked ) != 1 || tracked <= 0 ) {
        ErrorMessage();
        ClearArr( arr, &sorted );
        return EXIT_FAILURE;
    }
    printf( "Pozadavky:\n");

    char key = def;
    int controlVal = scanf( " %c", &key );
    while( controlVal == 1 ) {

        // adding certain thing
        if( key == add ) {
            char str[ MAX_LEN ];
            
            if( scanf( " %99s", str ) != 1 ) {
                ErrorMessage();
                ClearArr( arr, &sorted );
                return EXIT_FAILURE;
            }

            int len = strlen( str );
            if( len == MAX_LEN ) {
                if( getchar() != '\n' ) {
                    ErrorMessage();
                    ClearArr( arr, &sorted );
                    return EXIT_FAILURE;
                }
            }

            list *ref = &arr[ len - 1 ];
            long i;
            for( i = 0; i < ref->length; i++ ) {
                if( strcmp( str, ref->names[ i ]->name ) == 0 )
                    break;
            }
            
            if( i == ref->length ) {
                // new record -> expand and save
                // sorted.newData true value indicates whether there is need for resorting the arr

                ExtendArr( ref, &sorted );
                sorted.pointers[ sorted.count - 1 ] = ref->names[ ref->length - 1 ];
                
                strncpy( ref->names[ ref->length - 1 ]->name, str, len );
                ref->names[ ref->length - 1 ]->count = 1;
                ref->names[ ref->length - 1 ]->index = sorted.count - 1;

                sorted.newData = true;
            }
            else {
                // already added record -> add 1 to count
                // swap with record on the top of sorted records with same count
                //  -> if on the top already, expand and move the indexes further

                ref->names[ i ]->count++;

                if( ref->names[ i ]->index < sorted.sorted ) {
                    if( sorted.indexes[ ref->names[ i ]->count - 2 ] == 0 ) {
                        sorted.indexes = ( long* )realloc( sorted.indexes, sizeof( long ) * ref->names[ i ]->count );
                        sorted.indexes[ ref->names[ i ]->count - 1 ] = 0;
                    }

                    if( ref->names[ i ]->index != 0 )
                        Swap( &( sorted.pointers[ sorted.indexes[ ref->names[ i ]->count - 2 ] ] ),
                                &( sorted.pointers[ ref->names[ i ]->index ] ) );
                    sorted.indexes[ ref->names[ i ]->count - 2 ]++;
                    
                }
            }
        }
        // sorting and printing all data loaded
        else if( key == all ) {
            if( sorted.newData )
                AllTogether( &sorted );

            long count = 0;
            long sum = 0;
            for( long i = sorted.pointers[ 0 ]->count - 1; count < tracked && i >= 0 ; i-- ) {
                long temp = 0;
                // determines how many records are there with same count
                //  -> to print the range
                if( i != 0 )
                    temp = ( sorted.indexes[ i - 1 ] - sorted.indexes[ i ] );
                else
                    temp = ( sorted.count - sorted.indexes[ i ] );

                if( temp == 1 ) {
                    record *ref = sorted.pointers[ sorted.indexes[ i ] ];
                    printf( "%ld. %s, %ldx\n", sorted.indexes[ i ] + 1, ref->name, ref->count );
                }
                else if( temp > 1 ) {
                    for( long y = 0; y < temp; y++ ) {
                        record *ref = sorted.pointers[ sorted.indexes[ i ] + y ];
                        printf( "%ld.-%ld. %s, %ldx\n", sorted.indexes[ i ] + 1, sorted.indexes[ i ] + temp, ref->name, ref->count );
                    }
                }

                // dont print anything, if there is no record of this count
                if( temp > 0 ) {
                    count += temp;
                    sum += temp * sorted.pointers[ sorted.indexes[ i ] ]->count;
                }
            }
            printf( "Nejprodavanejsi zbozi: prodano %ld kusu\n", sum );
        }
        // sorting and printing just the number of sold things
        else if( key == num ) {
            if( sorted.newData )
                AllTogether( &sorted );

            long count = 0;
            long sum = 0;
            for( long i = sorted.pointers[ 0 ]->count - 1; count < tracked && i >= 0 ; i-- ) {
                long temp = 0;

                // determines how many records are there with same count
                //  -> to print the range
                if( i != 0 )
                    temp = ( sorted.indexes[ i - 1 ] - sorted.indexes[ i ] );
                else
                    temp = ( sorted.count - sorted.indexes[ i ] );

                if( temp > 0 ) {
                    count += temp;
                    sum += temp * sorted.pointers[ sorted.indexes[ i ] ]->count;
                }
            }
            printf( "Nejprodavanejsi zbozi: prodano %ld kusu\n", sum );

        }
        else {
            ErrorMessage();
            ClearArr( arr, &sorted );
            return EXIT_FAILURE;
        }

        controlVal = scanf( " %c", &key );
    }
    ClearArr( arr, &sorted );

    if( controlVal != EOF ) {
        ErrorMessage();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//

void ErrorMessage( void )
{
    printf( "Nespravny vstup.\n");
}

void ClearArr( list *arr, pointRecord *sorted )
{
    for( int i = 0; i < MAX_LEN; i++ ) {
        for( int y = 0; y < arr[i].length; y++ ) {
            free( arr[ i ].names[ y ]->name );
            free( arr[ i ].names[ y ] );
        }
        free( arr[i].names );
    }

    free( sorted->pointers );
    free( sorted->indexes );
}

void ExtendArr ( list *ref, pointRecord *sorted )
{
    void *help1 = NULL;

    help1 = realloc( ref->names, sizeof( record* ) * ( ++ref->length ) );
    ref->names = ( record** )help1;

    record *temp = ( record* )malloc( sizeof( record ) );
    temp->name = ( char* )malloc( sizeof( char ) * ref->nameLen );
    ref->names[ ref->length - 1 ] = temp;

    help1 = realloc( sorted->pointers, sizeof( record* ) * ( ++sorted->count ) );
    sorted->pointers = ( record** )help1;
    sorted->pointers[ sorted->count - 1 ] = temp;
}

int Partition( record **arr, int left, int right )
{
    srand( time(NULL) );
    long random = left + rand() % ( right - left );

    Swap( &( arr[ random ] ), &( arr[ right ] ) );
    long pivot = arr[ right ]->count;

    long i = left - 1;
    for( long j = left; j <= right - 1; j++ ) {
        if( arr[ j ]->count >= pivot ) {
            Swap( &( arr[ ++i ] ), &( arr[ j ] ) );
        }
    }
    Swap( &( arr[ i + 1 ] ), &( arr[ right ] ) );

    return (i + 1);
}

void Quicksort( record **arr, int left, int right )
{
    if( left < right ) {
        long pIndex = Partition( arr, left, right );

        Quicksort( arr, left, pIndex - 1 );
        Quicksort( arr, pIndex + 1, right );
    }
    return;
}

void Swap( record **a, record **b )
{
    record *temp = *a;
    *a = *b;
    *b = temp;

    long index = (*a)->index;
    (*a)->index = (*b)->index;
    (*b)->index = index;
}

void Merge( pointRecord *sorted )
{
    long in1 = 0;
    long in2 = sorted->sorted;
    long index = 0;
    record **pointers = (record **)malloc( sizeof( record* ) * sorted->count );

    while( in1 < sorted->sorted && in2 < sorted->count ) {
        if( sorted->pointers[ in1 ]->count > sorted->pointers[ in2 ]->count )
            pointers[ index ] = sorted->pointers[ in1++ ];
        else
            pointers[ index ] = sorted->pointers[ in2++ ];
        pointers[ index ]->index = index;
        index++;
    }

    while( in1 < sorted->sorted ) {
        pointers[ index ] = sorted->pointers[ in1++ ];
        pointers[ index]->index = index;
        index++;
    }
    while( in2 < sorted->count ) {
        pointers[ index ] = sorted->pointers[ in2++ ];
        pointers[ index ]->index = index;
        index++;
    }

    free( sorted->pointers );
    sorted->pointers = pointers;
}

void AllTogether( pointRecord *sorted )
{
    Quicksort( sorted->pointers, sorted->sorted, sorted->count - 1 );

    if( sorted->sorted != 0 )
        Merge( sorted );

    sorted->sorted = sorted->count;
    free( sorted->indexes );
    sorted->indexes = ( long* )malloc( sizeof( long ) * sorted->pointers[ 0 ]->count );
    
    long index = sorted->pointers[ 0 ]->count - 1;
    sorted->indexes[ index-- ] = 0;
    for( long i = 1; i < sorted->count; i++ ) {
        if( sorted->pointers[ i ]->count == index + 2 )
            continue;

        for( long y = sorted->pointers[ i ]->count - 1; y <= index; y++ )
            sorted->indexes[ y ] = i;
        index = sorted->pointers[ i ]->count - 2;
    }
    sorted->newData = false;
}

//
