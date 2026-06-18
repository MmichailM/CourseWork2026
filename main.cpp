#include "LevenshteinSolver.h"
#include "DamerauLevenshteinSolver.h"
#include "TrigramSolver.h"
#include "NoisyChannelSolver.h"

int main() {
    const std::unordered_set<std::string> test_dictionary = {
            // Базовые слова для проверки Левенштейна
            "алгоритм",
            "программа",
            "компьютер",
            "разработка",
            "курсовая",
            "телефон",
            "титафон",
            "корова",
            "корона",

            // Пара слов для проверки неоднозначности (когда расстояние одинаковое)
            "кот",
            "кит",
            "мат",
            "рот",

            // Слова для жесткой проверки Дамерау-Левенштейна (перестановки)
            "бурмалда",
            "бурмлажа",
            "шляпа",
            "салат",

            // Длинные и специфические слова (для триграммного поиска)
            "интерполяция",
            "дифференциал",
            "аппроксимация",
            "оптимизация",
            "база данных",

            // Короткие слова (граничные случаи для N-грамм)
            "код",
            "ток",
            "орк",
            "рок"
    };

    unordered_map<string, int> freqs = { {"корова", 5000}, {"корона", 10}, {"телефон", 500}, {"титафон", 500} };
    DamerauLevenshteinSolver lev(test_dictionary);
    NoisyChannelSolver ls(test_dictionary, freqs, lev);
    string word;
    getline(cin, word);
    Result res = ls.solve(word);
    for (string w : res.candidates) cout << w << ' ';
    cout << "\nРасстояние: " << res.score;
}