# Create executables
add_executable(MatplotTest matplot_test.cpp)

# Link libraries to executables
target_link_libraries(MatplotTest Matplot++::matplot)