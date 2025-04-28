#include <iostream>
#include <cmath>
#include <thread>
#include "debug_utils.hpp"
#include <boost/asio.hpp>

//-----------------------------------------------------------------------------

namespace std_like {

template<typename it>
void insertion_sort(it beg, it fin) {
	for (it i = beg + 1; fin - i >= 0; ++i) {
		auto key = *i;
		auto j = i - 1;
		while (j >= beg && *j > key) {
			*(j + 1) = *j;
			j--;
		}
		*(j + 1) = key;
	}
}

template<typename it>
void do_pool_quicksort(it beg, it fin, boost::asio::thread_pool& pool) {

	if (beg < fin) {

		if (fin - beg < 64) {
			insertion_sort(beg, fin);
			return;
		}

		auto pivot = beg; // select a pivot
		auto i = beg;
		auto j = fin;
		
		while (i < j) {
			while (*i <= *pivot && i <= fin) {
				i++;   
			}
			
			while (*j > *pivot && j >= beg) {
				j--;
			}
			
			if (i < j) {
				std::iter_swap(i, j);
			}
		}
		
		std::iter_swap(pivot, j);

		boost::asio::post(pool, [&pool, beg, j]() { do_pool_quicksort(beg, j - 1, pool); });
		boost::asio::post(pool, [&pool, j, fin]() { do_pool_quicksort(j + 1, fin, pool); });
	}
}

template<typename it>
void pool_quicksort(it beg, it fin) {

	boost::asio::thread_pool pool(8);
	boost::asio::post(pool, [&pool, beg, fin]() { do_pool_quicksort(beg, fin, pool); });
	pool.join();
}


//-----------------------------------------------------------------------------

template<typename it>
void paral_quicksort(it beg, it fin) {

	if (beg < fin) {
		auto pivot = beg; // select a pivot
		auto i = beg;
		auto j = fin;
        
		while (i < j) {
			while (*i <= *pivot && i <= fin) {
                i++;   
            }
            
			while (*j > *pivot && j >= beg) {
                j--;
            }
            
			if (i < j) {
                std::iter_swap(i, j);
			}
		}
        
		std::iter_swap(pivot, j);

		std::thread t1 (paral_quicksort<it>, beg, j - 1);
		std::thread t2 (paral_quicksort<it>, j + 1, fin);
        t1.join();
        t2.join();
	}
}

//-----------------------------------------------------------------------------

template<typename it>
void quicksort(it beg, it fin) {

	if (beg < fin) {
		auto pivot = beg; // select a pivot
		auto i = beg;
		auto j = fin;
        
		while (i < j) {
			while (*i <= *pivot && i <= fin) {
                i++;   
            }
            
			while (*j > *pivot && j >= beg) {
                j--;
            }
            
			if (i < j) {
                std::iter_swap(i, j);
			}
		}
        
		std::iter_swap(pivot, j);

		quicksort<it>(beg, j - 1);
		quicksort<it>(j + 1, fin);
	}
}

}