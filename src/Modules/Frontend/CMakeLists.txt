set(ExebutableName Frontend)
set(Library ${ExebutableName}lib)

set(sources
    Frontend.cpp
)

set(headers

)

add_library(${Library} STATIC
    ${sources}
    ${headers}
)

target_link_libraries(${Library}
    Qt6::Core 
    Qt6::DBus
)

add_executable(${ExebutableName} 
    ${sources} 
    ${headers}
)

target_link_libraries(${ExebutableName} 
    ${Library}
)

set_target_properties(${ExebutableName} PROPERTIES LINKER_LANGUAGE CXX)
