This repository contains the modernized C++ financial calculator, originally a non-functional legacy codebase from 2009. The project focused on bringing the application up to modern C++ standards, implementing robust testing, and ensuring proper documentation and structure.

 **Project Overview**

The primary goals of this modernization effort were:

**Code Stability:** Resolve critical compilation and linking errors (including C++ standard compatibility).

**Quality Assurance:** Implement comprehensive unit testing using the GoogleTest framework.

**Maintainability:** Refactor the monolithic main.cpp into modular headers (Loan.h, Utils.h) and source files.

**Robustness:** Implement input validation, precision fixes (using long double), and externalize configuration using config.txt.

 **Build and Dependencies**

**Dependencies**

C++ Compiler: Requires C++17 support (e.g., g++ or clang).

Testing: Uses GoogleTest (included as a submodule/dependency).

Building the Application

The build process generates two executables: calc.exe (the main calculator) and run_tests.exe (the unit test suite).

Compile the Main Application:

Command to compile.
g++ -std=c++17 src/main.cpp src/Loan.cpp src/Utils.cpp -o calc.exe


Compile the Unit Tests:

command used to compile test.
g++ -std=c++17 -I./googletest/googletest/include test/test_emi.cpp src/Loan.cpp src/Utils.cpp -o run_tests.exe [insert_gtest_link_flags_here]


** Running the Application**

The calculator can be run using either configuration defaults or command-line arguments.

Option 1: Using Command-Line Arguments (Recommended)

The program uses a modern flag-based interface for clear parameter passing:

Format:

./calc.exe -a <amount> -i <interest_percent> -n <months>


Example (20,000 principal, 4% interest, 60 months):

$ ./calc.exe -a 20000 -i 4 -n 60


Option 2: Using Configuration File

If no command-line arguments are provided, the program reads the loan parameters from the config.txt file located in the root directory.

 Running Tests

The full suite of unit tests, covering normal calculation, invalid input, and large tenure precision, can be run using the following command:

$ ./run_tests


Expected Output: All 4 implemented tests should pass, validating the application's correctness and stability.

 Project Structure

The repository adheres to the required structure, separating source code, tests, and documentation:

legacy-calc-2009/
├── src/            # Core C++ Source Files (main.cpp, Loan.*, Utils.*)
├── test/           # Unit Test Source Files (test_emi.cpp)
├── config.txt      # External configuration file
├── docs/           # Doxygen generated HTML documentation
├── Doxyfile        # Doxygen Configuration File
├── README.md       # This file
└── REPORT.pdf      # Final project report


