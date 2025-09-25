#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n, node;
    std::cin >> n;

    std::vector<int> col_v(n), chops(n+2);
    //std::vector<int> ;

    for(int i = 0; i < n; i++){
        std::cin >> node;
        col_v[i] = node;  //Add input to vector
        chops[node] += 1; //Count the amount of chops to a specific node
    }

    if(!chops[n+1] || col_v[n-1] != n+1){ //If the last chop isn't to the biggest node (the last remaining node should be the largest) or isn't chopped at all, error
        std::cout << "Error";
        return 0;
    }

    std::priority_queue<int, std::vector<int>, std::greater<int>> leaf; //sorted list of leafs, std::greater gives us a min-heap, vector supports pop()/push()

    for(int j = 1; j < n+1; j++){ //check which nodes are leafs and add to min-heap pq (excluding end-node)
        if(!chops[j]){ //If no chops have happened to this node, it is  a leaf
            leaf.push(j);
        }
    }

    /*
    //Print leafs
    std::cout << "\n";
    while(!leaf.empty()){
        std::cout << leaf.top() << "\n";
        leaf.pop();
    }*/

    for(int k = 0; k < n; k++){ //reconstruct u column by simulating original chopping
        if(!leaf.empty()){
            std::cout << leaf.top() << "\n"; //Print column u
            leaf.pop();
        }
        chops[col_v[k]] -= 1; //Reduce chops to this node by one
        if(!chops[col_v[k]]){ //if no more chops to this node, it is now a leaf
            leaf.push(col_v[k]);
        }
    }

    return 0;
}
