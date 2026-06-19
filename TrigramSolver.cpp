#include "TrigramSolver.h"

set<TrigramSolver::Trigram> TrigramSolver::getTrigrams(const string &word) {
    vector<int> w = utf8_to_codepoints(word);
    vector<int> padded;
    int pad = 95;
    padded.push_back(pad);
    padded.push_back(pad);
    for (int i : w) padded.push_back(i);
    padded.push_back(pad);

    set<Trigram> trigrams;
    if (padded.size() < 4) return trigrams;

    for (int i = 0; i < padded.size() - 2; i++) {
        Trigram trigram {padded[i], padded[i + 1], padded[i + 2]};
        trigrams.insert(trigram);
    }
    return trigrams;
}

string TrigramSolver::getAlgorithmName() const {
    return "Триграммный метод\n\n";
}

Result TrigramSolver::solve(const string &typo) {
    set<Trigram> typo_trigrams = getTrigrams(typo);
    map<string, int> candidate_intersection_counts;

    for (const Trigram& tg : typo_trigrams) {
        auto it = inverted_index.find(tg);
        if (it != inverted_index.end())
            for (const string& word : it->second) candidate_intersection_counts[word]++;
    }

    double best = 0;
    vector<string> candidates;

    for (const auto& pair : candidate_intersection_counts) {
        const string& word = pair.first;
        int intersection_size = pair.second;

        int typo_size = typo_trigrams.size();
        int word_size = getTrigrams(word).size();
        int union_size = typo_size + word_size - intersection_size;

        double coef = 0;
        if (union_size > 0) coef = (double) intersection_size / (double) union_size;

        if (fabs(coef - best) < eps) candidates.push_back(word);
        else if (coef > best) {
            best = coef;
            candidates.clear();
            candidates.push_back(word);
        }
    }

    return {typo, candidates, best};
}

void TrigramSolver::buildIndex() {
    inverted_index.clear();

    for (const string& word : dictionary) {
        set<Trigram> word_trigrams = getTrigrams(word);
        for (const Trigram& t : word_trigrams) inverted_index[t].push_back(word);
    }
}

void TrigramSolver::printResult(const Result &res) const {
    cout << getAlgorithmName();
    cout << "Введенное слово: " << res.word << "\n\n";
    cout << "Кандидаты:\n";
    int count = 1;
    for (string word : res.candidates) cout << "\t" + to_string(count++) + ") " << word << "\n";
    cout << "Коэффициент Жаккара: " << res.score << "\n";
}
