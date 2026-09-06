#include "Lab1Widget.h"

namespace Lab1
{
    int Call::get_level(Level level)
    {
        switch (level)
        {
            case EASY:   return easy;
            case MEDIUM: return medium;
            case HARD:   return hard;
            default:     return 0;
        }
    }

    int Call::get_level_all()
    {
        return policeman + special_forces + detective;
    }

    int Call::get_level_percent(Level level)
    {
        int levels_sum = get_level_all();
        if (levels_sum == 0) return 0;
        return static_cast<int>(get_level(level) * 100 / levels_sum);
    }

    int Call::get_profession(Profession profession)
    {
        switch (profession)
        {
            case POLICEMAN:      return policeman;
            case SPECIAL_FORCES: return special_forces;
            case DETECTIVE:      return detective;
            default:             return 0;
        }
    }
    int Call::get_profession_all()
    {
        return policeman + special_forces + detective;
    }

    int Call::get_profession_percent(Profession profession)
    {
        int professions_sum = get_profession_all();
        if (professions_sum == 0) return 0;
        return static_cast<int>(get_profession(profession) * 100 / professions_sum);
    }

    void Call::set_level(Level level, int value)
    {
        switch (level)
        {
            case EASY:   easy = value;   break;
            case MEDIUM: medium = value; break;
            case HARD:   hard = value;   break;
            default:                     break;
        }
    }

    void Call::set_profession(Profession profession, int value)
    {
        switch (profession)
        {
            case POLICEMAN:      policeman = value;      break;
            case SPECIAL_FORCES: special_forces = value; break;
            case DETECTIVE:      detective = value;      break;
            default:                                     break;
        }
    }
}
