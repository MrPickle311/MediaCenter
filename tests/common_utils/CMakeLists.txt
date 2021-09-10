set(common_utils_headers
    QFunctionWrapper.hpp
    Mock.hpp
    EventLoop.hpp
    WrappersList.hpp
    )

set(common_utils_sources
    QFunctionWrapper.cpp
    Mock.cpp
    EventLoop.cpp
    WrappersList.cpp
    )

add_library(COMMON_UTILS STATIC
    ${common_utils_headers}
    ${common_utils_sources} )

target_link_libraries(COMMON_UTILS Qt5::Core)

set_target_properties(COMMON_UTILS PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17 )
