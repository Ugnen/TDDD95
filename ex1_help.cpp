#include <map>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>


void replacePlaceholder(std::vector<std::string>& phrase, std::string placeholder, std::string word)
{
    for(int it = 0; it < phrase.size(); ++it){
            if(phrase[it] == placeholder){
                phrase[it] = word;
            }
    }
}

bool comparePlaceholder(std::string wordOne, std::string wordTwo) //if true wordOne is placeholder and wordTwo is not
{
    return (wordOne.find('<') != std::string::npos && wordTwo.find('<') == std::string::npos);
}

int main(void)
{

    std::string cases;
    std::string case_line1;
    std::string case_line2;
    int phrase_length = 0;
    int iterator = 0;
    int iterator_max = 0;
    std::getline(std::cin,cases);
    iterator_max = stoi(cases);

    while(iterator != iterator_max){
        std::getline(std::cin,case_line1);
        std::getline(std::cin,case_line2);
        std::vector<std::string> phrase_vector1;
        std::vector<std::string> phrase_vector2;
        std::stringstream stream_line1(case_line1);
        std::stringstream stream_line2(case_line2);

        std::string word1 = "";
        std::string word2 = "";

        while(std::getline(stream_line1, word1, ' ')){
            phrase_vector1.push_back(word1);
        }
        while(std::getline(stream_line2, word2, ' ')){
            phrase_vector2.push_back(word2);
        }
        if(phrase_vector1.size() != phrase_vector2.size()){
            std::cout << "-" << "\n";
            ++iterator;
            continue;
        }

labelCompare:
        for(int k = 0; k < phrase_vector1.size(); ++k){
            if(comparePlaceholder(phrase_vector1[k], phrase_vector2[k])){
                    //std::cout << phrase_vector1[k] << " before " << phrase_vector2[k] << std::endl;
                    replacePlaceholder(phrase_vector1, phrase_vector1[k], phrase_vector2[k]);
                    //std::cout << phrase_vector1[k] << " after " << phrase_vector2[k] << std::endl;
                    goto labelCompare;
            }
        }

        for(int j = 0; j < phrase_vector2.size(); ++j){
            if(comparePlaceholder(phrase_vector2[j], phrase_vector1[j])){
                    //std::cout << phrase_vector1[k] << " before " << phrase_vector2[k] << std::endl;
                    replacePlaceholder(phrase_vector2, phrase_vector2[j], phrase_vector1[j]);
                    //std::cout << phrase_vector1[k] << " after " << phrase_vector2[k] << std::endl;
                    goto labelCompare;
            }
        }

        for(int l = 0; l < phrase_vector1.size(); ++l){ //special compare if both are placeholders
            if(comparePlaceholder(phrase_vector1[l], "word") && comparePlaceholder(phrase_vector2[l], "word")){
                //std::cout << phrase_vector1.size() <<" : kommer hit\n";
                replacePlaceholder(phrase_vector1, phrase_vector1[l], "word");
                replacePlaceholder(phrase_vector2, phrase_vector2[l], "word");
                //std::cout << "först: " << phrase_vector1[l] << " andra: " << phrase_vector2[l] << "\n";
                goto labelCompare;
            }//else{
                //std::cout << "först: " << phrase_vector1[l] << " andra: " << phrase_vector2[l] << " iterator: ";
                //std::cout << l << "\n";
            //}
        }

        if(phrase_vector1 == phrase_vector2){
            for(int it2 = 0; it2 < phrase_vector1.size(); ++it2){
                std::cout << phrase_vector1[it2] << " ";
            }
            std::cout << "\n";
        }else{
            std::cout << "-" << "\n";
            ++iterator;
            continue;
        }

        /*if(comparePlaceholder("<hej>","hej")){

        }*/

        //std::cout << phrase_vector1.size() << std::endl;
        //std::cout << phrase_vector2.size() << std::endl;

        ++iterator;
    }
}

    /*
    std::unordered_map<std::string, std::string> dictionary;
    std::string cases;
    std::string case_line1;
    std::string case_line2;
    int phrase_length = 0;
    int iterator = 0;
    int iterator_max = 0;
    std::getline(std::cin,input_string);
    iterator_max = stoi(input_string);
    std::list<std::string> insertion_order;

    while(iterator != iterator_max){
        std::getline(std::cin,case_line1);
        std::getline(std::cin,case_line2);
        std::vector<std::string> phrase_vector1;
        std::vector<std::string> phrase_vector2;
        std::stringstream stream_line1(case_line1);
        std::stringstream stream_line2(case_line2);
        std::string value = "";
        std::string key = "";
        std::vector<std::string> final_phrase;
        int phrase_check = 0;

        while((std::getline(stream_line1, key, ' ') && std::getline(stream_line2, value, ' '))){

            if(key.find('<') != std::string::npos){
                auto it = find(phrase_vector1.begin(), phrase_vector1.end(), key);
                if(it != phrase_vector1.end()){
                    int index = it - phrase_vector1.begin();
                    if(value != final_phrase[index]){
                        final_phrase.clear();
                        final_phrase.push_back("-");
                        break;    //break here since the value (word) isn't the same as last time the placeholder appeared
                    }
                }
                final_phrase.push_back(value);   //found and correct

            }else if(value.find('<') != std::string::npos){
                auto it = find(phrase_vector2.begin(), phrase_vector2.end(), value);
                if(it != phrase_vector2.end()){
                    int index = it - phrase_vector2.begin();
                    if(key != final_phrase[index]){
                        final_phrase.clear();
                        final_phrase.push_back("-");
                        break;    //break here since the value (word) isn't the same as last time the placeholder appeared
                    }
                }
                final_phrase.push_back(key);     //not found

            }else{
                final_phrase.push_back(key);
                //neither is placeholder
            }

            phrase_vector1.push_back(key);
            phrase_vector2.push_back(value);
            ++phrase_length;
        }


        for(auto& element: final_phrase){
            std::cout << element << " ";
        }
        std::cout << std::endl;

        final_phrase.clear();
        phrase_vector1.clear();
        phrase_vector2.clear();
        phrase_length = 0;
        ++iterator;
    }

    */
//}
/*
get words, ++ on phrase length
check where there are placeholders in the phrases

**
case - only one of each <placeholder>:
Check: are there <placeholders> in the same place for both phrases?
* IF YES: -
* IF NO: replace with word from other phrase.


**
case 1 - if there are no placeholders in one but is in the other:
check if there are 2 or more the the same placeholders and in which position
-> is it the same word in the no-placeholder phrase?
* IF YES: use no placeholder phrase
* IF NO: -



FIND INDEX WITH PLACEHOLDERS
COMPARE PLACEHOLDERS (GET AMMOUNT OF EACH)
*/
