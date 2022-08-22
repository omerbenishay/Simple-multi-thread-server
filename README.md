# Simple-multi-thread-server

Simple multi-thread server. 
Simply run make and then:
./server [portnum] [number of threads] [queue_size] [schedalg]
● portnum: the port number that the web server should listen on.
● threads: the number of worker threads that should be created within the web server
● queue_size: the number of request connections that can be accepted at one time.
● schedalg: the scheduling algorithm to be performed. Must be one of "block", "dt", "dh", or "random". 

./client [hostname] [port] [filename]
