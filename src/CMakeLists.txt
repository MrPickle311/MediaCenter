set(backend_headers
    Backend.hpp
    Environment.hpp )

set(backend_sources
    Backend.cpp
    Environment.cpp)

add_subdirectory(data_backend)

add_library(ALL_LIBRARIES STATIC
    ${backend_headers}
    ${backend_sources} )

target_link_libraries(ALL_LIBRARIES Qt${QT_VERSION_MAJOR}::Core)

set_target_properties(ALL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
