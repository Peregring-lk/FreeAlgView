####### -> Preliminaries
if(NOT (DEFINED phantom_dir))
  set(phantom_dir ${src_dir}/.phantom-dir)
endif()

# Includes for finding libraries.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${phantom_dir}")

#include(${phantom_dir}/FindFlexc++.cmake)
#include(${phantom_dir}/FindBisonc++.cmake)

macro(call_phantoms)
  ####### -> Beginning of the action.
  # Applying Qt4.
  set(QT_USE_QTOPENGL TRUE)
  include(${QT_USE_FILE})
  add_definitions(${QT_DEFINITIONS})

  # Applying Flexc++ ("l" means lexer).
  set(faupp_l_desc      ${lexer_dir}/lexer.ll)

  set(faupp_l_h_gen     ${phantom_dir}/faupp-lexer-phantom.hpp)
  set(faupp_l_s_gen     ${phantom_dir}/faupp-lexer-phantom.cpp)

  set(faupp_l_gens      ${faupp_l_h_gen} ${faupp_l_s_gen})

  add_custom_command(OUTPUT ${faupp_l_gens}
    COMMAND ${Flexc++_executable}
    ARGS ${Flexc++_args} ${faupp_l_desc}
    COMMAND ${CMAKE_COMMAND}
    ARGS -Dphantom_dir="${phantom_dir}" -P ${phantom_dir}/faupp-lexer-trick.cmake
    DEPENDS ${faupp_l_desc}
    COMMENT "[Flexc++][Faupp] Building Faupp lexer with flexc++ ${flexcpp_version}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

  # Applying Bisonc++ ("p" means parser).
  set(faupp_p_desc      ${parser_dir}/faupp.yy
                        ${parser_dir}/grammar/script.yy
                        ${parser_dir}/grammar/exprs.yy
                        ${parser_dir}/grammar/stmnts.yy
                        ${parser_dir}/grammar/atomic.yy
                        ${parser_dir}/grammar/arithmetical.yy
                        ${parser_dir}/grammar/relational.yy
                        ${parser_dir}/grammar/unary.yy
                        ${parser_dir}/grammar/logical.yy
                        )

  set(faupp_p_h_gen     ${phantom_dir}/faupp-parser-phantom.hpp)
  set(faupp_p_s_gen     ${phantom_dir}/faupp-parser-phantom.cpp)

  set(faupp_p_gens      ${faupp_p_h_gen} ${faupp_p_s_gen})

  add_custom_command(
    OUTPUT ${faupp_p_gens}
    COMMAND ${Bisonc++_executable}
    ARGS ${Bisonc++_args} ${faupp_p_desc}
    COMMAND ${CMAKE_COMMAND}
    ARGS -Dphantom_dir="${phantom_dir}" -P ${phantom_dir}/faupp-parser-trick.cmake
    DEPENDS ${faupp_p_desc}
    COMMENT "[Bisonc++][Faupp] Building Faupp parser with bisonc++ ${bisoncpp_version}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  set(faupp_s_gens     ${faupp_l_s_gen} ${faupp_p_s_gen})

  ####### -> Endings: preparing phantom variables.
  set(fau_s_phantoms ${faupp_s_gens})

  set(fau_phantoms ${fau_s_phantoms} #${moc_headers}
  )

  add_custom_target(clean-phantoms
    ${CMAKE_COMMAND} -Dfau_phantoms="${fau_phantoms}" -P ${phantom_dir}/clean-phantoms.cmake
    COMMENT "Deleting phantom files"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  add_custom_target(clean-temporals
    ${CMAKE_COMMAND} -Dsrc_dir="${src_dir}" -P ${phantom_dir}/clean-temporals.cmake
    COMMENT "Deleting temporal files"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -Dsrc_dir="${src_dir}" -P ${phantom_dir}/clean-all.cmake
    DEPENDS clean-temporals
    COMMENT "Cleaning source tree"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

endmacro()
