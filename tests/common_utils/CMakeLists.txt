set(common_utils_headers
    QFunctionWrapper.hpp
    Mock.hpp
    EventLoop.hpp
    WrappersList.hpp
    MediatorPackage.hpp
    JsonPackage.hpp
    Caller.hpp
    MediatorTester.hpp
    )

set(common_utils_sources
    QFunctionWrapper.cpp
    Mock.cpp
    EventLoop.cpp
    WrappersList.cpp
    MediatorPackage.cpp
    JsonPackage.cpp
    Caller.cpp
    MediatorTester.cpp
    )

add_library(COMMON_UTILS STATIC
    ${common_utils_headers}
    ${common_utils_sources} )

target_link_libraries(COMMON_UTILS Qt6::Core)

set_target_properties(COMMON_UTILS PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17 )
