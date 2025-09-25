/*
########[Suffix sorting LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Task was to implement a data structure that sorts the suffixes of a string in lexicographical order (aka alphabetical order). It should contain
the following operations:
*) A constructor, that can take a string as input: should create a suffix object from the string
*) a function, 'getSuffix' that takes an integer and returns the position of the i:th smallest suffix

Regarding implementation: The task was solved by implementing counting sort with a modified radix sort. By first doing one pass with counting sort and setting
the equivalence classes. Then doing iterative runs of radix sort, that uses counting sort based on class equivalences (which is a stable sort). Each pass of
radix first sorts by the second element, then sorts by the first element and finally updates the class equivalences.

A few other helpful functions were added:
* getSorted : returns the indices of the sorted suffix array.
* printSuffixes : prints the suffixes of the input string in order of appearance
* printSorted : prints the suffixes of the input string in sorted order


///////////////////////////////////////////////
Pseudo code for counting sort algorithm i used:
    1) input: arr
    2) maxVal(arr)
    3) countingArr(0,maxVal)
    4) countingArr = count(arr) : count all occurrences in arr
    5) from i=1 to arr.size() -1 : countingArr[i] += countingArr[i-1]
    6) ansArr([], size(arr))
    7) from arr.size() - 1 to i = 0: ansArr[countingArr[arr[i]] - 1] = arr[i] && countingArr[arr[i]]--
    8) ==> ansArr is arr sorted
    //assignment in step 7) doesn't have to be arr[i], it could just as well be an index (that is what I used in this assignment)
///////////////////////////////////////////////



Regarding time complexity: Time complexity should be O(s*log(s)) where s is the length of the input string.

* One could bring the time down to O(s) by implementing SA-IS or SKEW, although that feels a bit overkill.

Additional notes: Lab 3.2 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.0, final-version + comments + fixed radixIterate
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

    void printSuffixes(){
        for(int64_t i{0}; i < n - 1; ++i){
            std::cout << text.substr(i) << "\n";
        }
        return;
    }

    void printSorted(){
        for(int64_t i{1}; i < n; ++i){
            std::cout << text.substr(permut[i]) << "\n";
        }
        return;
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

    text = caseRead(c);

    //Keep reading until EOF
    while(c != EOF){

        //dont print row after last case
        if(row){std::cout << "\n";}
        row = true;

        SuffixArray suffix_array(text);

        n = intRead(c);

        while(n--){
            query = intRead(c);

            std::cout << suffix_array.getSuffix(query) << " ";
        }

        text = caseRead(c);
    }


    return 0;
}
