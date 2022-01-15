set(Library Mediator)

set(sources
    Mediator.cpp
    Matcher.cpp
)

set(headers
    Mediator.hpp
    Matcher.hpp
)

add_library(${Library} STATIC
            ${sources}
            ${headers}
)

target_link_libraries(${Library} Qt6::Core Qt6::DBus)

set_target_properties(${Library} PROPERTIES LINKER_LANGUAGE CXX)
