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
        else if(position < 0) {
            break;
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

        numberArr = (int *)malloc(sizeof(int) * digits);
        if(numberArr == NULL) {
            printf("allocation problem\n");
            return EXIT_FAILURE;
        }
        for(int i = 0; i < digits; i++)
            numberArr[i] = 0;


        // kdyby byl problem
        // zbytkove pozice, ktere se stejne jakoby zapocitaji na konci jako pozice toho daneho cisla
        position -= arr[digits - 1].sum;
        if(digits != 1) {
            for(int i = 0; i < system - 1; i++){

                low = ((arr[digits - 1].count / (system - 1)) * i) * digits + 1;
                high = ((arr[digits - 1].count / (system - 1))  * (i + 1)) * digits;
                
                if(low <= position && position <= high) {
                    numberArr[0] = i + 1;
                    position -= low;
                    break;
                }
            }
            long inv = digits - 1;

            if(position >= arr[1].sum * digits + 1 && inv != 1) {
                
                position -= 2;
                while(position >= arr[1].sum + 1 && inv != 1) {
                    
                    if(position <= arr[inv - 1].sum) {
                        inv--;
                        continue;
                    }
                    else
                        position -= arr[inv - 2].count * digits;

                    if(1 <= position && position <= arr[inv - 1].count * digits) {
                        for(int i = 0; i < system - 1; i++) {
                            low = ((arr[inv - 1].count / (system - 1)) * i) * digits + 1;
                            high = ((arr[inv - 1].count / (system - 1))  * (i + 1)) * digits;

                            if(low <= position && position <= high) {
                                if(inv == digits - 1)
                                    i++;
                                numberArr[abs(inv - digits)] = i;
                                position -= low;
                                break;
                            }
                        }
                    }
                    inv--;
                }
            }
        }
        if(position >= digits) {
            for(int i = 1; i < system; i++) {
                if(i * digits <= position && position < (i + 1) * digits) {
                    numberArr[digits - 1] = i;
                    position = position - i * digits;
                    break;
                }
            }
        }

        for(int i = 0; i < digits; i++)
            printf("%d", numberArr[i]);
        putc('\n', stdout);
        for(int i = 0; i < position; i++)
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
