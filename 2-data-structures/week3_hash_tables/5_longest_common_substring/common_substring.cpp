#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

#ifdef LOCAL_ENV
#include "utils.hpp"
#else
#define PROFILE_FUNCTION()
#endif

inline void hash_combine(std::size_t& seed)
{}

template<typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hash_combine(seed, rest...);
}

struct Answer
{
    size_t i, j, len;

    Answer() = default;

    Answer(size_t tI, size_t tJ, size_t tLen)
        : i(tI)
        , j(tJ)
        , len(tLen)
    {}
};

struct HashPair
{
    int64_t hash1;
    int64_t hash2;

    HashPair() = default;

    HashPair(int64_t h1, int64_t h2)
        : hash1(h1)
        , hash2(h2)
    {}

    bool operator==(const HashPair& other) const
    {
        return hash1 == other.hash1 && hash2 == other.hash2;
    }

    struct Hash
    {
        std::size_t operator()(const HashPair& hp) const
        {
            std::size_t hash = 0;
            hash_combine(hash, hp.hash1, hp.hash2);
            return hash;
        }
    };
};

class LCS
{
private:
    const std::string& s;
    const std::string& t;

    static const int64_t cMod1 = 1000000007;
    static const int64_t cMod2 = 1000000009;
    static const int64_t cMultiplier1 = 83757631;
    static const int64_t cMultiplier2 = 83757632;

    std::vector<int64_t> mModPows1;
    std::vector<int64_t> mModPows2;
    bool mMemoryInitialized{ false };

    uint32_t mMaxPossibleSubstringLen{ 0 };

    std::vector<std::unordered_map<HashPair, uint32_t, HashPair::Hash>> sHashes;
    std::vector<std::unordered_map<HashPair, uint32_t, HashPair::Hash>> tHashes;

    template<typename T>
    inline T Mod(T a, T b)
    {
        return (a % b + b) % b;
    }

    template<typename T>
    T ModPow(T tBase, T tExp, T tModulus)
    {
        if (tBase == cMultiplier1 && tModulus == cMod1 && mMemoryInitialized &&
            tExp < mModPows1.size()) {
            return mModPows1[tExp];
        } else if (tBase == cMultiplier2 && tModulus == cMod2 && mMemoryInitialized &&
                   tExp < mModPows2.size()) {
            return mModPows2[tExp];
        }

        tBase %= tModulus;
        T result = 1;
        while (tExp > 0) {
            if (tExp & 1)
                result = (result * tBase) % tModulus;
            tBase = (tBase * tBase) % tModulus;
            tExp >>= 1;
        }

        return result;
    }

    std::size_t HashSubstring(std::string::const_iterator&& tBegin,
                              const std::size_t tSubstringLength,
                              const std::size_t tMod,
                              const std::size_t tMultiplier)
    {
        std::size_t hash = 0;

        for (uint32_t i = 0; i < tSubstringLength; i++) {
            hash =
                (hash + *(tBegin++) * ModPow(tMultiplier, tSubstringLength - i - 1, tMod)) % tMod;
        }

        return hash;
    }

    void PrecomputeModPow()
    {
        mModPows1.resize(std::max(s.size(), t.size()) + 1);
        mModPows2.resize(std::max(s.size(), t.size()) + 1);

        for (uint32_t i = 0; i < mModPows1.size(); ++i) {
            mModPows1[i] = ModPow(cMultiplier1, static_cast<int64_t>(i), cMod1);
            mModPows2[i] = ModPow(cMultiplier2, static_cast<int64_t>(i), cMod2);
        }
        mMemoryInitialized = true;
    }

