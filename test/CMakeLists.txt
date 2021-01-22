add_executable(SimpleRecognitionStrategyTest unit_test/_recog/simple_recognition_strategy_test.cpp)

target_link_libraries(SimpleRecognitionStrategyTest Delta-lib ${GTEST_LIBRARIES} Threads::Threads Matplot++::matplot)

add_test(SimpleRecognitionStrategyTest SimpleRecognitionStrategyTest)
