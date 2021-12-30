add_subdirectory(ModulesEssentials)
add_subdirectory(MultithreadingUtils)

add_library(Common STATIC
    # ${ModulesEssentials}
    # ${MultithreadingUtils} 
)

target_link_libraries(Common 
    ModulesEssentials
    MultithreadingUtils
)

set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX CMAKE_CXX_STANDARD 17)