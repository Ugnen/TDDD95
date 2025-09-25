#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>
#include <limits.h>

const int INF = 1000000000; //global infinity variable, larger than max_nodes*max_nodes (1e8)

//Class Graph
class Graph{
    private:
        std::vector<std::vector<std::pair<int64_t, int64_t>>> sparse_graph; //sparse graph for all nodes

    public:
        Graph(int64_t nodes = 0){ //Create matrix of nodes x nodes
            sparse_graph.resize(nodes);
        }

        //Adds weight-neighbour pairs (edges) from this node
        void add(int64_t node_one, int64_t node_two, int64_t weight){ //Add neighbouring edges to sparse graph (node and weight)
            sparse_graph[node_one].push_back(std::make_pair(weight, node_two));
        }

        //Returns a list of all weight-neighbour pairs (edges) from this node
        std::vector<std::pair<int64_t, int64_t>> edges(int64_t node){
            return sparse_graph[node];
        }

        //Return the number of nodes
        int64_t numberOfNodes(){
            return sparse_graph.size();
        }

        int64_t getWeight(int64_t node, int64_t nxt_node){
            std::vector<std::pair<int64_t, int64_t>> node_edges{edges(node)};
            for(int i{0}; i < node_edges.size(); ++i){
                if(node_edges[i].second == nxt_node){
                    return node_edges[i].first;
                }
            }
            return INF;
        }
};


std::pair<std::vector<int64_t>, std::vector<int64_t>> djikstra(int64_t start_node, Graph tree, Graph monkeyboy){
    int64_t nrOfNodes{tree.numberOfNodes()};

    std::vector<int64_t> shortest_path(nrOfNodes, INF); //shortest path, initial values for shortes path, all start on INF
    std::vector<int64_t> pred(nrOfNodes, -1);    //predecessors, are initialized as -1
    shortest_path[start_node] = 0; //starting node gets default value 0

    std::priority_queue<std::pair<int64_t, int64_t>, std::vector<std::pair<int64_t, int64_t>>, std::greater<std::pair<int64_t, int64_t>>> pseaduo_fib_heap; //a min heap with pairs, same as shortest path
    pseaduo_fib_heap.push({0,start_node}); //push starting node do heap

    while(!pseaduo_fib_heap.empty()){
        int64_t marked_weight = pseaduo_fib_heap.top().first; //get weight of node we want to examine
        int64_t marked_node =  pseaduo_fib_heap.top().second; //get index of node we want to examine
        pseaduo_fib_heap.pop(); //marked edge gets popped


        //Without this check (if it is an important pair) the time complexity increases to O(node*edges), this is bad ofc
        if(!(marked_weight == shortest_path[marked_node])){//if we are not looking at the currently known optimal edge, skip
            continue; //serves as ending condition, will empty the heap without looking at irrelevant pairs
        }


        std::vector<std::pair<int64_t, int64_t>> neighbours{tree.edges(marked_node)}; //get all neighbours for marked node

        //iterate through all neighbours
        for(int i{0}; i < neighbours.size(); ++i){
            int64_t nxt_node = neighbours[i].second;
            int64_t nxt_weight = neighbours[i].first;
            int64_t to_wait{0};

            /*
            1. Check if shortest_path[marked_node] is larger (or equal ... 10min...) than
                    t_0 (when george got there) AND less than t_0 + weight (george is still on road)
                -> Add wait time (= time remaining (which t_0 + weight - shortest_path[marked_node]
            */

            //FIGURE OUR HOW TO GET t_0 TO ALL NODES

            int64_t t_0 = monkeyboy.getWeight(marked_node, nxt_node);

            if(shortest_path[marked_node] >= t_0 && shortest_path[marked_node] < t_0 + nxt_weight){
                to_wait = t_0 + nxt_weight - shortest_path[marked_node];
            }

            if(to_wait + nxt_weight + shortest_path[marked_node] < shortest_path[nxt_node]){ //if shorter path, update

                shortest_path[nxt_node] = to_wait + nxt_weight + shortest_path[marked_node]; //update current optimal path
                pred[nxt_node] = marked_node; //update predecessor
                pseaduo_fib_heap.push({shortest_path[nxt_node],nxt_node});
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
    int64_t N{}, M{}, A{}, B{}, K{}, G{};

    //Read
    std::cin >> N >> M >> A >> B >> K >> G; //

    std::vector<uint64_t> george_nodes(G);

    for(int i {0}; i < G; ++i){
        int64_t temp{};
        std::cin >>  temp;
        george_nodes[i] = temp - 1;
    }

     Graph tree(N);
     uint64_t a{}, b{}, L{};

     for(int i{0}; i < M; ++i){

        std::cin >> a >> b >> L;
        //undirected graph -> add double

        tree.add(a-1,b-1,L); //stupid ass indexing
        tree.add(b-1,a-1,L);
     }

     //Curious George acting a menace
     Graph nicke_nyfiken(N);
     int64_t george_weight = -K;

     for(int i{0}; i < G-1; ++i){
        //undirected graph -> add double
        //"Every pair of adjacent integers denotes a street"
        nicke_nyfiken.add(george_nodes[i], george_nodes[i+1], george_weight);
        nicke_nyfiken.add(george_nodes[i+1], george_nodes[i], george_weight);

        george_weight += tree.getWeight(george_nodes[i], george_nodes[i+1]);
     }


    std::pair<std::vector<int64_t>, std::vector<int64_t>> path_pred = djikstra(A-1, tree, nicke_nyfiken);

    std::cout << path_pred.first[B-1] << "\n";


    return 0;
}
