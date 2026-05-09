#include "../include/Planner.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>

Planner::Planner(std::string warehouseName)
    : warehouse(std::move(warehouseName)) {}

void Planner::loadSampleCampusMap() {
    graph.addUndirectedEdge("FW", "I1", 1.0, 2.0, 1.5);
    graph.addUndirectedEdge("FW", "I2", 1.4, 2.6, 1.8);
    graph.addUndirectedEdge("I1", "DormA", 0.8, 1.5, 1.0);
    graph.addUndirectedEdge("I1", "Library", 1.0, 1.7, 1.2);
    graph.addUndirectedEdge("I2", "Lab", 0.9, 1.6, 1.1);
    graph.addUndirectedEdge("I2", "DormB", 1.2, 2.1, 1.4);
    graph.addUndirectedEdge("Library", "Cafeteria", 0.7, 1.2, 0.9);
    graph.addUndirectedEdge("DormA", "Cafeteria", 0.9, 1.6, 1.0);
    graph.addUndirectedEdge("Lab", "Gym", 0.8, 1.3, 0.9);
    graph.addUndirectedEdge("DormB", "Gym", 0.7, 1.1, 0.8);
    graph.addUndirectedEdge("Cafeteria", "Gym", 1.1, 1.9, 1.4);
}

void Planner::addRobot(const Robot& robot) {
    robots.push_back(robot);
}

bool Planner::addPendingOrder(const std::string& destination, int loadUnits) {
    if (loadUnits <= 0) {
        return false;
    }

    const auto canonical = canonicalNodeName(destination);
    if (!canonical.has_value()) {
        return false;
    }

    pendingOrders.push_back(Order{nextOrderId++, *canonical, loadUnits, false});
    return true;
}

Planner::AssignmentSummary Planner::assignAllPendingOrders() {
    AssignmentSummary summary;

    for (Order& order : pendingOrders) {
        if (order.assigned) {
            ++summary.alreadyAssigned;
            continue;
        }

        const auto candidate = findBestRobotForOrder(order);
        if (!candidate.has_value()) {
            ++summary.stillPending;
            continue;
        }

        Robot& robot = robots[candidate->robotIndex];
        robot.assignOrder(order);
        robot.setRoute(buildGreedyRoute(robot));
        order.assigned = true;
        ++summary.assignedNow;
    }

    summary.stillPending = 0;
    for (const Order& order : pendingOrders) {
        if (!order.assigned) {
            ++summary.stillPending;
        }
    }

    return summary;
}

bool Planner::blockPath(const std::string& a, const std::string& b, bool blocked) {
    return graph.setEdgeBlocked(a, b, blocked);
}

bool Planner::replanRobot(const std::string& robotId, const std::string& newCurrentLocation) {
    const auto index = robotIndexById(robotId);
    if (!index.has_value()) {
        return false;
    }

    Robot& robot = robots[*index];
    if (!newCurrentLocation.empty()) {
        const auto canonical = canonicalNodeName(newCurrentLocation);
        if (!canonical.has_value()) {
            return false;
        }
        robot.setCurrentLocation(*canonical);
    }

    if (robot.assignedOrders().empty()) {
        if (robot.currentLocation() == warehouse) {
            robot.setRoute({warehouse});
        } else {
            robot.setRoute({robot.currentLocation(), warehouse});
        }
        return true;
    }

    const std::vector<std::string> route = buildGreedyRoute(robot);
    if (route.empty()) {
        robot.setRoute({});
        return false;
    }

    robot.setRoute(route);
    return true;
}

Planner::ReplanSummary Planner::replanAllRobots() {
    ReplanSummary summary;
    summary.totalRobots = static_cast<int>(robots.size());

    for (Robot& robot : robots) {
        if (robot.assignedOrders().empty()) {
            if (robot.currentLocation() == warehouse) {
                robot.setRoute({warehouse});
            } else {
                robot.setRoute({robot.currentLocation(), warehouse});
            }
            ++summary.idle;
            continue;
        }

        const std::vector<std::string> route = buildGreedyRoute(robot);
        if (route.empty()) {
            robot.setRoute({});
            ++summary.unreachable;
            continue;
        }

        robot.setRoute(route);
        ++summary.replanned;
    }

    return summary;
}

