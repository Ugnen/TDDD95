#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

const int INF = 1000000000; //global infinity variable, larger than max_nodes*max_nodes (1e8)

//Class Graph
class Graph{
    private:
        std::vector<std::vector<std::pair<double, int64_t>>> sparse_graph; //sparse graph for all nodes

    public:
        Graph(int64_t nodes = 0){ //Create matrix of nodes x nodes
            sparse_graph.resize(nodes);
        }

        //Adds weight-neighbour pairs (edges) from this node
        void add(int64_t node_one, int64_t node_two, double weight){ //Add neighbouring edges to sparse graph (node and weight)
            sparse_graph[node_one].push_back(std::make_pair(weight, node_two));
        }

        //Returns a list of all weight-neighbour pairs (edges) from this node
        std::vector<std::pair<double, int64_t>> edges(int64_t node){
            return sparse_graph[node];
        }

        //Return the number of nodes
        int64_t numberOfNodes(){
            return sparse_graph.size();
        }
};


std::pair<std::vector<double>, std::vector<int64_t>> djikstra(int64_t start_node, Graph tree){
    int64_t nrOfNodes{tree.numberOfNodes()};

    double epsilon{0.0000000001};

    //std::cout << "nrOfNodes: " << nrOfNodes << "\n";
    std::vector<double> shortest_path(nrOfNodes, -INF); //shortest path, initial values for shortes path, all start on -INF
    std::vector<int64_t> pred(nrOfNodes, -1);    //predecessors, are initialized as -1
    shortest_path[start_node] = 1; //starting node gets default value 1

    std::priority_queue<std::pair<double, int64_t>, std::vector<std::pair<double, int64_t>>, std::less<std::pair<double, int64_t>>> pseaduo_fib_heap;
    pseaduo_fib_heap.push({1,start_node}); //push starting node do heap

    while(!pseaduo_fib_heap.empty()){
        double marked_weight = pseaduo_fib_heap.top().first; //get weight of node we want to examine
        int64_t marked_node =  pseaduo_fib_heap.top().second; //get index of node we want to examine
        pseaduo_fib_heap.pop(); //marked edge gets popped


        //Without this check (if it is an important pair) the time complexity increases to O(node*edges), this is bad ofc
        if((marked_weight*shortest_path[marked_node]) < epsilon){
            continue; //serves as ending condition, will empty the heap without looking at irrelevant pairs
        }


        std::vector<std::pair<double, int64_t>> neighbours{tree.edges(marked_node)}; //get all neighbours for marked node

        //iterate through all neighbours
        for(int i{0}; i < neighbours.size(); ++i){
            if(neighbours[i].first*shortest_path[marked_node] > shortest_path[neighbours[i].second]){ //if shorter path, update

                shortest_path[neighbours[i].second] = neighbours[i].first*shortest_path[marked_node]; //update current optimal path
                pred[neighbours[i].second] = marked_node; //update predecessor
                pseaduo_fib_heap.push({shortest_path[neighbours[i].second],neighbours[i].second});

            }
        }
    }

    return std::make_pair(shortest_path, pred);
}


//updates path vector with the correct path from starting_node to query node, need predecessor vector "preds"
void constructPath(std::vector<int64_t> & path, int64_t starting_node, int64_t query, std::vector<int64_t> preds){

    if(starting_node == query){
        path.push_back(query);
        return;
    }else{
        path.push_back(query);
    }


    if(preds[query] != starting_node){
        constructPath(path, starting_node, preds[query], preds);
    }else{
        path.push_back(starting_node);
        std::reverse(path.begin(), path.end());
    }

    return;
}


int main(){
    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Variable declarations
    int64_t n{}, m{}, s{0};

    while(1){
        std::cin >> n >> m;
        if(n || m){ //if input is 0 0 we stop

            Graph graph = Graph(n); //Initialize a graph object

            int64_t u{}, v{};
            double f{};

            for(int i{0}; i<m; ++i){
                std::cin >> u >> v >> f;
                graph.add(u, v, f); //add all the edges
                graph.add(v, u, f);
            }

            std::pair<std::vector<double>, std::vector<int64_t>> dist_pred = djikstra(s, graph);


            std::cout << std::fixed << std::setprecision(4) << dist_pred.first[n-1] << "\n";



        }else{
            break;
        }
        //std::cout << "\n";
    }

    return 0;
}
