# Glob for the inc and src directories in each module
file(GLOB HEADER_FILES "inc/*.h")
file(GLOB SOURCE_FILES "src/*.cpp")

qt5_wrap_cpp(MOC_SRCS ${HEADER_FILES})

# Add each module as a library and set its dependencies
add_library(GUI ${SOURCE_FILES} ${MOC_SRCS})
target_include_directories(GUI PUBLIC inc)
target_link_libraries(GUI Qt5::Widgets Qt5::Core RunInterface GUI_widgets)
