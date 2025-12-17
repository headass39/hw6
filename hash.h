#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iostream>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {

        // Add your code here
        unsigned long long w[5] = {};

        //loop through and give the value needed for each int in w array
        char numIntsNeeded = k.size() / 6;
        for(int i = 0; i < numIntsNeeded+1; ++i){
          //need to handle the case where the number of letters in the substring is not 6 to avoid segfault
          if(i == numIntsNeeded){
            for(size_t j = 0; j < (k.size() % 6); ++j){
              w[4-i] = w[4-i]*36 + letterDigitToNumber(k[j]);
              //debugging
              // std::cout << "outer loop iteration " << i << ", we are working on w[" << 4-i << "]. " <<
              // "inner loop iteration " << j << ". current letter = " << k[j] << ". " 
              // << "letterDigitToNumber(k[j + (6*i)]) = " << letterDigitToNumber(k[j]) << std::endl;
              // std::cout << "w[" << 4-i << "] = " << w[4-i] << std::endl;
            }
          }
          else{
            for(size_t j = ((numIntsNeeded-i-1)*6) + (k.size()%6); j < 6*(numIntsNeeded-i) + (k.size()%6); ++j){
              w[4-i] = w[4-i]*36 + letterDigitToNumber(k[j]);
              //debugging
              // std::cout << "outer loop iteration " << i << ", we are working on w[" << 4-i << "]. " <<
              // "inner loop iteration " << j << ". current letter = " << k[j] << ". " 
              // << "letterDigitToNumber(k[j + (6*i)]) = " << letterDigitToNumber(k[j]) << std::endl;
              // std::cout << "w[" << 4-i << "] = " << w[4-i] << std::endl;
            }
          }
        }
        //want it to access 1, 2, 3, 4, 5, 6, 0
        //what is the general form? it makes it hard that the msb letter is the first index (or does it?)
        //how do we get 1 instead of 0 here? j starts at 0. should we have it start at 1 instead? 
        //our edge case does have to be a loop that runs k.size%6 times, but how can we order it so that it accesses the msb's?
        //how can we order the non edge case loop to access the lower letters first? how can we change the indexing of j to
        //make it so that we access the lower ones. let's say we have a 13 letter word. Well, we want it to do
        //{7,8,9,10,11,12}, {1,2,3,4,5,6}, {0}
        //how can we get it to do this? what is 7 in terms of i and j, well the i term would have something like numIntsNeeded-i
        //this would equal 2, okay so maybe numIntsNeeded-1? and then the j would also have something to do with k.size mod 6 again, I think
        //I mean 13 mod 6 is 1, we want to start at 7 and 1 and work our way up to 12 and 6. how do we get from 7 to 12
        //well we could do j initialized to numIntsNeeded-i-1 + k.size()%6, which works for initializing to 7 and to 1.
        //then for the upper bound of j, we want 12 and 6, so that just seems like 6*(numIntsNeeded-i).
        //now for the edge case. what if the number of letter is 26 (the maximum), we want the first loop to start 4 times
        //we want the indexes to go {20,21,22,23,24,25}, {14,15,16,17,18,19}, {8,9,10,11,12,13}, {2,3,4,5,6,7}, {0,1}
        //what if we had a 9 letter word? we want {3,4,5,6,7,8}, {0,1,2}

        //initializing random values
        //generateRValues();
        //now we need to hash these integers and return the final number
        return (rValues[0]*w[0] + rValues[1]*w[1] + rValues[2]*w[2] + rValues[3]*w[3] + rValues[4]*w[4]);


    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        //want to see if it is a letter on a number
        if(letter >= 'A' && letter <= 'Z'){
          letter += 32; //32 is the distance between 'A' and 'a', so this converts uppercase to lowercase
        }
        if(letter >= 'a' && letter <= 'z'){
          //do stuff
          letter -= 'a'; //converting to a number
          return (HASH_INDEX_T)letter; //cast letter into an integer
        }
        else if(letter >= '0' && letter <= '9'){
          letter -= '0';
          letter += 26;
          return (HASH_INDEX_T)letter;
        }
        else return 10000000000;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
