#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>

using namespace std;

// Parameters
const int NUM_BINS = 10; // Number of histogram bins
const int MAX_VALUE = 100; // Max value for elements

// Function to get bin index for a value
int get_bin(int value) {
    return value * NUM_BINS / (MAX_VALUE + 1);
}

int main() {
    // Input data
    vector<int> data = {42, 23, 56, 78, 10, 3, 91, 17, 33, 65, 81, 12, 7, 88, 45, 60};
    int n = data.size();

    // Step 1: Create local histograms for each thread
    vector<vector<int>> bins(NUM_BINS);

    #pragma omp parallel
    {
        // Thread-local bin storage
        vector<vector<int>> local_bins(NUM_BINS);

        #pragma omp for nowait
        for (int i = 0; i < n; ++i) {
            int bin_index = get_bin(data[i]);
            local_bins[bin_index].push_back(data[i]);
        }

        // Merge local bins into global bins (critical section for safety)
        #pragma omp critical
        {
            for (int i = 0; i < NUM_BINS; ++i) {
                bins[i].insert(bins[i].end(), local_bins[i].begin(), local_bins[i].end());
            }
        }
    }

    // Step 2: Sort each bin (can also be done in parallel)
    #pragma omp parallel for
    for (int i = 0; i < NUM_BINS; ++i) {
        sort(bins[i].begin(), bins[i].end());
    }

    // Step 3: Merge all bins back into sorted data
    vector<int> sorted;
    for (int i = 0; i < NUM_BINS; ++i) {
        sorted.insert(sorted.end(), bins[i].begin(), bins[i].end());
    }

    // Print sorted data
    cout << "Sorted Data: ";
    for (int x : sorted) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
