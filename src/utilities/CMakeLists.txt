set(utilities_headers
    ThreadsafeQueue.hpp
    TaskManager.hpp)

set(utilities_sources
    TaskManager.cpp)

add_library(UTILITIES_LIB STATIC
    ${utilities_headers}
    ${utilities_sources} )

target_link_libraries(UTILITIES_LIB ${Boost_LIBRARIES} Qt5::Core)

set_target_properties(UTILITIES_LIB PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17 )
