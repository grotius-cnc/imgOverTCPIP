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
		will lock your gui app. This example is creating a thread in a
		c++ class and passing a int value trough it.
		
		class theClass {
		public:
			theClass(int theValue){
				myThread = new std::thread(&XClient::thread,this,theValue);
       			myThread->detach(); //! Execute the thread independent from other programs.
        	}
        	
       		void thread(int theValue){
       			//! This function will run like a while loop.
   			}
    	};
    	private:
    	std::thread *myThread;
    	
	
	2.  When using multi threading in a gui project, you could 
		use the std::mutex to lock variables being used by multiple
		threads at the same time. Setting up a mutex is quite easy.
		The thread calling the function() will lock acces from other 
		threads to the variable.
		
		std::mutex myMutex;
		void function(){
	    	myMutex.lock();
	    		// update your variable here. 
			myMutex.unlock();
		}
		
	3. 	Log the bytes send and recieved so you can check if data 
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


