#ifndef MAIN_NOISYCHANNELSOLVER_H
#define MAIN_NOISYCHANNELSOLVER_H

#include "Solver.h"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

class NoisyChannelSolver : public Solver {
private:
    unordered_map<string, double> word_frequencies;
    long long total_words_count = 0;
    Solver& distancer;
public:
    NoisyChannelSolver(
            const unordered_set<string>& dict,
            const unordered_map<string, double>& frequencies,
            Solver& distancer_
    );

    string getAlgorithmName() const override;
    Result solve(const string& typo) override;

    void printResult(const Result& res) const override;
};

#endif