#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <chrono>
struct Edge {
    double lon;
    double lat;
    double weight;
    // Время: O(1) для создания.
    // Память: O(1), хранит 3 double (координаты и вес).
};
struct Node {
    int node_id;
    double lon;
    double lat;
    std::vector<std::pair<Node*, double>> neighbors;

    Node(int id, double lon, double lat) : node_id(id), lon(lon), lat(lat) {}
    // Время: O(1) для создания.
    // Память: O(k), где k — количество соседей (вектор соседей с весами).
};
struct Graph {
    std::vector<Node*> nodes;
    std::unordered_map<std::string, Node*> node_cache; // Кэш для быстрого поиска узлов по координатам
    std::string createKey(double lon, double lat) {
        return std::to_string(lon) + "," + std::to_string(lat);
        // Время: O(L), где L — длина строки представления координат (зависит от точности double).
        // Память: O(L), строка сохраняется в памяти.
    }
    void addEdge(Node* from, Node* to, double weight) {
        from->neighbors.push_back({to, weight});
        to->neighbors.push_back({from, weight});
        // Время: O(1) для добавления ребра в список соседей.
        // Память: O(1) для хранения нового ребра.
    }
    Node* findNode(double lon, double lat) {
        std::string key = createKey(lon, lat);
        auto it = node_cache.find(key);
        return it != node_cache.end() ? it->second : nullptr;
        // Время: O(1) для поиска в unordered_map.
    }
    Node* findNearestNode(double lon, double lat) {
        double minDistance = std::numeric_limits<double>::max();
        Node* nearestNode = nullptr;
        // Время: O(V), где V — количество узлов.
        for (auto& node : nodes) {
            double distance = std::pow(node->lon - lon, 2) + std::pow(node->lat - lat, 2);
            if (distance < minDistance) {
                minDistance = distance;
                nearestNode = node;
            }
        }
        // Память: O(1)
        return nearestNode;
    }
    void parseFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            char separator;
            double lon, lat;
            iss >> lon >> separator >> lat >> separator;
            Node* currentNode = findNode(lon, lat);
            if (!currentNode) {
                currentNode = new Node(nodes.size(), lon, lat);
                nodes.push_back(currentNode);
                node_cache[createKey(lon, lat)] = currentNode;
            }
            while (!iss.eof()) {
                double neighborLon, neighborLat, weight;
                iss >> neighborLon >> separator >> neighborLat >> separator >> weight >> separator;
                Node* neighborNode = findNode(neighborLon, neighborLat);
                if (!neighborNode) {
                    neighborNode = new Node(nodes.size(), neighborLon, neighborLat);
                    nodes.push_back(neighborNode);
                    node_cache[createKey(neighborLon, neighborLat)] = neighborNode;
                }
                addEdge(currentNode, neighborNode, weight);
            }
        }
        file.close();
        // Время: O(E), где E — количество рёбер, так как каждый узел и его соседи добавляются один раз.
        // Память: O(V + E), где V — количество узлов, а E — количество рёбер.
    }
    double dfs(Node* start, Node* end) {
        std::stack<Node*> s;
        std::unordered_map<Node*, Node*> previous;
        std::unordered_set<Node*> visited;
        std::unordered_map<Node*, double> distances;
        s.push(start);
        distances[start] = 0.0;
        while (!s.empty()) {
            Node* current = s.top();
            s.pop();
            if (visited.count(current)) {
                continue;
            }
            visited.insert(current);
            if (current == end) {
                return distances[current];
            }
            for (const auto& neighbor : current->neighbors) {
                if (!visited.count(neighbor.first)) {
                    double newDistance = distances[current] + neighbor.second;
                    if (distances.find(neighbor.first) == distances.end() || newDistance < distances[neighbor.first]) {
                        distances[neighbor.first] = newDistance;
                        previous[neighbor.first] = current;
                        s.push(neighbor.first);
                    }
                }
            }
        }
        return -1.0;
        // Время: O(V + E), полный обход графа.
        // Память: O(V), так как используются структуры visited и distances.
    }
    double bfs(Node* start, Node* end) {
        std::queue<Node*> q;
        std::unordered_map<Node*, double> distances;
        std::unordered_set<Node*> visited;
        q.push(start);
        distances[start] = 0.0;
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            if (current == end) {
                return distances[current];
            }
            for (const auto& neighbor : current->neighbors) {
                if (!visited.count(neighbor.first)) {
                    visited.insert(neighbor.first);
                    distances[neighbor.first] = distances[current] + neighbor.second;
                    q.push(neighbor.first);
                }
            }
        }
        return -1.0;
        // Время: O(V + E), полный обход графа.
        // Память: O(V), используются visited и distances.
    }
    double dijkstra(Node* start, Node* end) {
        std::unordered_map<Node*, double> distances;
        for (Node* node : nodes) {
            distances[node] = std::numeric_limits<double>::infinity();
        }
        distances[start] = 0.0;
        auto compare = [](const std::pair<Node*, double>& a, const std::pair<Node*, double>& b) {
            return a.second > b.second;
        };
        std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, decltype(compare)> pq(compare);
        pq.push({start, 0.0});
        while (!pq.empty()) {
            Node* current = pq.top().first;
            double currentDistance = pq.top().second;
            pq.pop();
            if (current == end) {
                return currentDistance;
            }
            for (const auto& neighbor : current->neighbors) {
                double newDistance = currentDistance + neighbor.second;
                if (newDistance < distances[neighbor.first]) {
                    distances[neighbor.first] = newDistance;
                    pq.push({neighbor.first, newDistance});
                }
            }
        }
        return -1.0;
        // Время: O((V + E) * log(V)), благодаря использованию priority_queue.
        // Память: O(V), используется distances и очередь с приоритетом.
    }
    double astar(Node* start, Node* end) {
        auto heuristic = [end](Node* node) {
            return std::sqrt(std::pow(node->lon - end->lon, 2) + std::pow(node->lat - end->lat, 2));
        };
        std::unordered_map<Node*, double> distances;
        std::unordered_map<Node*, double> f_scores;
        for (Node* node : nodes) {
            distances[node] = std::numeric_limits<double>::infinity();
            f_scores[node] = std::numeric_limits<double>::infinity();
        }
        distances[start] = 0.0;
        f_scores[start] = heuristic(start);
        auto compare = [](const std::pair<Node*, double>& a, const std::pair<Node*, double>& b) {
            return a.second > b.second;
        };
        std::priority_queue<std::pair<Node*, double>, std::vector<std::pair<Node*, double>>, decltype(compare)> pq(compare);
        pq.push({start, f_scores[start]});
        while (!pq.empty()) {
            Node* current = pq.top().first;
            pq.pop();
            if (current == end) {
                return distances[current];
            }
            for (const auto& neighbor : current->neighbors) {
                double tentative_g_score = distances[current] + neighbor.second;
                if (tentative_g_score < distances[neighbor.first]) {
                    distances[neighbor.first] = tentative_g_score;
                    f_scores[neighbor.first] = tentative_g_score + heuristic(neighbor.first);
                    pq.push({neighbor.first, f_scores[neighbor.first]});
                }
            }
        }
        return -1.0;
        // Время: O((V + E) * log(V)), похожа на Dijkstra, но с дополнительной эвристикой.
        // Память: O(V), для хранения f_scores и distances.
    }
};
void runTests() {
    Graph graph;
    graph.parseFromFile("test_graph.txt");
    Node* start = graph.findNode(0.0, 0.0);
    Node* end = graph.findNode(1.0, 1.0);
    if (start && end) {
        std::cout << "Testing DFS:\n";
        double dfsDistance = graph.dfs(start, end);
        std::cout << "DFS Distance: " << dfsDistance << "\n";
        std::cout << "Testing BFS:\n";
        double bfsDistance = graph.bfs(start, end);
        std::cout << "BFS Distance: " << bfsDistance << "\n";
        std::cout << "Testing Dijkstra:\n";
        double dijkstraDistance = graph.dijkstra(start, end);
        std::cout << "Dijkstra Distance: " << dijkstraDistance << "\n";
        std::cout << "Testing A*:\n";
        double astarDistance = graph.astar(start, end);
        std::cout << "A* Distance: " << astarDistance << "\n";
    } else {
        std::cout << "Start or End node not found!\n";
    }
}
int main() {
    Graph graph;
    graph.parseFromFile("spb_graph.txt");
    //
    double startLon = 30.302593, startLat = 59.972629;
    double endLon = 30.295483, endLat = 59.944077;
    // биржа 30.295483 59.944077
    // ломо 30.337795 59.926835
    // кронва 30.308108 59.957238
    Node* startNode = graph.findNearestNode(startLon, startLat);
    Node* endNode = graph.findNearestNode(endLon, endLat);
    if (startNode && endNode) {
        auto start_time = std::chrono::high_resolution_clock::now();
        double pathLengthDFS = graph.dfs(startNode, endNode);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedDFS = end_time - start_time;
        std::cout << "DFS Path Length: " << pathLengthDFS << "\n";
        std::cout << "DFS Time: " << elapsedDFS.count() << " seconds\n";
        start_time = std::chrono::high_resolution_clock::now();
        double pathLengthBFS = graph.bfs(startNode, endNode);
        end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedBFS = end_time - start_time;
        std::cout << "BFS Path Length: " << pathLengthBFS << "\n";
        std::cout << "BFS Time: " << elapsedBFS.count() << " seconds\n";
        start_time = std::chrono::high_resolution_clock::now();
        double pathLengthDijkstra = graph.dijkstra(startNode, endNode);
        end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedDijkstra = end_time - start_time;
        std::cout << "Dijkstra Path Length: " << pathLengthDijkstra << "\n";
        std::cout << "Dijkstra Time: " << elapsedDijkstra.count() << " seconds\n";
        start_time = std::chrono::high_resolution_clock::now();
        double pathLengthAStar = graph.astar(startNode, endNode);
        end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedAStar = end_time - start_time;
        std::cout << "A* Path Length: " << pathLengthAStar << "\n";
        std::cout << "A* Time: " << elapsedAStar.count() << " seconds\n";
    } else {
        std::cout << "Start or end node not found.\n";
    }
    runTests();
    return 0;
}