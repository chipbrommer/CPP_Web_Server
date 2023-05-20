﻿# app name variable for ease of access 
set(THIS_APP CPP_Web_Server)

# Add source to this project's executable.
add_executable (
	${THIS_APP} 
	"main.cpp"  
	"Source/CPP_Web_Server/web_server.h" 
	"Source/CPP_Web_Server/web_server.cpp" 
	"Source/Mongoose/mongoose.h"
	"Source/Mongoose/mongoose.c"
)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/CPP_Web_Server/website")

if (UNIX)
    set(DESTINATION_DIR "/mnt/c/website")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E create_symlink "${SOURCE_DIR}" "${DESTINATION_DIR}")
elseif(WIN32)
    set(DESTINATION_DIR "C:/website")
    file(TO_NATIVE_PATH "${DESTINATION_DIR}" DST_DIR)
    file(TO_NATIVE_PATH "${SOURCE_DIR}" SRC_DIR)
    execute_process(COMMAND cmd.exe /c mklink /J "${DST_DIR}" "${SRC_DIR}")
endif()

# link libraries depending on the system architecture
# Check the system architecture and set a variable
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    #set(MONGOOSE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/CPP_Web_Server/third_party_libraries/mongoose/v7.9/lib/x86")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    #set(MONGOOSE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/CPP_Web_Server/third_party_libraries/mongoose/v7.9/lib/x64")
else()
    message(FATAL_ERROR "Unsupported target architecture")
endif()

# Include the libraries
#include_directories(${MONGOOSE_LIBRARY_PATH})
#target_link_libraries(${THIS_APP} "${MONGOOSE_LIBRARY_PATH}/mongoose.v7.9.a")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${THIS_APP} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.