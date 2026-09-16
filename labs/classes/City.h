#pragma once
#include <concepts>
#include "ssstl/Vector.h"
#include "ssstl/random.h"

namespace
{
    template<typename Lambda, typename... Args>
    concept CityBlockLambda = requires(Lambda lambda, int block, Args&&... args){
        { lambda(block, args...) } -> std::same_as<void>;
    };
}

class City
{
    ssstl::Vector<ssstl::Vector<int>> city;
    ssstl::random<int> crime_dist;

    int city_size;
    int median_criminality;
    int most_crime_blocks;

    // void callback(Block block)
    template<typename... Args>
    void for_each_block(CityBlockLambda auto&& callback, Args... args)
    {
        for (auto& row : city)
        {
            for (auto& col : row)
            {
                callback(col, std::forward<Args>(args)...);
            }
        }
    }

public:

    City(size_t rows, size_t cols) : city(rows, ssstl::Vector<int>(cols, 0)), crime_dist(0, 100)
    {
        gen_city();
        city_size = static_cast<int>(rows * cols);
    }

    void gen_city()
    {
        median_criminality = 0;
        for_each_block([this](auto& block){
                block = crime_dist;
                median_criminality += block;
                });
        median_criminality /= city_size;

        most_crime_blocks = 0;
        for_each_block([this](auto& block){
                if (block > median_criminality)
                {
                    ++most_crime_blocks;
                }
                });
    }

    int get_median_criminality() { return median_criminality; }
    int get_most_crime_blocks()  { return most_crime_blocks; }
};
