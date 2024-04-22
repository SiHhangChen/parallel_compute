#include <bits/stdc++.h>
using namespace std;

int main() {
    int M, N, P, K;
    cin >> M >> N >> P >> K;

    vector<vector<double>> denseMatrix(M, vector<double>(N));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> denseMatrix[i][j];
        }
    }

    map<pair<int, int>, double> sparseMatrix;
    for (int i = 0; i < K; ++i) {
        int row, col;
        double value;
        cin >> row >> col >> value;
        sparseMatrix[{row, col}] = value;
    }

    vector<vector<double>> result(M, vector<double>(P, 0.0));
    for (const auto& item : sparseMatrix) {
        int row = item.first.first;
        int col = item.first.second;
        double value = item.second;
        for (int i = 0; i < M; ++i) {
            result[i][col] += (double)denseMatrix[i][row] * value;
        }
    }

    for (const auto& row : result) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    return 0;
}