cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(nfs-save-rehash C CXX)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_sources(${PROJECT_NAME} PRIVATE FILE_SET HEADERS
                                       BASE_DIRS "src"
									   FILES "${HEADERS}")
if(WIN32)
	target_sources(${PROJECT_NAME} PRIVATE "src/version.rc")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE "/W3" "/Zc:throwingNew" "/EHsc")
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
if(CMAKE_CXX_LINKER_SUPPORTS_PDB)
	install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION bin OPTIONAL)
endif()
