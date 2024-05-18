# CppND-System-Monitor-Project
This repository contains C++ code to build a simple system monitoring system in a Linux-based system. This is a project from the Object Oriented Programming Course of the Udacity C++ Nanodegree program.

# Overview
The project is a system monitor that displays the CPU and memory utilization of a Linux-based system. The project uses the ncurses library to display the information in a terminal window. The project also uses the phtread library to create a separate thread to update the information displayed in the terminal window. The code reads the information from the /proc directory in the Linux system. Based on the information read from the /proc directory, the code calculates the CPU and memory utilization of the system.

# System Monitor Display
![Alt text](images/system_monitor.png)

## Dependencies for Running Locally
* cmake >= 3.7
* make >= 4.1
* gcc/g++ >= 5.4
* ncurses library
* phtread library

## Running the project locally
1. Clone the repository
2. Build the project
```
build make
```
3. Run the executable
```
./build/monitor
```

## Running the code in CodeSpace
There is an alternative way to run the code using CodeSpace. CodeSpace is a cloud-based development environment provided by GitHub. To run the code in CodeSpace, follow the steps below:
1. Click on the CodeSpace button in the repository
2. Click on the "New Codespace" button
3. The CodeSpace will open in a new tab
4. Build the project
```
build make
```
5. Run the executable
```
./build/monitor
```

![Alt text](images/system_monitor.gif)