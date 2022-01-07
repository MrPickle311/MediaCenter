set(Library SystemNode)

set(sources
    SystemNode.cpp
)

set(headers
    SystemNode.hpp
)

add_library(${Library} STATIC
            ${sources}
            ${headers}
)

target_link_libraries(${Library} Qt6::Core Qt6::DBus)

set_target_properties(${Library} PROPERTIES LINKER_LANGUAGE CXX)
