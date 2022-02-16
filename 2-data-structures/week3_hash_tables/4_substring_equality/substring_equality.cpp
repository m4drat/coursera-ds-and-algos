#include <iostream>
#include <iterator>
#include <vector>

class Solver
{
private:
    std::string mStr;
    std::vector<int64_t> mHashesMod1;
    std::vector<int64_t> mHashesMod2;

    std::vector<int64_t> mModPows1;
    std::vector<int64_t> mModPows2;
    bool mMemoryInitialized{ false };

    static const int64_t cMod1 = 1000000007;
    static const int64_t cMod2 = 1000000009;
    static const int64_t cMultiplier1 = 83757631;
    static const int64_t cMultiplier2 = 83757632;

public:
    Solver(std::string& tStr)
        : mStr{ tStr }
    {
        PrecomputeModPow();
        PrecomputeHashes();
    }

    template<typename T>
    inline T Mod(T a, T b)
    {
        return (a % b + b) % b;
    }

    template<typename T>
    T ModPow(T tBase, T tExp, T tModulus)
    {
        if (tBase == cMultiplier1 && tModulus == cMod1 && mMemoryInitialized) {
            return mModPows1[tExp];
        } else if (tBase == cMultiplier2 && tModulus == cMod2 && mMemoryInitialized) {
            return mModPows2[tExp];
        }

        tBase %= tModulus;
        T result = 1;
        while (tExp > 0) {
            if (tExp & 1)
                result = (result % tModulus) * (tBase % tModulus) % tModulus;
            tBase = (tBase % tModulus) * (tBase % tModulus) % tModulus;
            tExp >>= 1;
        }

        return result;
    }

    std::size_t HashSubstring(std::string::iterator&& tBegin,
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
        mModPows1.resize(mStr.size() + 1);
        mModPows2.resize(mStr.size() + 1);

        for (uint32_t i = 0; i < mModPows1.size(); ++i) {
            mModPows1[i] = ModPow(cMultiplier1, static_cast<int64_t>(i), cMod1);
            mModPows2[i] = ModPow(cMultiplier2, static_cast<int64_t>(i), cMod2);
        }
        mMemoryInitialized = true;
    }

    void PrecomputeHashes()
    {
        mHashesMod1.resize(mStr.size() + 1);
        mHashesMod2.resize(mStr.size() + 1);

        mHashesMod1[1] = HashSubstring(mStr.begin(), 1, cMod1, cMultiplier1);
        mHashesMod2[1] = HashSubstring(mStr.begin(), 1, cMod2, cMultiplier2);

        for (uint32_t i = 2; i <= mStr.size(); ++i) {
            mHashesMod1[i] =
                Mod(Mod(cMultiplier1 * mHashesMod1[i - 1], cMod1) + mStr[i - 1], cMod1);
            mHashesMod2[i] =
                Mod(Mod(cMultiplier2 * mHashesMod2[i - 1], cMod2) + mStr[i - 1], cMod2);
        }
    }

    bool Ask(std::size_t a, std::size_t b, std::size_t l)
    {
        int64_t mul1mod = ModPow(cMultiplier1, static_cast<int64_t>(l), cMod1);
        int64_t hash1Str1 = Mod(mHashesMod1[a + l] - (Mod(mul1mod * mHashesMod1[a], cMod1)), cMod1);
        int64_t hash1Str2 = Mod(mHashesMod1[b + l] - (Mod(mul1mod * mHashesMod1[b], cMod1)), cMod1);

        int64_t mul2mod = ModPow(cMultiplier2, static_cast<int64_t>(l), cMod2);
        int64_t hash2Str1 = Mod(mHashesMod2[a + l] - (Mod(mul2mod * mHashesMod2[a], cMod2)), cMod2);
        int64_t hash2Str2 = Mod(mHashesMod2[b + l] - (Mod(mul2mod * mHashesMod2[b], cMod2)), cMod2);

        // !std::char_traits<char>::compare(mStr.data() + a, mStr.data() + b, l)
        if (hash1Str1 == hash1Str2 && hash2Str1 == hash2Str2) {
            return true;
        }

        return false;
    }
};

int main()
{
    std::ios_base::sync_with_stdio(0), std::cin.tie(0);

    std::string str;
    uint32_t totalQueries;
    std::cin >> str >> totalQueries;
    Solver solver(str);

    for (uint32_t i = 0; i < totalQueries; ++i) {
        std::size_t a, b, l;
        std::cin >> a >> b >> l;
        std::cout << (solver.Ask(a, b, l) ? "Yes\n" : "No\n");
    }
}
