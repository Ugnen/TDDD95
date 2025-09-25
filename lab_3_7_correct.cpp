/*
########[General Chinese Remainder Theorem LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to solve a general version of the CRT (aka a Systems of Congruences). This was solved using a modified version of the previously
used Extended Euclidean that now also computes the GCD, such that: ax + by = gcd(a,b), or in our case: jm + k*(-n) = gcd(m,n). With this version of the euclidean
we always solve the diofantine for a*x + b*y = 1 instead of a*x + by = c. After solving for j we then have to scale it to compensate for 'using' c = 1 when
it should be c = a-b. As our actual equation to solve is jm - kn = a - b. We therefore scale j with (a-b)/gcd(m,n), the gcd we get from the improved extEuclid function.
After we have the correctly scaled j (or k depending on if m or n is smaller) we simply calculate x = a - jm (mod(lcm(m,n)))  (or x = b - kn (mod(lcm(m,n))))

To use:
Input any two (linear) congurences into the generalTso function.

Regarding time complexity: As in Lab 3.5 and Lab 3.6 no requirement on time complexity was put, but worth noting that time complexity  for the extended euclidean
is O(log(min(m,n))). The function oflow(x, y, K) runs in O(log(y)) so it matters what y is here, but as we only solve x = a - jm mod(K) or
 x = b - kn mod(K) an if statement was added to make sure the smallest of n or m was enter in the oflow function. Thus guaranteeing: O(log(min(m,n))).
 Without this we could get a scenario where m > n and the extended euclidean runs in O(log(n)) and oflow in O(log(m)) -> Total: O(log(n)) + O(log(m)) =
 O(log(max(m,n))). std::lcm(m,n) runs in O(log(min(m,n))) time.


Additional notes: Lab 3.7 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.0, cleaned version, with comments + added gcd in extended euclidian + used correct overflow multiplication.....
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <bitset>
#include <numeric>
#include <cmath>

//addition of x and y mod(n)
int64_t mAdd(int64_t x, int64_t y, int64_t n){
    int64_t res{(x%n + y%n)%n};
    return res;
}

//subtraction of x and y mod(n)
int64_t mSub(int64_t x, int64_t y, int64_t n){
    int64_t x_n = x%n;
    int64_t y_n = y%n;

    if(x_n < y_n){
        return (x_n - y_n + n)%n;
    }else{
        return (x_n - y_n)%n;
    }
}

//multiplication of x and y mod(n)
int64_t mMult(int64_t  x, int64_t y, int64_t n){
    int64_t res{(x%n * y%n)%n};
    return res;
}

//https://cp-algorithms.com/algebra/extended-euclid-algorithm.html + previous iteration
//Extended Euclidian algorithm aimed to solve the diofantine equation of a*x + b*y = 1, where x and y are unknown and, a, b are constants
int64_t extEuclid(int64_t  a, int64_t  b, int64_t & x, int64_t & y){
    int64_t gcd{0}, x_prev{0}, y_prev{0};

    if(b == 0){
        x = 1;
        y = 0;
        gcd = a;

    }
    else{
        gcd = extEuclid(b, a%b, x_prev, y_prev);
        x = y_prev;
        y = x_prev - (a/b)*y_prev;
    }

    return gcd;
}

bool overflowCheck(int64_t a, int64_t b, int64_t k){
    int64_t c{mMult(a,b,k)};

    //checks if (a*b)mod(k)/b = a
    return !(a == (double)c/(double)b);

}

//Performs multiplication, checks for overflow (by bitshift) and recursively combines the solution to fit in an int64_t
int64_t oflow(int64_t x, int64_t y, int64_t K){

    int64_t b_i = y & 1;
    int64_t y_shift = y >> 1;

    if(y_shift && overflowCheck(x, y_shift, K)){//checks if either overflow or we have shifted the entire number
        return x*b_i + mMult(2,oflow(x, y_shift, K),K); //if overflow we go again, see pseudo above.
    }

    return mAdd(x*b_i, mMult(2, mMult(x,y_shift,K),K), K);
}

//General CRT
void generalTso(int64_t  a, int64_t m, int64_t b, int64_t n){
    int64_t M{std::lcm(n,m)};
    int64_t j{}, k{}, x{};


    //solve the diofantine jm - kn = a-b, except we solve: jm - kn = gcd(m,-n) and then scale j later
    int64_t scale_gcd = extEuclid(m, -n, j, k);

    //scale j
    j *= (a-b)/scale_gcd;
    k *= (a-b)/scale_gcd;

    //modulo operations: x = a - jm mod(M)
    if(m <= n){
        x = mSub(a,oflow(j,m,M),M);
    }else{
        x = mSub(b,oflow(k,n,M),M);
    }
    //x = mSub(a,oflow(j,m,M),M);

    std::cout << x << " " << M << "\n";
    return;
}

//Reads an int64 from stdin
int64_t  intRead(){

    //init data
    int64_t  data{0};

    //read a char
    char c = getc(stdin);

    //Read while char is between 0 - 9 [ASCII]
    while(c > 47 && c < 58){
        data *= 10;
        data += c - 48;
        c = getc(stdin);
    }

    return data;
}

int main(){

    //desync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int64_t T{}, a{},m{},b{},n{};
    T = intRead();

    while(T--){
        a = intRead();
        m = intRead();
        b = intRead();
        n = intRead();

        //"solvable if gcd(m, n) divides a - b, otherwise no solution exists"
        if((a-b) % std::gcd(m,n) != 0){

            std::cout << "no solution\n";
            continue;
        }

        generalTso(a, m, b, n);
    }

    return 0;
}
