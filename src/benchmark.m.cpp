#include <assert.h>
#include <iostream>
#include <thread>
#include <vector>

#include <boost/log/trivial.hpp>
#define LOG BOOST_LOG_TRIVIAL
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>

#include "xorshift.h"

// std::sort
#include <algorithm>

// tbb
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_sort.h>

// pp::sort
#include "ppsort.hpp"

// boost::compute::sort
#include <boost/compute/system.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/types/struct.hpp>
#include <boost/compute/algorithm/sort.hpp>

namespace compute = boost::compute;
namespace options = boost::program_options;
namespace timer = boost::timer;

using sort_fn = void (*)(std::vector<int>&, size_t);

void std_sort(std::vector<int> &nums, size_t)
{
    std::sort(nums.begin(), nums.end());
}

void tbb_sort(std::vector<int> &nums, size_t cpus)
{
    tbb::task_scheduler_init init(cpus);
    tbb::parallel_sort(nums.begin(), nums.end(), [](std::int64_t first, std::int64_t end) { return first < end; });
}

void pp_sort(std::vector<int> &nums, size_t cpus)
{
    boost::asio::thread_pool pool(cpus);
    quick_sort(pool, nums.data(), 0, nums.size() - 1);
    pool.join();
}

void gpu_sort(std::vector<int> &nums, size_t)
{
    compute::device device = compute::system::default_device();

    LOG(debug) << "Starting analysis on GPU device.";
    LOG(debug) << "  Device name: " << device.name();
    LOG(debug) << "  Device vendor: " << device.vendor();
    LOG(debug) << "  Device profile: " << device.profile();
    LOG(debug) << "  Device version: " << device.version();
    LOG(debug) << "  Device driver version: " << device.driver_version();
    LOG(debug) << "  Global and local memory size: " << device.global_memory_size() << ", " << device.local_memory_size();
    LOG(debug) << "  Compute units: " << device.compute_units();

    compute::context context(device);
    compute::command_queue queue(context, device);

    compute::sort(nums.begin(), nums.end(), queue);
}

void run(const std::string& name, sort_fn func, const std::vector<int>& const_nums, size_t cpus)
{
    std::vector<int> nums(const_nums);

    LOG(trace) << "Running " << name << "...";
    timer::cpu_timer timer;
    func(nums, cpus);
    LOG(debug) << name << " took " << timer.format(timer::default_places, "%ws wall, %us user + %ss system = %ts CPU (%p%)");

    assert(std::is_sorted(nums.begin(), nums.end()));
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    // clang-format off
    options::options_description opts("Options");
    opts.add_options()
        ("count", options::value<int>(), "size of input vector")
        ("cpus", options::value<int>()->default_value(4096), "number of CPUs")
        ("runs", options::value<int>()->default_value(1), "number of runs")
        // Input control options.
        ("const", options::value<double>()->default_value(.0f), "fraction of constants")
        ("asc", options::value<double>()->default_value(.0f), "fraction numbers sorted in ascending order")
        ("desc", options::value<double>()->default_value(.0f), "fraction numbers sorted in descending order")
        // Sorting algorithms.
        ("std", options::value<bool>()->implicit_value(true), "use std::sort")
        ("tbb", options::value<bool>()->implicit_value(true), "use tbb::parallel_for")
        ("pp", options::value<bool>()->implicit_value(true), "use pp::sort")
        ("gpu", options::value<bool>()->implicit_value(true), "use boost::compute::sort");
    // clang-format on

    options::variables_map vm;
    options::store(options::command_line_parser(argc, argv).options(opts).run(), vm);
    options::notify(vm);

    /*
     * Generating numbers.
     */
    std::vector<int> nums(vm["count"].as<int>());
    xorshift128_t g = { (uint32_t) rand(), (uint32_t)  rand(),  (uint32_t) rand(), (uint32_t)  rand() };
    for (size_t i = 0; i < nums.size(); ++i) {
        nums[i] = xorshift128(g);
    }

    /*
     * Maximum CPUs.
     */
    size_t cpus = (size_t)std::min(vm["cpus"].as<int>(), static_cast<int>(std::thread::hardware_concurrency()));
    LOG(info) << "Running with at most " << cpus << " maximum CPUs.";

    /*
     * Benchmarking sort methods.
     */
    if (vm.count("std") && vm["std"].as<bool>()) {
        for (int i = 0; i < vm["runs"].as<int>(); ++i) {
            run("std::sort", std_sort, nums, cpus);
        }
    }

    if (vm.count("tbb") && vm["tbb"].as<bool>()) {
        for (int i = 0; i < vm["runs"].as<int>(); ++i) {
            run("tbb::parallel_sort", tbb_sort, nums, cpus);
        }
    }

    if (vm.count("pp") && vm["pp"].as<bool>()) {
        for (int i = 0; i < vm["runs"].as<int>(); ++i) {
            run("pp::sort", pp_sort, nums, cpus);
        }
    }

    if (vm.count("gpu") && vm["gpu"].as<bool>()) {
        for (int i = 0; i < vm["runs"].as<int>(); ++i) {
            run("boost::compute::sort", gpu_sort, nums, cpus);
        }
    }

    return 0;
}
