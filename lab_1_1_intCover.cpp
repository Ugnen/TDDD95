/*
########[IntervalCover LAB]########
Author: Oscar Wilkens (oscwi918)

Description: Assignment was to implement a function that takes a larger interval [A, B] and several smaller intervals [a_0 ... a_i, b_0 ... b_i], where
[a_i, b_i] constitutes an interval start/end pair, and compares these to see if any or several of the a/b intervals will completely cover the A/B interval.
If this is possible the optimal number of intervals required should be printed and their indices, if not "impossible" should be printed. My approach to solve
this was to use a greedy approach by first sorting the a/b intervals in ascending order then one by one check if they have a small start than the A and if
so save the index and build an "interval" that uses a and the largest of B or b. At the end just check if B is completely covered by our build interval or not
and return the used indices. This worked well except for a few snags with the sorting that I still don't understand, see additional notes.


Regarding time complexity: The total complexity is O(n*log(n)) but the time complexity for the solution is just O(N) as we are only looping through the
intervals once. O(n*log(n)) comes from the sorting of the indices according to the smallest interval start (should have been the intervals aswell, see notes..).
Might be able to bring the total time comlexity down by either creating a better sort or using the "Efficient approach" listed in:
https://www.geeksforgeeks.org/minimum-number-of-intervals-to-cover-the-target-interval/


Notes: Lab 1.1 in course TDDD95 via LINKÖPING UNIVERSITY
Version: 1.3, Final-version
Additional notes: During the implementation I wanted to keep a sorted list of all the intervals and a separate list of their indices (orignal ones) but
noticed that this for some reason didn't work when handing in on Kattis. The final approach was to only sort the index list (according to how the intervals
would be sorted) and just use their indices directly in the unsorted interval list. In my opinion a much uglier solution but I cannot figure out why these
two cases are treated differently as during all my test they are the same.
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>
#include <limits.h>


std::vector<int> calcCover(std::pair<double, double> A_B, std::vector<std::pair<double, double>> intervals){//returns indices intervals that cover first interval in 'intervals'
    std::vector<int> cover_index;

    std::vector<int> indices(intervals.size());
    std::iota(indices.begin(), indices.end(), 0);

    //Sort indices
    std::sort(indices.begin(), indices.end(),
           [&](int A, int B) -> bool {
                return intervals[A].first < intervals[B].first;
            });

    std::pair<double, double> max_pair(DBL_MAX, DBL_MAX);
    intervals.push_back(max_pair); //limit of 'double' inputs
    double curr_start{A_B.first};
    double curr_end{curr_start - 1};
    int push_index{0};
    int if_iterate{0};

    //Greedily loop through each interval and compare their starts and ends
    for(int i = 0; i < intervals.size(); i += if_iterate){

        //Check if i:th interval covers current start and if we should iterate or need to updated start
        if(intervals[indices[i]].first <= curr_start){

            //update end if i:th interval end is larger than current end
            if(curr_end <= intervals[indices[i]].second){
                curr_end = intervals[indices[i]].second;
                push_index = indices[i];    //index to be saved
            }

            if_iterate = 1; //Set to one if we should iterate

        }else{
            if_iterate = 0;

            curr_start = curr_end; //Update start
            cover_index.push_back(push_index); //add index to list

            //test if i:th interval COMPLETLY covers the looked at current interval
            if(intervals[indices[i]].first <= curr_start && intervals[indices[i]].second >= curr_end){
                curr_end = intervals[indices[i]].first;
                push_index = indices[i];
            }
        }

        //Check if we have reached goal
        if(intervals[indices[i]].first > curr_end || curr_end >= A_B.second){
            cover_index.push_back(push_index);
            break;
        }
    }

    //Only return empty vector if end of longest interval (with start included) doesn't include B
    if(curr_end < A_B.second){
        return {};
    }

    return cover_index;
}


int main(){
    //Sync
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    //Variable declarations
    int64_t n;
    double A,B,a_i,b_i;

    std::vector<std::pair<double, double>> interval_list;
    std::pair<double, double> interval;
    std::vector<int64_t> interval_lengths;

    //Read
    std::cin >> A >> B >> n;
    while(!std::cin.eof()){
        interval.first = A;
        interval.second = B;
        interval_list.push_back(interval);
        interval_lengths.push_back(n);

        while(n--){
            std::cin >> a_i >> b_i;
            interval.first = a_i;
            interval.second = b_i;
            interval_list.push_back(interval);
        }
        std::cin >> A >> B >> n;
    }

    std::vector<int> cover;
    int index_start{0};

    //Print
    for(auto &length : interval_lengths){
        std::pair<double, double> A_B (interval_list[index_start].first, interval_list[index_start].second);

        index_start++;
        std::vector<std::pair<double, double>> current_interval(interval_list.begin() + index_start, interval_list.begin() + length + index_start);

        cover = calcCover(A_B, current_interval);

        int64_t cover_size = cover.size();

        if(cover_size){
            std::cout << cover_size << "\n";
            for(int j = 0; j < cover_size; j++){
                    std::cout << cover[j] << " ";
            }
            std::cout << "\n";
        }else{
            std::cout << "impossible\n";
        }
        index_start += length;
    }
    return 0;
}
