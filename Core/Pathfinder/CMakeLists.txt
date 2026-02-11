find_program(GPRBUILD_EXECUTABLE NAMES gprbuild gnatmake)
if(NOT GPRBUILD_EXECUTABLE)
	message(FATAL_ERROR "gprbuild not found; please install GNAT or ensure gprbuild is on PATH")
endif()

include(ExternalProject)

set(ADA_OUTPUT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/ada_backend/lib/libpathfinding_ada.so")
set(ADA_PROJECT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ada_backend/libpathfinding_ada.gpr")

add_custom_command(
  OUTPUT "${ADA_OUTPUT_LIB}"
  COMMAND gprbuild -p -P "${ADA_PROJECT_FILE}"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  COMMENT "Building Ada library via gprbuild"
  VERBATIM
)

add_custom_target(Pathfinder_Ada_Proj ALL
  DEPENDS "${ADA_OUTPUT_LIB}"
)

add_library(Pathfinder_C_ABI STATIC ${CMAKE_CURRENT_SOURCE_DIR}/c_abi/src/pathfinding_c_inf.cpp)
target_include_directories(Pathfinder_C_ABI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/c_abi/inc)

add_library(Pathfinder_Ada SHARED IMPORTED GLOBAL)
set_target_properties(Pathfinder_Ada PROPERTIES
  IMPORTED_LOCATION "${ADA_OUTPUT_LIB}"
)

add_dependencies(Pathfinder_Ada Pathfinder_Ada_Proj Pathfinder_C_ABI)

target_link_libraries(Pathfinder_Ada INTERFACE Pathfinder_C_ABI)
add_dependencies(Pathfinder_Ada Pathfinder_Ada_Proj Pathfinder_C_ABI)
target_link_libraries(Pathfinder_C_ABI PRIVATE GridGenerator)

file(GLOB SOURCE_FILES "src/*.cpp")
add_library(Pathfinder ${SOURCE_FILES})
target_include_directories(Pathfinder PUBLIC inc)
target_link_libraries(Pathfinder GridGenerator Pathfinder_Ada Pathfinder_C_ABI)

