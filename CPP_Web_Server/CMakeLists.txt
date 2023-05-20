# CMakeList.txt : CMake project for CPP_Websocket_Utility, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (
	CPP_Websocket_Utility 
	"main.cpp"  
	"Source/CPP_Web_Server/web_server.h" 
	"Source/CPP_Web_Server/web_server.cpp" 
	"Source/Mongoose/mongoose.h"
	"Source/Mongoose/mongoose.c"
)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/CPP_Web_Server/website")
set(DESTINATION_DIR "C:/website")

if (UNIX)
    execute_process(COMMAND "${CMAKE_COMMAND}" -E create_symlink "${SOURCE_DIR}" "${DESTINATION_DIR}")
elseif(WIN32)
    file(TO_NATIVE_PATH "${DESTINATION_DIR}" DST_DIR)
    file(TO_NATIVE_PATH "${SOURCE_DIR}" SRC_DIR)
    execute_process(COMMAND cmd.exe /c mklink /J "${DST_DIR}" "${SRC_DIR}")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CPP_Websocket_Utility PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
