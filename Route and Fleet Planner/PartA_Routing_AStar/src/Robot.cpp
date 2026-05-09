#include "../include/Robot.h"

Robot::Robot(std::string id,
             std::string startLocation,
             int capacityUnits,
             double batteryPercent)
    : robotId(std::move(id)),
      location(std::move(startLocation)),
      maxCapacityUnits(capacityUnits),
      battery(batteryPercent) {}

const std::string& Robot::id() const {
    return robotId;
}

const std::string& Robot::currentLocation() const {
    return location;
}

int Robot::capacityUnits() const {
    return maxCapacityUnits;
}

int Robot::usedCapacity() const {
    return usedCapacityUnits;
}

int Robot::remainingCapacity() const {
    return maxCapacityUnits - usedCapacityUnits;
}

double Robot::batteryPercent() const {
    return battery;
}

bool Robot::canCarry(const Order& order) const {
    return order.loadUnits <= remainingCapacity() && battery >= 20.0;
}

void Robot::assignOrder(const Order& order) {
    orders.push_back(order);
    usedCapacityUnits += order.loadUnits;
}

void Robot::clearAssignments() {
    orders.clear();
    plannedRoute.clear();
    usedCapacityUnits = 0;
}

void Robot::setRoute(const std::vector<std::string>& newRoute) {
    plannedRoute = newRoute;
}

void Robot::setCurrentLocation(const std::string& newLocation) {
    location = newLocation;
}

void Robot::setBatteryPercent(double newBattery) {
    battery = newBattery;
}

const std::vector<Order>& Robot::assignedOrders() const {
    return orders;
}

const std::vector<std::string>& Robot::route() const {
    return plannedRoute;
}
