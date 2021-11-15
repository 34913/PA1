
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
int  AddUser        ( int id, database *db );
int  CalculateUsers ( int from, int to, database *db );

//

int main ( int argc, char * argv [] )
{
    char key        = defKey;
    int r           = 0;
    database db;
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
                ErrorMsg();
                return EXIT_FAILURE;
            }

            printf( "%d\n", AddUser( id, &db ) );

        }
        else if( key == calKey ) {
            int from = 0;
            int to = 0;

            if( CheckInput( scanf( " %d %d", &from, &to ), 2 )
                    || from < 0 || to >= db.len || from > to ) {
                free( db.data );
                ErrorMsg();
                return EXIT_FAILURE;
            }

            printf("%d / %d\n", CalculateUsers( from, to, &db ), to - from + 1 );
            
        }
    }
    free( db.data );

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

int AddUser( int id, database *db )
{
    void *help;
    if( db->len == 0 )
        help = malloc( sizeof( int ) );
    else
        help = realloc( db->data, sizeof( int ) * (db->len + 1) );

    if( help == NULL ) {
        printf( "Not enough memory\n" );
        free( db->data );
        exit( EXIT_FAILURE );
    }

    db->data = (int* )help;
    db->data[db->len] = id;

    db->len = db->len + 1;

    int count = 1;
    for( int i = 0; i < db->len - 1; i++ ) {
        if( (db->data)[i] == id )
            count++;
    }
    return count;
}

int CalculateUsers( int from, int to, database *db )
{
    int count = db->len;

    for( int i = 0; i < db->len; i++ ) {
        for( int y = i + 1; y < db->len; y++ ) {
            if( db->data[i] == db->data[y] ) {
                count--;
                break;
            }
        }
    }

    return count;
}
