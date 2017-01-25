cmake_minimum_required(VERSION 2.8)
project(stylit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES
        src/utils.h
        src/patch_distance.h
        src/quadruplet.h
        src/quadruplet.cpp
        src/single_scale_painter.h
        src/single_scale_painter.cpp
        src/multi_scale_painter.h
        src/multi_scale_painter.cpp
        src/logger.h
        src/regression.h
        src/patch_server.h
        src/restricted_nnf.h
        src/uniform_nnf.h
        src/misc.h src/misc.cpp
        )

set(TEST_FILES
        test/test_experiments.cpp
        test/test_regression.cpp
        test/test_patch_server.cpp
        test/test_misc.cpp
        )


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/test
        ${CMAKE_CURRENT_SOURCE_DIR}/cv-utils/include
        ${CMAKE_CURRENT_SOURCE_DIR}/patch_match/include
        ${CMAKE_CURRENT_SOURCE_DIR}/patch_match/core/include
)


add_executable(run_tests test/main.cpp ${SOURCE_FILES} ${TEST_FILES})


# EXTERNAL LIBRAIRIES =====================

# OpenCV
FIND_PACKAGE(OpenCV 3.1 REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
set(TARGET_LIBS ${TARGET_LIBS} ${OpenCV_LIBS})

find_package(Boost 1.36.0 COMPONENTS filesystem system)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(TARGET_LIBS ${TARGET_LIBS} ${Boost_LIBRARIES})
endif ()


TARGET_LINK_LIBRARIES(run_tests ${TARGET_LIBS})