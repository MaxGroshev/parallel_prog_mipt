#include "equations.hpp"
#include "utils.hpp"
#include "eq_solver.hpp"

// Вывод результатов в файл
// void output_statistics(int size, double exec_time) {
//     std::ofstream statistics{"statistics.csv", std::ios::app};
//     statistics << size << "," << exec_time << "," << t_step << "\n";
// }

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    equation_t<types_of_eq::transf_eq> my_eq{};
    eq_solver my_solver{my_eq};

    double start_time = MPI_Wtime();

    if (size > 1) {
        my_solver.parallel_solution(rank, size);
    }
    else {
        my_solver.serial_solution();
    }

    double end_time = MPI_Wtime();

    if (rank == ROOT) {
        dumper dump{};
        dump.output_solution(my_solver.get_t_knots_num(), my_solver.get_x_knots_num(), 
                             my_solver.get_grid(), my_eq.x_step, my_eq.t_step);
        // output_statistics(size, end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}