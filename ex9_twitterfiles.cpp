#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <unordered_map>


//Class SuffixTrie
class SuffixTree{
    private:
        std::unordered_map<char, SuffixTree *> children;
        char term_char;

    public:
        //Adds the string 'text' and its suffixes to the SuffixTree
        void addWord(std::string const & word){

            insertWord(word);

            return;
        }

        void insertWord(std::string const & text){

            //Pointer for suffix we are currently working on
            SuffixTree * base_node{this};

            for(int64_t i{0}; i < text.size(); ++i){

                //Check if current node has a child node that with the first letter of suffix
                if(base_node->children[text[i]] == NULL){

                    base_node->children[text[i]] = new SuffixTree();
                }

                //set base_node to the next node we may go to
                base_node = base_node->children[text[i]];

            }

            ///////////////////////////////////////////
            //: https://www.geeksforgeeks.org/pattern-searching-using-trie-suffixes/
            ///////////////////////////////////////////
            base_node->term_char = '\0';

            return;
        }
        int countSubstring(std::string message){
            int counter{0};
            std::vector<SuffixTree*> node_list;

            for(int i{0}; i < message.size(); ++i){

                //ALL NODES (start with last, should give shortest shortest word)
                for(int j{(int)node_list.size()-1}; j>=0; --j){

                    //if the next letter (current) exists as a child node -> word keeps going so we move towards end of word
                    if(node_list[j]->children.find(message[i]) != node_list[j]->children.end()){
                        node_list[j] = node_list[j]->children[message[i]];

                    }else{//else delete element as it wasn't a word when last checked and there are more letters
                        node_list.erase(node_list.begin() + j);
                    }
                }

                //check if at start of word
                if(this->children.find(message[i]) != this->children.end()){
                    //if the iterator exists (for letter) at top of dictionary (== not the end()) it is a start of a word
                    node_list.push_back(this->children[message[i]]);
                }

                //check if we have found a word yet <-- we do this at the end to make sure we always find the shortest word first
                for(auto node : node_list){//check if node is end of word
                    if(node->term_char == '\0'){
                        node_list.clear();
                        node_list.shrink_to_fit();
                        counter++;
                        break;
                    }
                }
            }


            return counter;
        }

        //Has to be below to use addSuffix
        SuffixTree(){ //init
            term_char = '-';
            //addWord(text);
        }

        ~SuffixTree(){
            for (auto iter = children.begin(); iter != children.end(); iter++) {
                delete iter->second;
            }
        }
};


std::string dickRead(){

    std::string data;
    //read a char
    char c = getc(stdin);

    //end-cond is if '\n'
    while(c != '\n'){
        data += c;
        c = getc(stdin);
    }

    return data;
}

std::string messRead(){
    std::string message;

    //read a char
    char c = getc(stdin);

    //check if end of messages
    if(c == '#'){
        message += c;
        c = getc(stdin);
        return message;
    }

    //keep reading until end char
    while(c != '|' && c != EOF){

        if(c != '\n'){
            message += c;
        }

        c = getc(stdin);
    }

    //read last line-break
    c = getc(stdin);

    return message;
}

int main(){
    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    //Variable declarations
    SuffixTree dictionary;

    //dictionary
    while(1){
        std::string input{dickRead()};
        if(input == "#"){
            break;
        }

        dictionary.addWord(input);
    }

    //message strings
    while(1){
        std::string message{messRead()};

        if(message == "#"){
            break;
        }

        std::cout << dictionary.countSubstring(message) << "\n";
    }

    return 0;
}

/*
Pseudo:
loop through entire message string:
###
COMPARE EACH PREVIOUS ITERATIONS:
check all saved nodes too see if next ('current') letter of message string is end of a word in dictionary
-> update node: go to it
-> else: not a complete word, remove current node being examined

COMPARE EACH LETTER TO START OF WORDS:
check if letter of message string is start of word
-> if yes: SuffixTree node to a list


check if ANY of the Nodes is the end of a word (order doesnt matter as they will cut text at same point)
-> ++word count
-> clear all current nodes as we found the shorts word (of given letters so far)
-> break
###

*/
