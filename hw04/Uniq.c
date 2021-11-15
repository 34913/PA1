
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//

bool CheckInput     ( int returnVal, int wantedVal );
void ErrorMsg       ( void );
int  AddUser        ( int id, database *db );
int  CalculateUsers ( int from, int to, database *db );

//

enum keysEnum
{
    addKey = '+',
    calKey = '?',
    defKey = ' ',
};

typedef struct 
{
    int *data;
    int len;
} database;

//

int main ( int argc, char * argv [] )
{
    char key        = defKey;
    long count      = 0;
    int r           = 0;
    database db;

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
                    || from < 0 || to >= count || from > to ) {
                free( db.data );
                ErrorMsg();
                return EXIT_FAILURE;
            }

            pritnf("%d / %d\n", CalculateUsers( from, to, &db ), to - from + 1 );
            
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

}

int CalculateUsers( int from, int to, database *db )
{

}
