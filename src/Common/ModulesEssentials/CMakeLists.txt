add_subdirectory(SystemNode)

set(Library ModulesEssentials)

set(sources
    Matcher.cpp
    Mediator.cpp
    Proxy.cpp
    MultimediaEngine.cpp
    TaskManager.cpp
    Backend.cpp
)

set(headers
    Matcher.hpp
    Mediator.hpp
    Proxy.hpp
    MultimediaEngine.hpp
    TaskManager.hpp
    ThreadsafeQueue.hpp
    Backend.hpp
)

add_library(${Library} STATIC
            ${sources}
            ${headers}
)

target_link_libraries(${Library} Qt6::Core SystemNode)

set_target_properties(${Library} PROPERTIES LINKER_LANGUAGE CXX)
