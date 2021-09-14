set(backend_headers
    Backend.hpp
    Environment.hpp
    Matcher.hpp
    Mediator.hpp
    Proxy.hpp )

set(backend_sources
    Backend.cpp
    Environment.cpp
    Matcher.cpp
    Mediator.cpp
    Proxy.cpp)

add_subdirectory(multimedia_engine)
add_subdirectory(utilities)

add_library(ALL_LIBRARIES STATIC
    ${backend_headers}
    ${backend_sources} )

set(LIBRARIES
    MULTIMEDIA_ENGINE
    UTILITIES_LIB)

target_link_libraries(ALL_LIBRARIES Qt${QT_VERSION_MAJOR}::Core ${LIBRARIES})

set_target_properties(ALL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../qml.qrc
    )   

if(BUILD_APP)



endif()
