#ifndef __PROGTEST__
#include <stdio.h>      // standard I/O
#include <stdlib.h>     // malloc, realloc, free
#include <stdbool.h>    // boolean true false logic values
#include <string.h>     // string functions
#include <ctype.h>      // isalpha, isupper, islower

#include <assert.h>

typedef struct TResult
{
  struct TResult * m_Next;
  int              m_ID;
  char           * m_Name;
} TRESULT;
#endif

//
// macros

#define type int

//
//  additional structs to save and use

typedef struct TRecord
{
  int              id;
  char           * name;

  struct TRecord * p1;
  struct TRecord * p2;

  bool             usedCommon;
} TRECORD;

typedef struct TBinary
{
  struct TBinary * left;
  struct TBinary * right;
  TRECORD        * person;
} TBINARY;

//
// main database

typedef struct TDatabase
{
  TBINARY * begin;
  type      length;
  type      newRecords;
} TDATABASE;

//
//  additional service functions
//  cause i dont know the "helping service functions" english word anymore

TBINARY **FindBinary       ( TDATABASE       * db,
                             int               id )
{
  TBINARY **help = &db->begin;
  while( *help != NULL ) {
    type idHelp = ( *help )->person->id;
    
    if( id == idHelp )
      break;
    else if( id < idHelp )
      help = &(( *help )->left);
    else
      help = &(( *help )->right);
  }
  return help;
}

void      FreeBinary       ( TBINARY         * item )
{
  if( item == NULL )
    return;
  FreeBinary( item->left );
  FreeBinary( item->right );
  free( item->person->name );
  free( item->person );
  free( item );
}

void      AddResult        ( TRESULT        ** end,
                             TRECORD         * person )
{
  if( person == NULL )
    return;

  TRESULT *newEnd = ( TRESULT* )malloc( sizeof( TRESULT ) );
  newEnd->m_Next = *end;
  newEnd->m_ID = person->id;
  newEnd->m_Name = person->name;

  *end = newEnd;

  AddResult( end, person->p1 );
  AddResult( end, person->p2 );

  return;
}

void      FindFirstCommon  ( TRECORD         * person )
{
  if( person == NULL )
    return;

  person->usedCommon = true;

  FindFirstCommon( person->p1 );
  FindFirstCommon( person->p2 );

  return ;
}

void      FindSecondCommon ( TRECORD         * person,
                             TRESULT        ** end )
{
  if( person == NULL )
    return;

  if( person->usedCommon )
    AddResult( end, person );
  else {
    FindSecondCommon( person->p1, end );
    FindSecondCommon( person->p2, end );
  }

  return;
}

void      UnsetFirstCommon ( TRECORD         * person )
{
  if( person == NULL )
    return;

  person->usedCommon = false;

  UnsetFirstCommon( person->p1 );
  UnsetFirstCommon( person->p2 );

  return ;
}

//
//  main database functions

void      initAll          ( TDATABASE       * db )
{
  db->length     = 0;
  db->newRecords = 0;
  db->begin      = NULL;
}

void      doneAll          ( TDATABASE       * db )
{
  FreeBinary( db->begin );
}

int       addPerson        ( TDATABASE       * db,
                             int               id,
                             const char      * name,
                             int               id1,
                             int               id2 )
{
  if( id == 0 )
    return 0;
  else if( id1 < 0 || id2 < 0 )
    return 0;
  else if( ( id1 == id2 && id1 != 0 ) || id == id1 || id == id2 )
    return 0;

  TBINARY **ptr = FindBinary( db, id );
  if( *ptr != NULL )
    return 0;

  //

  TRECORD *p1 = NULL;
  if( id1 != 0 ) {
    TBINARY **help = FindBinary( db, id1 );
    if( *help == NULL )
      return 0;
    p1 = ( *help )->person;
  }

  TRECORD *p2 = NULL;
  if( id2 != 0 ) {
    TBINARY **help = FindBinary( db, id2 );
    if( *help == NULL )
      return 0;
    p2 = ( *help )->person;
  }

  TRECORD *person    = ( TRECORD* )malloc( sizeof( TRECORD ) );
  if( person == NULL ) {
    return 0;
  }
  person->id         = id;
  person->p1         = p1;
  person->p2         = p2;
  person->usedCommon = false;
  person->name       = ( char* )malloc( sizeof( char ) * ( strlen( name ) + 1 ) );
  if( person->name == NULL )
    return 0;
  strcpy( person->name, name );

  TBINARY *newItem = ( TBINARY* )malloc( sizeof( TBINARY ) );
  if( newItem == NULL ) {
    free( person );
    return 0;
  }
  newItem->person  = person;
  newItem->left    = NULL;
  newItem->right   = NULL;

  *ptr = newItem;
  db->length ++;
  db->newRecords ++;

  return 1;
}

