#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>
#include <limits.h>

int main(){
    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Variable declarations
    //which outcomes can you get to from indexed starting outcome NNN = 0, YYY = 7
    std::vector<std::vector<int>> flip_one {{1, 2, 4}, {0, 3, 5}, {0, 3, 6}, {1, 2, 7}, {0, 5, 6}, {1, 4, 7}, {2, 4, 7}, {3, 5, 6}};
    std::vector<std::string> outcomes_table {"NNN", "NNY", "NYN", "NYY", "YNN", "YNY", "YYN", "YYY"}; //for easy output print

    int64_t n{};

    //Read
    std::cin >> n; //number of voting rounds

    while(n--){
        int64_t m{}, pref{}; //number of voting priest in this round

        std::cin >> m;

        std::vector<std::vector<int>> voting_matrix(m, std::vector<int>(8));
        std::vector<std::vector<int>> decision_matrix(m, std::vector<int>(8));

        //get voting rounds data
        for(int priest = 0; priest < m;  ++priest){
            for(int outcome = 0; outcome < 8; ++outcome){
                std::cin >> pref;
                voting_matrix[priest][outcome] = pref;
            }
        }

        for(int priest{m - 1}; priest  >= 0; --priest){
            for(int outcome{0}; outcome < 8; ++outcome){

                if(priest != m-1){ //most priest have to take into account current state and wanted outcome

                    //what outcome can this priest do in this scenario
                    int64_t choice_one = voting_matrix[priest][decision_matrix[priest+1][flip_one[outcome][0]]];
                    int64_t choice_two = voting_matrix[priest][decision_matrix[priest+1][flip_one[outcome][1]]];
                    int64_t choice_three = voting_matrix[priest][decision_matrix[priest+1][flip_one[outcome][2]]];

                    //choose according to lowest -> "Lower number indicate higher preference"

                    if(choice_one < choice_two && choice_one < choice_three){
                        decision_matrix[priest][outcome] = decision_matrix[priest+1][flip_one[outcome][0]];

                    }else if(choice_two < choice_three){
                        decision_matrix[priest][outcome] = decision_matrix[priest+1][flip_one[outcome][1]];
                    }else{
                        decision_matrix[priest][outcome] = decision_matrix[priest+1][flip_one[outcome][2]];
                    }

                }else{ //first iteration is different as it has no priest after it to account for

                    //what outcome can this priest do in this scenario
                    int64_t choice_one = voting_matrix[priest][flip_one[outcome][0]];
                    int64_t choice_two = voting_matrix[priest][flip_one[outcome][1]];
                    int64_t choice_three = voting_matrix[priest][flip_one[outcome][2]];

                    //choose according to lowest -> "Lower number indicate higher preference"
                    if(choice_one < choice_two && choice_one < choice_three){
                        decision_matrix[priest][outcome] =


                    }else if(choice_two < choice_three){
                        decision_matrix[priest][outcome] = flip_one[outcome][1];
                    }else{
                        decision_matrix[priest][outcome] = flip_one[outcome][2];
                    }
                }

            }
        }

        std::cout << outcomes_table[decision_matrix[0][0]] << "\n";


    }

    return 0;
}

