#include "DamerauLevenshteinSolver.h"

int DamerauLevenshteinSolver::calculateLevenshteinDistance(const string &s1, const string &s2) {
    vector<int> new_s1 = utf8_to_codepoints(s1);
    vector<int> new_s2 = utf8_to_codepoints(s2);

    int n = new_s1.size(), m = new_s2.size();
    vector<vector<int>> table(n + 1, vector<int>(m + 1));

    for (int i = 0; i <= n; i++) table[i][0] = i;
    for (int j = 0; j <= m; j++) table[0][j] = j;

    for (int i = 1; i < n + 1; i++) {
        for (int j = 1; j < m + 1; j++) {
            int cost = 0;
            if (new_s1[i - 1] != new_s2[j - 1]) cost = 1;
            table[i][j] = min(min(table[i - 1][j] + 1, table[i][j - 1] + 1), table[i - 1][j - 1] + cost);

            if (i > 1 && j > 1 && new_s1[i - 1] == new_s2[j - 2] && new_s1[i - 2] == new_s2[j - 1])
                table[i][j] = min(table[i][j], table[i - 2][j - 2] + 1);
        }
    }

    return table[n][m];
}

string DamerauLevenshteinSolver::getAlgorithmName() const {
    return "Алгоритм Дамерау-Левенштейна";
}

Result DamerauLevenshteinSolver::solve(const string &typo) {
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
