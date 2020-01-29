#include "prime.h"

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

bool isPrime(unsigned int x)
{
    bool ret = true;

    unsigned int rootX = sqrt(x);
    if (x == 1)
        ret = false;
    else if (x%2 == 0 && x != 2)
        ret = false;
    else{
        for(int i = 3; i <= rootX; i = i+2)
        {
            if (x%i == 0)
            {
                ret = false;
                break;
            }
        }
    }
    return ret;
}

unsigned int GetPrimeLargerThan(unsigned int x)
{
    if (x == 0)
    {
        return 2;
    }

    if ((x%2) == 0)
        x--;
    
    do {
        x = x + 2;
    } while (!isPrime(x));

    return x;
}

//return 0 ~ x - 1
unsigned int GetRandomValue(unsigned int x)
{
    srand(time(NULL));
    return rand() % x; 

}