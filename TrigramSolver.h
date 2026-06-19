#ifndef MAIN_TRIGRAMSOLVER_H
#define MAIN_TRIGRAMSOLVER_H

#include "Solver.h"
#include <algorithm>
#include <math.h>
#include <map>

const double eps = pow(10, -5);

class TrigramSolver : public Solver {
    struct Trigram {
        int c1, c2, c3;

        bool operator< (const Trigram& other) const {
            if (c1 != other.c1) return c1 < other.c1;
            else if (c2 != other.c2) return c2 < other.c2;
            return c3 < other.c3;
        }
    };

    map<Trigram, vector<string>> inverted_index;
    void buildIndex();

    set<Trigram> getTrigrams(const string& word);
public:
    explicit TrigramSolver(const unordered_set<string>& dict) : Solver(dict) {
        buildIndex();
    }

    string getAlgorithmName() const override;
    Result solve(const string& typo) override;

    void printResult(const Result& res) const override;
};


#endif
