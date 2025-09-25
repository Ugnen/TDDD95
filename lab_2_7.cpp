/*
########[Minimum Cut LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to implement an algorithm that finds the minimal cut in a flow graph, a minimal cut being the subset of of nodes U to the nodes V
where the sum of the capacities in U to V\U is minimal. This was done with the Max-flow min-cut theorem and a DFS checking the residual capacities.

Regarding time complexity: Time complexity should be O(V^2*E), the same as for Max Flow using Dinic as the additional DFS at most is O(V) if every node is
visited. -> O(V^2*E + V) = O(V^2*E). Same space complexity as Max Flow, aka O(V + E).

Additional notes: Lab 2.7 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.0, final version, with comments.
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <bitset>
#include <numeric>
#include <cmath>
#include <queue>
#include <utility>

const int64_t INF = 10000000000;

//data-structure for handling edges
struct Edge{
    int64_t from_vertice;
    int64_t next_vertice;
    int64_t capacity;
    int64_t flow;

    //constructor
    Edge(int64_t u, int64_t n, int64_t c, int64_t f){
        from_vertice = u;
        next_vertice = n;
        capacity = c;
        flow = f;
    }
};

//Class Graph
class Graph{
    private:
        std::vector<Edge> network; //an unsorted vector containing all the edges
        std::vector<int64_t> level; //levels for the layered network
        std::vector<int64_t> node_ptr; //used to say which edge is next to be used
        std::vector<std::vector<int64_t>> neighbour;
        std::vector<bool> reachable;

        //Breadth first search algorithm, finds and returns true if an augmented path between the source and sink is found
        //edits the level vector so that the augmented path is contains non-negative integers
        bool breadth_first_search(int64_t source, int64_t sink){

            std::fill(level.begin(), level.end(), -1);
            level[source] = 0;

            std::queue<int64_t> unvisited;
            unvisited.push(source);

            while(!unvisited.empty()){
                int64_t current_node = unvisited.front();
                unvisited.pop();

                for(int64_t i : neighbour[current_node]){
                    if(network[i].capacity == network[i].flow){continue;}
                    if(level[network[i].next_vertice] != -1){continue;}

                    level[network[i].next_vertice] = level[current_node] + 1;
                    unvisited.push(network[i].next_vertice);
                }
            }
            return level[sink] != -1;
        }

        //Depth-first-search algorithm finds and applies the allowed flow to all edges in the augmented path
        int64_t depth_first_search(int64_t node, int64_t sink, int64_t flow){

            if(flow == 0){return 0;}
            if(node == sink){return flow;}

            for(int64_t& i = node_ptr[node]; i < (int64_t)neighbour[node].size(); i++){
                int64_t edge_id = neighbour[node][i];
                int64_t nxt_node = network[edge_id].next_vertice;

                if(level[node] + 1 != level[nxt_node]){
                    continue;
                }

                //recursive to find the allowed flow (max residual capacity) aka the blocking flow
                int64_t current_flow = depth_first_search(nxt_node, sink, std::min(flow, network[edge_id].capacity - network[edge_id].flow));

                if(current_flow == 0){
                    continue;
                }

                network[edge_id].flow += current_flow;
                network[edge_id ^ 1].flow -= current_flow; //bitwise XOR

                return current_flow;

            }

            return 0;

        }

        //Depth first search for all reachable nodes
        void checkReach(int64_t node){

            //mark node as reachable
            reachable[node] = true;
            for(int64_t i : neighbour[node]){

                //if we have residual capacity and have not already marked node as reachable
                if((network[i].capacity - network[i].flow) && !reachable[network[i].next_vertice]){

                    //go deeper
                    checkReach(network[i].next_vertice);
                }
            }
        }

    public:
        Graph(int64_t nodes_amount){ //Constructor
            level.resize(nodes_amount);
            node_ptr.resize(nodes_amount);
            neighbour.resize(nodes_amount);
            reachable.resize(nodes_amount, false);
        }

        ~Graph(){
        }

        //Adds edges from this vertice
        void add(int64_t node_one, int64_t node_two, int64_t capacity){ //Add neighbouring edges to network (vertice and capacity)
            neighbour[node_one].push_back(network.size());
            network.push_back(Edge(node_one, node_two, capacity, 0));

            neighbour[node_two].push_back(network.size());
            network.push_back(Edge(node_two, node_one, 0, 0));
        }

        //Getter, returns a list of all edges in the network
        std::vector<Edge> list_edges(){
            return network;
        }

        //maxflow function that first finds the augmented path (if one exists) and the finds and applies the flow via dfs. returns the max-flow of the graph
        uint64_t maxFlow(int64_t source, int64_t sink){
            uint64_t flow{0};
            while(true){

                //While we can find an augmented path
                if(!breadth_first_search(source, sink)){break;}

                std::fill(node_ptr.begin(), node_ptr.end(), 0);
                //find and apply flow while possible
                while(int64_t new_flow = depth_first_search(source, sink, INF)){
                    flow += new_flow;
                }
            }
            return flow;
        }

        //minCut function using the fact the Max-flow min-cut theorem
        //after finding the max flow we can simply check the residual capacities of each edge starting from source node and mark if we can reach it or
        //if the edge is blocked.
        //all nodes that were reached are a single subset (U), and the rest is another subset (V\U)
        std::vector<int64_t> minCut(int64_t source, int64_t sink){
            uint64_t max_flow{maxFlow(source, sink)};

            checkReach(source);

            std::vector<int64_t> min_cut_nodes;

            //store nodes of subset U
            for(int i{0}; i < reachable.size(); ++i){
                if(reachable[i]){min_cut_nodes.push_back(i);}
            }
            return min_cut_nodes;
        }
};

//Reads an int64 from stdin
int64_t intRead(){

    //init data
    int64_t data{0};

    //read a char
    char c = getc(stdin);

    //Read while char is between 0 - 9 [ASCII]
    while(c > 47 && c < 58){
        data *= 10;
        data += c - 48;
        c = getc(stdin);
    }

    return data;
}

int main(){

    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int64_t n{}, m{}, s{}, t{}, u{}, v{}, c{};

    n = intRead();
    m = intRead();
    s = intRead();
    t = intRead();

    //Create graph
    Graph flow_net(n);

    //Read and add edges to graph
    for(int64_t i{0}; i < m; ++i){
        u = intRead();
        v = intRead();
        c = intRead();

        flow_net.add(u,v,c);
    }

    std::vector<int64_t> min_cut = flow_net.minCut(s, t);
    std::cout << min_cut.size() << "\n";
    for(int64_t i{0}; i < min_cut.size(); ++i){
        std::cout << min_cut[i] << "\n";
    }

    return 0;
}
