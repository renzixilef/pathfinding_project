# Glob for the inc and src directories in each module
file(GLOB SOURCE_FILES "src/*.cpp")

# Add each module as a library and set its dependencies
add_library(Application ${SOURCE_FILES})
target_include_directories(Application PUBLIC inc)
target_link_libraries(Application Qt5::Core RunInterface)
