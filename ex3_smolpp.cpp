#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <stdlib.h>

bool checkInt(double b, double epsilon){ //if smaller it is an int, b should be an integer not a decimal number
    bool checkVal = (abs(floor(b+epsilon) - b) < epsilon);
    return checkVal;
}

double pthRoot(long long int x,double p){
    double val = pow(x, 1/p);
    return val;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long int x{}; //x and p are always integers, p is atleast 1.
    double b{}, epsilon{0.0000000001},test_p{}; //from ex_12tritri.cpp, for testing if  b is an int
    std::cin >> x; //get number x, can be negative aswell

    while(x){//if x = 0, last input has been handled
        long long int p{1};
        if(x > 0){ //Positive x
            //std::cout << "kommer dit\n";
            test_p = 2; //start on 2 as p = 1 will be solved by checking size
            b = pthRoot(x, test_p); //perform p:th-root

            //while((floor(b + epsilon) - 2) >= 0){//checksize magnitude atleast 2 -> 4 is smallest where p isn't 1.
            while(b >= 2){//checksize magnitude atleast 2 -> 4 is smallest where p isn't 1.
                //std::cout << "kommer hit\n";
                if(checkInt(b, epsilon)){ //check if int or scuffed float/double after p:th-root
                    p = test_p; //if this happens next pth-root will be smaller than 2
                }

                test_p++; //iterate for more p
                b = pthRoot(x, test_p); //perform p:th-root again with next iteration
            }

        }else{ //Negative x, p can only be odd numbers
            //x = -x;
            test_p = 3; //start on 3 as p = 1 will solve by size of check
            b = pthRoot(-x, test_p); //perform p:th-root
            //while((floor(b + epsilon) - 2) >= 0){//checksize magnitude atleast 2 -> 4 is smallest where p isn't 1.
            while(b >= 2){//checksize magnitude atleast 2 -> 4 is smallest where p isn't 1.

                if(checkInt(b, epsilon)){ //check if int or scuffed float/double after p:th-root
                    p = test_p;
                }

                test_p += 2; //Jump two to stay on odd
                b = pthRoot(-x, test_p); //perform p:th-root again
            }
        }
        std::cout << p << "\n";
        std::cin >> x; //Finish by reading next number
    }

    return 0;
}
