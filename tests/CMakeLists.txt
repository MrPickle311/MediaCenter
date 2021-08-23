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
find_package(Qt5 COMPONENTS Core Quick SerialPort REQUIRED)

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
)

set(TESTED_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Backend.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Environment.hpp
    #${CMAKE_CURRENT_SOURCE_DIR}/../src/data_backend/Backend.hpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src build)

set(Tests_files
    main.cpp
    MainBackendTests.hpp
    MainBackendTests.cpp
    EnvironmentTests.hpp
    EnvironmentTests.cpp
    ${TESTED_HEADERS}
    )

enable_testing()
find_package(GTest)

function(add_test_exec test_name test_files test_libs)
    add_executable(${test_name} ${test_files} ${GTestFiles})
    target_include_directories(${test_name} PUBLIC ${TESTED_DIRECTORIES})
    target_compile_definitions(${test_name}
       PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
    gtest_discover_tests(${test_name})
    target_link_libraries(${test_name} PRIVATE
       Threads::Threads Qt5::Core Qt5::Quick Qt5::SerialPort ${test_libs})
endfunction()

function(add_test name)
    add_executable(${name} ${name}.hpp ${name}.cpp ${GTestFiles})
    target_include_directories(${name} PUBLIC ${TESTED_DIRECTORIES})
    target_compile_definitions(${name}
       PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
    gtest_discover_tests(${name})
    target_link_libraries(${name} PRIVATE
       Threads::Threads Qt5::Core Qt5::Quick Qt5::SerialPort ${test_libs} ALL_LIBRARIES )
endfunction()

#add_test_exec(Tests "${Tests_files}" "${LIBS}")
add_test(BackendTests)
#add_test(EnvironmentTests)

##qt service stage 2 END

