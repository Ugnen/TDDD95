/*
########[Single Source shortest path, time table LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Same as Lab 2.1 but add support to you Dijkstra implementation to handle time table graphs. Input changed from second row to include t_0
(starting time of departure for an edge), P (can be traversed every P minute), w->d;
Changed class Graph to handle these inputs instead of std::pair, now a vector with all inputs.
Added if-statement in Djikstra-function that handles calculation of waiting time for when it is possible to start traversing the edge. Special case is
P == 0 and we missed first departure, then just continue with examining the next edge. If we are before first departure (<t_0), we may just start traversing
the edge t_0 - marked_weight (time it took to get there). If we arrived after first departure but we know we may traverse it again, we start by calculating the
time since last traversal opportunity and then subtracting that from P.
When waiting time is known complexity of the problem is reduced to simple Djikstra (Lab 2.1) again as we simply add the waiting time as a weight when
doing the operation of updating the shortest path.

Regarding time complexity: Time complexity should be same as Lab 2.1, that is to say O(m*log(n)). Argument is still the same: While loop takes at most m (total
ammount of edges) times, multiplied with time it takes to perform selection of the shortest path and updating predecessor/distance (both take O(log(n)))
-> O(m*log(n)).

Copy paste from previous lab :
Time complexity is O(m*log(n)) as we make us of the priority_queue to only look at the unmarked nodes and then check if it is an important
node-weight pair, skipping many. The two operations (selecting the shortest path and updating predecessor/distance) both take O(log(n)) with priority_queue
making the time complexity O(n*log(n) + m*log(n)) = O(m*log(n) in worst case (as m can max be 30 000 and n can max be 10 000). If we could have used a fibonaci heap
the time complexity could have been lowered to O(n*log(n) + m) but it is much more complex to implement. See https://cp-algorithms.com/graph/dijkstra_sparse.html
for further explanation.

Additional notes: Lab 2.2 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 0.1, first version
*/

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
        std::vector<std::vector<std::vector<int64_t>>> sparse_graph; //sparse graph for all nodes

    public:
        Graph(int64_t nodes = 0){ //Create matrix of nodes x nodes
            sparse_graph.resize(nodes);
        }

        //Adds weight-neighbour pairs (edges) from this node
        void add(int64_t node_one, int64_t node_two, int64_t t_0, int64_t P, int64_t d){ //Add neighbouring edges to sparse graph (node and weight)
            sparse_graph[node_one].push_back(std::vector<int64_t>{d, node_two, t_0, P});
        }

        //Returns a list of all weight-neighbour pairs (edges) from this node
        std::vector<std::vector<int64_t>> edges(int64_t node){
            return sparse_graph[node];
        }

        //Return the number of nodes
        int64_t numberOfNodes(){
            return sparse_graph.size();
        }
};


std::pair<std::vector<int64_t>, std::vector<int64_t>> djikstra(int64_t start_node, Graph tree){
    int64_t nrOfNodes{tree.numberOfNodes()};
    //std::cout << "nrOfNodes: " << nrOfNodes << "\n";
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


        std::vector<std::vector<int64_t>> neighbours{tree.edges(marked_node)}; //get all neighbours for marked node

        //iterate through all neighbours
        for(int i{0}; i < neighbours.size(); ++i){
            int64_t d{neighbours[i][0]}; //weight for edge
            int64_t nxt_node{neighbours[i][1]}; //node we want to traverse to
            int64_t t_0{neighbours[i][2]}; //time to first be able to traverse edge
            int64_t P{neighbours[i][3]}; //can start to traverse edge on ever P minute

            if(P == 0 && shortest_path[marked_node] > t_0){ //If P is 0 traversing was a one-time event, traversed distance is larger than t_0 we missed the opportunity
                continue;
            }

            int64_t to_wait{0}; //added weight from waiting for correct time to traverse node

            if(t_0 >= shortest_path[marked_node]){ //Current weight traversed is less than t_0 and we can enter edge on t_0

                to_wait = t_0 - shortest_path[marked_node]; //remaining time until we may traverse

            }else{//Current weight traversed is larger than t_0 and we can only enter on next t_0

                int64_t last_departure = (shortest_path[marked_node] - t_0)%P; //=always positive bcuz of if statement : time since last departure

                if(!last_departure){ //Incase next traversal is same time we arrive -> dont want to wait until next time
                    to_wait = 0; //never have to wait more than
                }else{
                    to_wait = P - last_departure; //never have to wait more than P
                }


            }

            if(d + to_wait + shortest_path[marked_node] < shortest_path[nxt_node]){ //if shorter path, update

                shortest_path[nxt_node] = d + to_wait + shortest_path[marked_node] ; //update current optimal path
                pred[nxt_node] = marked_node; //update predecessor
                pseaduo_fib_heap.push({shortest_path[nxt_node],nxt_node});

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
    int64_t n{}, m{}, q{}, s{};

    while(1){
        std::cin >> n >> m >> q >> s;
        if(n || m || q || s){ //if input is 0 0 0 0 we stop

            Graph graph = Graph(n); //Initialize a graph object

            int64_t u{}, v{}, t_0{}, P{}, d{};

            for(int i{0}; i<m; ++i){
                std::cin >> u >> v >> t_0 >> P >> d;
                graph.add(u, v, t_0, P, d); //add all the edges
            }

            std::pair<std::vector<int64_t>, std::vector<int64_t>> dist_pred = djikstra(s, graph);

            int64_t query_node{};

            while(q--){
                std::cin >> query_node;
                int64_t max_dist = dist_pred.first[query_node];
                if(max_dist != INF){
                    std::cout << dist_pred.first[query_node] << "\n";
                }else{
                    std::cout << "Impossible\n";
                }
            }

            ////////////////////////////////////////
            //For printing path
            ////////////////////////////////////////
            /*std::vector<int64_t> path;
            constructPath(path, s, query_node, dist_pred.second);
            for(int i{0}; i < path.size(); ++i){
                std::cout << path[i] << " ";
            }*/
            ////////////////////////////////////////


        }else{
            break;
        }
        std::cout << "\n";
    }

    return 0;
}
