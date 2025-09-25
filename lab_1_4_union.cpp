/*
########[Union-Find LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to implement a data structure that was able to handle the comparison and merging (union) of two sets. For this to work
two functionalities union(int a, int b) & boolean(int a, int b) need to exist, where 'union' takes the two sets that 'a' and 'b' belong to and merge them.
This is the function "mergeSet" in the code. The 'boolean' functionality aims to find out if 'a' and 'b' belong to the same set or not. The "findSet" function
in the code does this part.

Regarding time complexity: Time complexity is O(\alpha(n)) where \alpha(n) is the inverse Ackerman function, the proof for which is quite long and complex. What
matters is that by using both path compression and union by rank (depth in this implementation) we achieve a near constant time. That is to say both
operations have atleast amorized O(log(n)) time complexity. If further proof/reading is wanted one can look at article:
 "Worst-Case Analysis of Set Union Algorithms" (Tarjan et al.)

Additional notes: Lab 1.4 in course TDDD95 via LINKÖPING UNIVERSITY
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

//Class UnionFind that produces sets with different ranks. Functions to create a set, find a the "Parent" set and merge two sets.
class UnionFind{
    private:
        std::vector<int64_t> parent_set;
        std::vector<int64_t> set_depth; //rank

    public:
        UnionFind(int64_t N = 0){ //Set correct lengths for vectors/arrays
            parent_set.resize(N);
            set_depth.resize(N);
            for(int i = 0; i < N; i++){
                makeSet(i);
            }
        }

        //Create a set with int_a
        void makeSet(int64_t int_a){

            parent_set[int_a] = int_a; //start by making int_a itself the top node

            set_depth[int_a] = 0; //0 depth as its just one node

        }

        //function to find the Set-parent for int_a
        int findSet(int64_t int_a){ //find the parent of int_a, if the parent is int_a then int_a is the top node
            if(int_a == parent_set[int_a]){
                return int_a;
            }

            return parent_set[int_a] = findSet(parent_set[int_a]); //Path compression
        }

        //function that returns true if int_a and int_b belong to the same set
        bool sameSet(int64_t int_a, int64_t int_b){
            return findSet(int_a) == findSet(int_b);
        }

        //combine set where int_a is member with set where int_b is member, the most shallow tree gets combined into the deeper tree
        void mergeSets(int64_t int_a, int64_t int_b){ //union by rank (depth of tree)
            int64_t parent_a = findSet(int_a);
            int64_t parent_b = findSet(int_b);
            if(parent_a != parent_b){
                if(set_depth[int_a] < set_depth[int_b]){ //int_a should be bigger by default, if not switch values
                    std::swap(int_a, int_b); //switcharoo
                }
                parent_set[parent_b] = parent_a; //set parent of b to a;
                if(set_depth[int_a] == set_depth[int_b]){ //if they have same depth increase
                    set_depth[int_a] += 1;
                }
            }
        }

};

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int64_t N, Q, int_a, int_b; //Number of elements in base ser (N) and number of queries (Q)
    std::cin >> N >> Q;

    UnionFind jointSet = UnionFind(N);

    char operation; //To read if '=' (union) or '?' (boolean)

    while(Q--){
        std::cin >> operation >> int_a >> int_b;

        if(operation == '='){
            jointSet.mergeSets(int_a, int_b);

        }else if(operation == '?'){
            if(jointSet.sameSet(int_a, int_b)){
                std::cout << "yes\n";
            }else{
                std::cout << "no\n";
            }
        }
    }
    return 0;
}
