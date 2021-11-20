// libraries

#include <stdio.h>      // standard I/O
#include <stdbool.h>    // boolean logic values
#include <stdlib.h>     // malloc, realloc, free
#include <string.h>     // using string relative functions
#include <time.h>       // using time to set srand

#define MAX_LEN 99      // max length of words on input

enum keys {
    add = '+',
    all = '#',
    num = '?',
    def = ' ',
};

typedef struct record_struct
{
    char *name;
    long count;
    long index;
} record;

typedef struct list_struct
{
    record **names;
    int nameLen;
    long length;
} list;

typedef struct pointRecord_struct
{
    record **pointers;
    long count;

    long sorted;
    //long *freq;
} pointRecord;

void ErrorMessage();
void ClearArr( list *arr, pointRecord *sorted );
bool ExtendArr ( list *arr, pointRecord *sorted );

int Partition_r( record **arr, int left, int right );
void Quicksort( record **arr, int left, int right );
void Swap( record **a, record **b );

//

int main(void)
{
    // saying how many of tracked things are going to be displayed
    int tracked = 0;

    list arr[MAX_LEN];
    pointRecord sorted;
    sorted.count = 0;
    sorted.sorted = 0;

    bool newData = false;

    for( int i = 0; i < MAX_LEN; i++ ) {
        arr[i].length = 0;
        arr[i].nameLen = i + 1;
        
        arr[i].names = NULL;
    }

    if( scanf( " %d", &tracked ) != 1 || tracked <= 0 ) {
        ErrorMessage();
        ClearArr( arr, &sorted );
        return EXIT_FAILURE;
    }

    char c = def;
    int d = scanf( " %c", &c );
    while( d == 1 ) {
        
        if( c == add ) {
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

            bool exists = false;
            list *r = &arr[ len - 1 ];

            for( long i = 0; i < r->length; i++ ) {
                if( strcmp( str, r->names[ i ]->name ) == 0 ) {
                    exists = true;
                    r->names[ i ]->count++;

                    if( r->names[ i ]->index < sorted.sorted ) {
                        long index = r->names[ i ]->index - 1;
                        for( ; index > 0 && sorted.pointers[ index - 1 ]->count == r->names[ i ]->count - 1; index-- );

                        Swap( &( sorted.pointers[ r->names[ i ]->index ] ), &( sorted.pointers[ index ] ) );
                    }

                    break;
                }
            }

            if( !exists ) {
                if( !ExtendArr( r, &sorted ) ) {
                    ErrorMessage();
                    ClearArr( arr, &sorted );
                    return EXIT_FAILURE;
                }
                
                strncpy( r->names[ r->length - 1 ]->name, str, len );
                r->names[ r->length - 1 ]->count = 1;
                r->names[ r->length - 1 ]->index = sorted.count - 1;

                newData = true;
            }
        }
        else if( c == all ) {
            if( newData ) {
                Quicksort( sorted.pointers, sorted.sorted, sorted.count - 1 );



                sorted.sorted = sorted.count;
                newData = false;
            }

            for( long i = 0; i < sorted.count; i++ ) {
                record *r = sorted.pointers[ i ];
                printf( "%s %ld - %p\n", r->name, r->count, r );
            }
            printf("\n");
        }
        else if( c == num ) {
            
        }
        else {
            ErrorMessage();
            ClearArr( arr, &sorted );
            return EXIT_FAILURE;
        }

        d = scanf( " %c", &c );
    }
    ClearArr( arr, &sorted );

    if( d != EOF ) {
        ErrorMessage();
        ClearArr( arr, &sorted );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void ErrorMessage()
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
}

bool ExtendArr ( list *r, pointRecord *sorted )
{
    void *help1 = NULL;
    if( r->length++ == 0 )
        help1 = malloc( sizeof( record* ) );
    else
        help1 = realloc( r->names, sizeof( record* ) * r->length );

    if( help1 == NULL )
        return false;
    r->names = ( record** )help1;
    
    record *temp = ( record* )malloc( sizeof( record ) );
    temp->name = ( char* )malloc( sizeof( char ) * r->nameLen );
    r->names[ r->length - 1 ] = temp;

    if( sorted->count++ == 0 )
        help1 = malloc( sizeof( record* ) );
    else
        help1 = realloc( sorted->pointers, sizeof( record* ) * sorted->count );

    if( help1 == NULL )
        return false;
    sorted->pointers = ( record** )help1;
    sorted->pointers[ sorted->count - 1 ] = temp;

    return true;
}

int Partition_r( record **arr, int left, int right )
{
    srand( time(NULL) );
    long random = left + rand() % ( right - left );
    record *temp;
    long index;

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
        long pIndex = Partition_r( arr, left, right );

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
