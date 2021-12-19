#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean true false logic values
#include <string.h>     // string functions
#include <ctype.h>      // isalpha, isupper, islower

//

typedef struct TResult
{
  struct TResult * m_Next;
  int              m_ID;
  char           * m_Name;
} TRESULT;

typedef struct TDatabase
{
  /* todo */
} TDATABASE;

//

void      initAll          ( TDATABASE       * db );
void      doneAll          ( TDATABASE       * db );
int       addPerson        ( TDATABASE       * db,
                             int               id,
                             const char      * name,
                             int               id1,
                             int               id2 );
TRESULT * ancestors        ( TDATABASE       * db,
                             int               id );
TRESULT * commonAncestors  ( TDATABASE       * db,
                             int               id1,
                             int               id2 );
void      freeResult       ( TRESULT         * res );

//

int main ( void )
{







    return;
}
