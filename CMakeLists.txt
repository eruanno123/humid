
cmake_minimum_required (VERSION 3.18)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set (CXX_STANDARD 17)
set (CXX_STANDARD_REQUIRED ON)

include (CPM)

project (HumidityTracker LANGUAGES C CXX)

add_subdirectory (software)
