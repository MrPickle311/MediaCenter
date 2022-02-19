set(ExebutableName MainMediator)

set(sources
        MainMediator.cpp
        MainMediatorNode.cpp
        )

set(headers
        MainMediatorNode.hpp
        )


add_executable(${ExebutableName}
        ${sources}
        ${headers}
        )

target_include_directories(${ExebutableName} PRIVATE ../../Common/ModulesEssentials/SystemNode)
target_include_directories(${ExebutableName} PRIVATE ../../Common/ModulesEssentials/Mediator)

set_target_properties(${ExebutableName} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${ExebutableName}
        SystemNode
        Mediator
        Qt6::Core
        Qt6::DBus
        )
