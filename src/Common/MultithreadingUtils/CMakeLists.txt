set(sources
    TaskManager.cpp
)

set(headers
    TaskManager.hpp
    ThreadsafeQueue.hpp
)

add_library(MultithreadingUtils STATIC
            ${sources}
            ${headers}
)

set_target_properties(MultithreadingUtils PROPERTIES LINKER_LANGUAGE CXX)
