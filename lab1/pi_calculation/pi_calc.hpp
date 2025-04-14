#include <iostream>
#include <cmath>
#include <mpi.h>
#include "debug_utils.hpp"

//-----------------------------------------------------------------------------

template<typename T>
class pi_calc {
    public:
        pi_calc() : actual_pi(M_PI) {};
        
        T actual_pi;
        T res;
        T calculate_pi(int start, int end, int total_steps) const {
            T sum = 0.0;
            T step = 1.0 / total_steps;
        
            for (int i = start; i < end; ++i) {
                T x = (i + 0.5) * step;
                sum += 4.0 / (1.0 + x * x); // \pi = \int_0^1 \frac{4}{1 + x^2} dx, получается
                                            // из arctg и его интегрального представления
            }
        
            return sum * step;
        }

        T get_error() const { return fabs(res - M_PI); };
};

//-----------------------------------------------------------------------------

template<typename T>
void run_prog(int argc, char**& argv) {
    pi_calc<T> helper {};
   
    MPI_Init(&argc, &argv);

    int work_cnt, work_id;
    MPI_Comm_size(MPI_COMM_WORLD, &work_cnt);
    MPI_Comm_rank(MPI_COMM_WORLD, &work_id);

    const int total_steps = 10000; 
    int steps_per_process = total_steps / work_cnt;
    int start = work_id * steps_per_process;
    int end = (work_id == work_cnt - 1) ? total_steps : start + steps_per_process;

    auto partial_sum = helper.calculate_pi(start, end, total_steps);

    MPI_Reduce(&partial_sum, &helper.res, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD); 

    MPI_Finalize();

    if (work_id == 0) {
        std::cout << GREEN_C << "Calculated Pi: " << RESET_C << helper.res 
        << GREEN_C  << "\nActual Pi: " << RESET_C  <<  helper.actual_pi
        << YELLOW_C <<"\nError: " << RESET_C << helper.get_error() << std::endl;
    }
    return;
}