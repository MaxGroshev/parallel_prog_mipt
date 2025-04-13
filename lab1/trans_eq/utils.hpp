#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

class dumper {
    public: 
        std::filesystem::path dump_file = std::filesystem::current_path();
        dumper() {
            dump_file /= "../lab1/trans_eq/process_data/results.csv"; 
        }

        void output_solution(int num_knots_t, int num_knots_x, const std::vector<std::vector<double>> &grid,
                             double x_step, double t_step) {
            std::ofstream results{dump_file, std::ofstream::out | std::ofstream::trunc};
            results << "x,t,u\n";

            for (int knot_t = 0; knot_t < num_knots_t; ++knot_t) {
                double t = knot_t * t_step;

                for (int knot_x = 0; knot_x < num_knots_x; ++knot_x) {
                    double x = knot_x * x_step;

                    results << x << "," << t << "," << u[knot_t][knot_x] << "\n";
                }
            }
    }
};