#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <vector>

struct CostWeights {
    double distanceWeight = 0.5;
    double timeWeight = 1.0;
    double energyWeight = 0.8;
};

struct Edge {
    std::string from;
    std::string to;
    double distance = 0.0;
    double travelTime = 0.0;
    double energyCost = 0.0;
    bool blocked = false;
};

struct PathResult {
    bool found = false;
    double totalDistance = 0.0;
    double totalTime = 0.0;
    double totalEnergy = 0.0;
    double weightedCost = 0.0;
    std::vector<std::string> nodes;
};

class Graph {
public:
    void addUndirectedEdge(const std::string& a,
                           const std::string& b,
                           double distance,
                           double travelTime,
                           double energyCost);

    bool setEdgeBlocked(const std::string& a,
                        const std::string& b,
                        bool blocked);

    bool hasNode(const std::string& node) const;

    PathResult shortestPath(const std::string& start,
                            const std::string& goal,
                            const CostWeights& weights) const;

    std::vector<std::string> nodeNames() const;
    const Edge* findEdge(const std::string& from, const std::string& to) const;
    const std::unordered_map<std::string, std::vector<Edge>>& adjacency() const;

private:
    std::unordered_map<std::string, std::vector<Edge>> adjacencyList;

    static double edgeWeight(const Edge& edge, const CostWeights& weights);
};

#endif
