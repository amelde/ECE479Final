# FoodieSystem

FoodieSystem is split into separate C++ subsystems. Each subsystem has its own `main.cpp` and should be built as its own executable.

## Subsystems

| Subsystem | Entry point | Build task | Output |
| --- | --- | --- | --- |
| SPA beverage recommender | `SPA/main.cpp` | `Build SPA subsystem` | `SPA/SPA.exe` |
| Route Planner - Dijkstra | `Route and Fleet Planner/PartA_Routing_Djikstra/main.cpp` | `Build Route Planner - Dijkstra` | `Route and Fleet Planner/PartA_Routing_Djikstra/routing_dijkstra.exe` |
| Route Planner - A* | `Route and Fleet Planner/PartA_Routing_AStar/main.cpp` | `Build Route Planner - AStar` | `Route and Fleet Planner/PartA_Routing_AStar/routing_astar.exe` |

## Requirements

- Visual Studio Build Tools or Visual Studio with the MSVC C++ compiler
- VS Code with the C/C++ extension
- C++17 support

The VS Code tasks already include the required MSVC flag:

```text
/std:c++17
```

## Build a Subsystem

1. Open the project folder in VS Code.
2. Press `Ctrl+Shift+P`.
3. Choose `Tasks: Run Task`.
4. Select the subsystem you want to build.

Building only compiles the program. It does not automatically run the executable.

## Run a Subsystem

Run these commands from the project root in PowerShell.

### SPA

```powershell
.\SPA\SPA.exe
```

The SPA subsystem asks yes/no questions and recommends a beverage.

### Route Planner - Dijkstra

```powershell
& ".\Route and Fleet Planner\PartA_Routing_Djikstra\routing_dijkstra.exe"
```

The Dijkstra route planner opens a menu for viewing the map, robots, orders, assignments, and blocked paths.

### Route Planner - A*

```powershell
& ".\Route and Fleet Planner\PartA_Routing_AStar\routing_astar.exe"
```

The A* route planner has the same menu workflow as the Dijkstra planner, but routes are computed with A* search.

## Route Planner Menu

Both route planner subsystems use this menu:

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

For a quick demo, use:

```text
1, 2, 3, 5, 6, 7, 6, 8, 6, 9
```

## Notes

- Paths with spaces need `& "..."` in PowerShell.
- If a build succeeds but nothing else appears, that means the program was compiled but not run yet.
- Each subsystem builds independently, so choose the matching VS Code task for the `main.cpp` you want to use.
