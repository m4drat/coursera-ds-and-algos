#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#define PROFILE_SCOPE(name) utils::profiling::ProfilerTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)

namespace utils {

    inline void hash_combine(std::size_t& seed)
    {}

    template<typename T, typename... Rest>
    inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        hash_combine(seed, rest...);
    }

    template<typename T>
    T GenerateRandomContainer(uint32_t length, uint32_t low, uint32_t high)
    {
        T data;
        data.resize(length);

        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<uint32_t> distr(low, high);

        auto gen = [&distr, &generator]() { return distr(generator); };

        std::generate(std::begin(data), std::end(data), gen);

        return data;
    }

    template<typename T>
    std::string VecToStr(const std::vector<T>& vec)
    {
        std::ostringstream oss;

        if (!vec.empty()) {
            // Convert all but the last element to avoid a trailing ","
            std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T>(oss, ", "));

            // Also can be implemented using std::accumulate
            // std::string s = std::accumulate(
            //     std::next(vec.begin()),
            //     vec.end(),
            //     std::to_string(vec[0]), // start with first element
            //     [](std::string a, T b) { return std::move(a) + ", " + std::to_string(b); });

            // Now add the last element with no delimiter
            oss << vec.back();
        }

        return oss.str();
    }

    template<typename T>
    bool CompareFloat(const T& a, const T& b, const T& eps = 0.00001f)
    {
        return std::abs(a - b) < eps;
    }

    namespace rng {
        static uint64_t s_X = 123456789, s_Y = 362436069, s_Z = 521288629;

        /**
         * @brief Fast rng taken from here:
         * https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
         * @return uint64_t
         */
        inline uint64_t xorshf96(void)
        { // period 2^96-1
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

    namespace profiling {
        class Timer
        {
        public:
            Timer(const char* t_name)
                : m_Name(t_name)
            {}

            Timer& TimerStart()
            {
                m_StartTimepoint = std::chrono::high_resolution_clock::now();
                return *this;
            }

            Timer& TimerEnd()
            {
                m_EndTimePoint = std::chrono::high_resolution_clock::now();
                return *this;
            }

            std::chrono::time_point<std::chrono::high_resolution_clock> GetStartTimepoint()
            {
                return m_StartTimepoint;
            }
            std::chrono::time_point<std::chrono::high_resolution_clock> GetEndTimepoint()
            {
                return m_EndTimePoint;
            }

            const char* GetName()
            {
                return m_Name;
            }

            template<class T = std::chrono::milliseconds>
            auto GetElapsed() -> auto
            {
                return std::chrono::duration_cast<T>(m_EndTimePoint - m_StartTimepoint);
            }

        private:
            const char* m_Name;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_EndTimePoint;
        };

        class ProfilerTimer : public Timer
        {
        public:
            ProfilerTimer(const char* t_name)
                : Timer(t_name)
                , m_Stopped(false)
            {
                this->TimerStart();
            }

            ~ProfilerTimer()
            {
                if (!m_Stopped) {
                    Stop();
                }
            }

            void Stop()
            {
                this->TimerEnd();

                int64_t start =
                    std::chrono::time_point_cast<std::chrono::milliseconds>(GetStartTimepoint())
                        .time_since_epoch()
                        .count();
                int64_t end =
                    std::chrono::time_point_cast<std::chrono::milliseconds>(GetEndTimepoint())
                        .time_since_epoch()
                        .count();

                std::cerr << GetName() << " -> " << end - start << "ms\n";

                m_Stopped = true;
            }

        private:
            bool m_Stopped;
        };

    } // profiling

} // utils
