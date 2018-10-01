# StreamLabs C++ Assingment

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

	For a load test pass --multi to client.exe as argument

	```
		<path/to/build>/bin/<Debug|Release>/client.exe --multi
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

PS. This document is a work in progress, I will finish as soon as I can, sorry for that.