    void PrecomputeHashes(int64_t tSubstrLen)
    {
        HashPair hp1(HashSubstring(s.begin(), tSubstrLen, cMod1, cMultiplier1),
                     HashSubstring(s.begin(), tSubstrLen, cMod2, cMultiplier2));

        HashPair hp2(HashSubstring(t.begin(), tSubstrLen, cMod1, cMultiplier1),
                     HashSubstring(t.begin(), tSubstrLen, cMod2, cMultiplier2));

        sHashes[tSubstrLen - 1] = std::unordered_map<HashPair, uint32_t, HashPair::Hash>();
        sHashes[tSubstrLen - 1][hp1] = 0;

        tHashes[tSubstrLen - 1] = std::unordered_map<HashPair, uint32_t, HashPair::Hash>();
        tHashes[tSubstrLen - 1][hp2] = 0;

        // For each substring of length substrLen
        HashPair tmpHp = hp1;
        for (auto i = 1; i <= s.size() - tSubstrLen; ++i) {
            tmpHp = HashPair(
                Mod(Mod(cMultiplier1 *
                            Mod(tmpHp.hash1 -
                                    Mod(s[i - 1] * ModPow(cMultiplier1, tSubstrLen - 1, cMod1),
                                        cMod1),
                                cMod1),
                        cMod1) +
                        s[i + tSubstrLen - 1],
                    cMod1),
                Mod(Mod(cMultiplier2 *
                            Mod(tmpHp.hash2 -
                                    Mod(s[i - 1] * ModPow(cMultiplier2, tSubstrLen - 1, cMod2),
                                        cMod2),
                                cMod2),
                        cMod2) +
                        s[i + tSubstrLen - 1],
                    cMod2));
            sHashes[tSubstrLen - 1][tmpHp] = i;
        }

        // For each substring of length substrLen
        tmpHp = hp2;
        for (auto i = 1; i <= t.size() - tSubstrLen; ++i) {
            tmpHp = HashPair(
                Mod(Mod(cMultiplier1 *
                            Mod(tmpHp.hash1 -
                                    Mod(t[i - 1] * ModPow(cMultiplier1, tSubstrLen - 1, cMod1),
                                        cMod1),
                                cMod1),
                        cMod1) +
                        t[i + tSubstrLen - 1],
                    cMod1),
                Mod(Mod(cMultiplier2 *
                            Mod(tmpHp.hash2 -
                                    Mod(t[i - 1] * ModPow(cMultiplier2, tSubstrLen - 1, cMod2),
                                        cMod2),
                                cMod2),
                        cMod2) +
                        t[i + tSubstrLen - 1],
                    cMod2));
            tHashes[tSubstrLen - 1][tmpHp] = i;
        }
    }

    Answer CheckSubstring(uint32_t index)
    {
        Answer ans{ 0, 0, 0 };

        PrecomputeHashes(index + 1);

        for (auto const& [key, val] : sHashes[index]) {
            if (tHashes[index].find(key) != tHashes[index].end()) {
                ans = Answer(val, tHashes[index][key], index + 1);
                break;
            }
        }

        return ans;
    }

public:
    Answer Solve()
    {
        PROFILE_FUNCTION();

        Answer globalAns{ 0, 0, 0 };

        int32_t left = 0;
        int32_t right = mMaxPossibleSubstringLen - 1;

        // Do binary search
        while (left <= right) {
            int32_t mid = left + (right - left) / 2;
            Answer localAns = CheckSubstring(mid);
            if (localAns.len == 0) {
                right = mid - 1;
            } else {
                globalAns = localAns;
                left = mid + 1;
            }
        }

        return globalAns;
    }

    LCS(const std::string& tS, const std::string& tT)
        : s{ tS }
        , t{ tT }
        , mMaxPossibleSubstringLen(std::min(tS.size(), tT.size()))
        , sHashes(mMaxPossibleSubstringLen)
        , tHashes(mMaxPossibleSubstringLen)
    {
        PROFILE_FUNCTION();

        PrecomputeModPow();
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::string s, t;
    while (std::cin >> s >> t) {
        auto ans = LCS(s, t).Solve();
        std::cout << ans.i << " " << ans.j << " " << ans.len << "\n";
    }
}
