#pragma once

// Необходимо сделать программу которая параллельно вычислит определенный  интеграл sin(1/x) 
// в пределе от некого положительного действительного числа до некоторого бОльшего 
// положительного действительного числа. 
// 1) Необходимо сбалансировать решение - время выполнения 
// задачи на всех нитях должно быть одинаковым. 
// 2) Шаги интегрирования для функции sin(1/x) должны быть динамическими 

#include <iostream>
#include <cmath>
#include <atomic>
#include <thread>
#include <functional>
#include "debug_utils.hpp"
#include <tuple>
#include <stack>
#include <vector>
#include <semaphore>
#include <numeric>
#include <future>
#include <algorithm>
#include <utility>
#include <cstddef>
#include <functional>
#include <ranges>


namespace integration
{

using segment = std::tuple<double,  // a
    double,  // f_a
    double,  // b
    double,  // f_b
    double>; // I_ab

class integrator final
{
public:

    integrator(std::function<double(double)> f, double epsilon_, size_t n_threads_): 
            integr_func(f), epsilon(epsilon_), n_threads(n_threads_)
    {
    }

    double integrate(double a, double b) const;

private:

    std::function<double(double)> integr_func;
    double epsilon;
    size_t n_threads;

    bool check_approx(double I_ab, double I_acb) const
    {
        return std::abs(I_ab - I_acb) > epsilon * std::abs(I_acb);
    }
    double integration_job() const;
    double integrate_segment(double a, double f_a, double b, double f_b, double I_ab) const;

};
}