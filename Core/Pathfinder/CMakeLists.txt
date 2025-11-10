# Glob for the inc and src directories in each module
file(GLOB SOURCE_FILES "src/*.cpp")

file(GLOB ADA_SOURCE_FILES "ada_backend/src/*.adb" "ada_backend/src/*.ads")

# Add each module as a library and set its dependencies
add_library(Pathfinder ${SOURCE_FILES})
target_include_directories(Pathfinder PUBLIC inc)
target_link_libraries(Pathfinder GridGenerator)

set(CMAKE_Ada_CREATE_STATIC_LIBRARY ON)

add_library(Pathfinder_Ada STATIC ${ADA_SOURCE_FILES})
target_include_directories(Pathfinder_Ada PUBLIC ada_backend/inc)
target_link_libraries(Pathfinder Pathfinder_Ada)
set_target_properties(Pathfinder_Ada PROPERTIES LINKER_LANGUAGE Ada)
set_target_properties(Pathfinder_Ada PROPERTIES COMPILE_OPTIONS "-gnaty")
