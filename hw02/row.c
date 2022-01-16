// libraries

#include <stdio.h>      // standard I/O
#include <stdbool.h>    // boolean values
#include <stdlib.h>     // malloc, exit
#include <math.h>
#include <float.h>
#include <cstring>

// global variables

typedef enum {
    inputError = 0,
} errors;

typedef struct {
    long count;
    long sum;
    
} order;

double deviation = 1024 * DBL_EPSILON;

// prototyping

bool CheckInput(int returnValue, int neededInput);
void ErrorMsg(errors e);

// main and functions body

int main(void)
{

    printf("Pozice a soustava:\n");
    int r;
    while(true) {
        long position;
        int system;

        r = scanf(" %ld %d", &position, &system);
        if(CheckInput(r, 2) || system < 2 || system > 36)
            break;

        int *numberArr = NULL;
        order *arr = NULL;

        if(position == 0) {
            printf("0\n^\n");
            continue;
        }

        long high = system;
        long low = 1;
        long digits = 1;
        arr = (order*)malloc(sizeof(order));
        arr[0].count = 9;
        arr[0].sum = 0;

        long sum = arr[0].count;

        while (position > sum) {
            low *= 10;
            high *= system;

            digits++;
            order *help = (order*)realloc(arr, sizeof(order) * digits);
            if(help == NULL) {
                printf("allocation problem\n");
                free(arr);
                return EXIT_FAILURE;
            }
            arr = help;
            arr[digits - 1].count = high - low;
            arr[digits - 1].sum = sum;

            sum = arr[digits - 1].count * digits + arr[digits - 1].sum;
        }
        
        /*
        printf("%ld %ld %ld\n", low, high, digits);
        for(int i = 0; i < digits; i++)
            printf("%ld %ld .. ", arr[i].count, arr[i].sum);
        printf("\n\n");
        */

        numberArr = (int *)malloc(sizeof(int) * digits);
        if(numberArr == NULL) {
            printf("allocation problem\n");
            return EXIT_FAILURE;
        }
        for(int i = 0; i < digits; i++)
            numberArr[i] = 0;

        long inv = digits;

        // position -= position % digits;
        // kdyby byl problem
        // zbytkove pozice, ktere se stejne jakoby zapocitaji na konci jako pozice toho daneho cisla

        //printf("%ld\n", position);
        while(position >= digits && inv != 1) {
            
            if(arr[inv - 1].sum + 1 <= position && position <= arr[inv - 1].count * digits + arr[inv - 1].sum) {
                for(int i = 0; i < system - 1; i++) {
                    low = ((arr[inv - 1].count / (system - 1)) * i) * digits + arr[inv - 1].sum + 1;
                    high = ((arr[inv - 1].count / (system - 1))  * (i + 1)) * digits + arr[inv - 1].sum;
                    //printf("%ld - %ld\n", low, high);

                    if(low <= position && position <= high) {
                        numberArr[abs(inv - digits)] = i + 1;

                        position -= low - 1;
                        break;
                    }
                }
            }
            //printf("%ld %d\n", position, numberArr[abs(inv - digits)]);

            inv--;
        }
        if(position > digits) {
            for(int i = 9; i >= 0; i--){
                if(position - (i * digits) >= 0) {
                    numberArr[digits - 1] = i;
                    position -= i * digits;
                    break;
                }
            }
        }

        for(int i = 0; i < digits; i++)
            printf("%d", numberArr[i]);
        putc('\n', stdout);
        for(int i = 0; i < position - 1; i++)
            putc(' ', stdout);
        printf("^\n");

        free(numberArr);
        free(arr);
    }

    if(r != EOF)
        ErrorMsg(inputError);

    return EXIT_SUCCESS;
}

// returns true if input is wrong or the input is closed for reading
bool CheckInput(int returnValue, int neededInput)
{
    return (returnValue != neededInput || returnValue == EOF);
}

// prints error message and quits the program entirely
void ErrorMsg(errors e)
{
    switch(e) {
    case inputError:
        printf("Nespravny vstup.\n");
        break;
    }
    exit(EXIT_FAILURE);
}
