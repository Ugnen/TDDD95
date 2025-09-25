#include <iostream>
#include <vector>
#include <bitset>

//Returns true if any combination of literals fulfills all clauses, otherwise false
bool checkClauses(std::vector<std::vector<int32_t>> clauses, int32_t n){
    int32_t nr_var{n};
    int64_t big_end = 2<<(nr_var-1);

    //for each possible literal (max 2^20 = 1 048 576)
    for(int64_t i{0}; i < big_end; ++i){
        std::bitset<21> literals(i);
        bool all_satisfied{false};

        //for all clauses
        for(int j{0}; j<clauses.size(); ++j){


            bool satisfied_clause{false};

            //for each literal in a clause
            for(int k{0}; k < nr_var  ; ++k){

                //check if clause is already satisfied
                if(satisfied_clause){break;}

                //check the literal
                if(clauses[j][k+1] == 2){
                    satisfied_clause = (satisfied_clause || true);
                }else if(clauses[j][k+1] == 1){
                    satisfied_clause = (satisfied_clause || (!(literals[k] ^ 1)));

                }else if(clauses[j][k+1] == -1){
                    satisfied_clause = (satisfied_clause || (!(literals[k] ^ 0)));
                }

            }

            if(!satisfied_clause){
                break;
            }

            if(j == clauses.size() - 1){
                return true; //all_satisfied;
            }
        }
    }

    return false;
}

//Reads an int32 from stdin
int32_t intRead(char & c){

    //init data
    int32_t data{0};

    //read a char
    c = getc(stdin);

    //Read while char is between 0 - 9 [ASCII]
    while(c > 47 && c < 58){
        data *= 10;
        data += c - 48;
        c = getc(stdin);
    }

    return data;
}

//reads clauses
void santaRead(std::vector<int32_t> & claus){
    int32_t index{};
    bool neg{false};

    //read a char
    char c{};

    //reading until end char
    while(c != '\n' && c != EOF){

        //check if char is ~ == 126 (literal is a negative)
        if(c == 126){
            neg = true;
            c = getchar();
        }


        //check if char is X == 88
        if(c == 88){

            //get number, save c
            index = intRead(c);

            //if negative literal
            if((claus[index] == 1 && neg) || (claus[index] == -1 && !neg) || claus[index] == 2){
                claus[index] = 2;
                neg = false;
            }
            else{
                if(neg){
                    claus[index] = -1;
                    neg = false;
                }
                else{
                    claus[index] = 1;
                }
            }

            continue;
        }

        c = getc(stdin);
    }
    return;
}



int main(){

    //desync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    char discard;

    int32_t cases{intRead(discard)};

    while(cases--){
        int32_t n{intRead(discard)}, m{intRead(discard)};

        std::vector<std::vector<int32_t>> clauses;

        for(int i{0}; i < m; ++i){
            std::vector<int32_t> claus(n+1,0);
            santaRead(claus);
            clauses.push_back(claus);
        }


        if(checkClauses(clauses, n)){
            std::cout << "satisfiable\n";
        }else{
            std::cout << "unsatisfiable\n";
        }

        //debug print
        /*
        for(int k{0}; k < m; ++k){
            for(int j{1}; j < clauses[k].size(); ++j){
                std::cout << "[" << clauses[k][j] << "]";
            }
            std::cout << "\n";
        }*/

    }

    return 0;
}
