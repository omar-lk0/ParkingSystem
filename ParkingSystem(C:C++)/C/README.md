# Online Parking Payment System

A simple terminal-based C app for paying for parking, viewing transaction history, deleting transactions, and managing user settings. Data is stored in a CSV file.

## Build & Run

```bash
gcc assignment_final.c -o parking
./parking
```

> **Note:** Before compiling, update the `FILENAME` path at the top of the source file to a valid path on your machine, it currently points to a fixed local path.

## Main Menu

| Option | Action |
|--------|--------|
| `1` | Pay for Parking |
| `2` | View History |
| `3` | Delete a Transaction |
| `4` | Settings |
| `0` | Exit |

## 1. Pay for Parking

You'll be prompted for:
- **License Plate** : at least 3 characters (e.g. `TM99XYZ`)
- **Zone** : `Yellow`, `Red`, or `Green` (case-insensitive)
  - Yellow: 1.0 RON/hour
  - Red: 1.5 RON/hour
  - Green: 2.0 RON/hour
- **Duration** : whole number of hours, ≥ 1

The cost is calculated automatically and saved to the transaction file.

## 2. View History

Lists all saved transactions with plate, zone, hours, and cost (converted to your selected currency).

## 3. Delete a Transaction

Shows the transaction list with index numbers, enter the number of the one you want to remove, or `0` to go back.

## 4. Settings

- **Change Username** : letters/spaces, up to 49 characters
- **Change Currency** : choose between RON, EUR, or USD; all costs convert automatically

## Notes

- Invalid input is rejected with a message explaining the expected format, just try again.
- Transactions persist across runs in the CSV file defined by `FILENAME`.
