/*
########[Chinese Remainder LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Task was to implement a function that solves the equations x = a(mod n) && x = b(mod m). This was done with the Extended Euclidian algorithm
implemented in Lab 3.5 and overflow multiplication. The overflow multiplication was solved by bit-wise multiplying and shifting one of the numbers. Where the
bit index is 'kept track off' and multiplied with 2 to match its original value.

Regarding time complexity: Time complexity should same as in Lab 3.5 for the Extended Euclidian algorithm.

Additional notes: Lab 3.6 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.1, : Made solution more general
Extra notes: Most of the time of this lab was spent on the overflow protection. I solved it in the end by recursion but would have liked to do it with a
simple for-loop.

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
#include <cmath>
#include <numeric>

//addition of x and y mod(n)
int64_t mAdd(int64_t x, int64_t y, int64_t n){
    int64_t res{(x%n + y%n)%n};
    return res;
}

//multiplication of x and y mod(n)
int64_t mMult(int64_t  x, int64_t y, int64_t n){
    int64_t res{(x%n * y%n)%n};
    return res;
}

//Extended Euclidian algorithm aimed to solve the diofantine equation of a*x + b*y = c, where x and y are unknown and, a, b and c are constants
std::pair<int64_t , int64_t > extEuclid(int64_t  a, int64_t  b, int64_t  c){
    int64_t  x{0}, y{0};
    if(b == 0){
        if(!(c%a)){
            x = c/a;
            y = 0;
        }

    }else{
        std::pair<int64_t , int64_t> x0_y0 = extEuclid(b, a%b, c);
        x = x0_y0.second;
        y =x0_y0.first-(a/b)*x0_y0.second;
    }

    return std::make_pair(x, y);
}

/*
Chinese remainder
say:
(x*y)%d = z
b_0(x) = / x = 5 = 101/ = 1;
--> remainder: 10 = 2
b_1(x) = /x = 5 = 101 / = 0;
--> remainder: 1 = 1
b_2(x) = /x = 5 = 101 / = 1;
--> remainder: 0 STOP
etc.


For solution:
(x*b_0(y))/d = z_0;

-> (x*b_i(y) + 2*z_(i-1))%d = z_i
*/

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

//takes 4 integers and returns the solution to:
// x = a mod(n)
// x = b mod(m)
int64_t CRT(int64_t a, int64_t n, int64_t b, int64_t m){

    int64_t K = n*m;

    std::pair<int64_t , int64_t> n1_n2 = extEuclid(n, m, 1);

        //check for negative inverses
    if(n1_n2.first < 0){
        n1_n2.first = (n1_n2.first%m + m)%m;
    }

    if(n1_n2.second < 0){
        n1_n2.second = (n1_n2.second%n + n)%n;
    }

    //does the multiplication: x = a*n2*m + b*n1*n mod(m*n)
    //with overflow protection
    int64_t x{mAdd(oflow(a, oflow(n1_n2.second,m,K), K), oflow(b, oflow(n1_n2.first,n,K), K), K)};

    return x;
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

    int64_t  T{}, a{}, n{}, b{}, m{}, K{};

    T = intRead();

    while(T--){
        a = intRead();
        n = intRead();
        b = intRead();
        m = intRead();

        K = n*m;

        std::cout << CRT(a, n, b, m) << " " << K << "\n";
    }

    return 0;
}
