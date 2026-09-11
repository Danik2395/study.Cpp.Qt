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

    enum Spec_Cells_Num
    {
        FRST = 11,
        SEC,
        THRD,
        FRTH
    };

private:
    ssstl::Vector<Cell> prison;
    ssstl::Vector<int> special_cells_crimnals_count;

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


        for (int n = 0; n < prison.size(); ++n)
        {
            Cell& cell = prison[n];

            Cell rand_cell;

            if (n >= FRST || n <= FRTH)
            {
                rand_cell = Cell(special_cells_crimnals_count[n]);
            }
            else
            {
                rand_cell = Cell(criminals_dist);
            }

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

    void set_special_cell_cnt(Spec_Cells_Num num, int count)
    {
        special_cells_crimnals_count[num] = count;
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
    ssstl::Vector<Cell> get_filtered_prisoners(bool crouded, bool exp)
    {
        ssstl::Vector<Cell> quiery_pr = prison;

        for (auto& cell : quiery_pr)
        {
            if (crouded)
            {
                if (cell.size() <= CROUDED_SIZE)
                {
                    cell = {};
                    continue;
                }
            }

            for (auto& criminal : cell)
            {
                if (exp)
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
