#include<math.h>

#include "prime.h"

/* Return whether x is prime or not

Returns: 
    1 == prime

    0 == not prime
    
    -1 == undefined
*/

int is_prime(const int x) {
    if (x < 2){ return -1;} // prime numbers greater than 1
    if (x < 4){ return 1;} // 2 and 3 are prime
    if((x % 2)==0) { return 0; } //if divisible by 2 not prime
    for (int i = 3; i <= floor(sqrt((double) x)); i+=2){
        if ((x % i)==0) {
            return 0;
        }
    }
    return 1;
}

/*the for loop starts from 3 and adds 2 each time to check all 
the odd numbers to see if they are prime. it continues 
until i is greater than the square root of x*/

/* return the next prime after x, or if x is prime*/

int next_prime(int x) {
    while (is_prime(x) !=1){
        x++;
    }
    return x;
}