
MESSAGE(STATUS "building main")

FILE(GLOB SOURCES "*.cu" "*.cpp" "*.c" "*.h")

if(ENABLE_OMP)
       find_package(OpenMP REQUIRED)
       set(CMAKE_C_FLAGS "${OpenMP_C_FLAGS} ${CMAKE_C_FLAGS}")
       set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_C_FLAGS}")
endif(ENABLE_OMP)

find_package(MPI REQUIRED)
# find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR} ${MPI_INCLUDE_PATH})

add_executable(cnstrec ${SOURCES})

# list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

target_link_libraries(cnstrec ${MPI_LIBRARIES} mrcmx)# mrcimg)



