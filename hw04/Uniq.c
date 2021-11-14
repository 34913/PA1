
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//

bool CheckInput( int returnVal, int wantedVal );


//

int main ( int argc, char * argv [] )
{
    char key;
    printf("Pozadavky:\n");
    if( CheckInput( scanf("%c", &key ), 1 ) )
        return EXIT_FAILURE;





    return EXIT_SUCCESS;
}

bool CheckInput( int returnVal, int wantedVal )
{
    if( returnVal != wantedVal ) {
        printf("Nespravny vstup.\n");
        return true;
    }
    return false;
}
