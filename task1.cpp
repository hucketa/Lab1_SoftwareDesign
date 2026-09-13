#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <iomanip>
#include <matplot/matplot.h>
#include <windows.h>

using namespace matplot;

int main() {
    // ===== Налаштування консолі =====
    SetConsoleOutputCP(65001);

    // ===== Параметри =====
    const size_t student_number   = 1;
    const size_t numbers_amount   = student_number * 100;
    const size_t interval_amount  = 20;
    const double interval_start   = 0.0;
    const double interval_end     = 1.0;

    // ===== Генерація рівномірних чисел =====
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(interval_start, interval_end);

    std::vector<double> random_numbers(numbers_amount);
    for (size_t i = 0; i < numbers_amount; ++i) {
        random_numbers[i] = dist(gen);
    }

    // ===== Вивід згенерованої послідовності =====
    std::cout << "ЗГЕНЕРОВАНА ПОСЛІДОВНІСТЬ (" << numbers_amount << " чисел)\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(4);

    for (size_t i = 0; i < numbers_amount; ++i) {
        std::cout << std::setw(8) << random_numbers[i];
        if ((i + 1) % 10 == 0) {
            std::cout << "\n";
        }
    }
    if (numbers_amount % 10 != 0) {
        std::cout << "\n";
    }
    std::cout << "---------------------------------------------------------------\n\n";

    // ===== Математичне сподівання =====
    double mean = std::accumulate(random_numbers.begin(), random_numbers.end(), 0.0) / numbers_amount;
    std::cout << "Середнє значення: " << mean << std::endl;

    // ===== Дисперсія =====
    double variance = 0.0;
    for (double x : random_numbers) {
        variance += (x - mean) * (x - mean);
    }
    variance /= numbers_amount;
    std::cout << "Дисперсія: " << variance << std::endl;

    // ===== Частотна таблиця =====
    double delta = (interval_end - interval_start) / interval_amount;
    std::vector<size_t> frequency_table(interval_amount, 0);

    for (double x : random_numbers) {
        size_t index = static_cast<size_t>((x - interval_start) / delta);
        if (index >= interval_amount) {
            index = interval_amount - 1;
        }
        frequency_table[index]++;
    }

    std::vector<double> relative_frequency(interval_amount);
    for (size_t i = 0; i < interval_amount; ++i) {
        relative_frequency[i] = static_cast<double>(frequency_table[i]) / numbers_amount;
    }

    // ===== Вивід частотної таблиці =====
    std::cout << "\nЧАСТОТНА ТАБЛИЦЯ\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << std::left
              << std::setw(6)  << "№"
              << std::setw(25) << "Інтервал"
              << std::setw(15) << "Частота "
              << std::setw(20) << "\t\tВідносна частота" << "\n";
    std::cout << "---------------------------------------------------------------\n";

    size_t frequency_sum = 0;
    double relative_sum = 0.0;

    for (size_t i = 0; i < interval_amount; ++i) {
        double left  = interval_start + i * delta;
        double right = interval_start + (i + 1) * delta;

        frequency_sum += frequency_table[i];
        relative_sum  += relative_frequency[i];

        std::cout << std::left << std::setw(6) << (i + 1);
        std::cout << std::fixed << std::setprecision(3)
                  << "[" << left << "; " << right << ")";
        std::cout << std::setw(7) << " "
                  << std::setw(15) << frequency_table[i]
                  << std::setprecision(4) << relative_frequency[i] << "\n";
    }

    std::cout << "---------------------------------------------------------------\n";
    std::cout << std::left
              << std::setw(31) << "Разом"
              << std::setw(12) << frequency_sum
              << std::setprecision(4) << relative_sum << "\n";
    std::cout << "---------------------------------------------------------------\n\n";
    /*
                // ===== Гістограма =====
        std::vector<double> interval_midpoints(interval_amount);
        for (size_t i = 0; i < interval_amount; ++i) {
            interval_midpoints[i] = interval_start + (i + 0.5) * delta;
        }

        double theoretical_pj = 1.0 / interval_amount;

        auto bars = bar(interval_midpoints, relative_frequency);
        bars->face_color({0.85f, 0.9f, 0.6f});
        bars->edge_color("black");

        hold(on);

        auto exp_line = plot(interval_midpoints, relative_frequency, "b-o");
        exp_line->line_width(2);
        exp_line->marker_size(4);

        auto theor_line = plot({interval_start, interval_end}, {theoretical_pj, theoretical_pj}, "r--");
        theor_line->line_width(2);

        hold(off);

        title("Перевірка гіпотези про рівномірний розподіл (метод гістограм)");
        xlabel("Значення X");
        ylabel("Відносна частота");
        
        ::matplot::legend({
            "Гістограма vj/n",
            "Експериментальна vj/n",
            "Теоретична pj = 1/L"
        })->location(legend::general_alignment::bottomright);

        ylim({0.0, 0.12});
        grid(on);
        show();
    */

    // ===== Гістограма за формулою з методички =====
    // Центри інтервалів
    std::vector<double> interval_midpoints(interval_amount);

    for (size_t i = 0; i < interval_amount; ++i) {
        interval_midpoints[i] =
            interval_start + (i + 0.5) * delta;
    }

    // Висота гістограми за методичкою:
    // h_j = (ν_j / n) / Δ_j
    std::vector<double> histogram_height(interval_amount);

    for (size_t i = 0; i < interval_amount; ++i) {
        histogram_height[i] =
            relative_frequency[i] / delta;
    }

    // Теоретична щільність рівномірного розподілу на [0;1]
    // f(x) = 1 / (b - a)
    double theoretical_density =
        1.0 / (interval_end - interval_start);

    // Побудова гістограми
    auto bars = bar(interval_midpoints, histogram_height);

    bars->face_color({0.85f, 0.9f, 0.6f});
    bars->edge_color("black");

    hold(on);

    // Теоретична щільність
    auto theor_line = plot(
        {interval_start, interval_end},
        {theoretical_density, theoretical_density},
        "r--"
    );

    theor_line->line_width(2);

    hold(off);

    title("Перевірка гіпотези про рівномірний розподіл");
    xlabel("Значення X");
    ylabel("Щільність розподілу");

    ::matplot::legend({
        "Гістограма",
        "Теоретична щільність"
    })->location(legend::general_alignment::bottomright);

    grid(on);
    show();

    return 0;
}