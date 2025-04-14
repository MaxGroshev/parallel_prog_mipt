#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include "debug_utils.hpp"

class dumper {
    public: 
        std::filesystem::path dump_res_file = std::filesystem::current_path();
        std::filesystem::path dump_stat_file = std::filesystem::current_path();
        dumper() {
            dump_res_file /= "../lab1/trans_eq/process_data/results.csv"; 
            dump_stat_file /= "../lab1/trans_eq/process_data/stat.csv"; 
        }

        void output_solution(int num_knots_t, int num_knots_x, const std::vector<std::vector<double>> &grid,
                             double x_step, double t_step) {
            std::ofstream results{dump_res_file, std::ofstream::out | std::ofstream::trunc};
            results << "x,t,u\n";

            for (int knot_t = 0; knot_t < num_knots_t; ++knot_t) {
                double t = knot_t * t_step;

                for (int knot_x = 0; knot_x < num_knots_x; ++knot_x) {
                    double x = knot_x * x_step;

                    results << x << "," << t << "," << grid[knot_t][knot_x] << "\n";
                }
            }
        
        }
        void output_statistics(int size, double exec_time, double x_step, double t_step) {
            std::ofstream statistics{dump_stat_file, std::ios::app};
            statistics << size << "," << exec_time << "," << t_step << "\n";

            std::cout << GREEN_C << "Calculation time: " << RESET_C << exec_time 
            << GREEN_C  << "\nNumber of workers: " << RESET_C  <<  size
            << "\n---------------------------------------------------------\n"
            << GREEN_C <<"x step: " << RESET_C << x_step
            << GREEN_C <<"\nt step: " << RESET_C << t_step << std::endl;
        }
};