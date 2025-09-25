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

//memory allocation
char* read_buffer = (char*)malloc(256); //Lines are guaranteed to be no longer than 254 characters (+ terminator)
char* data_buffer = (char*)malloc(1000000); //total size of the input file will not exceed 10^6 characters (SEGMENTAITON FAULT)

//creates and returns a new char* that is sanitized from whitespaces
char* sanitize(const char* input_line){

    //std::cout << "inside1:" << input_line << "\n";

    char* clean_input = data_buffer; //create a new 'current' pointer : at end of function it should contain only valid chars from input | use databuffer

    bool consecutive{false}, not_leading{false};
    for (; *input_line; ++input_line) {
        //std::cout << "inside2:[" << *input_line << "]\n";

        if(isspace(*input_line)){//*input_line == ' ' || *input_line == '\n'){ //if whitespace
            consecutive = true;
        }else{
            //not a whitespace -> check if whitespace was leading or not
            if(consecutive && not_leading){ //check if white spaced not leading and should be compressed
                *data_buffer++ = ' '; //save one whitespace before adding valid charachter
            }

            *data_buffer++ = *input_line; //save valid char and iterate
            consecutive = false; //compresses consecutive white spaces into single one
            not_leading = true; //guarantees first char is not a white space
        }
    }

    //std::cout << "after loop\n";
    //std::cout << "data_buffer: " << *data_buffer << "\n";
    *data_buffer++ = '\0'; //end for current pointer + iterate
    //data_buffer++;

    return clean_input;
}


class SuffixArray{
private:
    //length of input string
    int32_t n, cnt;

    //number of unique eqv_class
    int32_t class_val{1};
    std::vector<int32_t> docID;

    //permutations p, and equivalence classes c
    std::vector<int32_t> permut, eqv_class;

    //LCP array: lcp_array[i] is the longest common prefix for suffixes starting at p[i] and p[i + 1] (sorted)
    std::vector<int32_t> lcp_array;
    int32_t max_lcp{0};

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
    void countingSort(const std::vector<int32_t> & input){

        //2)
        int32_t max = *std::max_element(input.begin(), input.end());
        cnt = max;

        //3)
        std::vector<int32_t> countingArr(max+1, 0);

        //4)
        for(auto val : input){
            countingArr[val] += 1;
        }

        //add a smaller character than anything else in input string. As s is all printable ASCII chars we are safe just to add +1
        //to any of the places 0-31 in countingArr
        countingArr[0] += 1;

        //5)
        for(int32_t i{1}; i < countingArr.size(); ++i){
            countingArr[i] += countingArr[i-1];
        }

        for(int32_t i{n-2}; i >= 0 ; --i){

            permut[countingArr[input[i]] - 1] = i;
            countingArr[input[i]]--;
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

            if(input[permut[i]] != input[permut[i-1]]){
                ++class_val;
            }

            eqv_class[permut[i]] = class_val - 1;
        }

        //radix sort
        radixIterate();

        return;
    }

public:

    //returns the i:th smallest suffix
    int32_t getSuffix(int32_t index){

        //+1 to account for the (small) added character
        return permut[index+1];
    }

    //returns the indices of the sorted vector
    std::vector<int32_t> getSorted(){
        return std::vector<int32_t>(permut.begin() + 1, permut.end());
    }

    //Modified Kasai LCP-algorithm to work for two concatenated input strings
    void longestCommonPrefix(const std::vector<int32_t> & input){

        //Document ID: each suffix belongs to one of the two input strings that have been concatenated
        docID.resize(n);
        int32_t file_ID{0};
        for(int32_t i{0}; i < n; ++i){
            if(input[i] == cnt){
                ++file_ID;
            }
            docID[i] = file_ID;
        }

        lcp_array.resize(n-1, 0);
        int64_t lcp_val{0};//, max_lcp{0};

        //p is sorted suffix array. rank is each suffixes index in the sorted array
        std::vector<int64_t> rank(n,0);

        for(int64_t i{0}; i < n; ++i){
            rank[getSuffix(i)] = i;
        }


        for(int64_t i{0}; i < n; ++i){

            //if suffix to examine is last in the sorted list, none comes after, continue to next suffix
            if(rank[i] == n - 1){
                lcp_val = 0;
                continue;
            }

            //let j be the next suffix after i in the sorted order
            int64_t j{getSuffix(rank[i] + 1)};

            //check if suffixes are from different strings
            if(docID[i] == docID[j]){
                continue;
            }

            //increase the LCP value while the suffixes have matching charachters (neither can go beyond n-1 = size of input string)
            while((i + lcp_val < n)&& (j + lcp_val < n) && (input[i+lcp_val] == input[j + lcp_val])){
                ++lcp_val;
            }

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

        return;
    }

    int32_t maxLCP(){

        return max_lcp;
    }

    //Constructor: pass strings as reference to minimize copying
    //SuffixArray(std::string const& s){
    SuffixArray(const std::vector<int32_t> & input){
        n = input.size() + 1;
        permut.resize(n);
        eqv_class.resize(n);
        countingSort(input);
        n--;
        longestCommonPrefix(input);
    }



};

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
    //std::ios_base::sync_with_stdio(false);
    //std::cin.tie(NULL);
    //std::cout.tie(NULL);

