# Glob for the inc and src directories in each module
file(GLOB HEADER_FILES "inc/*.h")
file(GLOB SOURCE_FILES "src/*.cpp")

qt5_wrap_cpp(MOC_SRCS ${HEADER_FILES})

# Add each module as a library and set its dependencies
add_library(GUI_widgets ${SOURCE_FILES} ${MOC_SRCS})
target_include_directories(GUI_widgets PUBLIC inc)
target_link_libraries(GUI_widgets GUI GridGenerator RunInterface Pathfinder Qt5::Core Qt5::Widgets ${OPENCV_LIBS})
