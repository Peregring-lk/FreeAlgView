set(cmake_generated ${src_dir}/CMakeCache.txt
                    ${src_dir}/cmake_install.cmake
                    ${src_dir}/Makefile
                    ${src_dir}/CMakeFiles
)

foreach(file ${cmake_generated})

  if (EXISTS ${file})
    file(REMOVE_RECURSE ${file})
  endif()

endforeach(file)