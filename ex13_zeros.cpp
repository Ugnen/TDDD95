#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

#include <fstream>

unsigned long long int zerosOfNines(unsigned long long int number){
    unsigned long long int zero_count{1};
    int counter{0};
    while(number){
        number /= 10;
        zero_count += number*pow(10,counter);
        ++counter;
    }
    return zero_count;
}

unsigned long long int tenSum(unsigned long long int exponent){
    unsigned long long int series_sum{0};
    for(int i = 1; i <= exponent; ++i){
        series_sum += pow(10, i);
    }
    return series_sum;
}

unsigned long long int calcZeros(unsigned long long int number){
    unsigned long long int zeros{0};
    int mod_num{0};
    int counter{0};
    unsigned long long int rest{0};
    unsigned long long int rest_cntr{0};
    if(number == 0){zeros++;}
    while(number){
        mod_num = number%10;
        number/=10;
        ++counter;
        //std::cout << "counter : " << counter << " modnum " << mod_num << "\n";
        if(counter == 1){
            zeros += 1;
            rest += mod_num*pow(10,rest_cntr);
            ++rest_cntr;
            continue;
        }

        if(number == 0){
            zeros += zerosOfNines(pow(10,counter-1)-1);

            zeros += (mod_num-1)*(tenSum(counter-2) + zerosOfNines(pow(10,counter-1)-1));

            return zeros;
        }

        if(mod_num){
            rest += mod_num*pow(10,rest_cntr);
            ++rest_cntr;

            zeros += pow(10,counter-1); //first number
            zeros += mod_num*(zerosOfNines(pow(10,counter-1)-1));
            for(int i = 1; i < counter - 1; ++i){
                zeros += mod_num*pow(10,i);
            }
        }else{
            ++rest_cntr;
            //std::cout << "befo: " << zeros << " rest: " << rest << "\n";
            zeros += rest +1;//*1; //first number
        }
    }
    return zeros;
}


int main(void){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long int m, n; //can be very large real numbers
    //std::ofstream myfile;
    //myfile.open ("output.txt");

    while(1){//max 15000 cases
        //scanf("%lld%lld", &m, &n);
        //scanf("%lld", &m);
        std::cin >> m >> n;
        if(m<0){
            break;
        } //break when -1 is input
        unsigned long long int totalZeros{0};

        if(m == 0){
            totalZeros = calcZeros(n); //only value on n
        }else{
            totalZeros = calcZeros(n) - calcZeros(m-1);
        }
        //totalZeros = calcZeros(m-1);

        //unsigned long long int zerosM{0};
        //zerosM = calcZeros(m);
        //unsigned long long int sum = tenSum(3);

        std::cout <<  totalZeros << "\n";


        //myfile << totalZeros << "\n";

    }
    //myfile.close();
}
