set(backend_headres
    Backend.hpp)

set(backend_sources
    Backend.cpp)

add_subdirectory(data_backend)

add_library(ALL_LIBRARIES STATIC STATIC ${backend_headres} ${backend_sources} )

target_link_libraries(ALL_LIBRARIES  backend_headres backend_sources Qt${QT_VERSION_MAJOR}::Core)

set_target_properties(ALL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
