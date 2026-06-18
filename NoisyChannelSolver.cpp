#include "NoisyChannelSolver.h"
#include <algorithm>
#include <cmath>
#include <iostream>

NoisyChannelSolver::NoisyChannelSolver(const unordered_set<string>& dict, const unordered_map<string, int>& frequencies, Solver& distancer_)
        : Solver(dict), word_frequencies(frequencies), distancer(distancer_) {

    for (const auto& pair : word_frequencies) total_words_count += pair.second;
    if (total_words_count == 0) total_words_count = 1;

    initConfusionMatrices();
}

string NoisyChannelSolver::getAlgorithmName() const {
    return "Метод зашумленного канала";
}

void NoisyChannelSolver::initConfusionMatrices() {
    cm_del["т"]["е"] = 0.15;
    cm_del["а"]["а"] = 0.05;

    cm_sub["о"]["а"] = 0.12;
    cm_sub["и"]["е"] = 0.09;
    cm_sub["с"]["ы"] = 0.01;

    cm_trans["л"]["е"] = 0.08;
}

double NoisyChannelSolver::calculateChannelProbability(const string& typo, const string& target) {
    if (typo == target) return 1.0;

    double p_err = 1.0;
    bool op_found = false;

    if (target.length() == typo.length() + 1) {
        for (size_t i = 0; i < target.length(); ++i) {
            if (i == typo.length() || target[i] != typo[i]) {
                string context = (i > 0) ? string(1, target[i - 1]) : "^";
                string deleted = string(1, target[i]);

                if (cm_del.count(context) && cm_del[context].count(deleted)) {
                    p_err *= cm_del[context][deleted];
                    op_found = true;
                }
                break;
            }
        }
    }
    else if (target.length() == typo.length()) {
        for (size_t i = 0; i < target.length(); ++i) {
            if (target[i] != typo[i]) {
                if (i + 1 < target.length() && target[i] == typo[i + 1] && target[i + 1] == typo[i]) {
                    string char1 = string(1, target[i]);
                    string char2 = string(1, target[i + 1]);
                    if (cm_trans.count(char1) && cm_trans[char1].count(char2)) {
                        p_err *= cm_trans[char1][char2];
                        op_found = true;
                    }
                    break;
                }
                else {
                    string from_char = string(1, target[i]);
                    string to_char = string(1, typo[i]);
                    if (cm_sub.count(from_char) && cm_sub[from_char].count(to_char)) {
                        p_err *= cm_sub[from_char][to_char];
                        op_found = true;
                    }
                    break;
                }
            }
        }
    }

    if (!op_found) p_err = 1e-4;

    return p_err;
}

Result NoisyChannelSolver::solve(const string &typo) {
    Result levenshtein_res = distancer.solve(typo);

    if (levenshtein_res.candidates.empty()) return { {}, 0.0 };

    vector<string> final_candidates;
    double best_score = -1e18;

    for (const string& word : levenshtein_res.candidates) {

        int freq = 0;
        auto it = word_frequencies.find(word);
        if (it != word_frequencies.end()) freq = it->second;
        double p_w = (double)(freq + 1) / (total_words_count + dictionary.size());

        double p_s_w = calculateChannelProbability(typo, word);

        double current_score = log(p_s_w) + log(p_w);

        if (final_candidates.empty() || current_score > best_score) {
            best_score = current_score;
            final_candidates.clear();
            final_candidates.push_back(word);
        }
        else if (fabs(current_score - best_score) < 1e-5) final_candidates.push_back(word);
    }
    return { final_candidates, exp(best_score) };
}