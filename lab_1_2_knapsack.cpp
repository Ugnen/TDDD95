/*
########[Knapsack LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to implement a function that solves the classic Knapsack problem. We are presented with a sack that may carry
a certain Capacity, C, and we get a certain ammout of objects, n. These objects all have different values and weights and its the functions
assignment to figure out how to maximize the value of our knapsack and then present i) how many objects we took, ii) which these objects were (indices).
To solve this problem a vector<pair>> was created with all the input values as pairs. This was then sent to the function solveKnapsack that uses
a Bottom-up-Approach to create a capacity/object table where each cell is the maximum value we can get for that object+capacity combo. Then to find the
maximum value we can have in our knapsack we look at the bottom-right cell (largest value/weight) and use this max capacity to select our objects,
starting with the largest possible.

Regarding time complexity: Time complexity is O(n*capacity) since we loop through the max number of objects and maximum capacity to create our decision
table.


Additional notes: Lab 1.2 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.3, Clean version - sort{slight diff but looks ugly on output} + sync input() {no diff but good praxis}.
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

std::vector<std::pair<int, int>> readCases(){
    std::vector<std::pair<int, int>> case_list;
    int value, weight;
    std::pair<int, int> value_weight;
    std::string case_line{""};
    std::getline(std::cin, case_line);

    //read from 1 - 30 cases
    while(!case_line.empty()){
        std::stringstream stream_line(case_line);

        std::getline(stream_line, case_line, ' ');
        value = stoi(case_line);
        std::getline(stream_line, case_line, ' ');
        weight = stoi(case_line);

        value_weight = {value, weight};
        case_list.push_back(value_weight);

        std::getline(std::cin, case_line); //get next line
    }

    return case_list;
}

//https://www.geeksforgeeks.org/0-1-knapsack-problem-dp-10/ -> "Bottom-up Approach"
std::vector<int> solveKnapsack(int Capacity, int n, std::vector<std::pair<int, int>> sacks){
    std::vector<int> chosen_objects; //Output vector containing indices

    int max_rows{n+1},max_columns{Capacity+1};
    std::vector<std::vector<int>> decision_table(max_rows , std::vector<int>(max_columns)); //Rows are objects, columns are their weight

    //Create table, bottom up
    for(int r = 0; r < max_rows ; r++){ //for every object in the case
        for(int k = 0; k < max_columns; k++){
            if(!r || !k){ //If on row 0 or column 0

                decision_table[r][k] = 0; //popoulate row & column 0 with 0s (no object)

            }else if(sacks[r-1].second <= k){
                decision_table[r][k] = std::max(sacks[r-1].first + decision_table[r-1][k - sacks[r-1].second],
                                           decision_table[r-1][k]); //select maximum value that can fit into table slot

            }else{
                decision_table[r][k] = decision_table[r-1][k]; //No room more knapsack (table slot), set it to previous objects value
            }
        }
    }
    //decision_table[n][Capacity] now contains the maximum value we can enter into knapsack
    //find out which objects these are by first selecting the objects with largest value
    //reduce until no more objects can be chosen

    //Will add indices in largest -> smallest
    while(n && Capacity){ //While not at object 0 (no object/end of sack)
        if(decision_table[n][Capacity] == decision_table[n-1][Capacity]){ //If object was not added knapsack (else statement above/too heavy)

            --n; //object on current row too heavy, reduce to next row/object

        }else{

            chosen_objects.push_back(n-1); //Objects are index from 0 to N according to problem statement
            Capacity -= sacks[n-1].second; //remove weight of chosen object from capacity
            --n; //go to next row to look for a possible object

        }
    }

    return chosen_objects;
}


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Variable declarations
    int C, n; //Capacity of knapsack (C) and number of objects (n)
    std::vector<std::pair<int, int>> knapsacks;
    std::vector<int> dp_choices;

    //Read
    knapsacks = readCases();

    int tmp{};
    for(int i = 0; i < knapsacks.size(); i += n + 1){
        //Variables
        C = knapsacks[i].first;
        tmp = i+1;
        n = knapsacks[i].second;
        std::vector<std::pair<int,int>> objects(knapsacks.begin() + tmp, knapsacks.begin() + tmp + n);

        //Solve
        dp_choices = solveKnapsack(C, n, objects); //send in the correct number of objects

        //Sort, 0.01s diff
        //std::sort(dp_choices.begin(), dp_choices.end()); //not necessary but look clean, remove if too slow

        //Print
        std::cout << dp_choices.size() << "\n";
        for(int k = 0; k < dp_choices.size(); k++){
            std::cout << dp_choices[k] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
