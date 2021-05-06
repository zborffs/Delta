# Create executables
add_executable(MatplotTest matplot_test.cpp)
add_executable(mlpackTest mlpack_test.cpp)

# Link libraries to executables
target_link_libraries(MatplotTest Matplot++::matplot)
target_link_libraries(mlpackTest mlpack::mlpack)