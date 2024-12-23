#include <iostream>
#include <vector>
int change(int amount, std::vector<int>& coins) {
    int size = coins.size();
    std::vector<int> dp(amount + 1, 0);
    dp[0] = 1;
    for (int coin = size - 1; coin >= 0; coin--) {
        for (int target = 0; target <= amount; target++) {
            if (target >= coins[coin]) {
                dp[target] += dp[target - coins[coin]];
            }
        }
    }
    return dp[amount];
}
int jump(std::vector<int>& nums) {
    int size = nums.size();
    std::vector<int> dp(size + 1, INT_MAX);
    dp[0] = 0;
    for (int i = 1; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if (j + nums[j] >= i) {
                dp[i] = std::min(dp[i], dp[j] + 1);
            }
        }
    }
    return dp[size - 1];
}
int main() {
    int amount = 5;
    std::vector<int> coins = {1, 2, 5};
    std::vector<int> nums = {2, 3, 1, 1, 4};
    std::cout << "Решение задачи с монетами:" << change(amount, coins) << std::endl;
    std::cout << "Решение задачи с прыжками" << jump(nums) << std::endl;
    return 0;
}