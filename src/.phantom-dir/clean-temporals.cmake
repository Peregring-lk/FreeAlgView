file(GLOB_RECURSE temporal_files "*[~#]")

if (NOT ("${temporal_files}" STREQUAL ""))
  file(REMOVE ${temporal_files})
endif()