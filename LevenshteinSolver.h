#ifndef MAIN_LEVENSHTEINSOLVER_H
#define MAIN_LEVENSHTEINSOLVER_H

#include "Solver.h"

class LevenshteinSolver : public Solver {
    int calculateLevenshteinDistance(const string& s1, const string& s2);
public:
    using Solver::Solver;

    string getAlgorithmName() const override;
    Result solve(const string& typo) override;

    void printResult(const Result& res) const override;
};


#endif
