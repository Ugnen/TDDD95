/*
########[Fast Fourier Transform LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to implement polynomial multiplication using the Cooley-Tukey fft. The fft was implemented by using divide and conquer, where we
split the coefficient array (called pol_a) into an even and odd part. Then recursively applying the fft and applying the roots of unity (eulers forumla +
 butterfly transform). The polynomial multiplication was solved by first padding each polynomial to correct lengt, then taking the fourier transform of each
 vector separately. Then element wise multiplying the both vectors together before finally taking the inverse fourier transform back. Like this:
 DFT(A*B) = DFT(A)*DFT(B) && IDFT(DFT(A*B)) = A*B ==> A*B = IDFT(DFT(A)*DFT(B)) (where DFT is the discrete fourier transform applied by the fft function).

Regarding time complexity: Total time complexity should be O(n*log(n)) (the complexity of Cooley-Tukey fft algorithm) and the algorithm works for multiplying
numbers smaller than 10^6 before the precision of the floating point numbers falters. The polymul function runs in O(n*log(n)) + O(n) = O(n*log(n)).

Additional notes: Lab 1.6 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 0.1, first version + comments
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
#include <bitset>
#include <numeric>
#include <cmath>
#include <complex>

const double pi = std::acos(-1);

//Cooley and Tukey FFT
void fft(std::vector<std::complex<double>> & pol_a, bool inverse){
    int32_t n = pol_a.size();
    if(n == 1){return;}

    //DIVIDE
    //split into even and odd powers
    std::vector<std::complex<double>> pol_even(n/2), pol_odd(n/2);
    for(int32_t i{0}; i < n/2; ++i){
        pol_even[i] = pol_a[2*i]; //even
        pol_odd[i] = pol_a[1 + 2*i]; //odd
    }

    //recursively call fft() on each vector (recursive DFT, or IDFT)
    fft(pol_even, inverse);
    fft(pol_odd, inverse);

    double angle{};

    //decide angle change if IDFT or DFT
    if(inverse){
        angle = -2*pi/n;
    }else{
        angle = 2*pi/n;
    }

    //roots of unity : e^2*pi*i*k/n
    std::complex<double> unity_root(1,0), unity_change(cos(angle), sin(angle));

    //CONQUER
    //evaluate at roots of unity
    for(int32_t i{0}; i < n/2; ++i){

         //butterfly transform: x + y & x - y
         pol_a[i] = pol_even[i] + unity_root*pol_odd[i];
         pol_a[i + n/2] = pol_even[i] - unity_root*pol_odd[i];

         //Inverse DFT : division of 2 at each level of recursion gives final division of n
         if(inverse){
            pol_a[i] /= 2;
            pol_a[i + n/2] /= 2;
         }

         //update unity angle
         unity_root *= unity_change;
    }
}


//polynomial multiplication : pass polynomial vectors as references to minimize copying
std::vector<int32_t> polymul(std::vector<int32_t> const & pol_one, std::vector<int32_t> const & pol_two){

    //create 'new' fft vectors that we may resize to fit criteria
    std::vector<std::complex<double>> fft_one(pol_one.begin(), pol_one.end());
    std::vector<std::complex<double>> fft_two(pol_two.begin(), pol_two.end());

    //size criteria:
    //*) n, the number of coefficients, is a power of 2
    //*) when multiplying the two polynomials should have the same degree
    // => pad both 'new' vectors with zeros so they are a power of 2 in size and larger than the polynomials degree added:
    int32_t pol_deg = pol_one.size() + pol_two.size();
    int32_t new_size{1}; // 2^0

    while(pol_deg > new_size){
        new_size = (new_size << 1);
    }

    //resize vectors to same size (padded with zeros)
    fft_one.resize(new_size);
    fft_two.resize(new_size);

    //fft on both vectors
    fft(fft_one, false);
    fft(fft_two, false);

    //multiply the vectors, pairwise multiplication of the vector elements: DFT(A)*DFT(B) = DFT(A*B)
    for(int32_t i{0}; i < new_size; ++i){
        fft_one[i] *= fft_two[i];
    }

    //InverseDFT(DFT(A*B)) = A*B
    fft(fft_one, true); //set inverse = true for iDFT

    //fft_one is now A*B but complex valued, we want real valued part
    std::vector<int32_t> product(new_size);


    //for(auto prod : fft_one){
    for(int32_t i{0}; i < new_size; ++i){

        //get real value & fix rounding error introduced by complex numbers
        product[i] = round(std::real(fft_one[i]));

    }


    return product;
}

//Reads an int32 from stdin
int32_t  intRead(){

    //init data
    int32_t  data{0};
    bool neg{false};

    //read a char
    char c = getc(stdin);
    if(c == 45){
        neg = true;
        c = getc(stdin);
    }

    //Read while char is between 0 - 9 [ASCII]
    while(c > 47 && c < 58){
        data *= 10;
        data += c - 48;
        c = getc(stdin);
    }

    if(neg){data *= -1;}

    return data;
}

int main(){

    //desync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int32_t T = intRead();
    //std::vector<std::vector<int32_t>> pol(2, std::vector<int32_t>());
    std::vector<int32_t> pol_one, pol_two;
    //std::vector<int32_t> n_;

    int32_t n{},a{};

    //read first polynomial
    n = intRead();
    n += 1;
    while(n--){
        a = intRead();
        pol_one.push_back(a);
    }

    //read second polynomial
    n = intRead();
    n += 1;
    while(n--){
        a = intRead();
        pol_two.push_back(a);
    }

    //perform fft and polynomial multiplication
    std::vector<int32_t> prod_pol = polymul(pol_one, pol_two);

    //remove trailing zeros
    for(int32_t i{prod_pol.size() - 1}; i >= 0; --i){
        if(!prod_pol[i]){
            prod_pol.erase(prod_pol.begin() + i);
        }else{
            break;
        }
    }

    //print
    std::cout << (prod_pol.size() - 1) << "\n";
    for(auto coefficient : prod_pol){
        std::cout << coefficient << " ";
    }
/*
    for(int i{0}; i < 2; ++i){
        int32_t n{},a{};
        n = intRead();
        n_.push_back(n);
        n += 1;

        while(n--){
            a = intRead();
            pol[i].push_back(a);
        }
    }

    for(int i{0}; i < 2; ++i){
        for(int j{0}; j < pol[i].size(); ++j){

            std::cout << "a[" << j <<" = " << pol[i][j] << "]x^" << j << " ";
        }

        std::cout << "\n";
    }

    std::cout << "Real polynomials:\n";

    for(int i{0}; i < 2; ++i){
        for(int j{0}; j < pol[i].size(); ++j){
            if(pol[i][j] == 0){
                continue;
            }

            if(j == 0){
                std::cout <<  pol[i][j] << " ";
                continue;
            }


            if(pol[i][j] == 1){
                if(j == 1){
                    std::cout << "+ x" << " ";
                    continue;
                }

                std::cout <<  "+ x^" << j << " ";
                continue;
            }

            if(j == 1){
                if(pol[i][j] >= 0){
                    std::cout << "+ "<< pol[i][j] << "x" << " ";
                }else{
                    std::cout << pol[i][j] << "x" << " ";
                }

                continue;
            }


            if(pol[i][j] >= 0){
                std::cout << "+ " << pol[i][j] << "x^" << j << " ";
            }else{
                std::cout << pol[i][j] << "x^" << j << " ";
            }
        }

        std::cout << "\n";
    }
    */


    return 0;
}
