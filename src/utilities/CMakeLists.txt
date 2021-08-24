set(utilities_headers
    ThreadsafeQueue.hpp
    ThreadPool.hpp)

set(utilities_sources
    ThreadPool.cpp)

add_library(UTILITIES_LIB STATIC
    ${utilities_headers}
    ${utilities_sources} )

target_link_libraries(UTILITIES_LIB ${Boost_LIBRARIES})

set_target_properties(UTILITIES_LIB PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17 )
