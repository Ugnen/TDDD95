#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

int main(){
    //std::ios_base::sync_with_stdio(false);
    //std::cin.tie(NULL);

    int64_t n, two_pow{0}, five_pow{0}, N{1}, M, max_int{1000};
    std::cin >> n; //Use factorial representation as: N*pow(2,x)*pow(5,y), N non-divisable by 2/5;

    for(int i = 1; i<=n; i++){
        M = i;
        while(!(M%2)){
            M /= 2;
            two_pow++;
        }

        while(!(M%5)){
            M /= 5;
            five_pow++;
        }

        N *= M;
        N %= max_int;
    }

    // only want last three digits: (N*pow(2,two_pow-five_pow))%1000

    int64_t output{N};
    int64_t power = two_pow - five_pow;
    while(power--){
        output *= 2;
        output %= max_int;
    }
    output %= max_int;

    //print
    if(n < 7){ //7! factorial is first number that is more than 3 digits
        std::cout << output;
    }else{
        printf("%03lld", output); //will print 3 digits even if trailing zeros are >3
    }

    return 0;
}
