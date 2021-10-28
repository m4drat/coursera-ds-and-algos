#include <iostream>
#include <linux/limits.h>
#include <vector>

int32_t ComputeMinRefills(uint32_t t_Tank, std::vector<uint32_t>& t_Stops) {
    uint32_t minRefills{ 0 };
    uint32_t currentPos{ 0 };
    uint32_t nextGasStation{ 0 };

    // Condition, to terminate the loop
    while (nextGasStation < t_Stops.size() - 1) {
        // Move to the next gas station
        currentPos = nextGasStation;

        // Find the farthest gas station, that our car can travel to
        for (uint32_t nextGasStationIdx = currentPos + 1; nextGasStationIdx < t_Stops.size(); ++nextGasStationIdx) {
            if (t_Stops.at(nextGasStationIdx) - t_Stops.at(currentPos) <= t_Tank) {
                nextGasStation = nextGasStationIdx;
            } else {
                // We've found the farthest gas station
                break;
            }
        }

        // It's impossible, to reach the city B
        if (nextGasStation == currentPos) {
            return -1;
        }

        if (nextGasStation < t_Stops.size() - 1) {
            minRefills++;
        }
    }

    return minRefills;
}

int32_t main(int32_t argc, char* argv[]) {
    uint32_t distanceBetweenCities{ 0 };
    std::cin >> distanceBetweenCities;
    uint32_t fullTankTravellingDistance{ 0 };
    std::cin >> fullTankTravellingDistance;
    uint32_t numberOfStops{ 0 };
    std::cin >> numberOfStops;

    std::vector<uint32_t> stops(numberOfStops + 2);
    stops.back() = distanceBetweenCities;

    for (uint32_t i = 1; i < numberOfStops + 1; ++i) {
        std::cin >> stops.at(i);
    }

    std::cout << ComputeMinRefills(fullTankTravellingDistance, stops) << '\n';

    return 0;
}
