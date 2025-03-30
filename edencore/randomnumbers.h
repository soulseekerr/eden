#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

// Function to generate a vector of random integers
// Function to generate a vector of random numbers
template <typename T>
std::vector<T> generateRandomNumbers(int num, T min, T max) {
    std::vector<T> randomNumbers(num);

    // Seed with a real random value, if available
    std::random_device rd;

    // Initialize the random number generator
    std::default_random_engine eng(rd());

    // Define the range
    std::uniform_int_distribution<int> distr(min, max);

    // Generate random numbers
    for (int i = 0; i < num; ++i) {
        randomNumbers[i] = distr(eng);
    }

    return randomNumbers;
}