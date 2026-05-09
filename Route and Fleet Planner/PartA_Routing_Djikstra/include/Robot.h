#ifndef ROBOT_H
#define ROBOT_H

#include "Order.h"

#include <string>
#include <vector>

class Robot {
public:
    Robot() = default;
    Robot(std::string id,
          std::string startLocation,
          int capacityUnits,
          double batteryPercent);

    const std::string& id() const;
    const std::string& currentLocation() const;
    int capacityUnits() const;
    int usedCapacity() const;
    int remainingCapacity() const;
    double batteryPercent() const;

    bool canCarry(const Order& order) const;

    void assignOrder(const Order& order);
    void clearAssignments();
    void setRoute(const std::vector<std::string>& newRoute);
    void setCurrentLocation(const std::string& location);
    void setBatteryPercent(double battery);

    const std::vector<Order>& assignedOrders() const;
    const std::vector<std::string>& route() const;

private:
    std::string robotId;
    std::string location;
    int maxCapacityUnits = 0;
    int usedCapacityUnits = 0;
    double battery = 100.0;
    std::vector<Order> orders;
    std::vector<std::string> plannedRoute;
};

#endif
