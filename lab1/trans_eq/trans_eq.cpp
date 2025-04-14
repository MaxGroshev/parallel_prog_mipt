#include "equations.hpp"
#include "utils.hpp"
#include "eq_solver.hpp"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    equation_t<types_of_eq::transf_eq> my_eq{};
    eq_solver my_solver{my_eq};

    double start = MPI_Wtime();
    if (size > 1) {
        my_solver.parallel_solution(rank, size);
    }
    else {
        my_solver.serial_solution();
    }

    double end = MPI_Wtime();

    if (rank == 0) {
        dumper dump{};
        dump.output_solution(my_solver.get_t_knots_num(), my_solver.get_x_knots_num(), 
                             my_solver.get_grid(), my_eq.x_step, my_eq.t_step);
        dump.output_statistics(size, end - start, my_eq.x_step, my_eq.t_step);
    }

    MPI_Finalize();

    return 0;
}