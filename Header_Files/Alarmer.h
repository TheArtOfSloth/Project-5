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
	Alarmer();					// Default constructor (prompt to create file)
	Alarmer(std::string);		// Constructor taking file name
	void mainLoop();			// Main polling loop to hold threads
	~Alarmer()					// Class destructor
private:
	struct Node					// Struct to hold each individual alarm
	{
		std::string label;		// Alarm label
		struct tm time;			// ctime object to hold time
		Node *next;				// Pointer to next Node
	};
	Node *head;					// Head pointer to Node list
	bool isRunning, soundAlarm;	// Semaphore booleans for threads
	std::string filename;		// String to hold file name
	std::thread t1, t2;			// Threads to run loops
	void addAlarm();			// Function to add a new alarm
	void alarmLoop();			// Thread function to handle alarm checking
	void deleteAlarm();			// Function to remove alarm from list
	void saveFile();			// Function to sort alarms and save to filename
	void userLoop();			// Thread function to handle user input
	void viewAlarms();			// Function to display all alarms
};

// FUNCTIONS : Alarmer

/**
 * Default consructor. Creates blank text file "schedule(#).txt".
 */
Alarmer::Alarmer() : head(nullptr), isRunning(true), soundAlarm(false)
{
	std::fstream file;
	int count = 0;
	do
	{
		filename = "schedule(" + std::to_string(count++) + ").txt";
		file.open(filename, ios::in);
	} while(file.good());
	file.open(filename, ios::out | ios::trunc);
	file.close();
}

/**
 * Class constructor to load/create a schedule file.
 * @param	string representation of filename containing schedule as text
 */
Alarmer::Alarmer(std::string filename) : head(nullptr), isRunning(true), soundAlarm(false), filename(filename)
{
	std::fstream file;
	file.open(filename, ios::in | ios::beg);
	if (!file.good())
	{
		file.open(filename, ios::out | ios::trunc);
		file.close();
	}
	else
	{
		bool initHead = true;
		Node *nodeptr;
		while (!file.eof())
		{
			if (initHead)
			{
				head = new Node;
				nodeptr = head;
				initHead = false;
			}
			else
			{
				nodeptr->next = new Node;
				nodeptr = nodeptr->next;
			}
			nodeptr->next = nullptr;
			file >> nodeptr->time->tm_year;
			file >> nodeptr->time->tm_mon;
			file >> nodeptr->time->tm_mday;
			file >> nodeptr->time->tm_hour;
			file >> nodeptr->time->tm_min;
			nodeptr->time->tm_sec = 0;
			file.ignore(); // Ignore blank space between time and label
			std::getline(file, nodeptr->label);
		}
		saveFile();
	}
}

/**
 * Main loop function to control class threads.
 */
void Alarm::mainLoop()
{
	t1 = std::thread(&Alarmer::userLoop, this);
	t2 = std::thread(&Alarmer::alarmLoop, this);
	t1.join();
	t2.join();
}

/**
 * Class destructor, saves file before deletion.
 */
Alarmer::~Alarmer()
{
	saveFile();
	Node *nodeptr = head;
	while (nodeptr)
	{
		head = head->next;
		delete nodeptr;
		nodeptr = head;
	}
}

#endif
