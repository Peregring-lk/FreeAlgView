find_program(Bisonc++_executable bisonc++ DOC "path to the bisonc++ executable")
mark_as_advanced(Bisonc++_executable)

if(Bisonc++_executable)

  execute_process(COMMAND ${Bisonc++_executable} --version
    OUTPUT_VARIABLE Bisonc++_version_output
    ERROR_VARIABLE Bisonc++_version_error
    RESULT_VARIABLE Bisonc++_version_result
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  ## Dangerous "EQUAL 1" (1 means fail). Due to a bisonc++ bug.
  if(NOT ${Bisonc++_version_result} EQUAL 1)
    if(Bisonc++_FIND_REQUIRED)
      message(SEND_ERROR "Command \"${Bisonc++_executable} --version\" failed with output:\n${Bisonc++_version_output}\n${Bisonc++_version_error}")
    else()
      message("Command \"${Bisonc++_executable} --version\" failed with output:\n${Bisonc++_version_output}\n${Bisonc++_version_error}\nBisonc++_version will not be available")
    endif()
  else()
    # older versions of bison printed "/full/path/to/executable version X.Y"
    # newer versions use "basename(executable) X.Y"
    get_filename_component(Bisonc++_exe_name "${Bisonc++_executable}" NAME)
    string(REPLACE "++" "\\+\\+" Bisonc++_exe_name ${Bisonc++_exe_name})
    string(REGEX REPLACE "^${Bisonc++_exe_name} V([0-9.]+)$" "\\1"
      Bisonc++_version "${Bisonc++_version_output}")
    unset(Bisonc++_exe_name)
  endif()

endif(Bisonc++_executable)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Bisonc++ REQUIRED_VARS Bisonc++_executable
                                           VERSION_VAR Bisonc++_version)

# FindBisonc++.cmake ends here