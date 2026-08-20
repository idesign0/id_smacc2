cmake_minimum_required(VERSION 3.5)
project(smacc2)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(smacc2_msgs REQUIRED)
find_package(Boost COMPONENTS thread REQUIRED)
find_package(tracetools)
# LTTng userspace tracing is Linux-only (no macOS/Windows port). smacc2's tracing
# wrappers already no-op without it: tracetools is built with TRACETOOLS_DISABLED on
# non-Linux, so TRACETOOLS_TRACEPOINT() expands to a no-op and smacc_tracing.cpp's
# TRACETOOLS_LTTNG_ENABLED branch is never taken (smacc2 never defines that macro).
# Only the CTF probe-provider TU (src/smacc2/trace_provider.cpp) hard-includes
# <lttng/tracepoint.h>, so make LTTng optional and drop that single TU when absent.
# LTTNGUST_INCLUDE_DIRS / LTTNGUST_LIBRARIES are empty when not found (harmless).
find_package(LTTngUST QUIET)
if(NOT LTTngUST_FOUND)
  # FindLTTngUST sets these to *-NOTFOUND (not empty) on failure. Leaving them poisons
  # include_directories()/target_link_libraries() below with a NOTFOUND token, which
  # CMake rejects at generate time. Force them empty when LTTng is absent (macOS/arm64).
  set(LTTNGUST_INCLUDE_DIRS "")
  set(LTTNGUST_LIBRARIES "")
endif()

set(dependencies
  rclcpp
  smacc2_msgs
  rclcpp_action
  tracetools
)

include_directories(include ${LTTNGUST_INCLUDE_DIRS})

file(GLOB_RECURSE SRC_FILES src *.cpp)
if(NOT LTTngUST_FOUND)
  # trace_provider.cpp is the LTTng CTF probe-definition TU; it is only referenced by
  # the (never-taken) TRACETOOLS_LTTNG_ENABLED path, so exclude it when LTTng is absent.
  list(FILTER SRC_FILES EXCLUDE REGEX "trace_provider\\.cpp$")
endif()
add_library(smacc2 SHARED ${SRC_FILES})

# removes unused parameters warning related with lttng, compiler warning
set_source_files_properties(
  src/smacc2/smacc_tracing.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-unused-parameter"
)

ament_target_dependencies(smacc2 ${dependencies})
target_link_libraries(smacc2
  ${Boost_LIBRARIES}
  ${LTTNGUST_LIBRARIES}
)

ament_export_include_directories(include)
ament_export_libraries(smacc2)
ament_export_dependencies(${dependencies})

install(
  DIRECTORY include/
  DESTINATION include
)

install(TARGETS
  smacc2
  DESTINATION lib/)

install(FILES
  scripts/trace.sh
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
