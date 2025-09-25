#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>
#include <limits.h>

std::vector<std::vector<char>> map_matrix; //Matrix with characters (map)
std::vector<std::vector<int32_t>> visited; //Matrix with information if tile has been visited



int64_t checkTile(int32_t X, int32_t Y){
    int64_t gold_count;

    if(visited[Y][X] || map_matrix[Y][X] == '#') //if visited or wall -> return 0
    {
        //std::cout << "visited or wall\n";
        return 0;
    }

    if(map_matrix[Y][X] == 'G')//if gold -> gold_count = 1 else gold_count = 0
    {

        gold_count = 1;
    }else{
        gold_count = 0;
    }

    visited[Y][X] = 1; //set tile as visited


    if(map_matrix[Y][X-1] == 'T' || map_matrix[Y][X+1] == 'T' || map_matrix[Y-1][X] == 'T' || map_matrix[Y+1][X] == 'T')//if any tile directly next to this has T -> return gold_count
    {
        return gold_count;
    }
    gold_count += checkTile(X-1, Y) + checkTile(X, Y-1) + checkTile(X+1, Y) + checkTile(X, Y+1); //recursively check all cardinal directions


    return gold_count;
}


int main(){
    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Variable declarations
    int64_t W{}, H{};

    //Read
    std::cin >> W >> H; //width and height of map


    char tile;
    std::pair<int32_t,int32_t> player_pos; //starting position <x, y>


    //get map info
    for(int32_t y_pos{0}; y_pos < H;  ++y_pos){
        std::vector<char> temp_vec(W);
        std::vector<int32_t> temp_visit(W);

        for(int32_t x_pos{0}; x_pos < W; ++x_pos){
            std::cin >> tile;
            temp_vec[x_pos] = tile;
            temp_visit[x_pos] = 0;

            if(tile == 'P'){ //Save starting position
                player_pos.first = x_pos;
                player_pos.second = y_pos;
            }
        }
        map_matrix.push_back(temp_vec);
        visited.push_back(temp_visit);
    }

    std::cout << checkTile(player_pos.first, player_pos.second)<< "\n";


    return 0;
}

