# Glob for the inc and src directories in each module
file(GLOB SOURCE_FILES "src/*.cpp")

# Add each module as a library and set its dependencies
add_library(Pathfinder ${SOURCE_FILES})
target_include_directories(Pathfinder PUBLIC inc)
target_link_libraries(Pathfinder GridGenerator)
