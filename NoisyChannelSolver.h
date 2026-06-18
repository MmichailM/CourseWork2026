#ifndef MAIN_NOISYCHANNELSOLVER_H
#define MAIN_NOISYCHANNELSOLVER_H

#include "Solver.h"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

struct EditOperation {
    string type; // "ins", "del", "sub", "trans"
    string from;
    string to;
};

class NoisyChannelSolver : public Solver {
private:
    unordered_map<string, int> word_frequencies;
    long long total_words_count = 0;
    Solver& distancer;

    unordered_map<string, unordered_map<string, double>> cm_del;
    unordered_map<string, unordered_map<string, double>> cm_ins;
    unordered_map<string, unordered_map<string, double>> cm_sub;
    unordered_map<string, unordered_map<string, double>> cm_trans;

    void initConfusionMatrices();
    double calculateChannelProbability(const string& typo, const string& target);

public:
    NoisyChannelSolver(
            const unordered_set<string>& dict,
            const unordered_map<string, int>& frequencies,
            Solver& distancer_
    );

    string getAlgorithmName() const override;
    Result solve(const string& typo) override;
};

#endif