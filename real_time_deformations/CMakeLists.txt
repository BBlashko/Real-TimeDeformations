set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -W4 -std=gnu++11")

set(PROJECT ${CMAKE_CURRENT_LIST_DIR})
set(PROJECT_INCLUDE "${PROJECT}/include")
set(PROJECT_SHADERS "${PROJECT}/shaders")
set(PROJECT_SOURCE "${PROJECT}/src")
set(PROJECT_IMAGES "${PROJECT}/images")

add_subdirectory(${PROJECT_INCLUDE})
add_subdirectory(${PROJECT_SHADERS})
add_subdirectory(${PROJECT_SOURCE})
add_subdirectory(${PROJECT_IMAGES})

source_group("include" FILES ${PROJECT_INCLUDE_FILES})
source_group("shaders" FILES ${PROJECT_SHADER_FILES})
source_group("source" FILES ${PROJECT_SOURCE_FILES})

include_directories(${PROJECT_INCLUDE})
add_executable(real_time_deformations ${PROJECT_SOURCE_FILES} ${PROJECT_INCLUDE_FILES} ${PROJECT_SHADER_FILES})
target_link_libraries(real_time_deformations ${ATLAS_LIBRARIES} SOIL)
