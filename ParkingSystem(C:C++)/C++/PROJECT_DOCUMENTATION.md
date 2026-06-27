# Parking Payment CLI - Project Documentation

## 1) Project overview

This project is a C++ command-line application for managing parking payments.  
It is the final OOP version of the previous C implementation and keeps the same core behavior:

- add a parking payment entry
- validate all user input
- store records in a local file
- display payment history

Main commands:

- `help`
- `history`
- `pay <car> <zone> <hours> <street>`

---

## 2) Technical requirements mapping

- **Use of classes and objects**: `Application`, `CommandHandler`, `HistoryRepository`, `ParkingRecord`, `ParkingZone`, validators
- **Code documentation**: comments in headers and implementation files for non-obvious logic
- **Clean code**: split into `include/` and `src/`; single-responsibility classes
- **File handling**: reading/writing `history.txt` in `HistoryRepository`
- **STL containers**: `std::vector<ParkingRecord>` for loaded history
- **Operator overloading**: `operator<<` for `ParkingRecord` display
- **Template usage**: generic `inInclusiveRange<T>()` in `Utils.hpp`
- **Class relationships**:
  - composition: `Application` owns `CommandHandler` and `HistoryRepository`
  - association: `CommandHandler` uses `HistoryRepository`
  - inheritance: concrete validators derive from `IValidator`
- **Functionality implementation**: help/history/payment flow implemented according to project description
- **Command-line parsing and validation**: performed in `CommandHandler::execute` and `handlePay`
- **Extra credit (smart pointers)**: `std::unique_ptr` used in `Application`

---

## 3) Application architecture

Flow:

1. `main.cpp` creates `Application`
2. `Application::run(argc, argv)` delegates to `CommandHandler`
3. `CommandHandler` parses command and validates arguments
4. `HistoryRepository` persists and loads records from `history.txt`
5. `ParkingRecord` models each entry and formats output/file lines

---

## 4) Data model and persistence

Each parking payment is stored as:

- car plate
- street
- zone
- hours
- calculated price

Storage format in `history.txt`:

`<car>|<street>|<zone>|<hours>|<price>`

Example:

`TM99XYZ|Main Street|Red|3|4.50`

---

## 5) Validation rules

### Car plate (Romanian format)

Supported standard formats:

- county: `CC + 2 digits + 3 letters` (example: `TM99XYZ`)
- Bucharest: `B + 2 digits + 3 letters` (example: `B12ABC`)
- Bucharest: `B + 3 digits + 3 letters` (example: `B123ABC`)

Rules:

- county code must be valid
- digits cannot be `00`/`000`
- letters cannot contain `Q`
- letter block cannot start with `I` or `O`
- `III` and `OOO` are rejected
- input normalization supports lowercase, spaces, and hyphens

### Zone

Allowed values:

- `red`
- `yellow`
- `green`

### Hours

- integer in range `1..24`

### Street

- must be non-empty

---

## 6) Pricing logic

Rates:

- yellow: `1.0` RON/hour
- red: `1.5` RON/hour
- green: `2.0` RON/hour

Final price = `rate * hours`

---

## 7) Build and run

Build:

```bash
make clean
make
```

Run examples:

```bash
./parking help
./parking history
./parking pay TM99XYZ red 3 "Main Street"
```

---

## 8) Test data

Use the following commands to quickly test both successful and failing scenarios.

### Valid test commands

```bash
./parking pay TM99XYZ red 3 "Main Street"
./parking pay B12ABC yellow 1 "Bulevardul Revolutiei"
./parking pay B123ABC green 2 "Calea Victoriei"
./parking pay CJ01ABC red 4 "Memorandumului Street"
./parking pay SV45MNP yellow 5 "Stefan cel Mare"
./parking history
```

### Invalid test commands (validation checks)

```bash
./parking pay BMW red 3 "Main Street"         # invalid Romanian plate format
./parking pay TM00XYZ red 3 "Main Street"     # invalid number group (00)
./parking pay TM99QXY red 3 "Main Street"     # invalid suffix (contains Q)
./parking pay TM99IXY red 3 "Main Street"     # invalid suffix (starts with I)
./parking pay TM99XYZ blue 3 "Main Street"    # invalid zone
./parking pay TM99XYZ red 30 "Main Street"    # invalid hours (>24)
./parking pay TM99XYZ red 3 ""                # invalid empty street
```

### Example records for `history.txt`

```txt
TM99XYZ|Main Street|Red|3|4.50
B12ABC|Bulevardul Revolutiei|Yellow|1|1.00
B123ABC|Calea Victoriei|Green|2|4.00
CJ01ABC|Memorandumului Street|Red|4|6.00
SV45MNP|Stefan cel Mare|Yellow|5|5.00
```

---

## 9) File-by-file explanation

This section explains each important file so a new reader can quickly understand the codebase.

### Root files

- `Makefile`  
  Build script for the project. Compiles all `.cpp` files into the executable `parking`.

- `history.txt`  
  Data file used as local storage for saved parking payments.

- `PROJECT_DOCUMENTATION.md`  
  Full project documentation (this file).

- `README.md`  
  Quick-start instructions (build/run) and documentation link.

### Header files (`include/`)

- `include/Application.hpp`  
  Declares `Application`, the top-level coordinator class. It owns the repository and command handler through `std::unique_ptr`.

- `include/CommandHandler.hpp`  
  Declares CLI parsing and command execution methods (`help`, `history`, `pay`).

- `include/HistoryRepository.hpp`  
  Declares file I/O operations for loading and appending parking records.

- `include/ParkingRecord.hpp`  
  Declares the data model for one parking entry and related serialization/parsing logic.

- `include/ParkingZone.hpp`  
  Declares zone validation and pricing utilities (rate per hour and display name).

- `include/Validators.hpp`  
  Declares concrete validators used by commands: zone, hours, and car plate validators.

- `include/IValidator.hpp`  
  Declares the abstract base validator interface (inheritance requirement).

- `include/RomanianPlate.hpp`  
  Declares Romanian plate-specific validation helpers and normalization logic.

- `include/Utils.hpp`  
  Contains reusable template helper `inInclusiveRange<T>()` for generic range checks.

- `include/AnsiColors.hpp`  
  Defines ANSI color constants used for selective terminal highlighting.

### Source files (`src/`)

- `src/main.cpp`  
  Program entry point. Creates `Application` and starts execution.

- `src/Application.cpp`  
  Builds core objects and forwards runtime control to `CommandHandler`.

- `src/CommandHandler.cpp`  
  Implements argument parsing, command dispatching, validation messages, and payment flow.

- `src/HistoryRepository.cpp`  
  Implements reading from and writing to `history.txt` using streams and `std::vector`.

- `src/ParkingRecord.cpp`  
  Implements record creation, text-file conversion, parsing, and `operator<<` for display.

- `src/ParkingZone.cpp`  
  Implements zone normalization, zone validation, and price-per-hour lookup.

- `src/Validators.cpp`  
  Implements zone/hour/plate validators and expected-format error text.

- `src/RomanianPlate.cpp`  
  Implements Romanian license plate rules (county/Bucharest formats, letter restrictions, normalization).

---

