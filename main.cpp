#include "LevenshteinSolver.h"
#include "DamerauLevenshteinSolver.h"
#include "TrigramSolver.h"
#include "NoisyChannelSolver.h"
#include <iomanip>
#include <chrono>

vector<pair<string, string>> read_typos(const string& filename);
double Accurate1(const string& filename, Solver* solver);
double Accurate3(const string& filename, Solver* solver);
double GetTime(const string& filename, Solver* solver);

unordered_set<string> form_dictionary(const string& filename);
unordered_map<string, double> form_frequencies(const string& filename);

void printTableAccurates(Solver* solver, const string& filename10, const string& filename100, const string& filename500);

int main() {
    string dict_filename = "./input_data/dictionary.txt";
    string freq_filename = "./input_data/freqdictionary.txt";

    unordered_set<string> test_dictionary = form_dictionary(dict_filename);
    unordered_map<string, double> freqs = form_frequencies(freq_filename);

    LevenshteinSolver ls(test_dictionary);
    DamerauLevenshteinSolver dls(test_dictionary);
    TrigramSolver ts(test_dictionary);
    NoisyChannelSolver ncs(test_dictionary, freqs, dls);

    printTableAccurates(&ls, "./input_data/test10.txt", "./input_data/test100.txt", "./input_data/test500.txt");
    printTableAccurates(&dls, "./input_data/test10.txt", "./input_data/test100.txt", "./input_data/test500.txt");
    printTableAccurates(&ts, "./input_data/test10.txt", "./input_data/test100.txt", "./input_data/test500.txt");
    printTableAccurates(&ncs, "./input_data/test10.txt", "./input_data/test100.txt", "./input_data/test500.txt");
}

vector<pair<string, string>> read_typos(const string& filename) {
    ifstream fin(filename);
    vector<pair<string, string>> vec;
    string line;
    while (getline(fin, line)) {
        int i = 0;
        string s1, s2;
        while (line[i] != ' ' && line[i] != '\t') s1.push_back(line[i++]);
        while (line[i] == ' ' || line[i] == '\t') i++;
        for (i; i < line.size(); i++) s2.push_back(line[i]);
        vec.push_back({s1, s2});
    }
    fin.close();
    return vec;
}

unordered_set<string> form_dictionary(const string& filename) {
    ifstream fin(filename);
    unordered_set<string> dictionary;
    string word;

    while (getline(fin, word)) dictionary.insert(word);

    fin.close();
    return dictionary;
}

unordered_map<string, double> form_frequencies(const string& filename) {
    ifstream fin(filename);
    unordered_map<string, double> frequencies;
    string line;
    while (getline(fin, line)) {
        string word;
        string number;
        int freq;
        int i = 0;
        while (line[i] != '\t') word.push_back(line[i++]);
        i++;
        for (i; i < line.size(); i++) number.push_back(line[i]);
        freq = stod(number);
        frequencies[word] = freq;
    }

    fin.close();
    return frequencies;
}

double Accurate1(const string& filename, Solver* solver) {
    auto typos = read_typos(filename);
    int acc1 = 0;
    for (auto typo : typos) {
        Result res = solver->solve(typo.first);
        if (res.candidates.size() != 0 && res.candidates[0] == typo.second) acc1++;
    }

    return (double) acc1 / (double) typos.size() * 100;
}

double Accurate3(const string& filename, Solver* solver) {
    auto typos = read_typos(filename);
    int acc3 = 0;
    for (const auto& typo : typos) {
        Result res = solver->solve(typo.first);
        bool correct = false;
        for (int i = 0; i < min(3, (int)res.candidates.size()); i++) {
            if (res.candidates[i] == typo.second) {
                correct = true;
                break;
            }
        }
        if (correct) acc3++;
    }

    return (double) acc3 / (double) typos.size() * 100;
}

void printTableAccurates(Solver* solver, const string& filename10, const string& filename100, const string& filename500) {
    cout << solver->getAlgorithmName();
    cout << "\tМетрика\t\t10\t100\t500\n"
    << "\tAccuracy@1\t" << Accurate1(filename10, solver) << "%" << "\t" << Accurate1(filename100, solver) << "%" << "\t" << Accurate1(filename500, solver) << "%" << "\n"
    << "\tAccuracy@3\t" << Accurate3(filename10, solver) << "%" << "\t" << Accurate3(filename100, solver) << "%" << "\t" << Accurate3(filename500, solver) << "%" << "\n"
    << "\tСр. время работы\t" << GetTime(filename10, solver) << " мс" << "\t" << GetTime(filename100, solver) << " мс" << "\t" << GetTime(filename500, solver) << " мс" << "\n\n\n";
}

double GetTime(const string& filename, Solver* solver) {
    auto typos = read_typos(filename);

    auto start = chrono::high_resolution_clock::now();
    for (const auto& typo : typos) solver->solve(typo.first);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> duration = end - start;

    return duration.count() / (double)typos.size();
}