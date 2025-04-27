#include "latency.hpp"

int main() {
    latency::latency_helper helper{};
    const int iterations = 400000;
    double avg_time = latency::measure_communication_time_thread(helper, iterations);
    
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Average thread communication time: " << avg_time * 1e9 << " ns per message\n";
    std::cout << "Total thread time: " << avg_time * iterations * 2 * 1e9 << " ns\n";

    avg_time = latency::measure_communication_time_pipe(iterations);
    
    std::cout << "Average pipe communication time: " << avg_time * 1e9 << " ns per message\n";
    std::cout << "Total pipe time: " << avg_time * iterations * 2 * 1e9 << " ns\n";

    return 0;
}