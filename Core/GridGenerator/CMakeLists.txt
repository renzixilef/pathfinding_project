# Glob for the inc and src directories in each module
file(GLOB SOURCE_FILES "src/*.cpp")

# Add each module as a library and set its dependencies
add_library(GridGenerator ${SOURCE_FILES})
target_include_directories(GridGenerator PUBLIC inc)
target_link_libraries(GridGenerator Qt5::Widgets)
