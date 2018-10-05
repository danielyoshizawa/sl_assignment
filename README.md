# StreamLabs C++ Assignment

## Getting Started

### Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

### Running
```
<path/to/build>/bin/<Debug|Release>/server.exe
<path/to/build>/bin/<Debug|Release>/client.exe
```

For a load test pass **--multi** to client.exe as argument

```
<path/to/build>/bin/<Debug|Release>/client.exe --multi
```

the **.exe** files can also be found in

```
binaries/
```

## Built With

* [Named Pipes WinAPI](https://docs.microsoft.com/en-us/windows/desktop/ipc/using-pipes) - Windows named pipes API
* [CMake](https://cmake.org/) - Pre-builder
* [Cereal](https://uscilab.github.io/cereal/) - Serizalization library
* [Visual Studio 2017](https://visualstudio.microsoft.com/pt-br/?rr=https%3A%2F%2Fwww.google.com.br%2F) - IDE
* [Windows 8.1](https://www.microsoft.com/) - OS
* [Git](https://git-scm.com/) - Version control tool

## Author

* **Daniel Yoshizawa**

## Project explanation

### Common

##### messages

Messages are the basic struct used for comunication between the client and the server, it's a very simple struct containing only a string to represent the method name and a vector of strings for the parameters needed for the method.

For communication this struct is serealized using the ***Cereal*** library in JSON format and passed through ***named pipes*** and after deserialization it is interpreted on the client or server, the mechanics involved will be explained later.

##### console

It's a basic implementation of an inline function ***print*** with the purpose to keep the output messages padronized, a future improvement would be to save this output to a file and keep a log of the operations.

Severity levels were adopted to keep easy to debug, ***Notice*** is used for regular messages as success on operations, output of methods or anything that has no impact on the application functionality. ***Warning*** is used to alert the user about any problem that might be occured but that is not critical for the proper function of the application and ***Error*** is used when something have impact on the execution of the application.

##### common

Contain the includes, defines and structs used for the client and the server. Just to keep it on a single place.

##### commandsName

Definition of the command names with a similar purpose of common.h

### Custom

A small library representing the **custom class** to be used for this assingment, I chose to create a library to keep modularity and avoid unnecessary recompilation.

##### manager

Handles custom objects creation and manipulation, it is passed as a parameter for the commands that perform operations on custom objects. Possible improvements would be object updates and deletion.

All the custom objects are stored in a map that uses the objects names as a key, here I assumed that the objects names are unique.

##### custom

A simple representation of an object with a name and a message used to ilustrate what can be done in this application, it has a method for printing information on the console and some setter and getter that are called on the commands, it is just to show how to use any object between client/server.

### Server

##### server

The server start with the creation of the named pipe, for the purpose of multi client support 4 instances are created and saved on an array of *PIPEINST*, struct representing the pipe declared in *common.h*, also *HANDLES* are created for overlap, initialized with **CreateEvent** and passed to *oOverlap* struct inside of *PIPEINST*, on **CreateNamedPipe** the pipe is configured, important settings are *PIPE_ACCESS_DUPLEX*, for read and write, *FILE_FLAG_OVERLAPPED*, for async calls, and *PIPE_TYPE_MESSAGE* was chosen for communication type. After the **ConnectToNewClient** will be called with a overlapped struct to wait for clients to connect, the states could be *CONNECTING_STATE* or *READING_STATE* depending if the client is connected yet or not.

The method **Run** will be initiated, this contains the main loop of the application, which can be in three possible states, *CONNECTING_STATE*, *READING_STATE* or *WRITING_STATE*, the first is when the pipe is waiting for a client to connect, which will change to *READING_STATE* when it is done, after a read operation success the handle will change to *WRITING_STATE* to perform writing operations returning to *READING_STATE* when it is done.

The main loop first checks if there is a pending IO on that Handle, if so it will try to get the overlapped result using **GetOverlappedResult**, if the return is true means that the previous operation is done and the state of the handle can change to the next state and the next operation can be performed, if not it will jump for the next loop interaction and wait for the operation to finish before continue.

After the operation is done on that pipe the read or write methods will be called, if completed the state will change for the next possible state, if not the pipe will have the pendingIO variable set to true and will loop until it is finished, in case of the write operation the method **GetAnswerToRequest** will be called to create the reply for the previous message read by the server, there will be called the **Processor** that will return the proper stream to answer for the client. 

##### processor

Process the messages and invoke the right commands acording to the method specified in the message.

All commands are initialized here and kept in a map, representing a pool of commands, when a message arrives on the server the *Process* method is called passing the buffer received in the named pipe, then the message is deserialized and the right command is invoked. After this process the message returned by the command is serialized and send to the client.

#### Commands
##### command

Command pattern was used in this application, the **Execute** method receives the parameters vector from the message and returns another message in response with the command output, it could be a 200OK, 401 for failure or a serialized object depending which command was invoked.

##### createCustomCommand

Command for custom object creation, receives a **manager** on creation and when executed calls the creation method on the manager passing the name and the message received as parameters of the method, it returns 200 on success or 401 if any argument is missing. A possible improvement would be another message in case the object already exists.

##### customsListCommand

Command to retrieve the names of the custom objects already created.

##### printCommand

Prints custom messages sent by the client.

##### retrieveCustomCommand

Retrieves the custom object for the client, the idea here is to send the custom object serialized as a parameter of the response message and the client deserilize it to consume the object, it is serialized as a shared_ptr and this is handles by the **cereal** library. It is not the best approach but for the porpose of this assignment I think is a valid implementation.

This command allows to retrieve the whole object so I assumed that attributes recovery and methods invocation are covered, if this is not the desired implementation I think that would be very simple to create another command to invoke methods remotely passing the custom object name and the method desired and for the attributes just have to pass the attributes as arguments of another method as response. 

### Client

Client to read and write syncronous and asyncronous messages to the server.

First it connects to the pipe using the **createFile** method form the windows API, the pipe is open with both **GENERIC_READ** and **GENERIC_WRITE** attributes and **FILE_FLAG_OVERLAPPED** as well, the main difference for sync or async reads and writes is the use of a *LPOVERLAPPED* struct on **ReadFile** or **WriteFile**.

When using **sendSync** method the message to be send is serialized and passed to the **write** method which will block because the *LPOVERLAPPED* parameter is *NULL*, after the whole message is written the method **read** will be called, once again it will block because *LPOVERLAPPED* is null, if the response is a succes, 200OK, the method is over and everything went well, if the response is an error the error message will be printed and a retry will be performed, it is a very simple retry mechanism that will perform 3 times at most. In case of any other response the method will abort and it will be considered an **Error**.

For the **sendAsync** two *OVERLAPPED* objescts are created, one for write and one for read, so them can be passed to the read and write functions to perform async calls to the pipe, if the response to **WriteFile** are succes the operation is considered done, in case of success equals *false* some other variables need to be checked. If **GetLastError** return *ERROR_IO_PENDING* means that the operation still performing, so the method **WaitForSingleObject** is called to wait for the operation to return, if the response is *WAIT_OBJECT_0* the operation succed and **GetOverlappedResult** is called to retrieve the message, if any other response is returned the IO is canceled using **CancelIo**. All invalid responses, *WAIT_ABANDONED*, *WAIT_TIMEOUT* and *WAIT_FAILED* are threated the same.  

The mechanics applied to sync and async calls are the same, write a message and read a message, so the commands created in the client are just differentiated by a boolean value that will call one method or another, also I kept it simple to save some time but was possible to use the same principle of command pattern here as in the server.

For the commands *PrintMessage* and *CreateCustom* the return is not used, it will be an error or a success that could be handle by resending the request or printing to console, for *CustomsList* it just print the name of the custom objects if any, in case of error nothing happen.

On **RetrieveCustom** the parameter corresponding to the custom object in the response is deserialized in a *shared_ptr* and the method **printInfo** is called on the object to show that the object was passed through pipe.

Improviments would be create verification on types, more meaningful error messages and create the proper commands mechanism.

To show the functionalities of the application I created 2 ways to run the client, one single thread that creates some objects, retrieve them, get the list of objects and print some messages, calling sync and async methods, also a multi threads that create 4 threads and calls the same routine multiple times, using sync and async methods, this can be invoked passing *--multi* as an argument of the client, just to ilustrate a more real scenario.

### Methods, Parameters and Responses

| Method | Parameters | Response |
| - | - | - |
| PRINT_CMD | Message to be printed | 200 Ok message or 401 Message was missing |
| CREATE_CUSTOM_CMD | Custom object name | 200 OK or 401 Missing Arguments |
|| Custom object message | |
| RETRIEVE_CUSTOM_CMD | Custom object name | 200 OK with the serialized object as parameter or 401 Object name was missing |
| CUSTOM_LIST_CMD | None | 200 OK with a vector with the names of the custom objects |
