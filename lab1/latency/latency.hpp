#include <iostream>
#include <cmath>
#include <mpi.h>
#include "debug_utils.hpp"

class test_helper {
    public:
        int n_rank = 0;
        int size   = 0;
        size_t data_size = 0;
        const int warmup_iters = 10;
        const int measure_iters = 100;

        double avg_time = 0;

        const std::vector<int> suite = {      
            16 * 1024,      
            64 * 1024,      
            256 * 1024,     
            1 * 1024 * 1024,
            10 * 1024 * 1024
        };

    public: 
        test_helper(int n_rank_, int size_, size_t data_size_) : n_rank(n_rank_), size(size_), data_size(data_size_) {};
        test_helper() {};
        void run() {
            for (int sz : suite) {
                data_size = sz;
                measure_communication();
            }
        }
    private:
        void measure_communication() {
            std::vector<char> send_buf(data_size, '0');
            std::vector<char> recv_buf(data_size);
        
            MPI_Barrier(MPI_COMM_WORLD);
        
            if (n_rank == 0) {
                for (int i = 0; i < warmup_iters; ++i) {
                    MPI_Send(send_buf.data(), data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(recv_buf.data(), data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                double total_time = 0.0;
                for (int i = 0; i < measure_iters; ++i) {
                    auto start = MPI_Wtime();
                    MPI_Send(send_buf.data(), data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(recv_buf.data(), data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    auto end = MPI_Wtime();
                    total_time += (end - start);
                }
        
                avg_time = total_time / measure_iters;
                dump_res();
            }
        
            else if (n_rank == 1) {
                for (int i = 0; i < warmup_iters + measure_iters; ++i) {
                    MPI_Recv(recv_buf.data(), data_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Send(recv_buf.data(), data_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
                }
            }
        }

        void dump_res() {
            std::cout << "Message size: " << data_size / 1024 << " KB"
                        << ", Avg time: " << avg_time * 1e6 << " us" << std::endl;
        }
};