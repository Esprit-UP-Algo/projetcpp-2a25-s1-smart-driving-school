# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\tttttetetetetet_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\tttttetetetetet_autogen.dir\\ParseCache.txt"
  "tttttetetetetet_autogen"
  )
endif()
