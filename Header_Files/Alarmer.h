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
#include<string>

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
	bool isRunning, soundAlarm, placeholder;	// Semaphore booleans for threads
	std::string filename;		    // String to hold file name
	std::thread t1, t2;			    // Threads to run loops
	void addAlarm();			      // Function to add a new alarm
	void alarmLoop();			      // Thread function to handle alarm checking
	void deleteAlarm();			    // Function to remove alarm from list
	void saveFile();			      // Function to sort alarms and save to filename
	void userLoop();			      // Thread function to handle user input
	void viewAlarms();			    // Function to display all alarms
};


//runs user input
void Alarmer::userLoop()
{
	string command1;
	while (isRunning)
	{
		if (soundAlarm)
		{
			cout << "ALARM TIME!!! Type any entry to exit." << endl;
			cin >> command1;
			soundAlarm = false;
			deleteNextAlarm();
		}
		else
		{
			
			cout <<  "1 - View Next Alarm" << endl;
			cout <<  "2 - Add New Alarm" << endl;
			cout <<  "3 - Delete Alarm" << endl;
			cout <<  "4 - Exit" << endl;
			cout <<  "5 - Test Dummy Alarm" << endl;
			cin >> command1;
			cout << endl;
			switch (command1)
			{
			case 1:
			{
				viewAlarms();
				break;
			};
			case 2:
			{
				addAlarm();
				break;
			};
			case 3:
			{
				deleteAlarm();
				break;
			};
			case 4:
			{
				isRunning = false;

				break;
			};
			case 5:
			{
				placeholder = true;
				break;
			};
			default:
			{
				cout << "Invalid option. Try again." << endl;
				break;
			};
			};
		}
	};
};

//runs the alarm
void Alarmer::alarmLoop()
{
	Events* viewer = new Events;
	Events* viewed = new Events;
	viewer = head;
	viewed = viewer;
	using chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());
	struct tm * ptm = localtime(&tt);
	while (isRunning)
	{
		tt = system_clock::to_time_t(system_clock::now());
		ptm = localtime(&tt);    //Syntax error
		if (placeholder /*|| (head && (mktime((&head->when)) <= tt))*/)
		{
			placeholder = false;
			soundAlarm = true;
		};
		if (head && (mktime((&head->when)) <= tt))
			soundAlarm = true;
		while (viewer)
		{
			if (viewer && (mktime((&viewer->when)) <= tt))
			{
				soundAlarm = true;
				viewed->next = viewer->next;
				viewer->next = head;
				head = viewer;
				break;
			}

		}
		if (soundAlarm)
		{
			while (soundAlarm)
			{
				cout << '\a';
				this_thread::sleep_for(chrono::seconds(1));
			};
		}
		this_thread::sleep_for(chrono::seconds(1));
	}
};

//runs the threads
void doomClock::mainLoop()
{
	t1 = thread(&Alarmer::userLoop, this);
	t2 = thread(&Alarmer::alarmLoop, this);
	t1.join();
	t2.join();
};
// FUNCTIONS : Alarmer
// NONE

#endif
