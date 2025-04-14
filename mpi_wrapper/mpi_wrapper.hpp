#pragma once 

#include <iostream>
#include <vector>
#include <mpi.h>

//-----------------------------------------------------------------------------

namespace mpi {
    class naive_wrapper {
        public: 
            naive_wrapper(int argc, char**& argv) {
                MPI_Init(&argc, &argv);
            }
            ~naive_wrapper() {
                MPI_Finalize();
            }
    }
}