#include <vector>
#include <iostream>
#include <algorithm>
static bool sorting(std::vector<int>& a, std::vector<int>& b) {
    if(a[1] < b[1])return 1;
    else if(a[1] > b[1])return 0;
    else if(a[2] > b[2])return 1;
    else if(a[2] < b[2])return 0;
    else if(a[0] < b[0])return 1;
    return 0;
}
int find_min_time(std::vector<std::vector<int>>& tasks) {
    std::sort(tasks.begin(), tasks.end(), sorting);
    std::vector<bool> track(2001,0);
    for(int i = 0; i < tasks.size(); i++){
        int counter = 0;
        for(int j = tasks[i][0]; j <= tasks[i][1]; j++) {
            if (track[j]) {
                counter++;
            }
        }
        if (counter >= tasks[i][2]) {
            continue;
        }
        tasks[i][2] -= counter;
        for(int j = tasks[i][1]; tasks[i][2]; j--){
            if(track[j]) {
                continue;
            }
            tasks[i][2]--;
            track[j] = 1;
        }
    }
    return std::count(track.begin(),track.end(),1);
}
int main(){
    int num_of_tasks;
    std::cout << "Введите количество задач: " << std::endl;
    std::cin >> num_of_tasks;
    std::vector<std::vector<int>> tasks(num_of_tasks);
    std::cout << "Введите начало, конец, время: " << std::endl;
    for (int i = 0; i < num_of_tasks; i++) {
        int start, end, time;
        std::cin >> start >> end >> time;
        tasks[i] = {start, end, time};
    }
    std::cout << find_min_time(tasks) << std::endl;
    return 0;
    }