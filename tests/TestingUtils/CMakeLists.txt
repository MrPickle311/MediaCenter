set(headers
    QFunctionWrapper.hpp
    # Mock.hpp
    EventLoop.hpp
    WrappersList.hpp
    # MediatorPackage.hpp
    JsonPackage.hpp
    # Caller.hpp
    # MediatorTester.hpp
    )

set(sources
    QFunctionWrapper.cpp
    # Mock.cpp
    EventLoop.cpp
    WrappersList.cpp
    # MediatorPackage.cpp
    JsonPackage.cpp
    # Caller.cpp
    # MediatorTester.cpp
    )

add_library(TestingUtils STATIC
    ${headers}
    ${sources} )

target_link_libraries(TestingUtils Qt6::Core Qt6::DBus)

set_target_properties(TestingUtils PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17 )
