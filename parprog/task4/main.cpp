#include "omp.h"
#include <iostream>
#include <vector>
#include <random>

using std::cout;
using std::endl;
using std::vector;

void printMatrix (vector<vector<int>> matrix)
{
    for (size_t i = 0; i < matrix.size (); i++)
    {
        for (size_t j = 0; j < matrix.front ().size (); j++)
            cout << matrix[i][j] << "\t";
        cout << endl;
    }    

}
const int N = 2, M = 2, P = 2;

int main (int argc, char* argv[])
{
    // Here we initialize random integers generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distribution (1, 3);

    // Dimensions of matrix. First is N x M, second is M x P 
    

    vector<vector<int>> A, B, C;      // A, B - Given matrix, C - resulting matrix
    A.resize (N);
    B.resize (M);
    C.resize (N);
    for (int i = 0; i < N; i++)
    {
        A.at (i).resize (M);
        for (auto& val : A.at (i))
            val = distribution (rng);

        C.at (i).resize (P, 0);                 // initialization...
    }
    
    for (int i = 0; i < M; i++)
    {
        B.at (i).resize (P);
        for (auto& val : B.at (i))
            val = distribution (rng);
    }
        // here we compute multiplication of matrix
    #pragma omp parallel
    {
        #pragma omp for schedule (static)
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < P; j++)
            {
                for (int k = 0; k < M; k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    cout << "A matrix:\n";
    printMatrix (A);
    cout << endl << "B matrix:\n";
    printMatrix (B);
    cout << endl << "C, resulting matrix:\n";;
    printMatrix (C);

    return 0; 
}