/*
########[LongestSeq LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to implement a function that computes the longest increasing subsequence (LIS) in a given sequence. If there are several
sub-sequences then any is fine. A sequence is a string of numbers in random order. An increasing sequence has the numbers in increasing order. A
increasing subsequence is a subsequence to the original input sequence where elements are skipped (not moved around) to create an increasing order.
The task was to find the longest possible of these increasing subsequences.

Regarding time complexity: Time complexity is O(n*log(n)) for finding the largest possible sequence length, as as we are looping trough each element
once but also at worst doing an additional binary search to find the wanted index. When we assemble the actual index order no additional time-complexity is
added since we are just traversing backwards through the original sequence and picking out the elements fitting our requirements.


Additional notes: Lab 1.3 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.1, clean version + comments
Additional notes++: I found this lab a bit extra hard, spent perhaps 1 hour just fudging and fixing my read() function. Might be tired
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

std::vector<std::vector<int64_t>> readCases(){
    std::vector<std::vector<int64_t>> case_list;
    std::vector<int64_t> seq_digits;

    int64_t n, digit;
    std::cin >> n;

    while(!std::cin.eof()){
        if(seq_digits.size() != n){
            seq_digits.resize(n);
        }

        for(int i = 0; i < n; i++){
            std::cin >> digit;
            seq_digits[i] = digit;
        }
        case_list.push_back(seq_digits);
        std::cin >> n;
    }

    return case_list;
}

//Using Binary Search: https://www.geeksforgeeks.org/longest-monotonically-increasing-subsequence-size-n-log-n/
std::vector<int64_t> calcSubSeq(std::vector<int64_t> sequence){
    std::vector<int64_t> sub_seqs(sequence.size(), 1); //Represents the length of each subsequence (min 1)
    std::vector<int64_t> max_seq;   //size of this vector is maximum length of a subsequence for the given sequence
    int64_t n{sequence.size()}; //Size of input sequence

    max_seq.push_back(sequence[0]); //Always add first element as start to sub-sequence

    for(int i = 1; i < sequence.size(); i++){
        if(sequence[i] > max_seq.back()){//Check if larger than last element in sub-sequence, if yes push it to the sequence
            max_seq.push_back(sequence[i]);
            sub_seqs[i] = max_seq.size();

        }else{ //else find out where it can fit
            int insert_index = lower_bound(max_seq.begin(), max_seq.end(), sequence[i]) - max_seq.begin(); //find where to insert
            sub_seqs[i] = insert_index + 1; //+1 since this is lengths and they start are min 1 but the inser_index might be 0

            max_seq[insert_index] = sequence[i];
        }
    }

    int64_t max_length = max_seq.size();
    int64_t max_input = *std::max_element(sequence.begin(), sequence.end());
    std::vector<int64_t> seq_indices; //return indices

    while(n--){//Go trough sequence in backwards order and check for the
        if(sub_seqs[n] == max_length && sequence[n] <= max_input){ //If this triggers a
            seq_indices.push_back(n); //add index to vecotr
            --max_length; //reduce max length left to search trough
            max_input = sequence[n]; //Update to largest number in subsequence that has been processed
        }
    }

    return seq_indices;
}


int main(){
    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Variable declarations
    std::vector<std::vector<int64_t>> cases;
    std::vector<int64_t> subseq;

    //Read
    cases = readCases();

    //Prints
    for(int i = 0; i < cases.size(); i++){
        subseq = calcSubSeq(cases[i]); //sequence is stored in reverse order
        int64_t sub_size = subseq.size();
        std::cout << sub_size << "\n";
        for(int j = 0; j < sub_size; j++){ //print them in correct order
            std::cout << subseq.back() << " ";
            subseq.pop_back();
        }
        std::cout << "\n";
    }

    return 0;
}
