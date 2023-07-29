cmake_policy(SET CMP0048 NEW)
project("cydonia" VERSION 0.1.0)
cmake_minimum_required(VERSION 3.13.0)

set(TARGET_GROUP production CACHE STRING "Group to build")

SET(GCC_WARNINGS_ALL "-Wall")
SET(GCC_WARNINGS_PEDANTIC "-pedantic")

add_subdirectory(cydonia)

# add external subdirs here
#add_subdirectory(external/some_library_we_will_use)

if(TARGET_GROUP STREQUAL test)
    include(CTest)
    add_subdirectory(tests)
# could also build as a shared library
#elseif(TARGET_GROUP STREQUAL lib)
#    add_subdirectory(so)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()
