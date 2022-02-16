#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#ifdef LOCAL_ENV
#include "utils.hpp"
#endif

static const std::size_t cMultiplier = 1;
static const std::size_t cPrime = 1000000007;

struct Data
{
    std::string pattern, text;
};

Data ReadInput()
{
    Data data;
    std::cin >> data.pattern >> data.text;
    return data;
}

void PrintOccurrences(const std::vector<int32_t>& output)
{
    for (size_t i = 0; i < output.size(); ++i)
        std::cout << output[i] << " ";
    std::cout << "\n";
}

std::size_t HashString(const std::string& tStr)
{
    std::size_t hash = 0;
    std::for_each(tStr.rbegin(), tStr.rend(), [&](const char& a) {
        hash = (hash * cMultiplier + a) % cPrime;
    });
    return hash;
}

std::vector<std::size_t> PrecomputeHashes(const std::string& tText, uint32_t tPatternLength)
{
    std::vector<std::size_t> hashes(tText.size() - tPatternLength + 1);
    hashes[tText.size() - tPatternLength] =
        HashString(tText.substr(tText.size() - tPatternLength, tPatternLength));

    std::size_t y = 1;
    for (uint32_t i = 0; i < tPatternLength; ++i) {
        y = (y * cMultiplier) % cPrime;
    }

    for (int32_t i = tText.size() - tPatternLength - 1; i >= 0; --i) {
        hashes[i] =
            (cMultiplier * hashes[i + 1] + tText[i] - y * tText[i + tPatternLength]) % cPrime;
    }

    return hashes;
}

std::vector<int32_t> RabinKarp(const Data& input)
{
    const std::string& p = input.pattern;
    const std::string& t = input.text;
    std::vector<int32_t> ans;

    std::size_t patternHash = HashString(p);
    std::vector<std::size_t> precomputedHashes = PrecomputeHashes(t, p.size());

    for (uint32_t i = 0; i < t.size() - p.size() + 1; ++i) {
        if (patternHash != precomputedHashes[i]) {
            continue;
        }

        // Collision or the substrings are equal
        // if (std::string_view(t).substr(i, p.size()) == p) {
        //     ans.push_back(i);
        // }

        if (!std::char_traits<char>::compare(t.data() + i, p.data(), p.size())) {
            ans.push_back(i);
        }
    }

    return ans;
}

int32_t main()
{
    std::ios_base::sync_with_stdio(false);
    PrintOccurrences(RabinKarp(ReadInput()));
    return 0;
}
