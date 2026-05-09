# FOODIE Part A - Route and Fleet Planner

This is a simple, organized C++ implementation of Part A of the FOODIE final project.

## What it does

- Builds a campus-like map as a weighted graph
- Stores multiple delivery robots with capacity and battery limits
- Accepts pending orders with destination and load size
- Assigns each order to the cheapest feasible robot
- Computes routes using A* search
- Supports path blocking and automatic route replanning when roads are blocked or reopened
- Shows route totals for distance, travel time, and energy
- Accepts node names case-insensitively to make input easier

## Project structure

```text
PartA_Routing/
  include/
    Graph.h
    Order.h
    Robot.h
    Planner.h
  src/
    Graph.cpp
    Robot.cpp
    Planner.cpp
  main.cpp
  README.md
```

## Build

### g++

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp src/Graph.cpp src/Robot.cpp src/Planner.cpp -Iinclude -o routing.exe
```

## Run

```bash
./routing.exe
```

On Windows PowerShell, you can also run:

```powershell
.\routing.exe
```

## Usability improvements added

These extra features were added to make the program easier to use in a demo:

- **Help menu available anytime** with option `0`
- **Case-insensitive node input** such as `dorma`, `DORMA`, or `DormA`
- **Valid node list shown automatically** after invalid input
- **Safer path editing** by warning when a path is already open or already blocked
- **Assignment summary messages** after running the order assignment step
- **Route metrics shown** for each assigned robot

## How to use the program

When the program starts, it automatically loads:
- a sample campus map
- 3 sample robots: `R1`, `R2`, `R3`
- 3 sample pending orders

You will then see this menu:

```text
0. Show help and valid nodes
1. Show map
2. Show robots
3. Show orders
4. Add order
5. Assign all pending orders
6. Show assignments and routes
7. Block path (auto replan all routes)
8. Reopen path (auto replan all routes)
9. Exit
```

### Recommended demo order

1. **Show map**  
   Displays the campus nodes and weighted edges.

2. **Show robots**  
   Displays each robot's start location, capacity, battery, and status.

3. **Show orders**  
   Displays the current pending orders waiting to be assigned.

4. **Assign all pending orders**  
   The planner checks feasible robots and assigns each pending order to the cheapest one. It then prints a short summary telling you how many orders were assigned and how many remain pending.

5. **Show assignments and routes**  
   Displays which robot got each order, the route from its current location through deliveries and back to the warehouse, and the total route distance, time, and energy.

6. **Block path (auto replan all routes)**  
   Simulates a blocked road. Enter the two endpoint nodes of the blocked edge. The planner then automatically replans every assigned robot route using the latest map. If no alternate path exists for a robot, that route is marked unreachable.

7. **Reopen path (auto replan all routes)**  
   Reopens a previously blocked edge and automatically replans all assigned robot routes again, which may produce a shorter or newly feasible route.

8. **Help and valid nodes**  
   Option `0` prints the startup help again and shows all valid node names.

9. **Exit**  
   Ends the program.

## Input examples

### Add order
Example:

```text
Enter destination node name: library
Enter load units: 2
Order added for Library.
```

### Block path with automatic replanning
Example:

```text
Enter first node of blocked path: fw
Enter second node of blocked path: i1
Path FW <-> I1 blocked. Auto replan finished: 2 assigned robot(s) updated, 0 route(s) unreachable, 1 idle robot(s) refreshed.
```

### Reopen path with automatic replanning
Example:

```text
Enter first node of path to reopen: FW
Enter second node of path to reopen: I1
Path FW <-> I1 reopened. Auto replan finished: 2 assigned robot(s) updated, 0 route(s) unreachable, 1 idle robot(s) refreshed.
```

## Important notes

- The planner uses a weighted cost based on distance, time, and energy.
- The A* heuristic is an optimistic cost-to-go estimate computed on the relaxed map with all roads treated as open, so it stays valid after blockages.
- Robot IDs remain available in the data model, but the menu now auto-replans all routes after a path change.
- If a robot cannot carry an order because of capacity or route feasibility, that order stays unassigned.
- Blocking or reopening a path automatically recalculates every assigned route from each robot's stored current location.
- If no alternate path exists after a map change, that robot's route is shown as `UNREACHABLE after latest map change`.
- This is a clear course-project version, not a full industrial fleet optimizer.

## Suggested report wording

This implementation demonstrates a graph-based route and fleet planner for FOODIE. The campus is modeled as a weighted graph, robots are stored with capacity and battery constraints, and pending delivery orders are assigned to the cheapest feasible robot. A* search is used for routing, and the system automatically replans all assigned routes whenever a path is blocked or reopened.
