#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::vector<long long>> strassen(const std::vector<std::vector<long long>>& A, const std::vector<std::vector<long long>>& B) {
    int n = A.size();
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }

    // Splitting matrices into submatrices
    std::vector<std::vector<long long>> A11(n / 2), A12(n / 2), A21(n / 2), A22(n / 2);
    std::vector<std::vector<long long>> B11(n / 2), B12(n / 2), B21(n / 2), B22(n / 2);
    std::vector<std::vector<long long>> C11(n / 2), C12(n / 2), C21(n / 2), C22(n / 2);

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

    // Computing intermediate products
    std::vector<std::vector<long long>> M1 = strassen(addMatrices(A11, A22), addMatrices(B11, B22));
    std::vector<std::vector<long long>> M2 = strassen(addMatrices(A21, A22), B11);
    std::vector<std::vector<long long>> M3 = strassen(A11, subtractMatrices(B12, B22));
    std::vector<std::vector<long long>> M4 = strassen(A22, subtractMatrices(B21, B11));
    std::vector<std::vector<long long>> M5 = strassen(addMatrices(A11, A12), B22);
    std::vector<std::vector<long long>> M6 = strassen(subtractMatrices(A21, A11), addMatrices(B11, B12));
    std::vector<std::vector<long long>> M7 = strassen(subtractMatrices(A12, A22), addMatrices(B21, B22));

    // Combining results
    C11 = addMatrices(subtractMatrices(addMatrices(M1, M4), M5), M7);
    C12 = addMatrices(M3, M5);
    C21 = addMatrices(M2, M4);
    C22 = subtractMatrices(addMatrices(M1, M3), M2), addMatrices(M6, M7));

    // Constructing the result matrix
    std::vector<std::vector<long long>> C(n);
    for (int i = 0; i < n / 2; ++i) {
        C[i].insert(C[i].end(), C11[i].begin(), C11[i].end());
        C[i].insert(C[i].end(), C12[i].begin(), C12[i].end());
        C[i + n / 2].insert(C[i + n / 2].end(), C21[i].begin(), C21[i].end());
        C[i + n / 2].insert(C[i + n / 2].end(), C22[i].begin(), C22[i].end());
    }

    return C;
}

std::vector<std::vector<long long>> addMatrices(const std::vector<std::vector<long long>>& A, const std::vector<std::vector<long long>>& B) {
    int n = A.size();
    std::vector<std::vector<long long>> result(n, std::vector<long long>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
    return result;
}

std::vector<std::vector<long long>> subtractMatrices(const std::vector<std::vector<long long>>& A, const std::vector<std::vector<long long>>& B) {
    int n = A.size();
    std::vector<std::vector<long long>> result(n, std::vector<long long>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    std::ifstream infile(argv[1]);
    std::ofstream outfile(argv[2]);

    int n;
    infile >> n;

    std::vector<std::vector<long long>> A(n, std::vector<long long>(n));
    std::vector<std::vector<long long>> B(n, std::vector<long long>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            infile >> A[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            infile >> B[i][j];
        }
    }

    std::vector<std::vector<long long>> C = strassen(A, B);

    for (const auto& row : C) {
        for (long long val : row) {
            outfile << val << " ";
        }
        outfile << "\n";
    }

    infile.close();
    outfile.close();

    return 0;
}
