# Create executables
add_executable(RecognitionAccuracy recognition_accuracy.cpp)

# Link libraries to executables
target_link_libraries(RecognitionAccuracy Delta-lib Threads::Threads spdlog::spdlog ${OpenCV_LIBS}) # Matplot++::matplot

#set(DUMB ${GENERIC_INCLUDE_DIR})
#set(BITCH ${APPLE})
#message("Shut up ${DUMB}")