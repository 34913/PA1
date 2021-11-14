
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//

bool CheckInput( int returnVal, int wantedVal );
void ErrorMsg( void );

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
    char key = ' ';
    long count = 0;
    printf("Pozadavky:\n");

    if( CheckInput( scanf( " %c", &key ), 1 ) 
            || ( key != addKey && key != calKey ) ) {
        ErrorMsg();
        return EXIT_FAILURE;
    }

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
