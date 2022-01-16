// libraries

#include <stdio.h>      // standard I/O
#include <stdbool.h>    // boolean values
#include <stdlib.h>     // malloc, exit

// global variables

typedef enum {
    inputError = 0,
    cantError = 1,
} errors;

typedef struct {
    double heigth;
    double width;
} rectangle;

// prototyping

bool CheckInput(int returnValue, int neededInput);
bool NotInLimit(rectangle r);
void ErrorMsg(errors e);
//void Swap(rectangle *r);
double Add(double side, double avaiable, double overlay);
int Cycles(double wanted, double avaiable, double overlay);

// main and functions body

int main(void)
{
    double overlay = 0;
    rectangle latka, plachta;

    printf("Velikost latky:\n");
    if(CheckInput(scanf("%lf %lf", &latka.width, &latka.heigth), 2)
        || NotInLimit(latka))
        ErrorMsg(inputError);

    printf("Velikost plachty:\n");
    if(CheckInput(scanf("%lf %lf", &plachta.width, &plachta.heigth), 2) 
        || NotInLimit(plachta))
        ErrorMsg(inputError);

    if(plachta.width > latka.width || plachta.heigth > latka.heigth){
        printf("Prekryv:\n");
        if(CheckInput(scanf("%lf", &overlay), 1) || overlay < 0)
            ErrorMsg(inputError);
    }

    //int arr[2] = { Add(latka.heigth, latka.heigth, overlay), 
    //               Add(latka.width, latka.width, overlay) };
    //if(arr[0] <= 0 && latka.heigth != plachta.heigth )

    int piecesOne = Cycles(plachta.width, latka.width, overlay) *
                    Cycles(plachta.heigth, latka.heigth, overlay);
    int piecesTwo = Cycles(plachta.width, latka.heigth, overlay) *
                    Cycles(plachta.heigth, latka.width, overlay);
    
    if(piecesOne == 0 && piecesTwo == 0)
        ErrorMsg(cantError);
    else
        printf("Pocet kusu latky: %d\n", piecesTwo == 0 || (piecesOne <= piecesTwo && piecesOne != 0) ? piecesOne : piecesTwo);
    return EXIT_SUCCESS;
}

// returns true if input is wrong or the input is closed for reading
bool CheckInput(int returnValue, int neededInput)
{
    return (returnValue != neededInput || returnValue == EOF);
}

// checks whether the value is within the limits
bool NotInLimit(rectangle r) 
{
    return r.width <= 0 || r.heigth <= 0;
}

// prints error message and quits the program entirely
void ErrorMsg(errors e)
{
    if(e == inputError)
        printf("Nespravny vstup.\n");
    else if(e == cantError)
        printf("Nelze vyrobit.\n");
    exit(EXIT_FAILURE);
}

/*
// swaps the values in two sides
void Swap(rectangle *r)
{
    int height = r->heigth;
    r->heigth = r->width;
    r->width = height;
}
*/

// returns value of side added with available and subtracted overlay
double Add(double side, double avaiable, double overlay)
{
    if(side == 0)
        return avaiable;
    return side + avaiable - overlay;
}

// returns the count of used available size to fill the wanted size
int Cycles(double wanted, double avaiable, double overlay)
{
    int count = 0;
    double size = 0;

    while(wanted > size) {
        count++;
        size = Add(size, avaiable, overlay);
        
        if(size <= 0)
            return 0;
    }
    return count;
}
