#ifndef MAIN_SOLVER_H
#define MAIN_SOLVER_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <string>

using namespace std;

struct Result {
    vector<string> candidates;
    double score;
};

class Solver {
protected:
    const unordered_set<string>& dictionary;
    vector<int> utf8_to_codepoints(const string& str);

public:
    explicit Solver(const unordered_set<string>& dict): dictionary(dict) {}

    virtual string getAlgorithmName() const = 0;
    virtual Result solve(const string& typo) = 0;

    virtual ~Solver() = default;
};


#endif
