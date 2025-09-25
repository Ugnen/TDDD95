#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

//Class FenwickTree that produces a Binary Indexed Tree
//Copied class from fenwick lab 1.5
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

    long long int N, n, M;

    std::cin >> N;// >> M;
    long long int index_vec[N+1];

    FenwickTree distTree = FenwickTree(N+1);

    for(int j = 1; j <= N; j++){
        distTree.add(j,1);
        std::cin >> n;
        //num_vec[j] = n;
        index_vec[n] = j; //[2 1 3] -> [1 0 2]
    }
    //std::cout << "sum: " << distTree.sum(index_vec[M]);

    long long int start_N{1}, end_N{N}, uneven_const{}; //help variables to calculate to relative start and end (moves 1 towards center for each distance calculated)

    int num{};
    //std::cout << "\n";
    for(int i = 1; i <= N; i++){
        if(i%2){ //For 1 to N/2, uneven
            std::cout << distTree.sum(index_vec[start_N] + 1) - 1 << "\n";
            distTree.add(index_vec[start_N], -1); //Remove one on position we want to calculate
            start_N += 1;
        }else{ //For N to N/2, even
            std::cout << end_N - distTree.sum(index_vec[end_N]+1) - (i/2) << "\n";
            distTree.add(index_vec[end_N] + 1, -1); //Remove one on position we want to calculate
            end_N -= 1;
        }
    }
    return 0;
}
