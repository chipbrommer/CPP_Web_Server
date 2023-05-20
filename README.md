# CPP_Websocket_Utility
A C++ multi platform web server using Mongoose, buildable with cmake. 

The 'main.cpp' shows an example of how to use the class and spawn a new webserver.
The example shows spawning the server on "localhost:80" which makes it accessible
via a web browser by going to "localhost". You should automatically seet he index.html
page displayed.

Currently, the class callback 'eventCallback' handles a new connection, 
an http request, and an update request that upgrades the socket to a websocket.

On the 'index.html' the 'connect' button sends the update request, and displays
when the connection has been established. The user can then send messages
to the server and get them echo'd back in real time. 

Next functionality added will be parsing the websocket messages in the event
handler and triggering c++ functions to execute. Giving a web UI interface
to c++; 

Stay tuned, more to come. 
