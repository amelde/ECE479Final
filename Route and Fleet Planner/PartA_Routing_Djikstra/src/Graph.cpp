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

    using QueueItem = std::pair<double, std::string>;
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> frontier;
    std::unordered_map<std::string, double> distance;
    std::unordered_map<std::string, std::string> previous;

    for (const auto& [node, _] : adjacencyList) {
        distance[node] = std::numeric_limits<double>::infinity();
    }

    distance[start] = 0.0;
    frontier.push({0.0, start});

    while (!frontier.empty()) {
        const auto [currentCost, currentNode] = frontier.top();
        frontier.pop();

        if (currentCost > distance[currentNode]) {
            continue;
        }

        if (currentNode == goal) {
            break;
        }

        const auto& edges = adjacencyList.at(currentNode);
        for (const Edge& edge : edges) {
            if (edge.blocked) {
                continue;
            }

            const double nextCost = currentCost + edgeWeight(edge, weights);
            if (nextCost < distance[edge.to]) {
                distance[edge.to] = nextCost;
                previous[edge.to] = currentNode;
                frontier.push({nextCost, edge.to});
            }
        }
    }

    if (!std::isfinite(distance[goal])) {
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
    result.weightedCost = distance[goal];

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
