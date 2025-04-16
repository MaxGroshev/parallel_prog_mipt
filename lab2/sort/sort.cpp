#include "sort.hpp"

//-----------------------------------------------------------------------------

int main() {
    std::vector<int> a {9,8,7,6,4,5,3,2,1,8, 10};

	std::cout<<"list before sort: ";
	
	std_like::quicksort(a.begin(), a.end() - 1); //bad
	std::cout<<"list after sort: ";
    for (const auto& v : a ) {
        std::cout << v << " ";
    } 
    return 0;
}