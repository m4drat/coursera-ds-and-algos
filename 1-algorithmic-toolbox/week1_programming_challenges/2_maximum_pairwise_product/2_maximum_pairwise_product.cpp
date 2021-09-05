#include <vector>
#include <iostream>
#include <algorithm>

int64_t maxPairwiseProduct(std::vector<int32_t>& t_Numbers) {
    std::sort(t_Numbers.begin(), t_Numbers.end());
    return (int64_t)(*(--t_Numbers.end())) * (int64_t)(*(t_Numbers.end() - 2));
}

// int64_t maxPairwiseProduct(const std::vector<int32_t>& t_Numbers) {
//     int32_t indexNumA{ -1 };
//     int32_t indexNumB{ -1 };

//     for (uint32_t i = 0; i < t_Numbers.size(); i++) {
//         if (indexNumA == -1 || t_Numbers[i] > t_Numbers[indexNumA]) {
//             indexNumA = i;
//         }
//     }

//     for (uint32_t i = 0; i < t_Numbers.size(); i++) {
//         if (i != indexNumA && (indexNumB == -1 || t_Numbers[i] > t_Numbers[indexNumB])) {
//             indexNumB = i;
//         }
//     }

//     return (int64_t)t_Numbers[indexNumA] * (int64_t)t_Numbers[indexNumB];
// }

int main(int argc, char* argv[]) {
    int32_t totalNumbers;
    std::vector<int32_t> numbers;

    std::cin >> totalNumbers;

    for (uint32_t idx = 0; idx < totalNumbers; idx++) {
        int32_t currentNumber;
        std::cin >> currentNumber;
        numbers.emplace_back(currentNumber);
    }

    std::cout << maxPairwiseProduct(numbers) << std::endl;

    return 0;
}