#	may be some day

#================ find package
#	if can't find package use git submodules
set(ZC_GIT_SUBMODULES OFF)
set(GIT_SUBMODULES)

list(LENGTH ZC_EXTERNAL_PACKAGES packagesCount)
math(EXPR packagesCount "(${packagesCount} / 2) - 1")
foreach(i RANGE ${packagesCount})
	math(EXPR nameIndex "${i} * 2")
	list(GET ZC_EXTERNAL_PACKAGES ${nameIndex} packageName)
	find_package(${packageName} QUIET)
	if(NOT ${packageName}_FOUND)
		math(EXPR linkIndex "${i} + 1")
		list(GET ZC_EXTERNAL_PACKAGES ${linkIndex} packageLink)
		list(APPEND GIT_SUBMODULES external/${packageName} ${packageLink})
		set(ZC_GIT_SUBMODULES ON)
	endif()
endforeach()

#================ git
#   must be sett:
#   GIT_SUBMODULES - a list of submodules in pairs, where the first is the submodule relativePath/name,
#   the second is the git repository URL (example: "sdl3" "https://github.com/libsdl-org/SDL.git")

#	fatatl error equal function
function(F_FATAL_EQUAL data1 data2 errorMessage)
	if(NOT ${data1} EQUAL ${data2})
		message(FATAL_ERROR ${errorMessage})
	endif()
endfunction()

find_package(Git QUIET)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "Could not find_package(Git QUIET)!")
endif()

#	git init
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/.git")
	message(STATUS "Init Git for submodules download")
	execute_process(COMMAND ${GIT_EXECUTABLE} init
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_INIT_RESULT)
	F_FATAL_EQUAL(${GIT_INIT_RESULT} 0 "git init => failed with: ${GIT_INIT_RESULT}")
endif()

#	git submodule add
function(F_GIT_SUBMODULE_CHECK libPathName link needUpdate)
	#	check add to git submodules
	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/.git/modules/${libPathName}")
		message(STATUS "Add submodule ${libPathName}")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule add -f ${link} ${libPathName}
	                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	                    RESULT_VARIABLE GIT_SUBMODULE_ADD_RESULT)
		F_FATAL_EQUAL(${GIT_SUBMODULE_ADD_RESULT} 0 "git submodule add ${link} ${libPathName} => failed with: ${GIT_SUBMODULE_ADD_RESULT}")
	endif()
	#	existing check
	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${libPathName}/.git")
		set(${needUpdate} ON PARENT_SCOPE)
		message(STATUS "Submodule: ${libPathName} - need update")
	endif()
endfunction()

set(GIT_SUBMODULES_UPDATE OFF)
list(LENGTH GIT_SUBMODULES submodulesCount)
math(EXPR submodulesCount "(${submodulesCount} / 2) - 1")
foreach(i RANGE ${submodulesCount})
	math(EXPR nameIndex "${i} * 2")
	math(EXPR linkIndex "${i} * 2 + 1")
	list(GET GIT_SUBMODULES ${nameIndex} ${linkIndex} data)
	F_GIT_SUBMODULE_CHECK(${data} GIT_SUBMODULES_UPDATE)
endforeach()

#	git submodule update --init --recursive
if(GIT_SUBMODULES_UPDATE)
	#	submodule update
    message(STATUS "Submodules update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE ZC_GIT_SUBMOD_RESULT)
	F_FATAL_EQUAL(${ZC_GIT_SUBMOD_RESULT} 0 "git submodule update --init --recursive => failed with: ${ZC_GIT_SUBMOD_RESULT}")
endif()

#	total submodules check
function(F_GIT_SUBMODULE_EXISTS libPathName)
	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${libPathName}/CMakeLists.txt")
    	message(FATAL_ERROR "Doesn't exists: ${PROJECT_SOURCE_DIR}/${libPathName}/CMakeLists.txt")
	endif()
endfunction()

foreach(i RANGE ${submodulesCount})
	math(EXPR nameIndex "${i} * 2")
	list(GET GIT_SUBMODULES ${nameIndex} data)
	F_GIT_SUBMODULE_EXISTS(${data})
endforeach()


#================ sqlite
# #set(ZC_SRC_FILES ${ZC_SRC_FILES} ${PROJECT_SOURCE_DIR}/external/sqlite/src/sqlite3.c)
# add_library(sqlite SHARED ${PROJECT_SOURCE_DIR}/external/sqlite/src/sqlite3.c)
# set(ZC_INCLUDE_DIRS ${ZC_INCLUDE_DIRS} PUBLIC ${PROJECT_SOURCE_DIR}/external/sqlite/include)
# set(ZC_LINK_LIBS ${ZC_LINK_LIBS} PUBLIC sqlite)