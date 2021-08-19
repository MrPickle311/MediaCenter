set(video_headers
)

set(video_sources
)

add_library(VIDEO_LIBRARIES STATIC
            ${video_headers}
            ${video_sources})

target_link_libraries(VIDEO_LIBRARIES Qt5::Core )
set_target_properties(VIDEO_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
