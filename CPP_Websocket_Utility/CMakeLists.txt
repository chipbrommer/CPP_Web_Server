# CMakeList.txt : CMake project for CPP_Websocket_Utility, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (
	CPP_Websocket_Utility 
	"main.cpp"  
	"Source/WebsocketServer/websocket_server.h" 
	"Source/WebsocketServer/websocket_server.cpp" 
	"Source/WebsocketClient/websocket_client.h" 
	"Source/WebsocketClient/websocket_client.cpp" 
	"Source/WebsocketServer/websocket_server_config.h"
	"Source/Mongoose/mongoose.h"
	"Source/Mongoose/mongoose.c"
	"Source/WebsocketServer/website/index.html"
	"Source/WebsocketServer/website/style.css"
)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/WebsocketServer/website")
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
