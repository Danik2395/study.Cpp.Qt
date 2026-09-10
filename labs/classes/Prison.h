#pragma once
#include <stdexcept>
#include <string>
#include <stdint.h>
#include "ssstl/Vector.h"
#include "ssstl/random.h"

constexpr size_t CROUDED_SIZE = 8;
constexpr size_t EXP_EXP      = 30;
constexpr size_t EXP_SKYRIM   = 50;

class Prison
{
public:

    typedef struct
    {
        bool place_active;
        int experience;
        std::string name;
        int level_in_the_skyrim;
    } Criminal;

    typedef ssstl::Vector<Criminal> Cell;

private:
    ssstl::Vector<Cell> prison;
    ssstl::random<int> exp_dist;
    ssstl::random<int> name_dist;
    ssstl::random<int> lvl_dist;
    ssstl::random<int> criminals_dist;

public:

    Prison(size_t cells_cnt) :
        prison(cells_cnt),
        exp_dist(0, 100),
        name_dist(0, 3),
        lvl_dist(0, 255),
        criminals_dist(0, 10)
    {}

    void random_fill()
    {
        std::string names[] = {"Alex", "Peter", "Andrew", "Jordan"};

        for (auto& cell : prison)
        {
            Cell rand_cell(criminals_dist);
            cell = std::move(rand_cell);

            for (auto& criminal : cell)
            {
                criminal = {
                    .place_active = true,
                    .experience = exp_dist,
                    .name = names[name_dist],
                    .level_in_the_skyrim = lvl_dist,
                };
            }
        }
    }

    void add_prisoner(int cell_num, std::string& name)
    {
        try
        {
            prison.at(cell_num).emplace_back(true, exp_dist, name, lvl_dist);
        }
        catch (std::out_of_range)
        {
            throw std::out_of_range("Invalid cell number.");
        }
    }

    void free_prisoner(int cell_num)
    {
        try
        {
            prison.at(cell_num) = {};
        }
        catch (std::out_of_range)
        {
            throw std::out_of_range("Invalid cell number.");
        }
    }

    // couded will firder out all not crouded cells,
    // exp    will firder out all non experienced criminals
    template<bool crouded, bool exp>
    ssstl::Vector<Cell> get_filtered_prisoners()
    {
        ssstl::Vector<Cell> quiery_pr = prison;

        for (auto& cell : quiery_pr)
        {
            if constexpr (crouded)
            {
                if (cell.size() <= CROUDED_SIZE)
                {
                    cell = {};
                    continue;
                }
            }

            for (auto& criminal : cell)
            {
                if constexpr (exp)
                {
                    if (criminal.experience          <= EXP_EXP ||
                        criminal.level_in_the_skyrim <= EXP_SKYRIM)
                    {
                        criminal = {};
                        continue;
                    }
                }
            }
        }

        return quiery_pr; // No std::move() because of NRVO
    }

    ssstl::Vector<int> get_crouded_cells_num()
    {
        ssstl::Vector<int> crouded_cells_num;
        for (int n = 0; n < prison.size(); ++n)
        {
            if (prison[n].size() <= CROUDED_SIZE) continue;
            crouded_cells_num.emplace_back(n);
        }
        return crouded_cells_num;
    }
};
