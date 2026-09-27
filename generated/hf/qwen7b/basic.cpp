#include <iostream>
#include <vector>

// Function to multiply two matrices using Strassen's algorithm
std::vector<std::vector<int>> strassenMultiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    
    // Base case: if matrix is 1x1
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }

    // Divide matrices into submatrices
    std::vector<std::vector<int>> A11(n / 2), A12(n / 2), A21(n / 2), A22(n / 2);
    std::vector<std::vector<int>> B11(n / 2), B12(n / 2), B21(n / 2), B22(n / 2);
    std::vector<std::vector<int>> C11(n / 2), C12(n / 2), C21(n / 2), C22(n / 2);

    for (int i = 0; i < n / 2; ++i) {
        for (int j = 0; j < n / 2; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + n / 2];
            A21[i][j] = A[i + n / 2][j];
            A22[i][j] = A[i + n / 2][j + n / 2];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + n / 2];
            B21[i][j] = B[i + n / 2][j];
            B22[i][j] = B[i + n / 2][j + n / 2];
        }
    }

    // Calculate seven products recursively
    std::vector<std::vector<int>> P1 = strassenMultiply(add(A11, A22), add(B11, B22));
    std::vector<std::vector<int>> P2 = strassenMultiply(add(A21, A22), B11);
    std::vector<std::vector<int>> P3 = strassenMultiply(A11, subtract(B12, B22));
    std::vector<std::vector<int>> P4 = strassenMultiply(A22, subtract(B21, B11));
    std::vector<std::vector<int>> P5 = strassenMultiply(add(A11, A12), B22);
    std::vector<std::vector<int>> P6 = strassenMultiply(subtract(A21, A11), add(B11, B12));
    std::vector<std::vector<int>> P7 = strassenMultiply(subtract(A12, A22), add(B21, B22));

    // Combine results to form the final product matrix
    C11 = add(subtract(add(P1, P4), P5), P7);
    C12 = add(P3, P5);
    C21 = add(P2, P4);
    C22 = subtract(subtract(add(P1, P3), P2), P6);

    // Combine submatrices to form the final result
    std::vector<std::vector<int>> C(n, std::vector<int>(n));
    for (int i = 0; i < n / 2; ++i) {
        for (int j = 0; j < n / 2; ++j) {
            C[i][j] = C11[i][j];
            C[i][j + n / 2] = C12[i][j];
            C[i + n / 2][j] = C21[i][j];
            C[i + n / 2][j + n / 2] = C22[i][j];
        }
    }

    return C;
}

// Helper function to add two matrices
std::vector<std::vector<int>> add(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    std::vector<std::vector<int>> result(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
    return result;
}

// Helper function to subtract two matrices
std::vector<std::vector<int>> subtract(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    std::vector<std::vector<int>> result(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
    return result;
}

// Main function to test Strassen's algorithm
int main() {
    std::vector<std::vector<int>> A = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> B = {{2, 0}, {1, 2}};

    std::vector<std::vector<int>> C = strassenMultiply(A, B);

    std::cout << "Result of multiplication:\n";
    for (const auto& row : C) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
