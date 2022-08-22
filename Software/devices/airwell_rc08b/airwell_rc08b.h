#pragma once

#include <map>

namespace airwell
{
namespace rc08b
{

constexpr uint8_t IR_FREQUENCY = 38;
constexpr uint8_t IR_FRAME_LENGTH = 99;
constexpr int8_t NEGATED_BYTE_OFFSET = -8;
constexpr uint16_t LOGIC_1_LENGHT = 1500;
constexpr uint16_t LOGIC_0_LENGHT = 500;
constexpr uint16_t GAP_LENGHT = 500;

enum class Temperature : uint8_t
{
    No_change = 14,
    T17 = 0,
    T18 = 1,
    T19 = 3,
    T20 = 2,
    T21 = 6,
    T22 = 7,
    T23 = 5,
    T24 = 4,
    T25 = 12,
    T26 = 13,
    T27 = 9,
    T28 = 8,
    T29 = 10,
    T30 = 11,
};

enum class FanSpeed : uint8_t
{
    No_change = 6,
    Locked = 1,
    Off = 7,
    Low = 9,
    Medium = 5,
    High = 3,
    Auto = 11,
    Turbo = 15,
};

enum class Swing : uint8_t
{
    No_change = 15,
    Toggle_swing = 11,
    Toggle_turbo = 5
};

enum class Turbo : uint8_t
{
    No_change = 2,
    Toggle_turbo = 5
};

// Other byte probably holds some options, but i did not find what.
enum class Other : uint8_t
{
    Other = 11
};

enum class Mode : uint8_t
{
    No_change = 2,
    Auto = 8,
    Heating = 12,
    Cooling = 0,
    Fan = 4,
    Dehumidification = 4,
};

// clang-format off
template<typename T> constexpr uint8_t get_position(T){return 255;}
template<> constexpr uint8_t get_position(Temperature){return 12;}
template<> constexpr uint8_t get_position(Mode)       {return 8;}
template<> constexpr uint8_t get_position(FanSpeed)   {return 28;}
template<> constexpr uint8_t get_position(Turbo)      {return 40;}
template<> constexpr uint8_t get_position(Swing)      {return 24;}
template<> constexpr uint8_t get_position(Other)      {return 44;}
// clang-format on

std::map<std::string, Mode> to_mode{
{"No_change", Mode::No_change}, {"Auto", Mode::Auto}, {"Heating", Mode::Heating},
{"Cooling", Mode::Cooling},     {"Fan", Mode::Fan},   {"Dehumidification", Mode::Dehumidification},
};

std::map<std::string, FanSpeed> to_fan_speed{
{"No_change", FanSpeed::No_change}, {"Off", FanSpeed::Off},       {"Low", FanSpeed::Low},   {"Medium", FanSpeed::Medium},
{"Turbo", FanSpeed::Turbo},         {"Locked", FanSpeed::Locked}, {"High", FanSpeed::High}, {"Auto", FanSpeed::Auto},
};
std::map<std::string, Swing> to_swing{{"No_change", Swing::No_change}, {"Toggle_swing", Swing::Toggle_swing}, {"Toggle_turbo", Swing::Toggle_turbo}};
std::map<std::string, Turbo> to_turbo{{"No_change", Turbo::No_change}, {"Toggle_turbo", Turbo::Toggle_turbo}};

std::map<std::string, Temperature> to_temperature{{"No_change", Temperature::No_change},
                                                  {"17", Temperature::T17},
                                                  {"18", Temperature::T18},
                                                  {"19", Temperature::T19},
                                                  {"20", Temperature::T20},
                                                  {"21", Temperature::T21},
                                                  {"22", Temperature::T22},
                                                  {"23", Temperature::T23},
                                                  {"24", Temperature::T24},
                                                  {"25", Temperature::T25},
                                                  {"26", Temperature::T26},
                                                  {"27", Temperature::T27},
                                                  {"28", Temperature::T28},
                                                  {"29", Temperature::T29},
                                                  {"30", Temperature::T30}};
template<typename T>
uint64_t to_raw_buffer(T value)
{
    uint64_t output = 0;
    uint8_t position = get_position(value);
    output = (static_cast<uint64_t>(0x0f & static_cast<uint8_t>(value)) << position) |
             (static_cast<uint64_t>(0x0f & (~static_cast<uint8_t>(value))) << (position + NEGATED_BYTE_OFFSET));
    return output;
}

void get_buffer_to_send(uint16_t * result, Mode mode, Temperature temperature, FanSpeed fan_speed, Turbo turbo, Swing swing)
{

    // begging of ir transmission
    result[0] = 4500;
    result[1] = 4500;
    result[2] = 500;

    uint64_t data =
    to_raw_buffer(fan_speed) | to_raw_buffer(mode) | to_raw_buffer(temperature) | to_raw_buffer(swing) | to_raw_buffer(turbo) | to_raw_buffer(Other::Other);

    uint8_t index = 3;
    for (int8_t current_bit = 47; current_bit >= 0; --current_bit)
    {
        result[index++] = (data & (static_cast<uint64_t>(1) << current_bit)) ? LOGIC_1_LENGHT : LOGIC_0_LENGHT;
        result[index++] = GAP_LENGHT;
    }
}

}  // namespace rc08b
}  // namespace airwell