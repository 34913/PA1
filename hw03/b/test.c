#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

// 1 - 10
long DigitChange(int from, int to, int digitChange[]) {
  long sum = 0;

  for(int i = from % 10; i < to; i++) 
    sum += digitChange[i];

  printf("%ld\n", sum);
  return sum;
}

// 1 - 60
void Minutes(int from, int to, int digitChange[], long long int *consumption) {
  if(from == to)
    return;
  
  long sum = 0;

  int fromOne = from / 10;
  int fromTwo = from % 10;

  int toOne = to / 10;
  int toTwo = to % 10;

  if(toOne - fromOne > 1) {
    sum += DigitChange(fromOne + 1, toOne, digitChange);
    sum += DigitChange(1, 10, digitChange) * (toOne - fromOne - 1);

    printf("%ld %ld\n", toOne - fromOne - 1, sum);
  }

  if(fromOne == toOne)
    sum += DigitChange(fromTwo + 1, toTwo + 1, digitChange);
  else {
    sum += DigitChange(fromTwo + 1, 10, digitChange);
    sum += DigitChange(1, toTwo, digitChange);
  }
  
  *consumption = sum;
}

void Hours(int from, int to, int digitChange[], long long int *consumption) {
  if(from == to)
    return;
  
  long sum = 0;

  int fromOne = from / 10;
  int fromTwo = from % 10;

  int toOne = to / 10;
  int toTwo = to % 10;

  if(toOne - fromOne > 1) {
    sum += DigitChange(fromOne + 1, toOne, digitChange);
    sum += DigitChange(1, 10, digitChange) * (toOne - fromOne - 1);

    printf("%ld %ld\n", toOne - fromOne - 1, sum);
  }

  if(fromOne == toOne)
    sum += DigitChange(fromTwo + 1, toTwo + 1, digitChange);
  else {
    sum += DigitChange(fromTwo + 1, 10, digitChange);
    sum += DigitChange(1, toTwo, digitChange);
  }
  
  *consumption = sum;
}

int energyConsumption ( int y1, int m1, int d1, int h1, int i1,
                        int y2, int m2, int d2, int h2, int i2, long long int * consumption )
{
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

  long hours = 0;
  long minutes = 0;

  *consumption = 0;

  int digit[10] = { 6, 2, 5, 5, 4, 5, 6, 3, 7, 6 };
  int digitChange[10] = { 2, 4, 5, 2, 3, 3, 1, 4, 4, 1 };
  int sum = 0;
  for(int i = 0; i < 10; i++)
    sum += digitChange[i];

  Minutes(i1, i2, digitChange, consumption);
  

  printf("%lld\n", *consumption);

  return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  long long int consumption;

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
