#include <iostream>
#include <cmath>
#include <thread>
#include "debug_utils.hpp"

//-----------------------------------------------------------------------------

namespace std_like {

template<typename it>
void p_quicksort(it beg, it fin) {

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

		std::thread t1 (p_quicksort<it>, beg, j - 1);
		std::thread t2 (p_quicksort<it>, j + 1, fin);
        t1.join();
        t2.join();
	}
}

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

		std::thread t1 (quicksort<it>, beg, j - 1);
		std::thread t2 (quicksort<it>, j + 1, fin);
        t1.join();
        t2.join();
	}
}

}