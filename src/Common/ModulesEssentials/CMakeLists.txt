add_subdirectory(SystemNode)
# add_subdirectory(Mediator)

set(Library ModulesEssentials)

set(sources
    # Proxy.cpp
    # MultimediaEngine.cpp
    TaskManager.cpp
    # Backend.cpp
)

set(headers
    # Proxy.hpp
    # MultimediaEngine.hpp
    TaskManager.hpp
    ThreadsafeQueue.hpp
    # Backend.hpp
)

add_library(${Library} STATIC
            ${sources}
            ${headers}
)

target_link_libraries(${Library} 
    SystemNode 
    # Mediator
)

set_target_properties(${Library} PROPERTIES LINKER_LANGUAGE CXX)
