
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//

bool CheckInput( int returnVal, int wantedVal );
void ErrorMsg( void );
void AddUser( int id, int *database );
void CalculateUsers( int from, int to, int *database );

//

enum keysEnum
{
    addKey = '+',
    calKey = '?',
    defKey = ' ',
};

//

int main ( int argc, char * argv [] )
{
    char key        = defKey;
    long count      = 0;
    int r           = 0;
    int *database   = NULL;

    printf("Pozadavky:\n");

    while( true ) {
        r = CheckInput( scanf( " %c", &key ), 1 );
            
        if( r || ( key != addKey && key != calKey ) )
            break;

        if( key == addKey ) {
            int id = 0;

            if( CheckInput( scanf( " %d", &id ), 1 ) 
                    || id < 0 || id > 99999 ) {
                ErrorMsg();
                return EXIT_FAILURE;
            }

        }
        else if( key == calKey ) {
            int from = 0;
            int to = 0;

            if( CheckInput( scanf( " %d %d", &from, &to ), 2 )
                    || from < 0 || to >= count || from > to ) {
                ErrorMsg();
                return EXIT_FAILURE;
            }

        }
    }

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

void AddUser( int id, int *database )
{

}

void CalculateUsers( int from, int to, int *database )
{

}
