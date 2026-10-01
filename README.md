# Grocery Frequency Tracker · C++

A small console program that reads a grocery purchase list, counts how often each item appears, and offers a lookup, a complete frequency list and a text histogram. It also writes the counts to `frequency.dat` when it starts.

**Available here** — complete C++ source, an original sample input and focused regression checks. No external libraries are required.

## Build and run

Use a C++17 compiler. From the repository folder, build with GCC or Clang and run the result.

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o grocery-tracker
./grocery-tracker
```

On Windows with MinGW, build with `g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o grocery-tracker.exe` and run `.\grocery-tracker.exe` in PowerShell. The automated checks were run on Linux; Windows execution was not tested.

Keep `groceries.txt` in the current working directory. The included sample produces the following counts, in alphabetical order.

```text
Apples 3
Bread 1
Milk 1
Pears 2
```

Choose **1** and enter `Apples` to see its count, **2** for all counts, **3** for the histogram, or **4** to exit. For example, the histogram displays `Apples ***` for the three apple entries. An item that is absent returns a count of zero.

## Input and output

The input is a whitespace-separated list of single-word item names. The sample uses one name per line, though spaces and tabs also separate entries. Matching is case-sensitive, so `Apples` and `apples` are different items. Multiword product names and CSV records are outside this version's scope.

Each run creates or overwrites `frequency.dat` in the current directory. It contains the same sorted counts as menu option 2. Startup stops with an error if the input cannot be read or the output cannot be written. An empty input file produces an empty count list and backup.

## Implementation

`GroceryTracker` holds the counts and handles file loading, lookup and display. The menu stays in `main()`. The output methods accept an `ostream` by reference, which lets the same frequency-list function write to either the console or the backup file.

The collection is a `std::map`, an ordered associative container. It provides sorted iteration and logarithmic lookup and insertion in the number of distinct items. It is not a hash table. The histogram prints one asterisk per occurrence, which suits a small input but can produce long lines for large counts.

## Verification and cleanup

The portfolio pass retains the original class and menu structure. It changes menu and lookup input to whole-line validation, handles closed input at both prompts, and checks file read and backup write/close failures. Inputs such as `4junk` are rejected before a menu action runs.

The C++17 build completed without warnings on October 1, 2026. Seven focused regression tests passed, covering lookup, sorted counts, histogram and backup contents, malformed input, closed input, a missing input file and backup failures.

To rerun the checks after compiling, use Python 3 with no additional packages.

```bash
python3 test_tracker.py ./grocery-tracker
```

The backup-write failure check uses Linux's `/dev/full` device and is skipped where unavailable. These checks cover the main workflows and the cleanup changes, rather than every possible input or filesystem condition.

## Background and contribution

This began as my SNHU CS-210 coursework for a fictional grocery business. I implemented the tracker around file input/output, class design, maps and the different ways of displaying the results. The supplied assignment input has been replaced with a small original example.

My original reflection acknowledged ChatGPT assistance with input-buffer handling, which I reviewed before using. AI assistance was also used for this portfolio review, focused fixes, regression checks and documentation. The original school repository remains separate from this edition.
