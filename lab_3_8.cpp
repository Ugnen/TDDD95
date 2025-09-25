/*
########[Prime Sieve LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Implement Erathostenes sieve to find all primes upp to a limit n. This was done as a class (FineMeshStrainer = Sieve) where the constructor
take 'n' as input and precalculates the the seive and the number of primes up to that point. A bool array stores which numbers are primes and an int stores
the total primes, calculated at the end of the constructor. Two functions isPrime and getPrime(x) were created, isPrime has constant time queries to the
vector if the input number x is a prime, and getPrimes just returns the member variable which contains the total number of primes.

Regarding time complexity: Time complexity should be O(N*log(log(N))) as we iteratively mark all multiples of the prime we are examining as not primes. We do
this until sqrt(n) (or as in this implementation we just square i as this is a faster operation than std::sqrt). Each inner loop greedily goes through all
multiples until 'n'. Or rather the sum up to prime p: n/2 + n/3 + ... + p = n*(1/2 + 1/3 + 1/5 + ... + p) takes n*(the time to sum) = n*(log(log(n))).
Space complexity should be O(n) as we allocate a vector of size n.

Additional notes: Lab 3.8 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1., clean version + optimization (only check odd numbers,
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <cmath>

//https://www.geeksforgeeks.org/sieve-of-eratosthenes/
//https://cp-algorithms.com/algebra/sieve-of-eratosthenes.html
class FineMeshStrainer{
private:
    //vector with with each being a prime
    std::vector<bool> primes;
    int64_t nrOfPrimes;

public:

    void calcPrimes(){
        for(int64_t i{0}; i <primes.size(); ++i){
            if(primes[i]){
                ++nrOfPrimes;
            }
        }

        return;
    }

    FineMeshStrainer(int64_t n){
        nrOfPrimes = 0;
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
                for(int64_t nMultiple{i*i}; nMultiple <= n; nMultiple += i){
                    primes[nMultiple] = false;
                }
            }
        }

        calcPrimes();
    }

    //returns true if x is a prime, queried in constant time
    bool isPrime(int64_t x){

        return primes[x];
    }

    //getter for nrOfPrimes variable, pre-calculated how many primes in this sieve of Eratosthenes
    int64_t getPrimes(){

        return nrOfPrimes;
    }


};

int main(){

    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int64_t n{}, q{}, x{};

    std::cin >> n >> q;

    FineMeshStrainer pSieve{n};

    std::cout << pSieve.getPrimes() << "\n";

    while(q--){
        std::cin >> x;
        std::cout << pSieve.isPrime(x) << "\n";
    }

    return 0;
}
