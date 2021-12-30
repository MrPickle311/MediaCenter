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

add_library(ModulesEssentials STATIC
            ${sources}
            ${headers}
)

target_link_libraries(ModulesEssentials Qt6::Core)

set_target_properties(ModulesEssentials PROPERTIES LINKER_LANGUAGE CXX)
