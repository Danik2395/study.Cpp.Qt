#pragma once

class Call
{
    int easy, medium, hard;
    int policeman, special_forces, detective;

    public:
    enum Level {
        EASY,
        MEDIUM,
        HARD
    };

    enum Profession
    {
        POLICEMAN,
        SPECIAL_FORCES,
        DETECTIVE
    };

    Call() : easy(0), medium(0), hard(0), policeman(0), special_forces(0), detective(0) {}

    int get_level(Level level);
    int get_level_percent(Level level);
    int get_level_all();

    int get_profession(Profession profession);
    int get_profession_percent(Profession profession);
    int get_profession_all();

    void set_level(Level level, int value);
    void set_profession(Profession profession, int value);
};
