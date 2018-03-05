/**
 * File holding Alarm class, methods, and necessary values.
 * @author	KLAPSTEIN_DANIEL
 * @author	SMITH_EVAN
 */
#ifndef ALARMER_H
#define ALARMER_H 1
#define _CRT_SECIRE_NO_WARNINGS 1

// LIBRARIES
#include "Alarmer.h"

#include<chrono>
#include<ctime>
#include<fstream>
#include<iostream>
#include<stdexcept>
#include<thread>

// GLOBAL CONSTANTS / VARIABLES
// NONE

// CLASSES / STRUCTS

/**
 * Alarmer class to hold and manage a list of alarams, dates, and times.
 */
class Alarmer
{
public:
	Alarmer();					        // Default constructor (prompt to create file)
	Alarmer(std::string);		    // Constructor taking file name
	void mainLoop();			      // Main polling loop to hold threads
	~Alarmer()					        // Class destructor
private:
	struct Node					        // Struct to hold each individual alarm
	{
		std::string label;		    // Alarm label
		struct tm time;			      // ctime object to hold time
		Node *next;				        // Pointer to next Node
	};
	Node *head;					        // Head pointer to Node list
	bool isRunning, soundAlarm;	// Semaphore booleans for threads
	std::string filename;		    // String to hold file name
	std::thread t1, t2;			    // Threads to run loops
	void addAlarm();			      // Function to add a new alarm
	void alarmLoop();			      // Thread function to handle alarm checking
	void deleteAlarm();			    // Function to remove alarm from list
	void saveFile();			      // Function to sort alarms and save to filename
	void userLoop();			      // Thread function to handle user input
	void viewAlarms();			    // Function to display all alarms
};

// FUNCTIONS : Alarmer
// NONE

#endif
