# Multithreading and Network

## Multithreading

Our game is designed to be multithreaded. On the client side, the game display runs in a separate thread from the game engine. Similarly, data reception from the network is launched in a dedicated thread. On the server side, each new client accepted launches a data reception function in a dedicated thread. When the game is ready to be launched, it is started in a dedicated thread.

## Network connection

For the network, we chose a TCP connection between the client and the server. This connection has the advantage of being bidirectional, which allows us greater freedom to communicate. We have associated this with a command system that allows us to exchange data in the form of text or serialized binary data. In a communication regime established between the server and the client, the dedicated thread for reception only performs data reception. The sending is done from other threads of the client or server using mutexes for reading or writing to the socket. The server can send data to a single client, all clients, or even make a blocking request to one of the clients. A system with a question, an answer, and a condition_variable allows for a request between two entities and receiving the answer, which therefore arrives in another thread. The thread responsible for data reception only performs functions with quick execution, to avoid blocking reception for an extended period of time. As a result, the server can make a request to a player and the player can make a request to the server completely simultaneously.
