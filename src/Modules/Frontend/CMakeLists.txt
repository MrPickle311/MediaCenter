set(ExebutableName Frontend)
set(Library ${ExebutableName}lib)

set(sources
        )

set(headers

        )

add_library(${Library} STATIC
        ${sources}
        ${headers}
        )
find_package(Qt6Core REQUIRED)
find_package(Qt6Quick REQUIRED)

include_directories(${Qt6Quick_INCLUDES})
include_directories(${Qt6Core_INCLUDES})
qt6_add_resources(sources_qrc ${CMAKE_CURRENT_SOURCE_DIR}/../../../qml.qrc)

add_executable(${ExebutableName}
        ${sources}
        ${sources_qrc}
        ${headers}
        Frontend.cpp
        )

set_target_properties(${ExebutableName} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(Frontend
        ${Library}
        Qt6::Core
        Qt6::DBus
        Qt6::Quick
        Qt6::Multimedia
        )
