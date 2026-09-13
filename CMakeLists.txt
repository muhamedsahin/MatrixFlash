cmake_minimum_required(VERSION 3.15)
project(MatrixFlash VERSION 1.0 LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MatrixLib STATIC
		 src/Matrix.cpp
		 src/MatrixArithmetic.cpp
		 src/MatrixActivation.cpp
		 src/MatrixTransforms.cpp
		 src/MatrixStatistics.cpp
		 src/MatrixInverse.cpp
		 src/MatrixDeterminant.cpp
		 src/MatrixMerging.cpp
		 src/MatrixBroadcasting.cpp
		 src/MatrixDecompose.cpp
		 src/MatrixEigenvalue.cpp
		 src/MatrixSerialization.cpp
)

add_library(MatrixFlash::Matrix ALIAS MatrixLib)

target_compile_features(MatrixLib PUBLIC cxx_std_17)
target_include_directories(MatrixLib
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Console
)

add_executable(MatrixApp src/main.cpp)

target_link_libraries(MatrixApp PRIVATE MatrixLib)

if (MINGW)
	target_link_options(MatrixApp PRIVATE -static-libgcc -static-libstdc++)
endif()

if (BUILD_TESTING)
	add_executable(MatrixTests tests/MatrixTests.cpp)
	target_include_directories(MatrixTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Console)
	target_link_libraries(MatrixTests PRIVATE MatrixLib)
	if (MINGW)
		target_link_options(MatrixTests PRIVATE -static-libgcc -static-libstdc++)
	endif()
	add_test(NAME MatrixTests COMMAND MatrixTests)
endif()

add_subdirectory(examples)

