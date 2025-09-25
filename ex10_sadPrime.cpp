#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <unordered_map>

class FineMeshStrainer{
private:
    //vector with with each being a prime
    std::vector<bool> primes;

public:
    FineMeshStrainer(int64_t n){
        primes.resize(n+1,true);

        //0 and 1 are not primes as the have 1 factor not 2
        primes[0] = false;
        primes[1] = false;

        //for all composites of 2
        for(int64_t nMultiple{4}; nMultiple <= n; nMultiple += 2){
            primes[nMultiple] = false;
        }

        //loop through entire vector/array
        for(int64_t i = 3; i*i <= n; i+=2){

            //if still marked as prime (from init)
            if(primes[i]){
                //loop through all multiples of the prime
                for(int64_t nMultiple{2*i}; nMultiple <= n; nMultiple += i){
                    primes[nMultiple] = false;
                }
            }
        }

    }

    //returns true if x is a prime, queried in constant time
    bool isPrime(int64_t x){

        return primes[x];
    }

};

int main(){

    //std::ofstream myfile;
    //myfile.open ("ex10output.txt");


    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    FineMeshStrainer PrimeSieve{10000}; // 1 <= n <= 10000

    int64_t P{}, K{}, m{};

    std::cin >> P;

    while(P--){
        std::vector<bool> visited(325,false); //size is 9999 -> 4*9^2 = 324, add one for indexing
        std::cin >> K >> m;
        //std::cout << "P: " << P << "\n";


        //check if candidate is a prime
        if(!PrimeSieve.isPrime(m)){
            //myfile << K << " " << m << " NO\n";

            std::cout << K << " " << m << " NO\n";
            continue;
        }

        int64_t sum{0}, digit{}, rest{m};
        //break up m into each of its component digits
        /*
        while(rest){
            digit = rest%10; //get right most digit
            rest = rest/10; //rest of digits

            sum += digit*digit;
        }*/

        //stopping condition (not happy), or if reduced enough (= 1)
        while(1){ // !visited[sum]){

            //break up m into each of its component digits
            while(rest){
                digit = rest%10; //get right most digit
                rest = rest/10; //rest of digits
                //std::cout << "digit: " << digit << " rest: " << rest << "\n";
                sum += digit*digit;
            }

            if(visited[sum]){
                //myfile << K << " " << m << " NO\n";
                std::cout << K << " " << m << " NO\n";
                break;
            }

            if(sum == 1){
                //myfile << K << " " << m << " YES\n";
                std::cout << K << " " << m << " YES\n";
                break;
            }

            visited[sum] = true;
            rest = sum;
            sum = 0;
        }

    }

    //myfile.close();

    return 0;
}
