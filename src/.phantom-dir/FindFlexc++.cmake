find_program(Flexc++_executable flexc++ DOC "path to the flexc++ executable")
mark_as_advanced(Flexc++_executable)

if(Flexc++_executable)

  execute_process(COMMAND ${Flexc++_executable} --version
    OUTPUT_VARIABLE Flexc++_version_output
    ERROR_VARIABLE Flexc++_version_error
    RESULT_VARIABLE Flexc++_version_result
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  ## Dangerous "EQUAL 1" (1 means fail). Due to a flexc++ bug.
  if(${Flexc++_version_result} EQUAL 1)
    if(Flexc++_FIND_REQUIRED)
      message(SEND_ERROR "Command \"${Flexc++_executable} --version\" failed with output:\n${Flexc++_version_output}\n${Flexc++_version_error}")
    else()
      message("Command \"${Flexc++_executable} --version\" failed with output:\n${Flexc++_version_output}\n${Flexc++_version_error}\nflexc++_version will not be available")
    endif()
  else()
    get_filename_component(Flexc++_exe_name "${Flexc++_executable}" NAME)
    string(REPLACE "++" "\\+\\+" Flexc++_exe_name ${Flexc++_exe_name})
    string(REGEX REPLACE "^${Flexc++_exe_name} V([0-9.]+)$" "\\1"
      Flexc++_version "${Flexc++_version_output}")
    unset(Flexc++_exe_name)

#    string(REGEX REPLACE "0([0-9]+)" "\\1"
#      Flexc++_version "${Flexc++_version}")

  endif()

endif(Flexc++_executable)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Flexc++ REQUIRED_VARS Flexc++_executable
                                          VERSION_VAR Flexc++_version)

# FindFlexc++.cmake ends here