    //Variable declarations
    int64_t N{};
    int32_t cnt{0}, tst{1};
    char c;

    char* input_line;
    char* file_line;
    std::vector<const char*> file_names;
    std::vector<std::vector<int32_t>> repo;
    std::vector<int32_t> stolen_code;

    N = intRead(c);

    while(tst--){
        std::unordered_map<size_t, int32_t> unique_input;

        for(int i{0}; i < N; ++i){

            //Read file name
            file_line = read_buffer;
            fgets(file_line, 256, stdin);
            const char * name = sanitize(file_line);
            file_names.push_back(name);

            //Push empty vector and read first code-line
            repo.push_back(std::vector<int32_t>());
            input_line = read_buffer;
            fgets(input_line, 256, stdin);

            //check if the end of code file
            while(std::strcmp(input_line, "***END***\n")){

                //Remove whitespaces
                const char* compressed_data = sanitize(input_line);

                //check if non-empty
                if(static_cast<bool>(std::strlen(compressed_data))){

                    //hash string to save space
                    size_t identity = std::hash<std::string_view>()(compressed_data);

                    //if not previously read string input
                    if(unique_input.find(identity) == unique_input.end()){
                        unique_input[identity] = cnt;
                        cnt++;
                    }

                    //assign unique ID to string
                    //push to repository
                    repo[i].push_back(unique_input[identity]);
                }

                //read next line
                input_line = read_buffer;
                fgets(input_line, 256, stdin);
            }

        }

        //read file to compare, same principle as above
        input_line = read_buffer;
        fgets(input_line, 256, stdin);
        while(std::strcmp(input_line, "***END***\n")){

            //remove whitespaces
            const char* compressed_data = sanitize(input_line);

            if(static_cast<bool>(std::strlen(compressed_data))){
                size_t identity = std::hash<std::string_view>()(compressed_data);

                //if not previously found input
                if(unique_input.find(identity) == unique_input.end()){
                    unique_input[identity] = cnt;
                    cnt++;
                }

                stolen_code.push_back(unique_input[identity]);
            }

            //read next line
            input_line = read_buffer;
            fgets(input_line, 256, stdin);
        }
    }

    //vector with indices for files with largest matching statistic
    std::vector<int32_t> indices;
    int max_ms{0};

    //finding the largest matching statistic = largest LCP for the concatenated strings where each suffix examined is from each string
    for(int32_t j{0}; j < N; ++j){
        int ms_temp{0};

        std::vector<int32_t> suffix_input;

        for(int i{0}; i < repo[j].size(); ++i){
            suffix_input.push_back(repo[j][i]);
        }

        //suffix_input.push_back(unique_input[divider]);
        suffix_input.push_back(cnt);

        for(int i{0}; i < stolen_code.size(); ++i){
            suffix_input.push_back(stolen_code[i]);
        }

        //Suffix Array of one file (at a time) and stolen code
        SuffixArray suffix_array(suffix_input);

        ms_temp = suffix_array.maxLCP();

        //clear indices if we found a strictly larger matching statistic
        if(ms_temp > max_ms){
            indices.clear();
        }

        //push current index and save new max
        if(ms_temp >= max_ms && ms_temp > 0){
            max_ms = ms_temp;
            indices.push_back(j);
        }
    }

    //output
    std::cout << max_ms;
    if(max_ms){
        for(auto idx : indices){
            std::cout << " "  << file_names[idx];
        }
    }

    return 0;
}

//matching statistics : https://www.youtube.com/watch?v=kogY5TZYgp8
