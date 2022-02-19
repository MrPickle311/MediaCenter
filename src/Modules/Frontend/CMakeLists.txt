set(ExebutableName Frontend)

set(sources
        AudioPageConnector.cpp
        Frontend.cpp
        )

set(headers
        AudioPageConnector.hpp
        )

qt6_add_resources(sources_qrc ${CMAKE_CURRENT_SOURCE_DIR}/../../../qml.qrc)

add_executable(${ExebutableName}
        ${sources}
        ${sources_qrc}
        ${headers}
        )

target_include_directories(${ExebutableName} PRIVATE ../../Common/ModulesEssentials/SystemNode)

set_target_properties(${ExebutableName} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${ExebutableName}
        SystemNode
        Qt6::Core
        Qt6::DBus
        Qt6::Quick
        Qt6::Multimedia
        )
