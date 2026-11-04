#include <limits>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdio>

int main( int argc, char** argv ) 
{
    int size = 0;
    std::sscanf(argv[1],"%i",&size);  // problem size

    int num_tests = 0;
    std::sscanf(argv[2],"%i",&num_tests);  // number of experiments

    // create array size n with values [-n/3,2n/3] !!!!!!!!!!!!!!!!!!!!!!!!!
    int *a = new int[ size ];
    for (int i = 0; i < size; ++i) { 
        a[i] = i - size/3;
    }

    // C++11 RNG
    std::random_device rd;
    std::mt19937 gen( rd() ); // random engine

    for ( int experiment =0; experiment < num_tests; ++experiment ) {
        std::shuffle( a, a+size, gen ); // C++11

        // Kadane's algorithm starts here
        int max_so_far = a[0], max_ending_here = a[0];

        for (int i = 0; i < size; ++i)
        {
            max_ending_here = max_ending_here + a[i];
            if (max_so_far < max_ending_here)
                max_so_far = max_ending_here;

            if (max_ending_here < 0)
                max_ending_here = 0;
        }
        // Kadane's algorithm ends here

        std::cout << max_so_far << " "; // solution
    }
    delete [] a;
}