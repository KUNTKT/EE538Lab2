#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<std::vector<long long>> strassen(
    const std::vector<std::vector<long long>>& A,
    const std::vector<std::vector<long long>>& B) {
    
    int n = A.size();
    
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }
    
    int half = n / 2;
    
    // Split A into four blocks
    std::vector<std::vector<long long>> A11(half, std::vector<long long>(half));
    std::vector<std::vector<long long>> A12(half, std::vector<long long>(half));
    std::vector<std::vector<long long>> A21(half, std::vector<long long>(half));
    std::vector<std::vector<long long>> A22(half, std::vector<long long>(half));
    
    std::vector<std::vector<long long>> B11(half, std::vector<long long>(half));
    std::vector<std::vector<long long>> B12(half, std::vector<long long>(half));
    std::vector<std::vector<long long>> B21(half, std::vector<long long>(half));
    std::vector<std::vector<long long>> B22(half, std::vector<long long>(half));
    
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + half];
            A21[i][j] = A[i + half][j];
            A22[i][j] = A[i + half][j + half];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + half];
            B21[i][j] = B[i + half][j];
            B22[i][j] = B[i + half][j + half];
        }
    }
    
    // Compute seven products recursively
    auto M1 = strassen({{A11[0][0], A11[0][1]}, {A11[1][0], A11[1][1]}},
                       {{B11[0][0], B11[0][1]}, {B11[1][0], B11[1][1]}});

    auto M2 = strassen({{A21[0][0], A21[0][1]}, {A21[1][0], A21[1][1]}},
                       {{B11[0][0], B11[0][1]}, {B11[1][0], B11[1][1]}});

    auto M3 = strassen({{A11[0][0], A11[0][1]}, {A11[1][0], A11[1][1]}},
                       {{B12[0][0], B12[0][1]}, {B12[1][0], B12[1][1]}});

    auto M4 = strassen({{A22[0][0], A22[0][1]}, {A22[1][0], A22[1][1]}},
                       {{B21[0][0], B21[0][1]}, {B21[1][0], B21[1][1]}});

    auto M5 = strassen({{A11[0][0], A11[0][1]}, {A11[1][0], A11[1][1]}},
                       {{B22[0][0], B22[0][1]}, {B22[1][0], B22[1][1]}});

    auto M6 = strassen({{A21[0][0], A21[0][1]}, {A21[1][0], A21[1][1]}},
                       {{B11[0][0], B11[0][1]}, {B11[1][0], B11[1][1]}});

    auto M7 = strassen({{A12[0][0], A12[0][1]}, {A12[1][0], A12[1][1]}},
                       {{B21[0][0], B21[0][1]}, {B21[1][0], B21[1][1]}});

    // Combine results
    std::vector<std::vector<long long>> C(n, std::vector<long long>(n));
    
    // C11 = M1 + M4 - M5 + M7
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
        }
    }
    
    // C12 = M3 + M5
    for (int i = 0; i < half; ++i) {
        for (int j = half; j < n; ++j) {
            C[i][j] = M3[i][j - half] + M5[i][j - half];
        }
    }
    
    // C21 = M2 + M4
    for (int i = half; i < n; ++i) {
        for (int j = 0; j < half; ++j) {
            C[i][j] = M2[i - half][j] + M4[i - half][j];
        }
    }
    
    // C22 = M1 - M2 + M3 + M6
    for (int i = half; i < n; ++i) {
        for (int j = half; j < n; ++j) {
            C[i][j] = M1[i - half][j - half] - M2[i - half][j - half] + M3[i - half][j - half] + M6[i - half][j - half];
        }
    }
    
    return C;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }
    
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    
    if (!input.is_open() || !output.is_open()) {
        return 1;
    }
    
    int n;
    input >> n;
    
    std::vector<std::vector<long long>> A(n, std::vector<long long>(n));
    std::vector<std::vector<long long>> B(n, std::vector<long long>(n));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            input >> A[i][j];
        }
    }
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            input >> B[i][j];
        }
    }
    
    auto C = strassen(A, B);
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j > 0) output << " ";
            output << C[i][j];
        }
        output << "\n";
    }
    
    input.close();
    output.close();
    
    return 0;
}
