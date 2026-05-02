# Node-based IDL / DEF generation (awtk_config_common.genIdlAndDefEx).

if(AWTK_RUN_IDL_CODEGEN)
  find_program(AWTK_NODE_EXECUTABLE NAMES node nodejs REQUIRED)
endif()

set(_awtk_codegen_stamp "${CMAKE_BINARY_DIR}/awtk_codegen.stamp")

if(AWTK_RUN_IDL_CODEGEN)
  add_custom_command(
    OUTPUT "${_awtk_codegen_stamp}"
    COMMAND "${AWTK_NODE_EXECUTABLE}" tools/idl_gen/tkc.js tools/idl_gen/tkc.json
    COMMAND "${AWTK_NODE_EXECUTABLE}" tools/dll_def_gen/index.js tools/idl_gen/tkc.json dllexports/tkc.def false
    COMMAND "${AWTK_NODE_EXECUTABLE}" tools/idl_gen/index.js tools/idl_gen/idl.json
    COMMAND "${AWTK_NODE_EXECUTABLE}" tools/dll_def_gen/index.js tools/idl_gen/idl.json dllexports/awtk.def false
    COMMAND "${CMAKE_COMMAND}" -E touch "${_awtk_codegen_stamp}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "AWTK: idl/def codegen (node)"
    VERBATIM
  )
else()
  add_custom_command(
    OUTPUT "${_awtk_codegen_stamp}"
    COMMAND "${CMAKE_COMMAND}" -E touch "${_awtk_codegen_stamp}"
    COMMENT "AWTK: skipping idl/def codegen (AWTK_RUN_IDL_CODEGEN=OFF)"
    VERBATIM
  )
endif()

add_custom_target(awtk_idl_codegen ALL DEPENDS "${_awtk_codegen_stamp}")