std::optional<std::string> Planner::canonicalNodeName(const std::string& input) const {
    const std::string normalizedInput = normalizeNodeToken(input);
    if (normalizedInput.empty()) {
        return std::nullopt;
    }

    for (const std::string& node : graph.nodeNames()) {
        if (normalizeNodeToken(node) == normalizedInput) {
            return node;
        }
    }

    return std::nullopt;
}

std::vector<std::string> Planner::nodeNames() const {
    return graph.nodeNames();
}

std::optional<bool> Planner::isPathBlocked(const std::string& a, const std::string& b) const {
    const auto from = canonicalNodeName(a);
    const auto to = canonicalNodeName(b);
    if (!from.has_value() || !to.has_value()) {
        return std::nullopt;
    }

    const Edge* edge = graph.findEdge(*from, *to);
    if (edge == nullptr) {
        return std::nullopt;
    }

    return edge->blocked;
}

Planner::RouteMetrics Planner::routeMetrics(const std::vector<std::string>& route) const {
    RouteMetrics metrics;
    if (route.size() < 2) {
        return metrics;
    }

    for (std::size_t i = 0; i + 1 < route.size(); ++i) {
        const Edge* edge = graph.findEdge(route[i], route[i + 1]);
        if (edge == nullptr) {
            continue;
        }
        metrics.distance += edge->distance;
        metrics.time += edge->travelTime;
        metrics.energy += edge->energyCost;
    }

    return metrics;
}

void Planner::printMap() const {
    std::cout << "\nCampus map edges\n";
    std::cout << "----------------\n";

    std::set<std::pair<std::string, std::string>> printed;
    for (const auto& [from, edges] : graph.adjacency()) {
        for (const Edge& edge : edges) {
            const auto key = std::minmax(edge.from, edge.to);
            if (printed.find(key) != printed.end()) {
                continue;
            }
            printed.insert(key);

            std::cout << std::left << std::setw(10) << edge.from
                      << " <-> "
                      << std::left << std::setw(10) << edge.to
                      << " dist=" << edge.distance
                      << " time=" << edge.travelTime
                      << " energy=" << edge.energyCost
                      << " status=" << (edge.blocked ? "BLOCKED" : "OPEN")
                      << '\n';
        }
    }
}

void Planner::printRobots() const {
    std::cout << "\nRobots\n";
    std::cout << "------\n";
    for (const Robot& robot : robots) {
        std::cout << robot.id()
                  << " | location=" << robot.currentLocation()
                  << " | battery=" << robot.batteryPercent() << "%"
                  << " | used=" << robot.usedCapacity()
                  << "/" << robot.capacityUnits();

        if (robot.assignedOrders().empty()) {
            std::cout << " | status=IDLE";
        } else if (robot.route().empty()) {
            std::cout << " | status=ASSIGNED but route unreachable";
        } else {
            std::cout << " | status=ASSIGNED";
        }
        std::cout << '\n';
    }
}

void Planner::printPendingOrders() const {
    std::cout << "\nOrders\n";
    std::cout << "------\n";
    for (const Order& order : pendingOrders) {
        std::cout << "Order " << order.id
                  << " -> " << order.destination
                  << " | load=" << order.loadUnits
                  << " | " << (order.assigned ? "assigned" : "pending")
                  << '\n';
    }
}

