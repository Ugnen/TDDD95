/*
########[Longest Common Prefix LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Task was to implement a function that finds the Longest Common Prefix (longest repeating substring) in a given string.

Regarding implementation: The task was solved by using the previously implemented SuffixArray from lab 3.2 and implementing Kasai's LCP array algorithm. It
works by creating a rank vector (which is the index for each suffix in sorted order). And looping over each adjacent suffix in this order. It checks if there
is a matching character and thus increments the value, when no longer matching the LCP for i has been found. Decrement by one at the end of each loop (except
if value is already zero) since the next pair of suffixes is guaranteed to match to current value - 1.

A few other functions were also implemented and/or used:
* getSuffix : from Lab 3.2
* printSuffix : print sorted suffix i. Used for Debugging
* getLCP : a public getter for the LCP array
* lcpQuery : returns the LCP for arbitrary suffixes starting at i and j. Assumes: i > j


Regarding time complexity: Time complexity should be O(n) for the LCP algorithm and O(n*log(n)) for the SuffixArray (as in Lab 3.2). We loop over each suffix
1 time and decrement a maximum n times, the while-loop increments a maximum of n-1 times (in total).

Additional notes: Lab 3.3 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 0.1, first-version + comments
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

class SuffixArray{
private:
    //length of input string
    int32_t n;
    std::string text;

    //number of unique eqv_class
    int32_t class_val{1};

    //permutations p, and equivalence classes c
    std::vector<int32_t> permut, eqv_class;

    //LCP array: lcp_array[i] is the longest common prefix for suffixes starting at p[i] and p[i + 1] (sorted)
    std::vector<int32_t> lcp_array;

    //sort based on same principle as radix sort: 1) sort by second element (stable sort). 2) sort by first (stable sort). 3) update class equivalence
    void radixIterate(){
        //temp vectors to store permutations and class indices
        std::vector<int32_t> permut_temp(n), class_temp(n);

        //iteration k, dealing with substrings of length 2^k.
        for(int32_t k{1}; k < n; k <<= 1){
            ////////////////////////////////
            //SORT BY SECOND ELEMENT IN PAIR
            ////////////////////////////////

            //temporary permutation vector to handle changes based on second element
            permut_temp = permut;

            //save permutations by second element by removing 2^(k-1)
            for(int32_t i{0}; i < n; ++i){
                permut_temp[i] -= k;

                //no negative permutations as they represent 'indices' or an order
                if(permut_temp[i] < 0){
                    permut_temp[i] += n;
                }
            }

            ////////////////////////////////
            //SORT BY FIRST ELEMENT IN PAIR
            ////////////////////////////////
            //'counting' sort again but class-based instead of occurrence-based

            //3) create countArr same size as number of classes is enough
            std::vector<int32_t> countingArr(class_val, 0);

            //4) again but use permutations of second element and eqv_classes
            for(auto val : permut_temp){
                countingArr[eqv_class[val]] += 1;
            }

            //5) add count to current pair from previous pair
            for(int32_t i{1}; i < class_val; ++i){
                countingArr[i] += countingArr[i-1];
            }

            //6) update permutations to permutations from second element
            for(int32_t i{n - 1}; i >= 0; --i){

                permut[countingArr[eqv_class[permut_temp[i]]] - 1] = permut_temp[i];
                countingArr[eqv_class[permut_temp[i]]]--;
            }

            ////////////////////////////////
            //UPDATE CLASS EQUIVALENCE
            ////////////////////////////////
            //7)
            class_val = 1;
            class_temp[permut[0]] = 0;

            //create 'pairs'
            std::pair<int32_t, int32_t> current, previous;

            //update equivalence classes by comparing pairs
            for(int32_t i{1}; i < n; ++i){

                //pairs starting at i and j = i-1: (c[i], c[i + 2^(k-1)]) && (c[j], c[j + 2^(k-1)])
                // modulo since cyclic shift
                current = {eqv_class[permut[i]], eqv_class[(permut[i] + k) % n]};
                previous = {eqv_class[permut[i-1]], eqv_class[(permut[i-1] + k) % n]};

                if(current != previous){
                    ++class_val;
                }

                class_temp[permut[i]] = class_val - 1;
            }

            eqv_class.swap(class_temp);
        }
        return;
    }

    //Original algorithm: count the frequency of each distinct element and sort according to that
    void countingSort(std::string const& s){

        //2)
        int32_t max = (int32_t)*std::max_element(s.begin(), s.end());

        //3)
        std::vector<int32_t> countingArr(max+1, 0);

        //4)
        for(auto val : s){
            countingArr[(int32_t)val] += 1;
        }

        //add a smaller character than anything else in input string. As s is all printable ASCII chars we are safe just to add +1
        //to any of the places 0-31 in countingArr
        countingArr[0] += 1;

        //5)
        for(int32_t i{1}; i < countingArr.size(); ++i){
            countingArr[i] += countingArr[i-1];
        }

        //for(int32_t i{0}; i < n - 1; ++i){
        //https://www.geeksforgeeks.org/dsa/counting-sort/
        for(int32_t i{n-2}; i >= 0 ; --i){

            permut[countingArr[(int32_t)s[i]] - 1] = i;

            countingArr[(int32_t)s[i]]--;
        }

        //assign smallest char n-1
        permut[countingArr[0] - 1] = n-1;

        //our added char is always the smallest
        eqv_class[permut[0]] = 0;

        //the letter examined is never the same as the one we added so we assign class 1 to it
        eqv_class[permut[1]] = class_val; //class_val = 1;
        ++class_val; //class_val = 2;

        //initialize equivalence classes by comparing permutations with previous permutations
        for(int32_t i{2}; i < n; ++i){

            if(s[permut[i]] != s[permut[i-1]]){
                ++class_val;
            }

            eqv_class[permut[i]] = class_val - 1;
        }


        //radix sort
        radixIterate();

        return;
    }

public:

    //Constructor: pass strings as reference to minimize copying
    SuffixArray(std::string const& s){
        text = s;
        n = s.size() + 1;
        permut.resize(n);
        eqv_class.resize(n);
        countingSort(s);
        n--;
    }

    //returns the i:th smallest suffix
    int32_t getSuffix(int32_t index){

        //+1 to account for the (small) added character
        return permut[index+1];
    }

    //returns the indices of the sorted vector
    std::vector<int32_t> getSorted(){

        return std::vector<int32_t>(permut.begin() + 1, permut.end());
    }

    void printArray(){
        for(int64_t i{0}; i < n - 1; ++i){
            std::cout << text.substr(i) << "\n";
        }
        return;
    }

    void printSorted(){
        for(int64_t i{1}; i < n + 1; ++i){
            std::cout << text.substr(permut[i]) << "\n";
        }
        return;
    }

    void printSuffix(int32_t index){
        std::cout << text.substr(permut[index + 1]) << "\n";
        return;
    }

    //Kasai LCP-algorithm
    int32_t longestCommonPrefix(){
        //printSorted();

        lcp_array.resize(n-1, 0);
        int32_t lcp_val{0}, max_lcp{0};

        //p is sorted suffix array. rank is each suffixes index in the sorted array
        std::vector<int32_t> rank(n,0);

        for(int32_t i{0}; i < n; ++i){
            rank[getSuffix(i)] = i;
        }


        for(int32_t i{0}; i < n; ++i){

            //if suffix to examine is last in the sorted list, none comes after, continue to next suffix
            if(rank[i] == n - 1){
                lcp_val = 0;
                continue;
            }

            //let j be the next suffix after i in the sorted order
            int32_t j{getSuffix(rank[i] + 1)};


            //debug print

            std::cout << "ranks: ";
            printSuffix(rank[i]);
            printSuffix(rank[j]);
            std::cout << "\n";



            //increase the LCP value while the suffixes have matching charachters (neither can go beyond n-1 = size of input string)
            while((i + lcp_val < n)&& (j + lcp_val < n) && (text[i+lcp_val] == text[j + lcp_val])){
                //std::cout << "text[i+lcp_val]: " << text[i+lcp_val] << "\n";
                ++lcp_val;
            }

            //std::cout << lcp_val << "\n\n";

            //std::cout << "text[i+lcp_val]: " << text[i+lcp_val] << "\n";
            //std::cout << "text[j+lcp_val]: " << text[j+lcp_val] << "\n";

            //save LCP for index i
            lcp_array[rank[i]] = lcp_val;

            //Update current longest common prefix value
            if(max_lcp < lcp_val){max_lcp = lcp_val;}

            //decrement LCP value by one since we know that the next pair (of i & j) will already match for current LCP - 1
            //maximum we may decrement is n times (but never when lcp_val is zero)
            if(lcp_val){
                --lcp_val;
            }
        }

        return max_lcp;
    }

    //returns the i:th LCP
    int32_t getLCP(int32_t index){

        return lcp_array[index];
    }

    //Returns the LCP for arbitrary suffixes starting at i and j. Assumes: i > j
    int32_t lcpQuery(int32_t i, int32_t j){

        if(j == i + 1){
            return lcp_array[i];
        }

        //min(lcp[i], lcp[i+1],...,lcp[j-1])
        int32_t ans{lcp_array[i]};
        i++;
        for(; i < j; ++i){
            ans = std::min(ans, lcp_array[i]);
        }

        return ans;
    }

};


//reads cases
std::string caseRead(char & c){

    std::string data;

    //read a char
    c = getc(stdin);

    while(c != '\n' && c != EOF){
        data += c;
        c = getc(stdin);
    }

    return data;
}

//Reads an int64 from stdin
int64_t intRead(char & c){

    //init data
    int64_t data{0};

    //read a char
    c = getc(stdin);

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

    //Variable declarations
    int64_t n{}, query{};
    char c;
    std::string text;
    bool row{false};

    n = intRead(c);
    text = caseRead(c);
    SuffixArray suffix_array(text);
    std::cout << suffix_array.longestCommonPrefix();

    return 0;
}
