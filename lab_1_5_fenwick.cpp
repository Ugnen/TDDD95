/*
########[FENWICK LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to implement a data structure that was able to handle prefix sums (a[0], ... , a[n])
with two operations, add and sum. The data structure known as a Fenwick tree (i.e a Binary Index Tree) makes doing these
two operations very fast for large arrays (O = log(n)). "add" adds the value delta to the indexed element, while
"sum" returns the sum of the preceeding numbers (not including the index entered).

Regarding time complexity: Time complexity is O(log(n)) for either operation. Since both operations use bitwise AND (= index & (-index))
(one as addition and one as subtraction) as a way to traverse the tree (or array) the amount of steps (= k) can be
found via n = 2^k where n is the start (sum) or end (add). For example: Sum for an array length of 15 = 1111
goes through: 15 (1111) -> 14 (1110) -> 12 (1100) -> 8 (1000) ==> 4 steps or log(2^4).

Additional notes: Lab 1.5 in course TDDD95 via LINKÖPING UNIVERSITY
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

//Class FenwickTree that produces a Binary Indexed Tree
class FenwickTree{
    private:
        std::vector<int64_t> vec;
        int64_t width; //size of vector/array

    public:
        FenwickTree(int64_t N = 0){ //Create vector of length N with only zeroes.
            width = ++N;
            vec.resize(width,0); //fill with zeros
        }

        //Updates/Adds delta to the indexed value
        void add(int64_t index, int64_t delta){
            index = index +1; //Assume BI-tree is index from 1-N not 0-N
            while(index < width){ //Update index and parent nodes
                vec[index] += delta; //increase vec[i] with delta
                index += index & (-index); //index & (-index) = bitwise AND: Addition of last set bit of index to index by using bitwise AND
            }

        }

        //Compute sum of numbers up to (but not including) end_index
        int64_t sum(int64_t end_index){
            int64_t sum_of_vec{0}; //Start sum on 0
            while(end_index > 0){
                sum_of_vec += vec[end_index];
                end_index -= end_index & (-end_index);// Subtraction of last set bit of index to index by using bitwise AND
            }
            return sum_of_vec;
        }

};

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int64_t N, Q, ind, delta; //Size of tree (N) and number of operations (Q), ind = index, delta = delta
    std::cin >> N >> Q;
    FenwickTree BITree = FenwickTree(N); //Initiate fenwick tree of size N
    char operation; //To read if '+' (add) or '?' (sum)
    while(Q--){
        std::cin >> operation;
        if(operation == '+'){
            std::cin >> ind >> delta;
            BITree.add(ind, delta);
        }else if(operation == '?'){
            std::cin >> ind;
            std::cout << BITree.sum(ind) << "\n";
        }
    }
    return 0;
}
