/*
########[Maximum flow LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to implement an algorithm that finds the maximum flow in a flow graph. This was solved with Dinics algorithm (as Edmund-Karps is
too slow) which consists of two phases: First phase creates a layered network which is a network with the shortest path from s to any node using only positive
capacity edges and then keeps only the edges where level[next_node] = level[current_node] + 1.
Second phase finds the 'blocking flow' which is to say the flow which saturates atleast ONE of the edges along the path from the source to the sink (the
smallest residual capacity) and adds it to the flow of the entire layered network.
Repeat phases until first phase cannot find an augmented path.

Regarding time complexity: Time complexity should be O(V^2*E) where V are the amount of vertices and E the edges. Space complexity is O(V+E) as we store all
vertices and edges. Motivation for time complexity is because the bfs has at worst a time complexity of O(V) (actually terminates in O(V-1)) and the
dfs takes O(VE), at worst, to find the blocking flow, giving the total of the algorithm O(V*VE) = O(V^2*E).

Additional notes: Lab 2.6 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.0, final version, now with Dinic + cleanup and comment.
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

    public:
        Graph(int64_t nodes_amount){ //Constructor
            level.resize(nodes_amount);
            node_ptr.resize(nodes_amount);
            neighbour.resize(nodes_amount);
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

    int64_t maxflow = flow_net.maxFlow(s, t);
    int32_t edges_total{0};

    //output: node-node-flow
    std::vector<std::tuple<int32_t,int32_t,int32_t>> constellation;
    std::vector<Edge> temp_edges = flow_net.list_edges();

    for(int64_t j{0}; j < temp_edges.size(); ++j){
        if(temp_edges[j].flow > 0){
            ++edges_total;
            constellation.push_back(std::make_tuple(temp_edges[j].from_vertice, temp_edges[j].next_vertice, temp_edges[j].flow));
        }
    }

    std::cout << n << " " << maxflow << " " << edges_total << "\n";

    for(int32_t i{0}; i < edges_total; ++i){
        std::cout << std::get<0>(constellation[i])<< " " << std::get<1>(constellation[i]) << " " << std::get<2>(constellation[i])<< "\n";
    }

    return 0;
}
