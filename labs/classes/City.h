#pragma once
#include <concepts>
#include "ssstl/Vector.h"
#include "ssstl/random.h"

namespace
{
    template<typename Lambda, typename... Args>
    concept CityBlockLambda = requires(Lambda lambda, int& block, Args&&... args){
        { lambda(block, args...) } -> std::same_as<void>;
    };
}

class City
{
    ssstl::Vector<ssstl::Vector<int>> city;
    ssstl::random<int> crime_dist;

    int median_criminality;
    int most_crime_blocks;

    size_t rows_;
    size_t cols_;

    ssstl::random<size_t> city_dist;

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

    City(size_t rows = 0, size_t cols = 0) :
        rows_(rows),
        cols_(cols),
        city(rows, ssstl::Vector<int>(cols, 0)),
        crime_dist(0, 100),
        city_dist(10, 100)
    {
        gen_city();
    }

    void gen_city()
    {
        rows_ = city_dist;
        cols_ = city_dist;
        gen_city(city_dist, city_dist);
    }

    void gen_city(size_t rows, size_t cols)
    {
        rows_ = rows;
        cols_ = cols;

        city = {rows_, ssstl::Vector<int>(cols_, 0)};//

        median_criminality = 0;
        for_each_block([this](auto& block){
                block = crime_dist;
                median_criminality += block;
                });
        median_criminality /= size();

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

    int size() { return static_cast<int>(rows_ * cols_); }
};
