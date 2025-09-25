/*
########[String Matching LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Task was to implement a function that takes a text and a pattern which finds and returns all occurrences of the pattern in the text. This was
using the Knuth-Morris-Pratt algorithm which creates a prefix-function of the combination of the text and pattern and compares the lengths to the pattern
length. The prefix-function stores the longest proper prefixes which is also a suffix and only performs O(n) operations.
The prefix-function is created via the "preFun" function that takes an input string and returns the lengths for each index
The string-match function "stringMatch" takes the text and pattern as input and returns the indices where the pattern matches.


Regarding time complexity: Time complexity should be O(n+m), where n is the size of the pattern and m is the size of the text. As we create a prefix function
for the string = patter + "separator" + text and it allows us to do constant time lookup for all operations. We simply loop over all the chars in this
combostring inside the function preFun() once and then after the calculation constant-time lookup is performed.

Additional notes: Lab 3.1 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.0, revamped-version
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

//https://cp-algorithms.com/string/prefix-function.html
std::vector<int64_t> preFun(std::string text){
    int64_t textSize{(int64_t)text.size()};

    //init 'pi' = vector with the longest proper prefixes which is also a suffix for the given substring
    std::vector<int64_t> pi(textSize,0);

    for(int64_t i{1}; i < textSize; ++i){

        //init length of 'best' suffix
        int64_t max_len = pi[i-1];

        //reduce max_length of suffix until same as prefix (or 0)
        while(text[i] != text[max_len]){
            if(max_len <= 0){
                break;
            }
            max_len = pi[max_len-1];
        }

        //test if suffix is also prefix, if yes we increment
        if(text[i] == text[max_len]){
            //increment
            ++max_len;
            //assign length of best suffix that equals a pro
            pi[i] = max_len;
        }else{
            pi[i] = 0;
        }
    }

    return pi;
}

//input as: a text and a pattern and returns all matching occurrences of the the pattern in text
std::vector<int64_t> stringMatch(std::string text, std::string pattern){

    std::string combo = pattern + "|" + text;
    std::vector<int64_t> matches;

    //length of pattern
    int64_t nLength = pattern.size();

    //run prefix-function on the 'pattern'
    std::vector<int64_t> patternPi = preFun(combo);

    //loop through the indices of the input text
    for(int64_t i{nLength + 1}; i < combo.size(); ++i){

        //check if any of the prefix-function lengths match the pattern-length
        if(patternPi[i] == nLength){

            //store the index: i - (n - 1) - n + 1 = i - 2*n
            matches.push_back(i-2*nLength);
        }
    }


    return matches;
}

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

int main(){
    //desync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    //Variable declarations
    char c;
    std::string pattern, text;

    pattern = caseRead(c);

    //Keep reading until EOF
    while(c != EOF){
        text = caseRead(c);

        //get matching occurrences
        std::vector<int64_t> occ = stringMatch(text, pattern);

        //Print
        for(int i{0}; i < occ.size(); ++i){
            std::cout << occ[i] << " ";
        }
        std::cout << "\n";

        pattern = caseRead(c);
    }

    return 0;
}
