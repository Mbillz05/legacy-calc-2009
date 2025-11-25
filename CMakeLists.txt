# This is the custom CMakeLists.txt for your legacy-calc-2009 project
cmake_minimum_required(VERSION 3.16)
project(LoanCalculatorTestProject CXX)

# Include the directory containing the Google Test source files
# This is often done by pointing to the directory that has the GTest CMakeLists.txt
add_subdirectory(googletest)

# Add your main source code files (e.g., Loan.cpp and Utils.cpp)
# Assuming they are in a 'src' folder
add_library(loan_calculator src/Loan.cpp src/Utils.cpp)
target_include_directories(loan_calculator PUBLIC src)

# Add the test executable
# Assuming your test file is named test_loan.cpp and is in the 'src/test' folder
add_executable(run_tests test/test_emi.cpp)

# Link your test executable to the library and the Google Test libraries
target_link_libraries(run_tests 
    loan_calculator 
    gtest_main 
    gmock
)

# Enable testing for CTest
enable_testing()