#include "integrate.hpp"

namespace integration
{

//-----------------------------------------------------------------------------------------

std::stack<segment, std::vector<segment>> global_stack;
const std::size_t max_segments = 10;

static std::binary_semaphore stack_sem{1};
static std::binary_semaphore job_present_sem{1};
static std::size_t workers_in_progress = 0;

//-----------------------------------------------------------------------------------------

double get_integr_val(std::vector<std::future<double>>& futures) {
    double res = 0.0;
    for (auto& f : futures) {
        if (f.valid()) {
            res += f.get();
        }
    }
    return res;
}

//-----------------------------------------------------------------------------------------

double integrator::integrate(double a, double b) const
{
    if (a == b) {
        return 0.0;
    }

    double f_a = integr_func(a), f_b = integr_func(b);
    global_stack.emplace(a, f_a, b, f_b, std::midpoint(f_a, f_b) * (b - a));

    std::vector<std::thread> workers;
    workers.reserve(n_threads);
    std::vector<std::future<double>> futures;
    futures.reserve(n_threads);

    for (size_t i = 0; i < n_threads; i++)
    {
        std::packaged_task<double()> work_to_do{std::bind(&integrator::integration_job, this)};
        futures.emplace_back(work_to_do.get_future());
        workers.emplace_back(std::move(work_to_do));
    }
    double integr_val = get_integr_val(futures);

    for (auto &worker : workers) {
        worker.join();
    }

    return integr_val;
}

double integrator::integration_job() const
{
    double integr_val = 0.0;

    while (true)
    {
        job_present_sem.acquire();
        stack_sem.acquire();

        auto [a, f_a, b, f_b, I_ab] = global_stack.top();
        global_stack.pop();

        if (!global_stack.empty()) {
            job_present_sem.release();
        }

        if (a < b)
        {
            workers_in_progress++;
            stack_sem.release();
        }
        else
        {
            stack_sem.release();
            return integr_val;
        }

        double partial_I = integrate_segment(a, f_a, b, f_b, I_ab);

        stack_sem.acquire();

        workers_in_progress--;
        if (workers_in_progress == 0 && global_stack.empty())
        {
            for (auto i : std::views::iota(size_t(0), n_threads))
                global_stack.emplace(1.0, NAN, 0.0, NAN, NAN);

            job_present_sem.release();
        }

        stack_sem.release();
        integr_val += partial_I;
    }
}

double integrator::integrate_segment(double a, double f_a, double b, double f_b, double I_ab) const
{
    double integr_val = 0;

    std::stack<segment, std::vector<segment>> stack;
    while (true)
    {
        double c = std::midpoint(a, b);
        double f_c = integr_func(c);

        double I_ac = std::midpoint(f_a, f_c) * (c - a);
        double I_cb = std::midpoint(f_c, f_b) * (b - c);
        double I_acb = I_ac + I_cb;

        if (check_approx(I_ab, I_acb))
        {
            stack.emplace(a, f_a, c, f_c, I_ac);
            a = c;
            f_a = f_c;
            I_ab = I_cb;
        }
        else
        {
            integr_val += I_acb;

            if (stack.empty())
                break;

            std::tie(a, f_a, b, f_b, I_ab) = stack.top();
            stack.pop();
        }

        if (stack.size() > max_segments && global_stack.empty())
        {
            stack_sem.acquire();

            if (global_stack.empty())
                job_present_sem.release();

            while (stack.size() > 1)
            {
                global_stack.emplace(stack.top());
                stack.pop();
            }

            stack_sem.release();
        }
    }

    return integr_val;
}

} // namespace integration