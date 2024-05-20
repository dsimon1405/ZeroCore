cmake_minimum_required(VERSION 3.22)
project(sqlite3)

add_library(${PROJECT_NAME} STATIC
    ${PROJECT_SOURCE_DIR}/src/sqlite3.c
    )

target_compile_options(${PROJECT_NAME}
    PRIVATE	$<IF:$<BOOL:${WIN32}>,-W3,-Wall>
            $<$<CONFIG:Release>:-O3>
)