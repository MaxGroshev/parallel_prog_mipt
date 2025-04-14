#include "latency.hpp"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    test_helper tester {};
    MPI_Comm_rank(MPI_COMM_WORLD, &tester.n_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tester.size);

    tester.run();

    MPI_Finalize();

    return 0;
}