void Planner::printAssignments() const {
    std::cout << "\nAssignments and routes\n";
    std::cout << "----------------------\n";

    for (const Robot& robot : robots) {
        std::cout << robot.id() << ":\n";
        if (robot.assignedOrders().empty()) {
            std::cout << "  No assigned orders\n";
            if (!robot.route().empty()) {
                if (robot.route().size() == 1 && robot.route().front() == warehouse) {
                    std::cout << "  Status: already at warehouse\n";
                } else {
                    std::cout << "  Return route: ";
                    for (std::size_t i = 0; i < robot.route().size(); ++i) {
                        std::cout << robot.route()[i];
                        if (i + 1 < robot.route().size()) {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << '\n';
                }
            }
            continue;
        }

        std::cout << "  Orders: ";
        for (std::size_t i = 0; i < robot.assignedOrders().size(); ++i) {
            const Order& order = robot.assignedOrders()[i];
            std::cout << "#" << order.id << "(" << order.destination << ", load="
                      << order.loadUnits << ")";
            if (i + 1 < robot.assignedOrders().size()) {
                std::cout << ", ";
            }
        }
        std::cout << '\n';

        const auto& route = robot.route();
        if (route.empty()) {
            std::cout << "  Route: UNREACHABLE after latest map change\n";
            continue;
        }

        std::cout << "  Route: ";
        for (std::size_t i = 0; i < route.size(); ++i) {
            std::cout << route[i];
            if (i + 1 < route.size()) {
                std::cout << " -> ";
            }
        }
        std::cout << '\n';

        const RouteMetrics metrics = routeMetrics(route);
        std::cout << std::fixed << std::setprecision(1)
                  << "  Route totals: distance=" << metrics.distance
                  << ", time=" << metrics.time
                  << ", energy=" << metrics.energy << '\n';
    }
}

void Planner::printValidNodes() const {
    std::cout << "Valid node names: ";
    const auto nodes = nodeNames();
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        std::cout << nodes[i];
        if (i + 1 < nodes.size()) {
            std::cout << ", ";
        }
    }
    std::cout << '\n';
}

std::optional<Planner::CandidateAssignment> Planner::findBestRobotForOrder(const Order& order) const {
    std::optional<CandidateAssignment> best;
    double bestCost = std::numeric_limits<double>::infinity();

    for (std::size_t i = 0; i < robots.size(); ++i) {
        const Robot& robot = robots[i];
        if (!robot.canCarry(order)) {
            continue;
        }

        const std::string start = robot.assignedOrders().empty()
            ? robot.currentLocation()
            : robot.assignedOrders().back().destination;

        const PathResult toDestination = graph.shortestPath(start, order.destination, weights);
        const PathResult returnToWarehouse = graph.shortestPath(order.destination, warehouse, weights);

        if (!toDestination.found || !returnToWarehouse.found) {
            continue;
        }

        const double extraCost = toDestination.weightedCost + returnToWarehouse.weightedCost;
        if (extraCost < bestCost) {
            bestCost = extraCost;
            best = CandidateAssignment{i, extraCost};
        }
    }

    return best;
}

std::vector<std::string> Planner::buildGreedyRoute(const Robot& robot) const {
    if (robot.assignedOrders().empty()) {
        if (robot.currentLocation() == warehouse) {
            return {warehouse};
        }
        return {robot.currentLocation(), warehouse};
    }

    std::vector<Order> remaining = robot.assignedOrders();
    std::vector<std::string> fullRoute{robot.currentLocation()};
    std::string current = robot.currentLocation();

    while (!remaining.empty()) {
        double bestCost = std::numeric_limits<double>::infinity();
        std::size_t bestIndex = 0;
        PathResult bestPath;

        for (std::size_t i = 0; i < remaining.size(); ++i) {
            const PathResult candidate = graph.shortestPath(current, remaining[i].destination, weights);
            if (candidate.found && candidate.weightedCost < bestCost) {
                bestCost = candidate.weightedCost;
                bestIndex = i;
                bestPath = candidate;
            }
        }

        if (!bestPath.found) {
            return {};
        }

        fullRoute = appendPath(fullRoute, bestPath.nodes);
        current = remaining[bestIndex].destination;
        remaining.erase(remaining.begin() + static_cast<std::ptrdiff_t>(bestIndex));
    }

    const PathResult returnPath = graph.shortestPath(current, warehouse, weights);
    if (!returnPath.found) {
        return {};
    }

    return appendPath(fullRoute, returnPath.nodes);
}

std::vector<std::string> Planner::appendPath(const std::vector<std::string>& base,
                                             const std::vector<std::string>& extension) {
    if (base.empty()) {
        return extension;
    }

    std::vector<std::string> combined = base;
    for (std::size_t i = 1; i < extension.size(); ++i) {
        combined.push_back(extension[i]);
    }
    return combined;
}

std::optional<std::size_t> Planner::robotIndexById(const std::string& robotId) const {
    for (std::size_t i = 0; i < robots.size(); ++i) {
        if (robots[i].id() == robotId) {
            return i;
        }
    }
    return std::nullopt;
}

std::string Planner::normalizeNodeToken(const std::string& input) {
    std::string normalized;
    normalized.reserve(input.size());

    for (const unsigned char ch : input) {
        if (std::isalnum(ch)) {
            normalized.push_back(static_cast<char>(std::tolower(ch)));
        }
    }

    return normalized;
}
