set(images_headers
)

set(images_sources
)

add_library(IMAGES_LIBRARIES STATIC
            ${images_headers}
            ${images_sources})

target_link_libraries(IMAGES_LIBRARIES Qt5::Core )
set_target_properties(IMAGES_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
