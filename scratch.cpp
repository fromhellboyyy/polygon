#include <iostream>
const int mod = 1000000007;
int count_ways(int m, int n) {
    if (m == 1 && n == 1){
        return 3;
    }
    int dp[m][n];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 3;
            } else {
                int prevRow = i - 1;
                int prevCol = j - 1;
                int red = dp[prevRow][prevCol];
                int green = dp[prevRow][j];
                int blue = dp[i][prevCol];
                dp[i][j] = (red + green + blue) % mod;
            }
        }
    }
    return dp[m - 1][n - 1];
}
int main() {
    int m, n;
    std::cout << "Введите количество строк" << std::endl;
    std::cin >> m;
    std::cout << "Введите количество столбцов" << std::endl;
    std::cin >> n;
    std::cout << count_ways(m, n) << std::endl;
    return 0;
}
