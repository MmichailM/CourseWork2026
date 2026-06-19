#include "NoisyChannelSolver.h"
#include <algorithm>
#include <cmath>
#include <iostream>

NoisyChannelSolver::NoisyChannelSolver(const unordered_set<string>& dict, const unordered_map<string, double>& frequencies, Solver& distancer_)
        : Solver(dict), word_frequencies(frequencies), distancer(distancer_) {

    for (const auto& pair : word_frequencies) total_words_count += pair.second;
    if (total_words_count == 0) total_words_count = 1;
}

string NoisyChannelSolver::getAlgorithmName() const {
    return "Метод зашумленного канала\n\n";
}


Result NoisyChannelSolver::solve(const string& typo) {
    Result base = distancer.solve(typo);
    string best_word;
    double best_p = -1e18;

    for (const string& w : base.candidates) {
        int freq = word_frequencies.count(w) ? word_frequencies.at(w) : 0;
        double log_p_w = log((double)(freq + 1) /
                             (total_words_count + dictionary.size()));
        if (log_p_w > best_p) {
            best_p  = log_p_w;
            best_word = w;
        }
    }
    return { typo, {best_word}, exp(best_p) };
}

void NoisyChannelSolver::printResult(const Result &res) const {
    cout << getAlgorithmName();
    cout << "Введенное слово: " << res.word << "\n\n";
    cout << "Наиболее вероятный кандидат: ";
    for (string word : res.candidates) cout << word << "\n";
    cout << "Вероятность: " << res.score << "\n";
}
