
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//

#define MAX_LEN 100000
#define ALLOC 10

//

enum keysEnum
{
    addKey = '+',
    calKey = '?',
    defKey = ' ',
};

typedef struct dbStruct
{
    int *data;
    int len;
} database;

typedef struct sequenceStruct
{
    int *data;
    int len;

    int allocated;
    
} sequence;

//

bool CheckInput     ( int returnVal, int wantedVal );
void ErrorMsg       ( void );
int  AddUser        ( int id, sequence *db, database *users );
int  CalculateUsers ( int from, int to, sequence *db );
void Printing       ( int entry );

//

int main ( void )
{
    char key        = defKey;
    int r           = 0;
    database users;
    sequence db;

    users.len = MAX_LEN;
    users.data = ( int * )malloc( sizeof( int ) * users.len );

    for( int i = 0; i < users.len; i++ )
        users.data[ i ] = 0;
    db.len = 0;
    db.allocated = 0;
    db.data = NULL;
    

    printf( "Pozadavky:\n" );

    while( true ) {
        r =  scanf( " %c", &key );
            
        if( CheckInput( r, 1 ) || ( key != addKey && key != calKey ) )
            break;

        if( key == addKey ) {
            int id = 0;

            if( CheckInput( scanf( " %d", &id ), 1 )
                    || id < 0 || id > MAX_LEN - 1 ) {
                free( db.data );
                free( users.data );

                ErrorMsg();
                return EXIT_FAILURE;
            }

            Printing( AddUser( id, &db, &users ) );
        }
        else if( key == calKey ) {
            int from = 0;
            int to = 0;

            if( CheckInput( scanf( " %d %d", &from, &to ), 2 )
                    || from < 0 || to >= db.len || from > to ) {
                free( db.data );
                free( users.data );

                ErrorMsg();
                return EXIT_FAILURE;
            }

            printf( "> %d / %d\n", CalculateUsers( from, to, &db ), to - from + 1 );
            
        }
    }
    free( db.data );
    free( users.data );

    if( r != EOF ) {
        ErrorMsg();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool CheckInput( int returnVal, int wantedVal )
{
    return returnVal != wantedVal;
}

void ErrorMsg( void )
{
    printf("Nespravny vstup.\n");
}

int AddUser( int id, sequence *db, database *users )
{
    void *help = NULL;
    
    if( db->len == db->allocated ) {
        db->allocated += ALLOC;
        help = realloc( db->data, sizeof( int ) * db->allocated );
        db->data = ( int* )help;
    }

    db->data[ db->len++ ] = id;
    users->data[ id ]++;

    return users->data[ id ];
}

int CalculateUsers( int from, int to, sequence *db )
{
    int count = to - from + 1;
    bool temp[ MAX_LEN ] = { 0 };

    for( int i = from; i <= to; i++ ) {
        if( temp[ db->data[ i ] ] )
            count--;
        else
            temp[ db->data[ i ] ] = true;
    }
    return count;
}

void Printing( int entry )
{
    if( entry == 1 )
        printf( "> prvni navsteva\n" );
    else
        printf( "> navsteva #%d\n", entry );
    return;
}
