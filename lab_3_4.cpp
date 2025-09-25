/*
########[Rational Arithmetic LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Implement a class for rational numbers which supports the common arithmetic opeartions +, +=, -, -=, *, *=, /, /* and comparison
operators ==, !=, >, >=, <, <=. These operators were implemented using operator overloading as C++ supports this.
A non-member (friend) overload of the output (<<) and input (>>) operators  was added.

Regarding time complexity: Time complexity shouldn't matter for this task as the task is to create a Class. I have used no specific algorithm and
therefore a breakdown of the complexity seems unimportant.

Additional notes: Lab 3.4 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.1, - Added input stream
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
#include <numeric>

class Rational{
private:
    //reduces the input nominator and denominator so numbers don't overflow
    void reduceNumber(int64_t & nomi, int64_t & denom){
        int64_t divisor{std::gcd(nomi,denom)};
        nomi = nomi/divisor;
        denom = denom/divisor;
        return;
    }

public:
    int64_t nominator;
    int64_t denominator;

    Rational(){
        nominator = 0;
        denominator = 0;
    }

    Rational(int64_t x, int64_t y){

        //check which sign, only nominator should be negative
        if((x < 0 && y < 0) || (x > 0 && y < 0)){
            nominator = -x;
            denominator = -y;

        }else if(y < 0){
            nominator = -x;
            denominator = -y;

        }else{
            nominator = x;
            denominator = y;
        }

        reduceNumber(nominator, denominator);
    }

    Rational operator+(Rational rhs){

        //overflow divisor to make sure we dont use two big numbers
        int64_t oDivisor{std::gcd(denominator,rhs.denominator)};

        int64_t n1{nominator*rhs.denominator/oDivisor};
        int64_t n2{rhs.nominator*denominator/oDivisor};
        int64_t d{denominator*rhs.denominator/oDivisor};

        return Rational(n1+n2,d);
    }

    Rational operator-(Rational rhs){

        //overflow divisor to make sure we dont use two big numbers
        int64_t oDivisor{std::gcd(denominator,rhs.denominator)};

        int64_t n1{nominator*rhs.denominator/oDivisor};
        int64_t n2{rhs.nominator*denominator/oDivisor};
        int64_t d{denominator*rhs.denominator/oDivisor};

        return Rational(n1-n2,d);
    }

    void operator+=(Rational rhs){

        //overflow divisor to make sure we dont use two big numbers
        int64_t oDivisor{std::gcd(denominator,rhs.denominator)};

        nominator = nominator*rhs.denominator/oDivisor + rhs.nominator*denominator/oDivisor;
        denominator = denominator*rhs.denominator/oDivisor;

        //reduce result
        reduceNumber(nominator, denominator);

        return;
    }

    void operator-=(Rational rhs){

        //overflow divisor to make sure we dont use two big numbers
        int64_t oDivisor{std::gcd(denominator,rhs.denominator)};
        nominator = nominator*rhs.denominator/oDivisor - rhs.nominator*denominator/oDivisor;
        denominator = denominator*rhs.denominator/oDivisor;

        //reduce result
        reduceNumber(nominator, denominator);

        return;
    }

    Rational operator*(Rational rhs){

        //Either component cannot be further reduced before operation
        int64_t n{nominator*rhs.nominator};
        int64_t d{denominator*rhs.denominator};

        return Rational(n,d);
    }

    Rational operator/(Rational rhs){

        //Either component cannot be further reduced before operation
        int64_t n{nominator*rhs.denominator};
        int64_t d{denominator*rhs.nominator};

        return Rational(n,d);
    }

    void operator*=(Rational rhs){

        nominator = nominator*rhs.nominator;
        denominator = denominator*rhs.denominator;

        //reduce result
        reduceNumber(nominator, denominator);

        return;
    }

    void operator/=(Rational rhs){

        nominator = nominator*rhs.denominator;
        denominator = denominator*rhs.nominator;

        //reduce result
        reduceNumber(nominator, denominator);

        return;
    }

    //equal operator
    bool operator==(Rational rhs){

        return (nominator == rhs.nominator && denominator == rhs.denominator);
    }

    //not-equal operator
    bool operator!=(Rational rhs){

        return (nominator != rhs.nominator || denominator != rhs.denominator);
    }

    //greater-than operator
    bool operator>(Rational rhs){

        int64_t oDivisor{std::gcd(denominator,rhs.denominator)};

        return (nominator*rhs.denominator > rhs.nominator*denominator);
    }

    //greater-than-equal operator
    bool operator>=(Rational rhs){
        int64_t oDivisor{std::gcd(denominator,rhs.denominator)};

        return (nominator*rhs.denominator >= rhs.nominator*denominator);
    }

    //less-than operator
    bool operator<(Rational rhs){

        int64_t oDivisor{std::gcd(denominator,rhs.denominator)};

        return (nominator*rhs.denominator < rhs.nominator*denominator);
    }

    //less-than-equal operator
    bool operator<=(Rational rhs){
        int64_t oDivisor{std::gcd(denominator,rhs.denominator)};

        return (nominator*rhs.denominator <= rhs.nominator*denominator);
    }


    //Non-member function, output stream
    //output on stream in format "x1 / y1"
    friend std::ostream& operator<<(std::ostream& os, Rational rhs){
        os << rhs.nominator << " / " << rhs.denominator;

        return os;
    }

    //Non-member function, input stream
    friend std::istream& operator>>(std::istream& is, Rational & rhs){
        is >> rhs.nominator >> rhs.denominator;
        return is;
    }

};

int main(){

    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int64_t n{},x1{},x2{},y1{},y2{};
    char op{};

    std::cin >> n;
    while(n--){

        /*
        Rational num1;
        Rational num2;
        std::cin >> num1 >> op >> num2;*/

        std::cin >> x1 >> y1 >> op >> x2 >> y2;

        Rational num1(x1,y1);
        Rational num2(x2,y2);

        if(op == '+'){
            Rational res(num1+num2);
            std::cout << res;
        }else if(op == '-'){
            Rational res(num1-num2);
            std::cout << res;
        }else if(op == '*'){
            Rational res(num1*num2);
            std::cout << res;
        }else if(op == '/'){
            Rational res(num1/num2);
            std::cout << res;
        }

        std::cout << "\n";

    }


    return 0;
}
