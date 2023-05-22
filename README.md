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

5/21/2022 - Weekly update

This weekend I have added more web design for the front end, converted the testing page into a 
nice dashboard style page and turned the log into a scrollable console style view. 

I converted the class into a singleton style instance which unlocked the capability for the
added functionality I have added. A user can now send messages or data to the website at their
desire using the 'SendConsoleLog' function. 

This functionality will be expanded upon in coming weeks to include ability for a user to publish
function access to the webpage and to send json structures to be displayed as a form. 

-------------

Stay tuned, there is more to come as I build a user customizable web UI interface to a c++ program;
