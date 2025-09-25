#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>

//Class SuffixTrie
//Helpful blog:
//https://www.geeksforgeeks.org/pattern-searching-using-trie-suffixes/
class SuffixTrie{
    private:
        std::unordered_map<char, SuffixTrie *> children;
        //std::vector<SuffixTrie *>children;
        std::vector<int64_t> indices;

    public:
        //Adds the string 'text' and its suffixes to the SuffixTrie
        void addSuffix(std::string const & text){

            //loops through all the suffixes in "text"
            for(int64_t i{0}; i < text.size(); ++i){
                insertSuffix(text.substr(i), i);
            }

            return;
        }

        void insertSuffix(std::string const & suffix, int64_t index){

            indices.push_back(index);

            //Pointer for suffix we are currently working on
            SuffixTrie * base_node{this};

            for(int64_t i{0}; i < suffix.size(); ++i){

                //Check if current node has a child node that with the first letter of suffix
                if(base_node->children[suffix[i]] == NULL){

                    //If no node for character exists, create it
                    base_node->children[suffix[i]] = new SuffixTrie();
                }

                //set base_node to the next node we may go to
                base_node = base_node->children[suffix[i]];
                base_node->indices.push_back(index);
            }

            return;
        }

        //Has to be below to use addSuffix
        SuffixTrie(std::string text = ""){ //init
            addSuffix(text);
        }

        //Finds the "pattern" suffix in the Suffix Trie
        std::vector<int64_t> findSuffix(std::string const & pattern){

            if(!pattern.size()){
                return indices;
            }else if(children[pattern[0]] != NULL){ //if child node exists for first letter of pattern

                //return the indices of the existing child node (recursive)
                return children[pattern[0]]->findSuffix(pattern.substr(1));
            }

            std::vector<int64_t> no_result;
            return no_result;
        }

        //returns true if if more than 3 digits in candidate, also appends indices to digits in candidate
        bool checkDigit(std::string const & candidate, std::vector<int64_t> & digit_idx){
            for(int i{0}; i < candidate.size(); ++i){
                if(isdigit(candidate[i])){
                    digit_idx.push_back(i);
                }
            }
            return digit_idx.size() > 3; //more than 3 digits indicate a strong password apparently
        }

        //checks if 'candidate' is a word, not a suffix, in the SuffixTrie: returns true if word is complete match
        bool findWord(std::string const & candidate){

            std::vector<int64_t> found_at{findSuffix(candidate)};

            //Suffix found
            if(found_at.size() > 0){
                //Suffix is actually one of the starting words and not a sub-string
                //if(found_at[0] == 0){
                    return true;
                //}
            }
            return false;
        }

        //swaps digits for letters then calls transpose() on the resulting 'word'
        bool digitSwapper(std::string candidate, std::vector<int64_t> digit_idx, int64_t digits_removed){

            if(digits_removed == digit_idx.size()){ //no more digits to remove
                if(transpose(candidate, digits_removed)){
                    return true;
                }

            }else{//have more digits to remove

                ///////////////////////
                //This loop will iterate through all digits candidate, example:
                //dictionary word: bOok
                //candidate: b00k
                //1st, will check: b[a-z][a-z]k : FAIL
                //2nd, will check: b[a-z][A-Z]k : FAIL
                //3rd, will check: b[A-z][a-z]k : PASS on bOok
                //4th check would have been: b[A-Z][A-Z]k
                ///////////////////////

                for(char letter{'a'}; letter <= 'z'; ++letter){
                    candidate[digit_idx[digits_removed]] = letter;
                    if(digitSwapper(candidate, digit_idx, digits_removed+1)){
                        return true;
                    }
                }

                for(char letter{'A'}; letter <= 'Z'; ++letter){
                    candidate[digit_idx[digits_removed]] = letter;
                    if(digitSwapper(candidate, digit_idx, digits_removed+1)){
                        return true;
                    }
                }
            }

            //if we get here, candidate is an acceptable password
            return false;
        }

