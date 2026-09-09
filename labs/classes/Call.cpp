#include "Call.h"
#include <new>
#include <algorithm>
#include <stdexcept>
#include <utility>

// Global constexpr, internal linkage
constexpr int DEP_MAX = 6;

Call::Call()
{
    depart = new int[DEP_MAX];
    std::fill(depart, depart + DEP_MAX, 0);
}

Call::Call(Call& call)
{
    depart = new int[DEP_MAX];
    std::copy(call.depart,  call.depart + DEP_MAX, depart);
}

Call::Call(Call&& call) noexcept
{
    depart = std::exchange(call.depart, nullptr);
}

Call::~Call()
{
    delete[] depart;
}

Call& Call::operator=(Call& call)
{
    if (&call == this) return *this;

    int* new_depart = new int[DEP_MAX];
    std::copy(call.depart,  call.depart + DEP_MAX, new_depart);
    delete [] depart;
    depart = new_depart;

    return *this;
}

Call& Call::operator=(Call&& call) noexcept
{
    if (&call == this) return *this;

    delete[] depart;
    depart = std::exchange(call.depart, nullptr);

    return *this;
}

int Call::get_level(Level level)
{
    if (level > 2 || level < 0)
    {
        throw std::out_of_range("First level EASY = 0, last level HARD = 2.");
    }
    return depart[level];
}

int Call::get_level_all()
{
    return depart[0] + depart[1] + depart[2];
}

int Call::get_level_percent(Level level)
{
    int levels_sum = get_level_all();
    if (levels_sum == 0) return 0;
    return static_cast<int>(get_level(level) * 100 / levels_sum);
}

int Call::get_profession(Profession profession)
{
    if (profession > 5 || profession < 3)
    {
        throw std::out_of_range("First level POLICEMAN = 3, last level DETECTIVE = 5.");
    }
    return depart[profession];
}

int Call::get_profession_all()
{
    return depart[3] + depart[4] + depart[5];
}

int Call::get_profession_percent(Profession profession)
{
    int professions_sum = get_profession_all();
    if (professions_sum == 0) return 0;
    return static_cast<int>(get_profession(profession) * 100 / professions_sum);
}

void Call::set_level(Level level, int value)
{
    if (level > 2 || level < 0)
    {
        throw std::out_of_range("First level EASY = 0, last level HARD = 2.");
    }
    depart[level] = value;
}

void Call::set_profession(Profession profession, int value)
{
    if (profession > 5 || profession < 3)
    {
        throw std::out_of_range("First level POLICEMAN = 3, last level DETECTIVE = 5.");
    }
    depart[profession] = value;
}
