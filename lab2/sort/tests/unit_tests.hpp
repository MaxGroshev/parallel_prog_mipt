#pragma once

#include <vector>
#include <gtest/gtest.h>
#include <filesystem>
#include <random>
#include <filesystem>
#include <limits>
#include "sort.hpp"
#include "debug_utils.hpp"
#include <sstream>

//-----------------------------------------------------------------------------------------

class sorting : public ::testing::Test {
    protected:
        void SetUp() {}
    public:
        std::filesystem::path dump_stat_file = std::filesystem::current_path() / "../lab2/sort/comp/res.txt"; 
        void init_data(std::vector<int>& data, size_t n_elems) {
            std::random_device rd;
            std::default_random_engine reng(rd());
            std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), 
                                                    std::numeric_limits<int>::max());
            std::generate(data.begin(), data.end(), [&dist, &reng] { return dist(reng); });
        }

        };

//----------------c-------------------------------------------------------------------------

TEST_F(sorting, p_sort) {
    std::ofstream statistics{dump_stat_file};

    size_t n_elems = 2;
    for (int i = 1; i < 25; i++) {
        std::vector<int> data(n_elems);
        init_data(data, n_elems);
        auto paral_data = data;
        auto pool_data = data;


        auto start_time = time_control::chrono_cur_time ();
        std_like::quicksort(data.begin(), data.end() - 1);
        ASSERT_TRUE(std::is_sorted(data.begin(), data.end()));
        auto end_time = time_control::chrono_cur_time ();
        auto time = std::chrono::duration<double>(end_time - start_time).count();

        // start_time = time_control::chrono_cur_time ();
        // std_like::paral_quicksort(paral_data.begin(), paral_data.end() - 1);
        // ASSERT_TRUE(std::is_sorted(paral_data.begin(), paral_data.end()));
        // end_time = time_control::chrono_cur_time ();
        // auto paral_time = std::chrono::duration<double>(end_time - start_time).count();
        // std::clog << "Total run time: " << (end_time - start_time) << std::endl;

        start_time = time_control::chrono_cur_time ();
        std_like::pool_quicksort(pool_data.begin(), pool_data.end() - 1);
        ASSERT_TRUE(std::is_sorted(pool_data.begin(), pool_data.end()));
        end_time = time_control::chrono_cur_time ();
        auto pool_time = std::chrono::duration<double>(end_time - start_time).count();
        std::clog << n_elems << std::endl;

        statistics << n_elems << " " << pool_time << "  " << time << "\n";

        n_elems *= 2;
    }
}

