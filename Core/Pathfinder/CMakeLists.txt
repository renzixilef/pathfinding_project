find_program(GPRBUILD_EXECUTABLE NAMES gprbuild gnatmake)
if(NOT GPRBUILD_EXECUTABLE)
	message(FATAL_ERROR "gprbuild not found; please install GNAT or ensure gprbuild is on PATH")
endif()

include(ExternalProject)

set(ADA_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ada_backend)
set(ADA_PROJECT_GPR libpathfinding_ada.gpr)
set(ADA_OUTPUT_LIB ${ADA_PROJECT_DIR}/lib/libpathfinding_ada.so)

ExternalProject_Add(Pathfinder_Ada_Proj
	SOURCE_DIR ${ADA_PROJECT_DIR}
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ${GPRBUILD_EXECUTABLE} -p -P ${ADA_PROJECT_GPR}
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND ""
)

add_library(Pathfinder_C_ABI STATIC ${CMAKE_CURRENT_SOURCE_DIR}/c_abi/src/pathfinding_c_inf.cpp)
target_include_directories(Pathfinder_C_ABI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/c_abi/inc)
target_link_libraries(Pathfinder_C_ABI PRIVATE GridGenerator)

add_library(Pathfinder_Ada SHARED IMPORTED)
set_target_properties(Pathfinder_Ada PROPERTIES IMPORTED_LOCATION ${ADA_OUTPUT_LIB})
add_dependencies(Pathfinder_Ada Pathfinder_Ada_Proj Pathfinder_C_ABI)
target_link_libraries(Pathfinder_Ada INTERFACE Pathfinder_C_ABI)

file(GLOB SOURCE_FILES "src/*.cpp")
add_library(Pathfinder ${SOURCE_FILES})
target_include_directories(Pathfinder PUBLIC inc)
target_link_libraries(Pathfinder GridGenerator Pathfinder_Ada Pathfinder_C_ABI)

