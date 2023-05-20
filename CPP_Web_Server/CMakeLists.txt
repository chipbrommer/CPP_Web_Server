# app name variable for ease of access 
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
set(DESTINATION_DIR "C:/website")

if (UNIX)
    execute_process(COMMAND "${CMAKE_COMMAND}" -E create_symlink "${SOURCE_DIR}" "${DESTINATION_DIR}")
elseif(WIN32)
    file(TO_NATIVE_PATH "${DESTINATION_DIR}" DST_DIR)
    file(TO_NATIVE_PATH "${SOURCE_DIR}" SRC_DIR)
    execute_process(COMMAND cmd.exe /c mklink /J "${DST_DIR}" "${SRC_DIR}")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${THIS_APP} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
