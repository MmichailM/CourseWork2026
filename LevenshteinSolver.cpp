#include "LevenshteinSolver.h"

int LevenshteinSolver::calculateLevenshteinDistance(const string &s1, const string &s2) {
    vector<int> new_s1 = utf8_to_codepoints(s1);
    vector<int> new_s2 = utf8_to_codepoints(s2);

    int n = new_s1.size(), m = new_s2.size();
    vector<vector<int>> table(n + 1, vector<int>(m + 1));

    table[0][0] = 0;

    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < m + 1; j++) {
            if (i == 0 && j != 0) table[i][j] = j;
            else if (j == 0 && i != 0) table[i][j] = i;
            else if (i != 0 && j != 0) {
                int cost = 0;
                if (new_s1[i - 1] != new_s2[j - 1]) cost = 1;
                table[i][j] = min(min(table[i - 1][j] + 1, table[i][j - 1] + 1), table[i - 1][j - 1] + cost);
            }
        }
    }

    return table[n][m];
}

string LevenshteinSolver::getAlgorithmName() const {
    return "Алгоритм Левенштейна";
}

Result LevenshteinSolver::solve(const string &typo) {
    vector<string> words;
    double best = 1e9;

    for (string word : dictionary) {
        int d = calculateLevenshteinDistance(typo, word);
        if (d == best) words.push_back(word);
        else if (d < best) {
            words.clear();
            words.push_back(word);
            best = d;
        }
    }

    return {words, best};
}
