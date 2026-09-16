#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <iomanip>
#include <matplot/matplot.h>
#include <windows.h>

using namespace matplot;

int main() {
    SetConsoleOutputCP(65001);

    const size_t student_number = 1;
    const size_t numbers_amount = student_number * 100;
    std::vector<double> xi = {5, 7, 17, 19, 21, 25, 55};
    std::vector<double> pi = {0.01, 0.05, 0.30, 0.30, 0.30, 0.02, 0.02};
    std::vector<double> limit_margin(pi.size());
    limit_margin[0] = pi[0];
    for (size_t i = 1; i < pi.size(); ++i) {
        limit_margin[i] = limit_margin[i - 1] + pi[i];
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<double> random_numbers(numbers_amount);
    for (size_t i = 0; i < numbers_amount; ++i) {
        double r = dist(gen);
        size_t index = 0;
        while (index < limit_margin.size() && r >= limit_margin[index]) {
            ++index;
        }
        random_numbers[i] = xi[index];
    }
    std::cout << "ЗГЕНЕРОВАНА ДИСКРЕТНА ПОСЛІДОВНІСТЬ (" << numbers_amount << " чисел)\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(0);
    for (size_t i = 0; i < numbers_amount; ++i) {
        std::cout << std::setw(6) << random_numbers[i];
        if ((i + 1) % 10 == 0) std::cout << "\n";
    }
    if (numbers_amount % 10 != 0) std::cout << "\n";
    std::cout << "---------------------------------------------------------------\n\n";
    double mean = std::accumulate(random_numbers.begin(), random_numbers.end(), 0.0) / numbers_amount;
    std::cout << "Середнє значення: " << std::setprecision(4) << mean << std::endl;
    double variance = 0.0;
    for (size_t i = 0; i < numbers_amount; ++i) {
        variance += (random_numbers[i] - mean) * (random_numbers[i] - mean);
    }
    variance /= numbers_amount;
    std::cout << "Дисперсія: " << variance << std::endl << std::endl;
    std::vector<size_t> frequency_table(xi.size(), 0);
    for (double value : random_numbers) {
        for (size_t j = 0; j < xi.size(); ++j) {
            if (value == xi[j]) {
                frequency_table[j]++;
                break;
            }
        }
    }
    std::vector<double> relative_frequency(xi.size());
    for (size_t i = 0; i < xi.size(); ++i) {
        relative_frequency[i] = static_cast<double>(frequency_table[i]) / numbers_amount;
    }
    std::cout << "ЧАСТОТНА ТАБЛИЦЯ\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << std::left
              << std::setw(8)  << "xi"
              << std::setw(13) << "Частота "
              << std::setw(21) << "Відносна частота "
              << std::setw(16) << "Теоретична pi" << "\n";
    std::cout << "---------------------------------------------------------------\n";
    size_t frequency_sum = 0;
    double relative_sum = 0.0;
    for (size_t i = 0; i < xi.size(); ++i) {
        frequency_sum += frequency_table[i];
        relative_sum += relative_frequency[i];
        std::cout << std::left
                  << std::setw(8)  << xi[i]
                  << std::setw(12) << frequency_table[i]
                  << std::fixed << std::setprecision(4)
                  << std::setw(20) << relative_frequency[i]
                  << std::setw(15) << pi[i] << "\n";
    }
    std::cout << "---------------------------------------------------------------\n";
    std::cout << std::left
              << std::setw(10)  << "Разом: "
              << std::setw(12) << frequency_sum
              << std::setw(20) << relative_sum << "\n";
    std::cout << "---------------------------------------------------------------\n\n";

    //ГІСТОГРАМА
    auto b = bar(xi, relative_frequency);
    b->face_color({0.85f, 0.9f, 0.6f});
    b->edge_color("black");

    hold(on);

    // Експериментальна лінія (синя з кружечками)
    auto exp_line = plot(xi, relative_frequency, "b-o");
    exp_line->line_width(2);
    exp_line->marker_size(5);

    // Теоретична лінія (червона пунктирна)
    auto theor_line = plot(xi, pi, "r--");
    theor_line->line_width(2);

    hold(off);

    title("Перевірка закону розподілу дискретної ВВ (метод гістограм)");
    xlabel("Значення xi");
    ylabel("Ймовірність / відносна частота");
    ::matplot::legend({"Гістограма vj/n", "Експериментальна vj/n", "Теоретична pi"})
        ->location(legend::general_alignment::topright);
    grid(on);
    show();

    return 0;
}