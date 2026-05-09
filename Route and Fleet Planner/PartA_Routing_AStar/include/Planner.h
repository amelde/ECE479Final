#ifndef PLANNER_H
#define PLANNER_H

#include "Graph.h"
#include "Order.h"
#include "Robot.h"

#include <optional>
#include <string>
#include <vector>

class Planner {
public:
    explicit Planner(std::string warehouseName = "FW");

    struct ReplanSummary {
        int totalRobots = 0;
        int replanned = 0;
        int unreachable = 0;
        int idle = 0;
    };

    struct AssignmentSummary {
        int assignedNow = 0;
        int alreadyAssigned = 0;
        int stillPending = 0;
    };

    struct RouteMetrics {
        double distance = 0.0;
        double time = 0.0;
        double energy = 0.0;
    };

    void loadSampleCampusMap();
    void addRobot(const Robot& robot);
    bool addPendingOrder(const std::string& destination, int loadUnits);

    AssignmentSummary assignAllPendingOrders();
    bool blockPath(const std::string& a, const std::string& b, bool blocked = true);
    bool replanRobot(const std::string& robotId, const std::string& newCurrentLocation = "");
    ReplanSummary replanAllRobots();

    std::optional<std::string> canonicalNodeName(const std::string& input) const;
    std::vector<std::string> nodeNames() const;
    std::optional<bool> isPathBlocked(const std::string& a, const std::string& b) const;
    RouteMetrics routeMetrics(const std::vector<std::string>& route) const;

    void printMap() const;
    void printRobots() const;
    void printPendingOrders() const;
    void printAssignments() const;
    void printValidNodes() const;

private:
    struct CandidateAssignment {
        std::size_t robotIndex = 0;
        double incrementalCost = 0.0;
    };

    std::string warehouse;
    Graph graph;
    CostWeights weights;
    std::vector<Robot> robots;
    std::vector<Order> pendingOrders;
    int nextOrderId = 1;

    std::optional<CandidateAssignment> findBestRobotForOrder(const Order& order) const;
    std::vector<std::string> buildGreedyRoute(const Robot& robot) const;
    static std::vector<std::string> appendPath(const std::vector<std::string>& base,
                                               const std::vector<std::string>& extension);
    std::optional<std::size_t> robotIndexById(const std::string& robotId) const;
    static std::string normalizeNodeToken(const std::string& input);
};

#endif
