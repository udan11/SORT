#include <assert.h>
#include <iostream>
#include <thread>
#include <vector>

#include <boost/log/trivial.hpp>
#define LOG BOOST_LOG_TRIVIAL
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>

#include <omp.h>

#include "xorshift.h"

namespace options = boost::program_options;
namespace timer = boost::timer;

void bubble_sort(int arr[], int n);
void bubble_sort_omp(int arr[], int n);
void insertion_sort(int arr[], int n);
void merge_sort(int arr[], int l, int r);
void merge_sort_omp(int arr[], int l, int r);
void quick_sort(int arr[], int low, int high);
void quick_sort_omp(int arr[], int low, int high);
void tim_sort(int arr[], int n);
void tim_sort_omp(int arr[], int n);

void run(const std::string& name, const std::vector<int>& const_nums, size_t cpus)
{
    std::vector<int> nums(const_nums);
    // omp_set_num_threads(cpus);

    LOG(trace) << "Running " << name << "...";
    timer::cpu_timer timer;

    if (name == "bubble") {
        bubble_sort(nums.data(), nums.size());
    } else if (name == "bubble_omp") {
        bubble_sort_omp(nums.data(), nums.size());
    } else if (name == "insertion") {
        insertion_sort(nums.data(), nums.size());
    } else if (name == "merge") {
        merge_sort(nums.data(), 0, nums.size() - 1);
    } else if (name == "merge_omp") {
        merge_sort_omp(nums.data(), 0, nums.size() - 1);
    } else if (name == "quick") {
        quick_sort(nums.data(), 0, nums.size() - 1);
    } else if (name == "quick_omp") {
        quick_sort_omp(nums.data(), 0, nums.size() - 1);
    } else if (name == "tim") {
        tim_sort(nums.data(), nums.size());
    } else if (name == "tim_omp") {
        tim_sort_omp(nums.data(), nums.size());
    }

    // LOG(debug) << name << " took " << timer.format(timer::default_places, "%ws wall, %us user + %ss system = %ts CPU (%p%)");
    LOG(debug) << "RESULT! " << name << "," << cpus << "," << nums.size() << "," << timer.format(timer::default_places, "%w,%u,%s,%t,%p");

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
        ("experiment", options::value<std::string>(), "name of the experiment");
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
     * Running experiments
     */
    for (int i = 0; i < vm["runs"].as<int>(); ++i) {
        run(vm["experiment"].as<std::string>(), nums, cpus);
    }

    return 0;
}
