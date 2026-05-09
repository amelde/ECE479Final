# SPA Subsystem

The SPA subsystem is a small rule-based expert system that recommends a beverage for a guest. It asks yes/no questions about the guest, meal, event, weather, and preferences, then prints the recommended beverage.

## Files

```text
SPA/
  main.cpp
  SPASystem.cpp
  SPASystem.h
  Rule.cpp
  Rule.h
  Beverage.cpp
  Beverage.h
  Fact.h
  Condition.h
```

## Build in VS Code

1. Press `Ctrl+Shift+P`.
2. Choose `Tasks: Run Task`.
3. Select `Build SPA subsystem`.

This creates:

```text
SPA/SPA.exe
```

## Run

From the project root in PowerShell:

```powershell
.\SPA\SPA.exe
```

## How to Use

Answer each question with `y` or `n`.

Example:

```text
Is the guest health-conscious? (y/n): y
Does the guest have a citrus allergy? (y/n): n
Recommended beverage: Orange Juice
```

If no specific rule matches the answers, the system recommends bottled water by default.
