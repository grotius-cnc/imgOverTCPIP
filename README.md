imgOverTCPIP forked from impedevted, credits to : Apiwat Pitaksin 

This c++ basic example is able to send and recieve :

		1. Text messages.
		2. Files, for example pictures.


![imgOverTCPIP](https://github.com/grotius-cnc/imgOverTCPIP/blob/main/GitHubSample.png)


Skynet Cyberdyne ported the original code 
to c++ and transferred the code into a cmake project.

The code has 2 projects wich are build by the toplevel cmake file.

How it works.

Program cyclus:

		1. 	The server will wait until the client is up.
		2. 	When client is up, it will transfer the picture trough 
			the socket connection to the server.
		3. 	The server will recieve packages and will send a confirmation 
			text message to the client to keep the process going.
		4. 	When the client has recieved the text message, 
			it will send the next package cq. chunk until data transfer 
			is completed.
		
Considerations:

		1. 	Multi threading. When integrating this code in a gui project, 
			threading with detach is a must, otherwise this process 
			will lock your gui app.
		2. 	Log the bytes send and recieved so you can check if data 
			is really transferred. 
			I have experienced data losses in other socket examples.

Performance:

		When not outcommented, std::chrono log's the transfer time. 
		
		The attached picture = 572,7 KiB (586403 B) 
		Transfer time : ~10ms

To build the project:
		
		clone https://github.com/grotius-cnc/imgOverTCPIP.git
		mkdir build
		cd build
		cmake .. 
		make

To run in terminal, first start the server: 

		$ ./server
		
If server run's, fire up the client :		

		$ ./client.

Or just open the cmakelist file in a code editor, 
this will problably load the project.

This code is tested and build on linux amd64.
