#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <unordered_map>

int main(void)
{
    std::string input = "";
    std::string strM = "";
    std::string strN = "";
    std::getline(std::cin,input);
    std::stringstream ss(input);
    std::getline(ss, strM, ' ');
    std::getline(ss, strN, ' ');
    unsigned long long int M = stoi(strM);
    unsigned long long int N = stoi(strN);
    unsigned long long int candy_deficit = 0;
    unsigned long long int anger = 0;
    //std::cout << "M: " << M << " N: " << N << "\n";
    std::vector<unsigned long long int> candies_want;
    std::unordered_map<unsigned long long int,unsigned long long int> candy_map;
    while(N--)
    {
        unsigned long long int want;
        std::cin >> want;
         //collect all the candy needs in a vector
        candy_deficit += want;
        if(!candy_map.count(want))
        {
            candy_map[want] = 1;
            candies_want.push_back(want);
        }
        else
        {
            candy_map[want] += 1;
        }
    }
    candy_deficit -= M;
    N = stoi(strN);

    sort(candies_want.begin(), candies_want.end());
    unsigned long long int largest_want_cnt = 0;
    while(M>0)
    {
        if(M >= candy_map[candies_want.back()])
        {
            largest_want_cnt = candy_map[candies_want.back()];
            M -= largest_want_cnt;
            if(!candy_map.count(candies_want.back()-1))  //One less than the highest want doesnt exist, create it
            {
                unsigned long long int lastWant = candies_want.back()-1;
                candy_map[lastWant] = largest_want_cnt;
                candies_want.pop_back();
                candies_want.push_back(lastWant);
            }
            else
            {
                candy_map[candies_want.back()-1] += largest_want_cnt;
                candies_want.pop_back();
            }
        }
        else
        {
            //std::cout << "gets here\n";
            candy_map[candies_want.back()] -= M;
            if(!candy_map.count(candies_want.back()-1))
            {
                candy_map[candies_want.back()-1] = M;
                candies_want.push_back(candies_want.back()-1);
            }
            else
            {
                candy_map[candies_want.back()-1] += M;
            }
            M = 0;
        }
    }
    unsigned long long int children = candies_want.size();
    //std::cout << "children: " << children << "\n";
    //std::cout << "anger before: " << anger << "\n";
    //std::cout << " candies want: " << candies_want[0] << " and " << candies_want[1] << "\n";
    for(int i = 0; i<children; i++)
    {
        //std::cout << "anger: " << anger << "\n";
        //std::cout << "candy_map: " << candy_map[candies_want[i]] << " candies_want: " << candies_want[i] << "\n";
        anger += candy_map[candies_want[i]]*(candies_want[i]*candies_want[i]);
        //N -= candy_map.count(candies_want.back());
        //std::cout << "anger: " << anger << "\n";
    }
    std::cout << anger << "\n";
}
