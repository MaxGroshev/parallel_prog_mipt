#include <iostream>
#include <cmath>
#include <chrono>
#include <tuple>
#include <optional>
#include <print>

#include "integrate.hpp"


int main(int argc, char *argv[])
{
    auto a = 0.0005;
    auto b = 10.0;
    size_t n_threads = 8;


    integration::integrator integrator{[](double x){ return std::sin(1 / x); }, 1e-8, n_threads};

    auto start = std::chrono::high_resolution_clock::now();
    double integr_val = integrator.integrate(a, b);
    auto finish = std::chrono::high_resolution_clock::now();

    using ms = std::chrono::milliseconds;
    std::cout << integr_val << std::endl;
    // std::println("Computation on {} threads took {} ms\n    integr_val = {}",
    //              n_threads, std::chrono::duration_cast<ms>(finish - start).count(), integr_val);

    return 0;
}