cmake_minimum_required(VERSION 3.23)
project(SelfDrivingCar)

set(CMAKE_CXX_STANDARD 14)

add_executable(SelfDrivingCar main.cpp Car.cpp Car.h utils/MathUtils.cpp utils/MathUtils.h Track.cpp Track.h utils/BezierUtils.cpp utils/BezierUtils.h utils/CollisionUtils.cpp utils/CollisionUtils.h neuralnetwork/NeuralNetwork.cpp neuralnetwork/NeuralNetwork.h neuralnetwork/Matrix.cpp neuralnetwork/Matrix.h neuralnetwork/Layer.cpp neuralnetwork/Layer.h Population.cpp Population.h UI.cpp UI.h)
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(SelfDrivingCar sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
