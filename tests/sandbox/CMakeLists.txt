project(sandbox LANGUAGES CXX)

set(CallerName Caller)
set(ReceiverName  Receiver)

find_package(QT NAMES Qt6 COMPONENTS Core DBus  REQUIRED)
find_package(Qt6 COMPONENTS Core DBus REQUIRED)

set(${CallerName}_sources
    ${CallerName}.cpp
)

set(${CallerName}_headers
    ${CallerName}.hpp
)

set(${ReceiverName}_sources
    ${ReceiverName}.cpp
)

set(${ReceiverName}_headers
    ${ReceiverName}.hpp
)

set(QtLibraries 
       Qt6::Core 
       Qt6::DBus 
)

add_executable(${CallerName} 
    ${${CallerName}_sources}
    ${${CallerName}_headers} 
)

add_executable(${ReceiverName} 
    ${${ReceiverName}_sources}
    ${${ReceiverName}_headers} 
)

target_link_libraries(${CallerName}
Qt6::Core 
Qt6::DBus 
)

target_link_libraries(${ReceiverName}
Qt6::Core 
Qt6::DBus 
)

set_target_properties(${CallerName} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${ReceiverName} PROPERTIES LINKER_LANGUAGE CXX)