        //changes which foot which sock is on or something
        std::string changingRoom(std::string Lfoot_Rfoot, int64_t sockOne, int64_t sockTwo){
            char dobby = Lfoot_Rfoot[sockOne]; //he is a free elf
            Lfoot_Rfoot[sockOne] = Lfoot_Rfoot[sockTwo];
            Lfoot_Rfoot[sockTwo] = dobby; //deprave dobby of his freedom: he is simply an enslaved house elf and the winds of freedom are temporary
            return Lfoot_Rfoot;
        }

        //returns true if transposing two adjacent characters creates a dictionary word
        bool transpose(std::string candidate, int64_t changes_done){

            //could actually change findWord(candidate) to : findSuffix(candidate).size() > 0
            if(findWord(candidate)){
                return true; //the candidate is a dictionary word: unacceptable password

            }else if(changes_done > 2){
                return false; //acceptable password

            }else{
                for(int i{0}; i < candidate.size() - 1; ++i){
                    if(transpose(changingRoom(candidate, i, i+1), changes_done+1)){
                        return true;
                    }
                }

                return false;
            }

        }

        //Returns true or false if transposing makes the candidate a correct word from dictionary
        bool checkTranspose(std::string candidate, std::vector<int64_t> digit_idx){

            //check if candidate contains digits
            if(digit_idx.size()){
                //check all possible scenarios of candidate by swapping digits for letters
                if(!digitSwapper(candidate, digit_idx, 0)){
                    return true;
                }

            }else{//candidate doesn't contain digits, check all possible transpose combinations
                if(!transpose(candidate, 0)){
                    return true;
                }
            }

            return false;
            //return false;
        }

        //Returns true or false if candidate is an acceptable password
        bool isAcceptable(std::string const & candidate){

            //Check number of digits in word, if more than 3 digits instantly return true
            std::vector<int64_t> digit_idx;
            if(checkDigit(candidate, digit_idx)){
                return true;
            }

            //checkTranspose returns true if it is an acceptable password
            return checkTranspose(candidate, digit_idx);
        }
};





int main(){
    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Variable declarations
    int64_t n{};

    std::string word, candidate;
    SuffixTrie dictionary;

    std::cin >> n;

    std::getline(std::cin, word);
    //Read dictionary words and add to SuffixTrie::dictionary
    while(n--){
        std::getline(std::cin, word);
        dictionary.addSuffix(word);
    }

    //get password candidates as long as we are not at EOF
    while(std::getline(std::cin, candidate)){

        if(dictionary.isAcceptable(candidate)){
            std::cout << candidate << "\n";
        }


    }

    return 0;
}


/*
DICK ATTACK:
less than 3 simple changes makes a unacceptable candidate:
* less than three digits (no transposes)
* less than three transposes (no digits)
* less than three transposes+digits

//IMPORTANT NOTICE: candidate CANNOT be a suffix either or suffix variant (from the hidden test cases).

Algo:
1. Check if number of digits in candidate
-> if > 3 it is acceptable, else go to 2.
2. loop through all possible iterations of word by swapping
each digit for a to z and A to Z
-> once all digits has been swapped to letters go to 3 (do this for all candidate combinations/possibilities)
3. check if it candidate matches with any word in SuffixTrie (found_at.size() > 0 and found_at[0] == 0)
ALT: skip SuffixTrie and just look through a list
OR if swapping any adjacent characters (transposing) makes it match
4. As soon as a candidate fulfills conditions: return true (is a candidate)
-> else return false.

std::vector<int64_t> found_at{dictionary.findSuffix(candidate)};

std::cout << candidate << " found_at.size(): " << found_at.size() << "\n";
for(int i{0}; i < found_at.size(); ++i){
    std::cout << "found_at[" << i << "]: " << found_at[i] << "\n";
}


https://www.learncpp.com/cpp-tutorial/chars/
capital letter go from: A (65) to Z (90)
small letters go from: a (97) to z (122)

//Print a to z:
for(char letter{'a'}; letter <= 'z'; ++letter){
    std::cout << letter << " ";
}


/////////////////////////////////
//Input:
5
book
apple
secret
December
vertigo
book
b00k
alppe
a1ppe
secret
esrcte
erscte
vreitg0
arithmetic
D3c3m63r
d3c3mb3r
ap1p3
/////////////////////////////////
/////////////////////////////////
5
book
apple
secret
December
vertigo
book
/////////////////////////////////

5
book
apple
secret
December
vertigo
testing
book
b00k


*/
