string(REPLACE " " ";" fau_phantoms "${fau_phantoms}")

if (NOT ("${fau_phantoms}" STREQUAL ""))

  foreach(file ${fau_phantoms})

    if(EXISTS ${file})
      file(REMOVE ${file})
    endif()

  endforeach(file)

endif()