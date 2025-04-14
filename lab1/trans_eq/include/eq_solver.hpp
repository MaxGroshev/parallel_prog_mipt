#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

//-----------------------------------------------------------------------------

static const int ROOT = 0;

template<typename T>
class eq_solver {
    private:
        T eq;
        size_t x_knots_num = 0;
        size_t t_knots_num = 0;
        std::vector<std::vector<double>> grid;

        void fill_layer(int knot_t, int knot_x) {
            double CFL = eq.alpha * eq.t_step / eq.x_step;
        
            grid[knot_t + 1][knot_x] = grid[knot_t][knot_x] -
                                    CFL * (grid[knot_t][knot_x] - grid[knot_t][knot_x - 1]) +
                                    eq.t_step * eq.func(knot_t * eq.t_step, knot_x * eq.x_step);
        }

    public:
        eq_solver(T& eq_) : eq(eq_), x_knots_num(eq_.x_max / eq_.x_step),
                            t_knots_num(eq_.t_max / eq_.t_step), grid(t_knots_num, std::vector<double>(x_knots_num)) {
            // Инициализация по времени (t=0)
            for (int m = 0; m < x_knots_num; ++m) {
                grid[0][m] = eq.fi(m * eq.x_step);
            }
        
            // Граничное условие (x=0)
            for (int k = 0; k < t_knots_num; ++k) {
                grid[k][0] = eq.ksi(k * eq.t_step);
            }
        }

        std::vector<std::vector<double>> get_grid() const {return grid;};
        size_t get_t_knots_num() const {return t_knots_num;};
        size_t get_x_knots_num() const {return x_knots_num;};
        void parallel_solution(int rank, int size) {
            int part = x_knots_num / size;
            int shift = x_knots_num % size;
            int num_knots = (rank == ROOT) ? part + shift : part;
            int x_start = (rank == ROOT)   ? 0 : part * rank + shift;
            int x_end = x_start + num_knots;

            for (int knot_t = 0; knot_t < t_knots_num - 1; ++knot_t) {
                for (int knot_x = x_start; knot_x < x_end; ++knot_x) {
                    fill_layer(knot_t, knot_x);
                }

                if (rank > 0) {
                    MPI_Send(&grid[knot_t + 1][x_start], 1, MPI_DOUBLE, rank - 1, 0,
                            MPI_COMM_WORLD);
                    MPI_Recv(&grid[knot_t + 1][x_start - 1], 1, MPI_DOUBLE, rank - 1,
                            0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }

                if (rank < size - 1) {
                    MPI_Send(&grid[knot_t + 1][x_end - 1], 1, MPI_DOUBLE, rank + 1, 0,
                            MPI_COMM_WORLD);
                    MPI_Recv(&grid[knot_t + 1][x_end], 1, MPI_DOUBLE, rank + 1, 0,
                            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                
                MPI_Barrier(MPI_COMM_WORLD);
                if (rank == ROOT) {
                    for (int r = 1; r < size; ++r) {
                        int first = part * r + shift;
                        int last = first + ((r == size - 1) ? part : part);
    
                        MPI_Recv(&grid[knot_t + 1][first], last - first, MPI_DOUBLE, r, 0,
                                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    }
                }
                else {
                    MPI_Send(&grid[knot_t + 1][x_start], num_knots, MPI_DOUBLE, ROOT, 0, MPI_COMM_WORLD);
                }
            }
        }

        void serial_solution() {
            for (int knot_t = 0; knot_t < t_knots_num - 1; ++knot_t) {
                for (int knot_x = 1; knot_x < x_knots_num; ++knot_x) {
                    fill_layer(knot_t, knot_x);
                }
            }
        }
};