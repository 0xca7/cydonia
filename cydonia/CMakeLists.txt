add_library(cydonia STATIC
    cydonia.c
)

target_include_directories(cydonia PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# libraries can be linked here if needed
target_link_libraries(cydonia PUBLIC
    global
)
