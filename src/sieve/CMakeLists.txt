cmake_minimum_required(VERSION 3.18)
project(NumericaLib C)

set(CMAKE_C_STANDARD 23)

# --- Configure Python Interpreter Location ---
set(Python3_EXECUTABLE ".venv/bin/python3") # Adjust if your Python is in a different location

# --- Find Python for Extension Module ---
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# --------------------------------------------------
# 🧩 Core library (shared between Python + CLI)
# --------------------------------------------------
add_library(corelib STATIC
        sieve.c
        sieve.h
        sieve_exceptions.c
        sieve_exceptions.h
)

target_include_directories(corelib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# --------------------------------------------------
# 🐍 Python extension module
# --------------------------------------------------
add_library(python_c_extension MODULE
        python_wrapper.c
)

target_include_directories(python_c_extension PRIVATE
        ${Python3_INCLUDE_DIRS} # This line ensures python.h is found
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(python_c_extension PRIVATE
        corelib
        ${Python3_LIBRARIES}
)

set_target_properties(python_c_extension PROPERTIES
        PREFIX ""
        SUFFIX "${Python3_EXTENSION_SUFFIX}"
)

add_custom_target(run_setup_py_build ALL
        COMMAND ${Python3_EXECUTABLE} setup.py build_ext --inplace bdist_wheel
        COMMAND ${Python3_EXECUTABLE} setup.py install
        COMMAND ${Python3_EXECUTABLE} setup.py clean --all
#        COMMAND rm -rf *egg-info
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running python3 setup.py build as part of CMake build"
        DEPENDS corelib # Ensure corelib is built first
)

# --------------------------------------------------
# 🖥️ CLI executable
# --------------------------------------------------
add_executable(NumericaLib
        main.c
)

target_include_directories(NumericaLib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Python3_INCLUDE_DIRS} # Included here as well, remove if CLI doesn't interact with Python
)

target_link_libraries(NumericaLib PRIVATE
        corelib
        ${Python3_LIBRARIES} # Included here as well, remove if CLI doesn't interact with Python
)