#pragma once
#include <stdexcept>
#include <string>
#include <ranges>

class Report
{
    std::string ascii_string;
    std::string converted_string;

public:

    Report() {}
    Report(std::string ascii_string) : ascii_string(ascii_string)
    {
        converted_string = ascii_to_string(ascii_string);
    }

    static std::string ascii_to_string(std::string& ascii_str)
    {
        try
        {
            std::string temp_conv_str;                                         // To not to corrupt converted_string
            for (auto word : std::views::split(ascii_str, " "))                // Creates range of subranges
            {
                int ascii_int = std::stoi(std::string(std::from_range, word)); // C++23 to construct std::string from range
                temp_conv_str += static_cast<char>(ascii_int);
            }
            return temp_conv_str;
        }
        catch (std::invalid_argument)
        {
            throw std::invalid_argument("Not an ASCII string.");
        }
        catch(std::out_of_range)
        {
            throw std::out_of_range("ASCII number is out of range.");
        }
    }

    void convert_string()
    {
        converted_string = ascii_to_string(ascii_string);
    }

    void set_ascii_string(std::string& ascii_str)
    {
        ascii_string = ascii_str;
        converted_string = ascii_to_string(ascii_string);
    }

    std::string get_converted_string()
    {
        return converted_string;
    }

    std::string get_ascii_string()
    {
        return ascii_string;
    }
};
