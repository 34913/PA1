
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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


//

bool CheckInput     ( int returnVal, int wantedVal );
void ErrorMsg       ( void );
int  AddUser        ( int id, database *db, database *users );
int  CalculateUsers ( int from, int to, database *db );
void Printing       ( int entry );

//

int main ( int argc, char * argv [] )
{
    char key        = defKey;
    int r           = 0;
    database users;
    database db;

    users.len = 100000;
    users.data = (int *)malloc( sizeof( int ) * users.len );

    for( int i = 0; i < users.len; i++ )
        users.data[i] = 0;
    db.len = 0;
    db.data = NULL;

    printf("Pozadavky:\n");

    while( true ) {
        r =  scanf( " %c", &key );
            
        if( CheckInput( r, 1 ) || ( key != addKey && key != calKey ) )
            break;

        if( key == addKey ) {
            int id = 0;

            if( CheckInput( scanf( " %d", &id ), 1 ) 
                    || id < 0 || id > 99999 ) {
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

int AddUser( int id, database *db, database *users )
{
    void *help;
    if( db->len == 0 )
        help = malloc( sizeof( int ) );
    else
        help = realloc( db->data, sizeof( int ) * (db->len + 1) );

    db->data = (int* )help;
    db->data[db->len] = id;

    db->len = db->len + 1;

    users->data[ id ] = users->data[ id ] + 1;
    return users->data[ id ];
}

int CalculateUsers( int from, int to, database *db )
{
    int count = to - from + 1;

    for( int i = from; i <= to; i++ ) {
        for( int y = i + 1; y <= to; y++ ) {
            if( db->data[i] == db->data[y] ) {
                count--;
                break;
            }
        }
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
