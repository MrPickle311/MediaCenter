project(Tests LANGUAGES CXX)

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


set(Includes
    ${GTestIncludes}
    TestingUtils
    )
##generated stuff END

##qt service stage 2

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src build)


set(Libs
    Common
    TestingUtils
)

set(SourceDir ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(CommonDir ${SourceDir}/Common)
set(ModulesEssentialsDir ${CommonDir}/ModulesEssentials)
set(SystemNodeDir ${ModulesEssentialsDir}/SystemNode)
set(MediatorDir ${ModulesEssentialsDir}/Mediator)

include_directories(${ModulesEssentialsDir})
include_directories(${SystemNodeDir})
include_directories(${MediatorDir})

add_subdirectory(sandbox)
add_subdirectory(TestingUtils)


include_directories(TestingUtils)

set(TestedHeaders
    ${ModulesEssentialsDir}/Backend.hpp
    ${ModulesEssentialsDir}/MultimediaEngine.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/../src/Environment.hpp
    ${MediatorDir}/Mediator.hpp
    ${ModulesEssentialsDir}/Proxy.hpp
    ${ModulesEssentialsDir}/ThreadsafeQueue.hpp
    ${ModulesEssentialsDir}/TaskManager.hpp
    ${SystemNodeDir}/SystemNode.hpp
)

enable_testing()

find_package(GTest)
find_package(Boost 1.71.0)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

function(add_test name)
    add_executable(${name} ${name}.hpp ${name}.cpp ${GTestFiles} ${TESTED_HEADERS})
    target_include_directories(${name} PUBLIC ${Includes})
    target_compile_definitions(${name}
       PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
    gtest_discover_tests(${name} DISCOVERY_TIMEOUT 300)
    target_link_libraries(${name} PRIVATE
       Threads::Threads 
       Qt6::Core 
       Qt6::Quick 
       Qt6::SerialPort 
       Qt6::DBus 
       ${Libs} 
       ${Boost_LIBRARIES} 
       )
endfunction()

add_subdirectory(Common)

# add_test(MediatorTests) 
#add_test(MultimediaEngineTests)
#add_test(ThreadsafeQueueTests)
#add_test(TaskManagerTests)
#add_test(EnvironmentTests)

##qt service stage 2 END

