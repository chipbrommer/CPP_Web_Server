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
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CPP_Websocket_Utility PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
