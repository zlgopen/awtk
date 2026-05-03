cmake_minimum_required(VERSION 3.12)
project(SheenBidi VERSION 3.0.0 LANGUAGES C)

set(SHEENBIDI_SOURCES
    Source/API/SBAlgorithm.c
    Source/API/SBAllocator.c
    Source/API/SBAttributeList.c
    Source/API/SBAttributeRegistry.c
    Source/API/SBBase.c
    Source/API/SBCodepoint.c
    Source/API/SBCodepointSequence.c
    Source/API/SBLine.c
    Source/API/SBLog.c
    Source/API/SBMirrorLocator.c
    Source/API/SBParagraph.c
    Source/API/SBScriptLocator.c
    Source/API/SBText.c
    Source/API/SBTextConfig.c
    Source/API/SBTextIterators.c
    Source/Core/List.c
    Source/Core/Memory.c
    Source/Core/Object.c
    Source/Core/Once.c
    Source/Data/BidiTypeLookup.c
    Source/Data/GeneralCategoryLookup.c
    Source/Data/PairingLookup.c
    Source/Data/ScriptLookup.c
    Source/Script/ScriptStack.c
    Source/Text/AttributeDictionary.c
    Source/Text/AttributeManager.c
    Source/UBA/BidiChain.c
    Source/UBA/BracketQueue.c
    Source/UBA/IsolatingRun.c
    Source/UBA/LevelRun.c
    Source/UBA/RunQueue.c
    Source/UBA/StatusStack.c
)

file(GLOB SHEENBIDI_PUBLIC_HEADERS "Headers/SheenBidi/*.h")

add_library(SheenBidi STATIC ${SHEENBIDI_SOURCES})

target_include_directories(SheenBidi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Headers>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

if(UNIX AND NOT APPLE AND NOT WIN32)
    set_target_properties(SheenBidi PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

include(GNUInstallDirs)

install(TARGETS SheenBidi
    EXPORT SheenBidiTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${SHEENBIDI_PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SheenBidi
)

install(EXPORT SheenBidiTargets
    FILE SheenBidiTargets.cmake
    NAMESPACE SheenBidi::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SheenBidi
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SheenBidiConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SheenBidiConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SheenBidi
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SheenBidiConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SheenBidiConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SheenBidiConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SheenBidi
)

message(STATUS "SheenBidi ${PROJECT_VERSION} configured")
