#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP
#include <string>
class SharedData
{
public:
    SharedData(){};
    static float base_freq;
    static float base_mult;
    static float multipliers[3];
};
#endif // !SHAREDDATA_HPP
