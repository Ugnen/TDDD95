/*
########[Single Source shortest path, negative weights LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to "Implement Bellman-Fords algorithm for finding the shortest path from a node to all other nodes in a graph where edge weights
may be negative.". Output is displayed in a single row for each query as an int (meaning the min distance) or as "-Infinity" if the path to the query is obstructed
by a negative cycle, or "Impossible" if the query node cannot be reached. A simple struct containing the information: from (node), to (node), and weight (distance
on edge between from and to). The Bellman-Ford algorithm is implemented in the "def_not_djik" function which takes the starting node and a vector of the
custom data-structure (all the edges), it returns a pair of vectors. The first vector containing the min-distances from the starting node to each node and the
second vector containing each predecessor. The second output solves the additional requirement imposed in this course which is that it should be possible to
recreate the min-distance path. Just loop backwards in the second vector from the query node (dist_preds.second[query]) until you reach the starting node.
The function ("def_not_djik")/bellman-ford algorithm works by looping through ALL the edges n-1 times and updating the min-distance according to:
min-distance = min(dist_from + edge_weight, current_dist_to). Theoretically the algorithm should be finished by n-1 iterations. This means we may check if
there is a negative cycle by running it MORE times (in my implementation just n-1 times again for each edge) as if a relaxation is possible that means -INF
is possible.

Regarding time complexity: Time complexity should be O(n*m) (nodes*edges) as we twice loop through ALL edges n-1 times.

Additional notes: Lab 2.3 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.0, final version with comments about assignment and time complexity.
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

const int64_t INF = 1000000000; //global infinity variable, larger than max_nodes*max_nodes (1e8)

//data-structure for handling edges in Bellman-Ford algorithm
struct Gooner{

    int64_t from{};
    int64_t to{};
    int64_t weight{};

};


std::pair<std::vector<int64_t>, std::vector<int64_t>> def_not_djik(int64_t start_node, int64_t n, std::vector<Gooner> edges){

    std::vector<int64_t> shortest_path(n, INF); //shortest path, initial values for shortes path, all start on INF
    std::vector<int64_t> pred(n, -1);    //predecessors, are initialized as -1

    shortest_path[start_node] = 0;

    for(int i{0}; i < n-1; ++i){
        for(auto edge: edges){
                if(shortest_path[edge.from] + edge.weight < shortest_path[edge.to] && shortest_path[edge.from] < INF){
                    shortest_path[edge.to] = shortest_path[edge.from] + edge.weight;
                    pred[edge.to] = edge.from;
                }
        }
    }

    //Just run it again, if it wants to do a relaxation there is a negative cycle there -> take note by marking -INF
    // ALL negative cycle weights will be marked -INF by this
    for(int i{0}; i < n-1; ++i){
        for(auto edge: edges){
            if(shortest_path[edge.from] + edge.weight < shortest_path[edge.to] && shortest_path[edge.from] < INF){
                shortest_path[edge.to] = -INF;
            }
        }
    }

    return std::make_pair(shortest_path, pred);
}

int main(){
    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Variable declarations
    int64_t n{}, m{}, q{}, s{};
    std::cin >> n >> m >> q >> s;

    if(!(n || m || q || s)){
        return 0;
    }

    while(1){
        std::vector<Gooner> edges(m);

        int64_t u{}, v{}, w{};

        for(int i{0}; i<m; ++i){
            std::cin >> u >> v >> w;
            edges[i] = {u, v, w}; //add all the edges
        }

        std::pair<std::vector<int64_t>, std::vector<int64_t>> dist_pred = def_not_djik(s, n, edges);

        int64_t query_node{};
        while(q--){ //no negative cycles
            std::cin >> query_node;

            if(dist_pred.first[query_node] == INF){ //If distance is INF, we can't reach node
                std::cout << "Impossible\n";

            }else if(dist_pred.first[query_node] == -INF){ //If distance is -INF node is in a negative cycle
                std::cout << "-Infinity\n";
            }else{
                std::cout << dist_pred.first[query_node] << "\n";
            }
        }

        std::cin >> n >> m >> q >> s;
        if(n || m || q || s){
            std::cout << "\n";
        }else{
            break;
        }
    }

    return 0;
}
