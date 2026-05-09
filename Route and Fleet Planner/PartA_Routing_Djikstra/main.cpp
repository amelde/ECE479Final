#include "include/Planner.h"
#include "include/Robot.h"

#include <iostream>
#include <limits>
#include <string>

namespace {

void printStartupGuide() {
    std::cout << "==============================================\n"
              << " FOODIE Part A - Route and Fleet Planner\n"
              << "==============================================\n"
              << "This demo loads a sample campus map, 3 robots, and 3 pending orders.\n\n"
              << "Sample robots:\n"
              << "  - R1 at FW, capacity 6, battery 95%\n"
              << "  - R2 at FW, capacity 5, battery 80%\n"
              << "  - R3 at FW, capacity 4, battery 60%\n\n"
              << "Quality-of-life features added:\n"
              << "  - Help can be printed again with menu option 0\n"
              << "  - Node names are matched case-insensitively\n"
              << "  - Invalid destinations show the valid node list\n"
              << "  - Routes now show distance, time, and energy totals\n"
              << "  - Blocking or reopening a path auto-replans all assigned routes\n\n"
              << "Recommended demo flow:\n"
              << "  1, 2, 3, 5, 6, 7, 6, 8, 6\n\n"
              << "Tip: you can type node names like dorma, DORMA, or DormA.\n";
}

void printMenu() {
    std::cout << "\n=== FOODIE Part A: Route and Fleet Planner ===\n"
              << "0. Show help and valid nodes\n"
              << "1. Show map\n"
              << "2. Show robots\n"
              << "3. Show orders\n"
              << "4. Add order\n"
              << "5. Assign all pending orders\n"
              << "6. Show assignments and routes\n"
              << "7. Block path (auto replan all routes)\n"
              << "8. Reopen path (auto replan all routes)\n"
              << "9. Exit\n"
              << "Choose: ";
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool readNodeInput(const Planner& planner,
                   const std::string& prompt,
                   std::string& canonicalNode) {
    std::string rawNode;
    std::cout << prompt;
    std::cin >> rawNode;

    const auto resolved = planner.canonicalNodeName(rawNode);
    if (!resolved.has_value()) {
        std::cout << "Unknown node name: " << rawNode << '\n';
        planner.printValidNodes();
        return false;
    }

    canonicalNode = *resolved;
    return true;
}

} // namespace

int main() {
    Planner planner;
    planner.loadSampleCampusMap();
    planner.addRobot(Robot("R1", "FW", 6, 95.0));
    planner.addRobot(Robot("R2", "FW", 5, 80.0));
    planner.addRobot(Robot("R3", "FW", 4, 60.0));

    planner.addPendingOrder("DormA", 2);
    planner.addPendingOrder("Lab", 2);
    planner.addPendingOrder("Gym", 1);

    printStartupGuide();
    planner.printValidNodes();

    bool running = true;
    while (running) {
        printMenu();

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInput();
            std::cout << "Invalid input. Try again.\n";
            continue;
        }

        switch (choice) {
        case 0:
            printStartupGuide();
            planner.printValidNodes();
            break;
        case 1:
            planner.printMap();
            break;
        case 2:
            planner.printRobots();
            break;
        case 3:
            planner.printPendingOrders();
            break;
        case 4: {
            std::string destination;
            int loadUnits = 0;

            if (!readNodeInput(planner, "Enter destination node name: ", destination)) {
                break;
            }

            std::cout << "Enter load units: ";
            if (!(std::cin >> loadUnits) || loadUnits <= 0) {
                clearInput();
                std::cout << "Load must be a positive integer.\n";
                break;
            }

            if (!planner.addPendingOrder(destination, loadUnits)) {
                std::cout << "Could not add order. Check destination and load.\n";
                planner.printValidNodes();
                break;
            }
            std::cout << "Order added for " << destination << ".\n";
            break;
        }
        case 5: {
            const Planner::AssignmentSummary summary = planner.assignAllPendingOrders();
            if (summary.assignedNow == 0) {
                std::cout << "No new orders could be assigned. "
                          << summary.stillPending << " order(s) remain pending.\n";
                break;
            }

            std::cout << "Assignment finished: "
                      << summary.assignedNow << " new order(s) assigned, "
                      << summary.stillPending << " still pending, "
                      << summary.alreadyAssigned << " already assigned earlier.\n";
            break;
        }
        case 6:
            planner.printAssignments();
            break;
        case 7: {
            std::string a;
            std::string b;

            if (!readNodeInput(planner, "Enter first node of blocked path: ", a)) {
                break;
            }
            if (!readNodeInput(planner, "Enter second node of blocked path: ", b)) {
                break;
            }
            if (a == b) {
                std::cout << "Pick two different nodes.\n";
                break;
            }

            const auto status = planner.isPathBlocked(a, b);
            if (!status.has_value()) {
                std::cout << "No direct path exists between " << a << " and " << b << ".\n";
                break;
            }
            if (*status) {
                std::cout << "That path is already BLOCKED. No map change applied.\n";
                break;
            }
            if (!planner.blockPath(a, b, true)) {
                std::cout << "Path not found.\n";
                break;
            }

            const Planner::ReplanSummary summary = planner.replanAllRobots();
            std::cout << "Path " << a << " <-> " << b
                      << " blocked. Auto replan finished: "
                      << summary.replanned << " assigned robot(s) updated, "
                      << summary.unreachable << " route(s) unreachable, "
                      << summary.idle << " idle robot(s) refreshed.\n";
            break;
        }
        case 8: {
            std::string a;
            std::string b;

            if (!readNodeInput(planner, "Enter first node of path to reopen: ", a)) {
                break;
            }
            if (!readNodeInput(planner, "Enter second node of path to reopen: ", b)) {
                break;
            }
            if (a == b) {
                std::cout << "Pick two different nodes.\n";
                break;
            }

            const auto status = planner.isPathBlocked(a, b);
            if (!status.has_value()) {
                std::cout << "No direct path exists between " << a << " and " << b << ".\n";
                break;
            }
            if (!*status) {
                std::cout << "That path is already OPEN. No map change applied.\n";
                break;
            }
            if (!planner.blockPath(a, b, false)) {
                std::cout << "Path not found.\n";
                break;
            }

            const Planner::ReplanSummary summary = planner.replanAllRobots();
            std::cout << "Path " << a << " <-> " << b
                      << " reopened. Auto replan finished: "
                      << summary.replanned << " assigned robot(s) updated, "
                      << summary.unreachable << " route(s) unreachable, "
                      << summary.idle << " idle robot(s) refreshed.\n";
            break;
        }
        case 9:
            running = false;
            break;
        default:
            std::cout << "Choose a number from 0 to 9.\n";
            break;
        }
    }

    return 0;
}
