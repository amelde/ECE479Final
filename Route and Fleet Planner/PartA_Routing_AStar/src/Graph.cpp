#include "../include/Graph.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <unordered_map>

void Graph::addUndirectedEdge(const std::string& a,
                              const std::string& b,
                              double distance,
                              double travelTime,
                              double energyCost) {
    const Edge forward{a, b, distance, travelTime, energyCost, false};
    const Edge reverse{b, a, distance, travelTime, energyCost, false};
    adjacencyList[a].push_back(forward);
    adjacencyList[b].push_back(reverse);
}

bool Graph::setEdgeBlocked(const std::string& a,
                           const std::string& b,
                           bool blocked) {
    bool updated = false;

    const auto applyUpdate = [&](const std::string& from, const std::string& to) {
        auto it = adjacencyList.find(from);
        if (it == adjacencyList.end()) {
            return;
        }
        for (Edge& edge : it->second) {
            if (edge.to == to) {
                edge.blocked = blocked;
                updated = true;
            }
        }
    };

    applyUpdate(a, b);
    applyUpdate(b, a);
    return updated;
}

bool Graph::hasNode(const std::string& node) const {
    return adjacencyList.find(node) != adjacencyList.end();
}

PathResult Graph::shortestPath(const std::string& start,
                               const std::string& goal,
                               const CostWeights& weights) const {
    PathResult result;

    if (!hasNode(start) || !hasNode(goal)) {
        return result;
    }

    const auto heuristic = relaxedShortestCostsToGoal(goal, weights);
    if (heuristic.find(start) == heuristic.end() || !std::isfinite(heuristic.at(start))) {
        return result;
    }

    struct QueueItem {
        double priority = 0.0;
        double pathCost = 0.0;
        std::string node;

        bool operator>(const QueueItem& other) const {
            return priority > other.priority;
        }
    };

    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> frontier;
    std::unordered_map<std::string, double> pathCost;
    std::unordered_map<std::string, std::string> previous;

    for (const auto& [node, _] : adjacencyList) {
        pathCost[node] = std::numeric_limits<double>::infinity();
    }

    pathCost[start] = 0.0;
    frontier.push({heuristic.at(start), 0.0, start});

    while (!frontier.empty()) {
        const QueueItem current = frontier.top();
        frontier.pop();

        if (current.pathCost > pathCost[current.node]) {
            continue;
        }

        if (current.node == goal) {
            break;
        }

        const auto adjacencyIt = adjacencyList.find(current.node);
        if (adjacencyIt == adjacencyList.end()) {
            continue;
        }

        for (const Edge& edge : adjacencyIt->second) {
            if (edge.blocked) {
                continue;
            }

            const double nextPathCost = current.pathCost + edgeWeight(edge, weights);
            if (nextPathCost >= pathCost[edge.to]) {
                continue;
            }

            const auto heuristicIt = heuristic.find(edge.to);
            if (heuristicIt == heuristic.end() || !std::isfinite(heuristicIt->second)) {
                continue;
            }

            pathCost[edge.to] = nextPathCost;
            previous[edge.to] = current.node;
            frontier.push({nextPathCost + heuristicIt->second, nextPathCost, edge.to});
        }
    }

    if (!std::isfinite(pathCost[goal])) {
        return result;
    }

    std::vector<std::string> reversedPath;
    for (std::string node = goal;; node = previous[node]) {
        reversedPath.push_back(node);
        if (node == start) {
            break;
        }
    }
    std::reverse(reversedPath.begin(), reversedPath.end());

    result.found = true;
    result.nodes = reversedPath;
    result.weightedCost = pathCost[goal];

    for (std::size_t i = 0; i + 1 < reversedPath.size(); ++i) {
        const std::string& from = reversedPath[i];
        const std::string& to = reversedPath[i + 1];
        for (const Edge& edge : adjacencyList.at(from)) {
            if (edge.to == to) {
                result.totalDistance += edge.distance;
                result.totalTime += edge.travelTime;
                result.totalEnergy += edge.energyCost;
                break;
            }
        }
    }

    return result;
}

std::vector<std::string> Graph::nodeNames() const {
    std::vector<std::string> nodes;
    nodes.reserve(adjacencyList.size());
    for (const auto& [node, _] : adjacencyList) {
        nodes.push_back(node);
    }
    std::sort(nodes.begin(), nodes.end());
    return nodes;
}

const Edge* Graph::findEdge(const std::string& from, const std::string& to) const {
    const auto it = adjacencyList.find(from);
    if (it == adjacencyList.end()) {
        return nullptr;
    }

    for (const Edge& edge : it->second) {
        if (edge.to == to) {
            return &edge;
        }
    }
    return nullptr;
}

const std::unordered_map<std::string, std::vector<Edge>>& Graph::adjacency() const {
    return adjacencyList;
}

double Graph::edgeWeight(const Edge& edge, const CostWeights& weights) {
    return edge.distance * weights.distanceWeight
         + edge.travelTime * weights.timeWeight
         + edge.energyCost * weights.energyWeight;
}

std::unordered_map<std::string, double> Graph::relaxedShortestCostsToGoal(
    const std::string& goal,
    const CostWeights& weights) const {
    using QueueItem = std::pair<double, std::string>;
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> frontier;
    std::unordered_map<std::string, double> optimisticCost;

    for (const auto& [node, _] : adjacencyList) {
        optimisticCost[node] = std::numeric_limits<double>::infinity();
    }

    const auto goalIt = adjacencyList.find(goal);
    if (goalIt == adjacencyList.end()) {
        return optimisticCost;
    }

    optimisticCost[goal] = 0.0;
    frontier.push({0.0, goal});

    while (!frontier.empty()) {
        const auto [currentCost, currentNode] = frontier.top();
        frontier.pop();

        if (currentCost > optimisticCost[currentNode]) {
            continue;
        }

        for (const Edge& edge : adjacencyList.at(currentNode)) {
            const double nextCost = currentCost + edgeWeight(edge, weights);
            if (nextCost < optimisticCost[edge.to]) {
                optimisticCost[edge.to] = nextCost;
                frontier.push({nextCost, edge.to});
            }
        }
    }

    return optimisticCost;
}
