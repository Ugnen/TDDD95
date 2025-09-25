/*
########[Modular Arithmetic LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Implement modular arithmetic (addition, subtraction, multiplication and division (modular inverse) modulo n). This was done with the functions
mAdd, mSub, mMult, and mDiv, where the division is a special case as I had to implement modular inverse and the Extended Euclidian algorithm. The Extended
Euclidian algorithm solves Diofantine equations, a*x + b*y = c, where x and y are unknown and a,b,c are constants. One could most likely improve the
implementation of the Extended Euclidian algo as c = 1 always here, but I've left it  in as in a future use case a Diofantine equation to be solved
might not have c = 1;

Regarding time complexity: No special requirments were put on the time complexity for this assignment. However since the Extended Euclidian algortihm is
used it is worth mentioning that it has compexity O(log(min(a, b))), where a and b are the integers used (in this case the a = y and b = modulo number n).

Additional notes: Lab 3.5 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.0, first version
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
#include <numeric>
#include <utility>

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
int64_t mMult(int64_t x, int64_t y, int64_t n){
    int64_t res{(x%n * y%n)%n};
    return res;
}

//Extended Euclidian algorithm aimed to solve the diofantine equation of a*x + b*y = c, where x and y are unknown and, a, b and c are constants
std::pair<int64_t, int64_t> extEuclid(int64_t a, int64_t b, int64_t c){
    int64_t x{0}, y{0};
    if(b == 0){
        if(!(c%a)){
            x = c/a;
            y = 0;
        }

    }else{
        std::pair<int64_t, int64_t> x0_y0 = extEuclid(b, a%b, c);
        x = x0_y0.second;
        y =x0_y0.first-(a/b)*x0_y0.second;
    }

    return std::make_pair(x, y);
}


//division of x and y mod(n)
int64_t mDiv(int64_t x, int64_t y, int64_t n){

    //if no gdc for y and n exists, we cannot divide them:
    if(std::gcd(y,n) == 1){
        int64_t y_inv = extEuclid(y, n, 1).first;

        if(y_inv < 0){
            //same as mSub
            y_inv = (y_inv%n + n)%n;
        }

        return (x%n * y_inv)%n;

    }else{
        return -1;
    }
}


int main(){

    //Desync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int64_t n{},t{},x{},y{};
    char op{};

    std::cin >> n >> t;
    while(n && t){
        while(t--){
            std::cin >> x >> op >> y ;

            if(op == '+'){
                std::cout << mAdd(x,y,n);

            }else if(op == '-'){
                std::cout << mSub(x,y,n);

            }else if(op == '*'){
                std::cout << mMult(x,y,n);

            }else if(op == '/'){
                std::cout << mDiv(x,y,n);
            }

            std::cout << "\n";
        }

        std::cin >> n >> t;
    }


    return 0;
}
