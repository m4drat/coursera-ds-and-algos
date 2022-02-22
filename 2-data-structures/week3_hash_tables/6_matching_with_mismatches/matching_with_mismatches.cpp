#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

class Solver
{
private:
    uint32_t mMaxMismatches;
    std::string mText;
    std::string mPattern;

    std::vector<int64_t> mTextSubstrHashes;
    std::vector<int64_t> mPatternSubstrHashes;

    std::vector<int64_t> mModPows1;
    std::vector<int64_t> mModPows2;
    bool mMemoryInitialized{ false };

    static const int64_t cMod1 = 1000000007;
    static const int64_t cMod2 = 1000000009;
    static const int64_t cMultiplier1 = 81985361;
    static const int64_t cMultiplier2 = 33717632;

    static const int32_t NO_MISMATCHES = std::numeric_limits<int32_t>::max();

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
        mModPows1.resize(mPattern.size() + 1);
        mModPows2.resize(mPattern.size() + 1);

        for (uint32_t i = 0; i < mPattern.size(); ++i) {
            mModPows1[i] = ModPow(cMultiplier1, static_cast<int64_t>(i), cMod1);
        }
        mMemoryInitialized = true;
    }

    void PrecomputeHashes()
    {
        mTextSubstrHashes.resize(mText.size() + 1);
        mTextSubstrHashes[1] = HashSubstring(mText.begin(), 1, cMod1, cMultiplier1);

        for (uint32_t i = 2; i <= mText.size(); ++i) {
            mTextSubstrHashes[i] =
                Mod(Mod(cMultiplier1 * mTextSubstrHashes[i - 1], cMod1) + mText[i - 1], cMod1);
        }

        mPatternSubstrHashes.resize(mPattern.size() + 1);
        mPatternSubstrHashes[1] = HashSubstring(mPattern.begin(), 1, cMod1, cMultiplier1);

        for (uint32_t i = 2; i <= mPattern.size(); ++i) {
            mPatternSubstrHashes[i] = Mod(
                Mod(cMultiplier1 * mPatternSubstrHashes[i - 1], cMod1) + mPattern[i - 1], cMod1);
        }
    }

    inline int64_t GetTextSubstrHash(uint32_t start,
                                     uint32_t len,
                                     int64_t multiplier = cMultiplier1,
                                     int64_t mod = cMod1)
    {
        int64_t mul1mod = ModPow(multiplier, static_cast<int64_t>(len), mod);
        int64_t textSubstrHash = Mod(mTextSubstrHashes.at(start + len) -
                                         (Mod(mul1mod * mTextSubstrHashes.at(start), mod)),
                                     mod);

        return textSubstrHash;
    }

    inline int64_t GetPatternSubstrHash(uint32_t start,
                                        uint32_t len,
                                        int64_t multiplier = cMultiplier1,
                                        int64_t mod = cMod1)
    {
        int64_t mul2mod = ModPow(multiplier, static_cast<int64_t>(len), mod);
        int64_t patternSubstrHash = Mod(mPatternSubstrHashes.at(start + len) -
                                            (Mod(mul2mod * mPatternSubstrHashes.at(start), mod)),
                                        mod);

        return patternSubstrHash;
    }

public:
    Solver(uint32_t tMaxMismatches, const std::string& tText, const std::string& tPattern)
        : mMaxMismatches{ tMaxMismatches } // !tMaxMismatches ? 1 : tMaxMismatches
        , mText{ tText }
        , mPattern{ tPattern }
    {
        PrecomputeModPow();
        PrecomputeHashes();
    }

    /**
     * @brief Returns the index of the first mismatch in the range [start, end]
     * @return int32_t - first mismatch index
     */
    int32_t FindFirstMismatchInRange(int32_t base, int32_t start, int32_t len)
    {
        if (len == 1) {
            return (mText.at(base + start) != mPattern.at(start)) ? start : NO_MISMATCHES;
        }

        if (len % 2 != 0) {
            if (mText.at(base + start) != mPattern.at(start)) {
                return start;
            }

            return FindFirstMismatchInRange(base, start + 1, len - 1);
        }

        len /= 2;

        // Check the left half
        if (GetTextSubstrHash(base + start, len) != GetPatternSubstrHash(start, len)) {
            return FindFirstMismatchInRange(base, start, len);
        }

        // Check the right half
        if (GetTextSubstrHash(base + start + len, len) != GetPatternSubstrHash(len + start, len)) {
            return FindFirstMismatchInRange(base, start + len, len);
        }

        return NO_MISMATCHES;
    }

    uint32_t CountMismatches(int32_t start,
                             uint32_t maxAllowedMismatches = std::numeric_limits<uint32_t>::max())
    {
        uint32_t totalMismatches = 0;
        int32_t nextMismatchPos = 0;

        while (mPattern.size() - nextMismatchPos > 0) {
            if (totalMismatches > maxAllowedMismatches) {
                return std::numeric_limits<uint32_t>::max();
            }

            nextMismatchPos =
                FindFirstMismatchInRange(start, nextMismatchPos, mPattern.size() - nextMismatchPos);
            if (nextMismatchPos == NO_MISMATCHES) {
                break;
            } else {
                totalMismatches++;
                nextMismatchPos++;
            }
        }

        return totalMismatches;
    }

    std::vector<uint32_t> Solve()
    {
        std::vector<uint32_t> startIndices;

        // For each position in the text - try to check
        for (uint32_t i = 0; i < mText.size() - mPattern.size() + 1; ++i) {
            if (CountMismatches(i, mMaxMismatches) <= mMaxMismatches) {
                startIndices.emplace_back(i);
            }
        }

        return startIndices;
    }
};

