#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to generate a random matrix
void generateRandomMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = rand() % 10; // Random numbers between 0 and 9
        }
    }
}

// Function to perform parallel matrix multiplication
void parallelMatrixMultiplication(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int num_threads) {
    int rows = A.size();
    int cols = B[0].size();
    int common_dim = B.size();

    #pragma omp parallel for num_threads(num_threads) collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < common_dim; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand(time(0)); // Seed for random number generation
    
    int rowsA, colsA, rowsB, colsB;
    int num_threads = 4; // Number of threads

    // User input for matrix dimensions
    cout << "Enter number of rows and columns for Matrix A: ";
    cin >> rowsA >> colsA;
    cout << "Enter number of rows and columns for Matrix B: ";
    cin >> rowsB >> colsB;
    
    if (colsA != rowsB) {
        cout << "Matrix multiplication not possible: Number of columns in A must be equal to number of rows in B." << endl;
        return 1;
    }

    vector<vector<int>> A(rowsA, vector<int>(colsA));
    vector<vector<int>> B(rowsB, vector<int>(colsB));
    vector<vector<int>> C(rowsA, vector<int>(colsB, 0));

    generateRandomMatrix(A, rowsA, colsA);
    generateRandomMatrix(B, rowsB, colsB);

    // Perform matrix multiplication
    parallelMatrixMultiplication(A, B, C, num_threads);

    // Print Matrix A
    cout << "Matrix A:\n";
    for (const auto& row : A) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }

    // Print Matrix B
    cout << "Matrix B:\n";
    for (const auto& row : B) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }

    // Print the result matrix
    cout << "Resultant Matrix:\n";
    for (const auto& row : C) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }

    return 0;
}
