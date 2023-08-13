include_directories(${CPPUTEST_SRC_DIR})
include_directories(${CPPUTESTEXT_SRC_DIR})
include_directories(${PROJECT_SOURCE_DIR}/cydonia/)

set(SOURCE_FILES
    main.cpp
    cydonia_ut.cpp
    #${PROJECT_SOURCE_DIR}/modules/cydonia/cydonia.c
)

add_executable(cydonia_ut ${SOURCE_FILES})

target_link_libraries(cydonia_ut 
    # linnk libraries here.
    cydonia
    CppUTest 
    CppUTestExt
    pthread
)

add_test(cydonia_test cydonia_ut)