#ifdef LOCAL_ENV

bool CheckSolution()
{
    struct ProblemStatement
    {
        int32_t maxMismatches;
        std::string text;
        std::string pattern;
        int32_t answer;
    };

    std::vector<ProblemStatement> problemSolutionPairs{
        ProblemStatement{ .maxMismatches = 0, .text{ "AAAA" }, .pattern{ "AAAA" }, .answer = 0 },
        ProblemStatement{ .maxMismatches = 0, .text{ "BAAA" }, .pattern{ "AAAA" }, .answer = 1 },
        ProblemStatement{ .maxMismatches = 0, .text{ "ABAA" }, .pattern{ "AAAA" }, .answer = 1 },
        ProblemStatement{ .maxMismatches = 0, .text{ "AABA" }, .pattern{ "AAAA" }, .answer = 1 },
        ProblemStatement{ .maxMismatches = 0, .text{ "AAAB" }, .pattern{ "AAAA" }, .answer = 1 },
        ProblemStatement{ .maxMismatches = 0, .text{ "AABB" }, .pattern{ "AAAA" }, .answer = 2 },
        ProblemStatement{ .maxMismatches = 0, .text{ "ABAB" }, .pattern{ "AAAA" }, .answer = 2 },
        ProblemStatement{ .maxMismatches = 0, .text{ "ABBA" }, .pattern{ "AAAA" }, .answer = 2 },
        ProblemStatement{ .maxMismatches = 0, .text{ "BABA" }, .pattern{ "AAAA" }, .answer = 2 },
        ProblemStatement{ .maxMismatches = 0, .text{ "BBAA" }, .pattern{ "AAAA" }, .answer = 2 },
        ProblemStatement{ .maxMismatches = 0, .text{ "BBBA" }, .pattern{ "AAAA" }, .answer = 3 },
        ProblemStatement{ .maxMismatches = 0, .text{ "ABBB" }, .pattern{ "AAAA" }, .answer = 3 },
        ProblemStatement{ .maxMismatches = 0, .text{ "BBBB" }, .pattern{ "AAAA" }, .answer = 4 },
        ProblemStatement{ .maxMismatches = 0, .text{ "BBBBB" }, .pattern{ "AAAAA" }, .answer = 5 },
        ProblemStatement{ .maxMismatches = 0, .text{ "BBABB" }, .pattern{ "AAAAA" }, .answer = 4 },
        ProblemStatement{ .maxMismatches = 0, .text{ "ABBBB" }, .pattern{ "AAAAA" }, .answer = 4 }
    };

    for (const auto& testcase : problemSolutionPairs) {
        int32_t answer =
            Solver(testcase.maxMismatches, testcase.text, testcase.pattern).CountMismatches(0);
        if (answer != testcase.answer) {
            throw std::runtime_error("Got: " + std::to_string(answer) +
                                     ". Expected: " + std::to_string(testcase.answer) +
                                     ". Text/Pattern: " + testcase.text + "/" + testcase.pattern);
        }
    }

    return true;
}

#endif

int main()
{
#ifdef LOCAL_ENV
    if (!CheckSolution()) {
        std::cout << "Check failed!" << std::endl;
    }
    std::cout << "Check passed!" << std::endl;
#else
    std::ios_base::sync_with_stdio(false); // std::cin.tie(NULL);
    int32_t k;
    std::string t, p;
    while (std::cin >> k >> t >> p) {
        auto a = Solver(k, t, p).Solve();
        std::cout << a.size();
        for (uint32_t x : a)
            std::cout << " " << x;
        std::cout << "\n";
    }
#endif
}
