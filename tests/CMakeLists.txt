cmake_minimum_required(VERSION 3.5)

project(Tests LANGUAGES CXX)

##qt service stage 1

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED)
find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core Quick SerialPort Test REQUIRED)

##qt service stage 1 END

##generated stuff

add_definitions(-DGTEST_LANGUAGE_CXX11)
set(GTEST_COLOR=1)


set(GOOGLETEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../extern/googletest")

if (EXISTS ${GOOGLETEST_DIR})
    set(GTestSrc ${GOOGLETEST_DIR}/googletest)
    set(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (UNIX AND EXISTS /usr/src/gtest)
    set(GTestSrc /usr/src/gtest)
    message(WARNING "Using gtest from system")
    if (EXISTS /usr/src/gmock)
        set(GMockSrc /usr/src/gmock)
    endif ()
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")

endif ()

set(GTestFiles ${GTestSrc}/src/gtest-all.cc)
set(GTestIncludes ${GTestSrc} ${GTestSrc}/include)
if (NOT ${GMockSrc} STREQUAL "")
    list(APPEND GTestFiles ${GMockSrc}/src/gmock-all.cc)
    list(APPEND GTestIncludes ${GMockSrc} ${GMockSrc}/include)
endif ()

include_directories(${GTestIncludes})

##generated stuff END

##tested .cpp files

##tested .cpp files END

##qt service stage 2

set(LIBS
    ALL_LIBRARIES
)

set(TESTED_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/utilities
)

set(TESTED_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Backend.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Environment.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utilites/ThreadsafeQueue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../utilites/ThreadPool.hpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src build)

set(COMMON_UTILS
    # CommonUtils.cpp
    # CommonUtils.hpp
    )

enable_testing()
find_package(GTest)

find_package(Boost 1.71.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

function(add_test name)
    add_executable(${name} ${name}.hpp ${name}.cpp ${COMMON_UTILS} ${GTestFiles})
    target_include_directories(${name} PUBLIC ${TESTED_DIRECTORIES})
    target_compile_definitions(${name}
       PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
    gtest_discover_tests(${name})
    target_link_libraries(${name} PRIVATE
       Threads::Threads Qt5::Core Qt5::Quick  Qt5::Test Qt5::SerialPort ${test_libs} ${LIBS} ${Boost_LIBRARIES})
endfunction()

#add_test(BackendTests) DONE
add_test(DataBackendTests)
#add_test(ThreadsafeQueueTests)
#add_test(TaskManagerTests)
#add_test(EnvironmentTests)

##qt service stage 2 END

