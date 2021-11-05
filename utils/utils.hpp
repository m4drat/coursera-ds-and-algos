#include <string>
#include <sstream>
#include <iterator>

namespace utils {
    template <typename T>
    std::string VecToStr(const std::vector<T>& vec) {
        std::ostringstream oss;

        if (!vec.empty())
        {
            // Convert all but the last element to avoid a trailing ","
            std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T>(oss, ", "));

            // Now add the last element with no delimiter
            oss << vec.back();
        }

        return oss.str();
    }

namespace rng {
static uint64_t s_X = 123456789, s_Y = 362436069, s_Z = 521288629;

/**
 * @brief Fast rng taken from here: https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
 * @return uint64_t 
 */
uint64_t xorshf96(void) { //period 2^96-1
    uint64_t t;
    
    s_X ^= s_X << 16;
    s_X ^= s_X >> 5;
    s_X ^= s_X << 1;

    t = s_X;
    s_X = s_Y;
    s_Y = s_Z;
    s_Z = t ^ s_X ^ s_Y;

    return s_Z;
}
} // rng
} // utils
