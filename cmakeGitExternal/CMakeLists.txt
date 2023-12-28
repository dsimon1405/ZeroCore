#================ git
#   must be set:
#	GIT_EXTERNAL_PATH - path to external libs directory (example: /home/dmitry/project/external);
#   GIT_EXTERNAL_LIBS - a list of submodules in 2 parameters:
#		first - libs directory name (name of cloned directory)
#   	second - repository URL (example: "sdl3" "https://github.com/libsdl-org/SDL.git"),
#		third - commit id or its SHA (example one of next:
#														id - cb4e5d2
#											  			SHA - 6d759b4f3935becf5228300b8a6926e3533edbfc).

#	fatal error equal function
function(F_FATAL_EQUAL data1 data2 errorMessage)
	if(NOT ${data1} EQUAL ${data2})
		message(FATAL_ERROR ${errorMessage})
	endif()
endfunction()

find_package(Git QUIET)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "Could not find_package(Git QUIET)!")
endif()

#	git clone external libs and checkout to commit
function(F_GIT_CLONE libName link commitID)
	if(NOT EXISTS ${GIT_EXTERNAL_PATH}/${libName})
		#	clone
		message(STATUS "Git clone ${libName}")
		execute_process(COMMAND ${GIT_EXECUTABLE} clone ${link}
						WORKING_DIRECTORY ${GIT_EXTERNAL_PATH}
						RESULT_VARIABLE result)
		F_FATAL_EQUAL(${result} 0 "git clone ${link} ${GIT_EXTERNAL_PATH} => failed with: ${result}")
		#	checkout
		message(STATUS "Git checkout ${libName} to ${commitID}")
		execute_process(COMMAND ${GIT_EXECUTABLE} checkout ${commitID}
						WORKING_DIRECTORY ${GIT_EXTERNAL_PATH}/${libName}
						RESULT_VARIABLE result)
		F_FATAL_EQUAL(${result} 0 "git checkout ${commitID} => failed with: ${result}")
	endif()
endfunction()

list(LENGTH GIT_EXTERNAL_LIBS libsCount)
math(EXPR libsCount "(${libsCount} / 3) - 1")
foreach(i RANGE ${libsCount})
	math(EXPR nameIndex "${i} * 3")
	math(EXPR linkIndex "${i} * 3 + 1")
	math(EXPR CommitIdIndex "${i} * 3 + 2")
	list(GET GIT_EXTERNAL_LIBS ${nameIndex} ${linkIndex} ${CommitIdIndex} data)
	F_GIT_CLONE(${data})
endforeach()

# #	git init
# if(NOT EXISTS "${PROJECT_SOURCE_DIR}/.git")
# 	message(STATUS "Init Git for submodules download")
# 	execute_process(COMMAND ${GIT_EXECUTABLE} init
#                     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#                     RESULT_VARIABLE GIT_INIT_RESULT)
# 	F_FATAL_EQUAL(${GIT_INIT_RESULT} 0 "git init => failed with: ${GIT_INIT_RESULT}")
# endif()


# #	git submodule add
# function(F_GIT_SUBMODULE_CHECK libPathName link needUpdate)
# 	#	check add to git submodules
# 	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/.git/modules/${libPathName}")
# 		message(STATUS "Add submodule ${libPathName}")
# 		execute_process(COMMAND ${GIT_EXECUTABLE} submodule add -f ${link} ${libPathName}
# 	                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
# 	                    RESULT_VARIABLE GIT_SUBMODULE_ADD_RESULT)
# 		F_FATAL_EQUAL(${GIT_SUBMODULE_ADD_RESULT} 0 "git submodule add ${link} ${libPathName} => failed with: ${GIT_SUBMODULE_ADD_RESULT}")
# 	endif()
# 	#	existing check
# 	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${libPathName}/.git")
# 		set(${needUpdate} ON PARENT_SCOPE)
# 		message(STATUS "Submodule: ${libPathName} - need update")
# 	endif()
# endfunction()

# set(GIT_SUBMODULES_UPDATE OFF)
# list(LENGTH GIT_EXTERNAL_LIBS libsCount)
# math(EXPR libsCount "(${libsCount} / 2) - 1")
# foreach(i RANGE ${libsCount})
# 	math(EXPR nameIndex "${i} * 2")
# 	math(EXPR linkIndex "${i} * 2 + 1")
# 	list(GET GIT_EXTERNAL_LIBS ${nameIndex} ${linkIndex} data)
# 	F_GIT_SUBMODULE_CHECK(${data} GIT_SUBMODULES_UPDATE)
# endforeach()


# #	git submodule update --init --recursive
# if(GIT_SUBMODULES_UPDATE)
# 	#	submodule update
#     message(STATUS "Submodules update")
#     execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#                     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#                     RESULT_VARIABLE ZC_GIT_SUBMOD_RESULT)
# 	F_FATAL_EQUAL(${ZC_GIT_SUBMOD_RESULT} 0 "git submodule update --init --recursive => failed with: ${ZC_GIT_SUBMOD_RESULT}")
# endif()


# #	total submodules check
# function(F_GIT_SUBMODULE_EXISTS libPathName)
# 	if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${libPathName}/CMakeLists.txt")
#     	message(FATAL_ERROR "Doesn't exists: ${PROJECT_SOURCE_DIR}/${libPathName}/CMakeLists.txt")
# 	endif()
# endfunction()

# foreach(i RANGE ${libsCount})
# 	math(EXPR nameIndex "${i} * 2")
# 	list(GET GIT_EXTERNAL_LIBS ${nameIndex} data)
# 	F_GIT_SUBMODULE_EXISTS(${data})
# endforeach()


#================ find package
# list(LENGTH ZC_EXTERNAL_PACKAGES packagesCount)
# math(EXPR packagesCount "(${packagesCount} / 2) - 1")
# foreach(i RANGE ${packagesCount})
# 	math(EXPR nameIndex "${i} * 2")
# 	list(GET ZC_EXTERNAL_PACKAGES ${nameIndex} packageName)
# 	find_package(${packageName} QUIET)
# 	if(NOT ${packageName}_FOUND)
# 		math(EXPR linkIndex "${i} + 1")
# 		list(GET ZC_EXTERNAL_PACKAGES ${linkIndex} packageLink)
# 		list(APPEND GIT_EXTERNAL_LIBS external/${packageName} ${packageLink})
# 		set(ZC_GIT_SUBMODULES ON)
# 	endif()
# endforeach()


#================ sqlite
# #set(ZC_SRC_FILES ${ZC_SRC_FILES} ${PROJECT_SOURCE_DIR}/external/sqlite/src/sqlite3.c)
# add_library(sqlite SHARED ${PROJECT_SOURCE_DIR}/external/sqlite/src/sqlite3.c)
# set(ZC_INCLUDE_DIRS ${ZC_INCLUDE_DIRS} PUBLIC ${PROJECT_SOURCE_DIR}/external/sqlite/include)
# set(ZC_LINK_LIBS ${ZC_LINK_LIBS} PUBLIC sqlite)