#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <utility>
#include <iomanip>

std::vector<double> distances(std::vector<int> points){
    std::vector<double> distance_vector;
/*
    distance_vector.push_back(std::abs(points[2] - points[0]) + std::abs(points[3] - points[1]));
    distance_vector.push_back(std::abs(points[4] - points[2]) + std::abs(points[5] - points[3]));
    distance_vector.push_back(std::abs(points[0] - points[4]) + std::abs(points[1] - points[5]));
    */

    distance_vector.push_back(std::sqrt(std::pow(points[2] - points[0], 2) + std::pow(points[3] - points[1], 2)));
    distance_vector.push_back(std::sqrt(std::pow(points[4] - points[2], 2) + std::pow(points[5] - points[3], 2)));
    distance_vector.push_back(std::sqrt(std::pow(points[0] - points[4], 2) + std::pow(points[1] - points[5], 2)));

    /*
    for(int k = 0; k<distance_vector.size(); ++k){
        std::cout  << std::setprecision(10) << "lengths: " << distance_vector[k] << "\n";
    }
    std::cout  << std::setprecision(10) << "sum: " << distance_vector[0] + distance_vector[2] << "\n";
    std::cout  << std::setprecision(10) << "diff: " << distance_vector[1] - (distance_vector[0] + distance_vector[2]) << "\n";
    */

    return distance_vector;
}

bool isTriangle(std::vector<double> lengths){ //return true if points create a triangle
    double epsilon{0.0000000001};
    return (lengths[0] + lengths[1] - lengths[2] > epsilon &&
            lengths[1] + lengths[2] - lengths[0] > epsilon &&
            lengths[2] + lengths[0] - lengths[1] > epsilon);
}

bool isIsosceles(std::vector<double> lengths){ //return true if triangle is isosceles
    return (lengths[0] == lengths[1] ||
            lengths[1] == lengths[2] ||
            lengths[2] == lengths[0]);
}

double dotProd(int x1, int x2, int y1, int y2){
    return (x1*x2 + y1*y2);
}


std::vector<std::pair<int, int>> createVectors(std::vector<int> input_points){
    std::vector<std::pair<int, int>> tri_vectors;

    std::pair<int, int> vec1 = {input_points[2] - input_points[0],input_points[3] - input_points[1]};
    tri_vectors.push_back(vec1);

    vec1 = {input_points[4] - input_points[0],input_points[5] - input_points[1]};
    tri_vectors.push_back(vec1);

    vec1 = {input_points[4] - input_points[2],input_points[5] - input_points[3]};
    tri_vectors.push_back(vec1);

    return tri_vectors;
}


int angleCheck(std::vector<std::pair<int,int>> tri_vec){ //returns the scalar product: if product is > 0 then angle is less than 90 degrees & vice versa.
    //vectors for points: one->two, one->three, two->three;

    int dotOne = dotProd(tri_vec[0].first, tri_vec[1].first, tri_vec[0].second, tri_vec[1].second); //corner One
    int dotTwo = dotProd(tri_vec[2].first, -tri_vec[0].first, tri_vec[2].second, -tri_vec[0].second); //corner Two
    int dotThree = dotProd(-tri_vec[1].first, -tri_vec[2].first, -tri_vec[1].second, -tri_vec[2].second); //corner Three

    if(dotOne == 0 || dotTwo == 0 || dotThree == 0){
        return 0;
    }else if(dotOne < 0 || dotTwo < 0 || dotThree < 0){
        return -1;
    }

    return 1;
}

int main(void){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N;
    std::cin >> N;
    std::string input = "";
    std::getline(std::cin,input);
    std::vector<int> input_points;
    std::string point{""};
    std::string case_line{""};

    for(int i = 0; i<N; ++i){
        std::getline(std::cin,case_line);
        std::stringstream stream_line(case_line);

        while(std::getline(stream_line, point, ' ')){
            input_points.push_back(stoi(point));
        }

        std::vector<double> triangle_distances = distances(input_points);

        std::vector<std::pair<int, int>> vectors_tri = createVectors(input_points); //get vectors for points: one->two, one->three, two->three;

        if(!isTriangle(triangle_distances)){
            goto case7;
        }

        if(isIsosceles(triangle_distances)){
            if(angleCheck(vectors_tri) > 0){goto case1;
            }else if(angleCheck(vectors_tri) < 0){goto case2;
            }
            goto case3;
        }else{
            if(angleCheck(vectors_tri) > 0){goto case4;
            }else if(angleCheck(vectors_tri) < 0){goto case5;
            }
            goto case6;
        }

//Cases:

case1:
    std::cout << "Case #" << i+1 << ": isosceles acute triangle\n";
    input_points.clear();
    continue;

case2:
    std::cout << "Case #" << i+1 << ": isosceles obtuse triangle\n";
    input_points.clear();
    continue;

case3:
    std::cout << "Case #" << i+1 << ": isosceles right triangle\n";
    input_points.clear();
    continue;

case4:
    std::cout << "Case #" << i+1 << ": scalene acute triangle\n";
    input_points.clear();
    continue;

case5:
    std::cout << "Case #" << i+1 << ": scalene obtuse triangle\n";
    input_points.clear();
    continue;

case6:
    std::cout << "Case #" << i+1 << ": scalene right triangle\n";
    input_points.clear();
    continue;

case7:
    std::cout << "Case #" << i+1 << ": not a triangle\n";
    input_points.clear();
    continue;

    }//All cases (inputs) handled
}
