#include "LevenshteinSolver.h"
#include "DamerauLevenshteinSolver.h"
#include "TrigramSolver.h"
#include "NoisyChannelSolver.h"
#include <iomanip>
#include <chrono>

struct Metric {
    double Acc1;
    double Acc3;
    double Time;
};

vector<pair<string, string>> read_typos(const string& filename);

Metric GetMetric(const string& filename, Solver* solver);

unordered_set<string> form_dictionary(const string& filename);
unordered_map<string, double> form_frequencies(const string& filename);

void printTableAccurates(Solver* solver, const string& filename10, const string& filename100, const string& filename500, const string& filename5000);
void printSolveTable(Solver* solver, const string& filename);

int main() {
    string dict_filename = "./input_data/dictionary.txt";
    string freq_filename = "./input_data/freqdictionary.txt";

    unordered_set<string> test_dictionary = form_dictionary(dict_filename);
    unordered_map<string, double> freqs = form_frequencies(freq_filename);

    LevenshteinSolver ls(test_dictionary);
    DamerauLevenshteinSolver dls(test_dictionary);
    TrigramSolver ts(test_dictionary);
    NoisyChannelSolver ncs(test_dictionary, freqs, dls);

    printTableAccurates(&ls, "./input_data/test10.txt", "./input_data/test100.txt", "./input_data/test500.txt", "./input_data/test5000.txt");
    printTableAccurates(&dls, "./input_data/test10.txt", "./input_data/test100.txt", "./input_data/test500.txt", "./input_data/test5000.txt");
    printTableAccurates(&ts, "./input_data/test10.txt", "./input_data/test100.txt", "./input_data/test500.txt", "./input_data/test5000.txt");
    printTableAccurates(&ncs, "./input_data/test10.txt", "./input_data/test100.txt", "./input_data/test500.txt", "./input_data/test5000.txt");

    printSolveTable(&ls, "./input_data/test5000many.txt");
    printSolveTable(&dls, "./input_data/test5000many.txt");
    printSolveTable(&ts, "./input_data/test5000many.txt");
    printSolveTable(&ncs, "./input_data/test5000many.txt");

//    Result res = ls.solve("разраоткба");
//    ls.printResult(res);
}

vector<pair<string, string>> read_typos(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) return {};

    vector<pair<string, string>> vec;
    string s1, s2;

    while (fin >> s1 >> s2) vec.emplace_back(std::move(s1), std::move(s2));

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
        double freq;
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

void printTableAccurates(Solver* solver, const string& filename10, const string& filename100, const string& filename500, const string& filename5000) {
    Metric f10 = GetMetric(filename10, solver);
    Metric f100 = GetMetric(filename100, solver);
    Metric f500 = GetMetric(filename500, solver);
    Metric f5000 = GetMetric(filename5000, solver);

    cout << solver->getAlgorithmName();
    cout << "\tМетрика\t\t10\t100\t500\t5000\n"

    << "\tAccuracy@1\t" << f10.Acc1 << "%" << "\t" << f100.Acc1 << "%" << "\t" << f500.Acc1 << "%" << "\t" << f5000.Acc1 << "%" << "\n"
    << "\tAccuracy@3\t" << f10.Acc3 << "%" << "\t" << f100.Acc3 << "%" << "\t" << f500.Acc3 << "%" << "\t" << f5000.Acc3 << "%" << "\n"
    << "\tСр. время работы\t" << f10.Time << " мс" << "\t" << f100.Time << " мс" << "\t" << f500.Time << " мс" << "\t" << f5000.Time << "мс" << "\n\n\n";
}

Metric GetMetric(const string& filename, Solver* solver) {
    auto typos = read_typos(filename);
    int acc1 = 0, acc3 = 0;

    auto start = chrono::high_resolution_clock::now();
    for (const auto& typo : typos) {
        Result res = solver->solve(typo.first);
        if (res.candidates.size() != 0 && res.candidates[0] == typo.second) acc1++;
        bool correct = false;
        for (int i = 0; i < min(3, (int)res.candidates.size()) && !correct; i++) {
            if (res.candidates[i] == typo.second) {
                correct = true;
            }
        }
        if (correct) acc3++;
    }
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> duration = end - start;

    return {acc1 / (double)typos.size() * 100, acc3 / (double)typos.size() * 100, duration.count() / (double)typos.size()};
}

void printSolveTable(Solver* solver, const string& filename) {
    Metric m = GetMetric(filename, solver);
    cout << "\tМетрика\t\t5000\n"

            << "\tAccuracy@1\t" << m.Acc1 << "%" << "\n"
            << "\tAccuracy@3\t" << m.Acc3 << "%" << "\n"
            << "\tСр. время работы\t" << m.Time << " мс" << "\n\n\n";
}