TRESULT * ancestors        ( TDATABASE       * db,
                             int               id )
{
  TRECORD *firstRec = ( *FindBinary( db, id ) )->person;
  if( firstRec == NULL )
    return NULL;
  TRESULT *list = NULL;

  AddResult( &list, firstRec->p1 );
  AddResult( &list, firstRec->p2 );

  return list;
}

TRESULT * commonAncestors  ( TDATABASE       * db,
                             int               id1,
                             int               id2 )
{
  TRECORD *firstRec = ( *FindBinary( db, id1 ) )->person;
  TRECORD *secondRec = ( *FindBinary( db, id2 ) )->person;
  if( firstRec == NULL || secondRec == NULL )
    return NULL;

  FindFirstCommon( firstRec->p1 );
  FindFirstCommon( firstRec->p2 );

  TRESULT *list = NULL;
  FindSecondCommon( secondRec->p1, &list );
  FindSecondCommon( secondRec->p2, &list );

  UnsetFirstCommon( firstRec->p1 );
  UnsetFirstCommon( firstRec->p2 );

  return list;
}

void      freeResult       ( TRESULT         * res )
{
  while( res != NULL ) {
    TRESULT *help = res;
    res = res->m_Next;
    free( help );
  }
}

//

#ifndef __PROGTEST__
int main                   ( int               argc,
                             char            * argv [] )
{
  char      name[100];
  TDATABASE a, b;
  TRESULT * l;

  initAll ( &a );
  assert ( addPerson ( &a, 1, "John", 0, 0 ) == 1 );
  strncpy ( name, "Jane", sizeof ( name ) );
  assert ( addPerson ( &a, 2, name, 0, 0 ) == 1 );
  assert ( addPerson ( &a, 3, "Caroline", 0, 0 ) == 1 );
  assert ( addPerson ( &a, 4, "Peter", 0, 0 ) == 1 );
  assert ( addPerson ( &a, 5, "George", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 6, "Martin", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 7, "John", 3, 4 ) == 1 );
  assert ( addPerson ( &a, 8, "Sandra", 3, 4 ) == 1 );
  assert ( addPerson ( &a, 9, "Eve", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 10, "Douglas", 1, 4 ) == 1 );
  strncpy ( name, "Phillipe", sizeof ( name ) );
  assert ( addPerson ( &a, 11, name, 6, 8 ) == 1 );
  strncpy ( name, "Maria", sizeof ( name ) );
  assert ( addPerson ( &a, 12, name, 5, 8 ) == 1 );
  l = ancestors ( &a, 11 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 6 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Martin" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );
  assert ( ancestors ( &a, 3 ) == NULL );
  assert ( ancestors ( &a, 13 ) == NULL );
  l = commonAncestors ( &a, 11, 12 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );
  l = commonAncestors ( &a, 10, 9 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next == NULL );
  freeResult ( l );
  assert ( commonAncestors ( &a, 7, 6 ) == NULL );
  l = commonAncestors ( &a, 7, 10 );
  assert ( l );
  assert ( l -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Name, "Peter" ) );
  assert ( l -> m_Next == NULL );
  freeResult ( l );
  assert ( addPerson ( &a, 13, "Quido", 12, 11 ) == 1 );
  l = ancestors ( &a, 13 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 5 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "George" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 6 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Martin" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 11 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Phillipe" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 12 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );
  l = commonAncestors ( &a, 9, 12 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next == NULL );
  freeResult ( l );
  assert ( addPerson ( &a, 1, "Francois", 0, 0 ) == 0 );
  initAll ( &b );
  assert ( addPerson ( &b, 10000, "John", 0, 0 ) == 1 );
  assert ( addPerson ( &b, 10000, "Peter", 0, 0 ) == 0 );
  assert ( addPerson ( &b, 20000, "Jane", 10000, 0 ) == 1 );
  assert ( addPerson ( &b, 30000, "Maria", 10000, 10000 ) == 0 );
  assert ( addPerson ( &b, 40000, "Joe", 10000, 30000 ) == 0 );
  assert ( addPerson ( &b, 50000, "Carol", 50000, 20000 ) == 0 );
  assert ( addPerson ( &b, 12, "Maria", 20000, 10000 ) == 1 );
  l = ancestors ( &a, 12 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 5 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "George" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );
  l = ancestors ( &b, 12 );
  assert ( l );
  assert ( l -> m_ID == 10000 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 20000 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next == NULL );
  freeResult ( l );
  assert ( ancestors ( &a, 20000 ) == NULL );
  assert ( commonAncestors ( &b, 7, 10 ) == NULL );
  doneAll ( &b );

  doneAll ( &a );

  return 0;
}
#endif

//