#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#endif /* __PROGTEST__ */


// decides if year is leap
bool IsLeap( int year ) {
  if( year % 4000 == 0)
    return false;
  if( year % 400 == 0 )
    return true;
  if( year % 100 == 0 )
    return false;
  if( year % 4 == 0 )
    return true;
  return false;
}

int energyConsumption ( int y1, int m1, int d1, int h1, int i1,
                        int y2, int m2, int d2, int h2, int i2, long long int * consumption )
{
  if( consumption == NULL )
    return 0;

  *consumption = 0;

  if(y1 < 1600 || y2 < 1600)
    return 0;
  if(m1 < 1 || m1 > 12 || m2 < 1 || m2 > 12)
    return 0;
  if(d1 < 1 || d1 > 31 || d2 < 1 || d2 > 31)
    return 0;
  if(h1 < 0 || h1 > 23 || h2 < 0 || h2 > 23)
    return 0;
  if(i1 < 0 || i1 > 59 || i2 < 0 || i2 > 59)
    return 0;

  if( IsLeap( y1 ) ) {
    if( m1 == 2 && d1 > 29 )
      return 0;
  }
  else if( m1 == 2 && d1 > 28 )
    return 0;

  if( IsLeap( y2 ) ) {
    if( m2 == 2 && d2 > 29 )
      return 0;
  }
  else if( m2 == 2 && d2 > 28 )
    return 0;

  if( y1 == y2 && m1 == m2 && d1 == d2 && h1 == h2 && i1 == i2 )
    return 1;

  char digitChange[ 10 ] = { 2, 4, 5, 2, 3, 3, 1, 5, 4, 1 };
  char allTen = 0;
  for( int i = 0; i < 10; i++ )
    allTen += digitChange[i];

  long long int days = 0;
  long long int hours = 0;
  long long int minutes = 0;

  long long int sum = 0;
  char months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  int twoZero = 3;
  int threeZero = 3;
  int fiveZero = 3;

// years

  if( y1 > y2 )
    return 0;
  else {
    for ( int i = y1 + 1; i < y2; i++ ) {
      days += 365;
      if( IsLeap( i ) )
        days++;
    }
  }

// months

  if( y1 == y2 ) {
    if( m1 > m2 )
      return 0;
    else {
      if( m1 + 1 <= 2 && 2 < m2 && IsLeap( y1 ) )
        days ++;
      for( int i = m1 + 1; i < m2; i++ )
        days += months[i - 1];
    }
  }
  else { 
    for( int i = m1 + 1; i <= 12; i++ )
      days += months[i - 1];
    for( int i = 1; i < m2; i++ )
      days += months[i - 1];

    if( m1 + 1 <= 2 && IsLeap( y1 ) )
        days ++;
    if( 2 < m2 && IsLeap( y2 ) )
        days ++;
  }
  
// days

  if( m1 == m2 && y1 == y2 ) {
    if( d1 > d2 )
      return 0;
    else {
      for( int i = d1 + 1; i < d2; i++ ) 
        days += 1;
    }
  }
  else {
    if( IsLeap( y1 ) && m1 == 2 )
      days ++;
    for( int i = d1 + 1; i < months[m1 - 1]; i++ )
      days ++;
    for( int i = 1; i <= d2; i++ )
      days ++;
  }

// hours

  if( d1 == d2 && m1 == m2 && y1 == y2 ) {
    if( h1 > h2 )
      return 0;
    else if( h1 != h2 ) {
      if( h1 / 10 == h2 / 10 ) {
        for( int i = h1 % 10; i < h2 % 10; i++ ) 
          sum += digitChange[ i + 1 ];
      }
      else {
        for( int i = h1 % 10 + 1; i <= 9; i++ )
          sum += digitChange[ i ];
        for( int i = h1 / 10 + 1; i < h2 / 10; i++)
          sum += allTen + digitChange[ i + 1 ];
        for( int i = 0; i <= h2 % 10; i++ )
          sum += digitChange[ i ];

        sum += digitChange[ h1 / 10 + 1 ];
      }
      hours = h2 - h1 - 1;
    }
  }
  else {
    if( h1 >= 20 ) {
      for( int i = h1 % 10 + 1; i <= 3; i++ )
        sum += digitChange[ i ];
    }
    else {
      if( h1 < 10 ) {
        sum += allTen;
        sum += digitChange[1];
      }

      for( int i = h1 % 10 + 1; i <= 9; i++)
        sum += digitChange[ i ];
      for( int i = 0; i <= 3; i++)
        sum += digitChange[ i ];
      sum += digitChange[2];
    }

    for( int i = 1; i <= h2 / 10; i++ )
        sum += allTen + digitChange[ i ];
    for( int i = 1; i <= h2 % 10; i++ )
      sum += digitChange[ i ];

    sum += twoZero + threeZero;

    hours = 23 - h1 + h2;
  }

// minutes

  if( h1 == h2 && d1 == d2 && m1 == m2 && y1 == y2 ) {
    if( i1 > i2 )
      return 0;
    else if( i1 != i2 ) {
      if( i1 / 10 == i2 / 10 ) {
        for( int i = i1 % 10 + 1; i <= i2 % 10; i++ )
          sum += digitChange[ i ];
      }
      else {
        for( int i = i1 % 10 + 1; i <= 9; i++ )
          sum += digitChange[ i ];
        for( int i = i1 / 10 + 2; i <= i2 / 10; i++)
          sum += allTen + digitChange[ i ];
         for( int i = 0; i <= i2 % 10; i++ )
          sum += digitChange[ i ];

        sum += digitChange[ i1 / 10 + 1 ];
      }

      minutes = i2 - i1;
    }
  }
  else {

    for( int i = i1 / 10 + 1; i <= 5; i++ )
      sum += allTen + digitChange[ i ];
    for( int i = i1 % 10 + 1; i <= 9; i++ )
      sum += digitChange[ i ];
    for( int i = 1; i <= i2 / 10; i++ )
      sum += allTen + digitChange[ i ];
    for( int i = 0; i <= i2 % 10; i++ )
      sum += digitChange[ i ];

    sum += fiveZero;

    minutes = 60 - i1 + i2;

  }

// the rest

  sum += days * 292886; // const for whole days
  sum += minutes * 200; // const for whole minutes
  sum += hours * 12200; // cosnt for whole hours

  //printf("%lld\n%lld %lld\n%lld\n\n", days, hours, minutes, sum);

  *consumption = sum;

  return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  long long int consumption;

  //

  assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  1, 18, 45, &consumption ) == 1
           && consumption == 67116LL );
  assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  2, 11, 20, &consumption ) == 1
           && consumption == 269497LL );
  assert ( energyConsumption ( 2021,  1,  1, 13, 15,
                               2021, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
  assert ( energyConsumption ( 2024,  1,  1, 13, 15,
                               2024, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81398919LL );
  assert ( energyConsumption ( 1900,  1,  1, 13, 15,
                               1900, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
  assert ( energyConsumption ( 2021, 10,  1,  0,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 146443LL );
  assert ( energyConsumption ( 2021, 10,  1,  0, 15,
                               2021, 10,  1,  0, 25, &consumption ) == 1
           && consumption == 2035LL );
  assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 0LL );
  assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  1, &consumption ) == 1
           && consumption == 204LL );
  assert ( energyConsumption ( 2021, 11,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2021, 10, 32, 12,  0,
                               2021, 11, 10, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2100,  2, 29, 12,  0,
                               2100,  2, 29, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2400,  2, 29, 12,  0,
                               2400,  2, 29, 12,  0, &consumption ) == 1
           && consumption == 0LL );

  return 0;
}
#endif /* __PROGTEST__ */
