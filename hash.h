#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <cctype>


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
        std::vector<unsigned long long> w(5, 0);
        int len = k.length();
        int wIndex = 4; // Fill from the end (w[4] from the last 6 chars)
    
        for (int i = len; i > 0 && wIndex >= 0; i -= 6, --wIndex)
        {
            int start = std::max(0, i - 6);
            unsigned long long value = 0;
            unsigned long long power = 1;
    
            for (int j = i - 1; j >= start; --j)
            {
                char c = k[j];
                value += letterDigitToNumber(c) * power;
                power *= 36;
            }
            w[wIndex] = value;
        }
    
        // Now compute the final hash using the r values
        unsigned long long hash = 0;
        for (int i = 0; i < 5; ++i)
        {
            hash += rValues[i] * w[i];
        }
    
        return hash;
    }
    
    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        char letter2 = std::tolower(letter);
        // Add code here or delete this helper function if you do not want it
        // First translate each letter into a value between 0 and 35
        // where a=0, z=25, ASCII: 97 to 122
        if (letter2 >= 97 && letter2 <= 122)
        {
            return letter2 - 97;
        }
        // '0'=26, '9'=35, ASCII: 48 to 57
        else if (letter2 >= 48 && letter2 <= 57)
        {
            return letter2 - 48 + 26;
        }
        return 0;
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
