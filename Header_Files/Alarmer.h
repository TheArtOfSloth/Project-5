/**
* File holding Alarm class, methods, and necessary values.
* @author	KLAPSTEIN_DANIEL
* @author	SMITH_EVAN
*/
#ifndef ALARMER_H
#define ALARMER_H 1
#define _CRT_SECURE_NO_WARNINGS 1

// LIBRARIES
#include "Alarmer.h"

#include<chrono>
#include<ctime>
#include<fstream>
#include<iostream>
#include<stdexcept>
#include<thread>
#include<string>
#include <list>

using namespace std;

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
	Alarmer(string);		    // Constructor taking file name
	void mainLoop();			      // Main polling loop to hold threads
	~Alarmer();				        // Class destructor
private:
	struct Node					        // Struct to hold each individual alarm
	{
		std::string label;		    // Alarm label
		struct tm time;			      // ctime object to hold time
									  //Node *next;				        // Pointer to next Node
	};
	list<Node> alarms;
	//Node *head;					        // Head pointer to Node list
	bool isRunning, soundAlarm, placeholder;	// Semaphore booleans for threads
	string filename;		    // String to hold file name
	thread t1, t2;			    // Threads to run loops
	void addAlarm();			      // Function to add a new alarm
	void alarmLoop();			      // Thread function to handle alarm checking
	void deleteAlarm();			    // Function to remove alarm from list
	void saveFile();			      // Function to sort alarms and save to filename
	void userLoop();			      // Thread function to handle user input
	void viewAlarms();			    // Function to display all alarms
};

//default constructor
Alarmer::Alarmer()
{

};

//standard constructor
Alarmer::Alarmer(string a)
{

};

//destructor
Alarmer::~Alarmer()
{

};

//delete an Alarm
void Alarmer::deleteAlarm()
{

};

//view alarms
void Alarmer::viewAlarms()
{

};

//adds alarm
void Alarmer::addAlarm()
{
	Node temp = Node();
	cout << "Please enter the name of the alarm: ";
	cin >> temp.label;
	cout << "Please enter the month of the alarm: ";
	cin >> temp.time.tm_mon;
	temp.time.tm_mon -= 1;
	cout << "Please enter the day of the alarm: ";
	cin >> temp.time.tm_mday;
	cout << "Please enter the year of the alarm: ";
	cin >> temp.time.tm_year;
	temp.time.tm_year -= 1900;
	cout << "Please enter the hour of the alarm, from 0-23: ";
	cin >> temp.time.tm_hour;
	cout << "Please enter the minute of the alarm, from 0-59: ";
	cin >> temp.time.tm_min;
	temp.time.tm_sec = 0;
	//temp->next = head;
	//head = temp;
	alarms.push_back(temp);
	//sortList();//-----------------------------------------------------------------------------------------
};


//runs user input
void Alarmer::userLoop()
{
	int command1;
	while (isRunning)
	{
		if (soundAlarm)
		{
			cout << "ALARM TIME!!! Type any entry to exit." << endl;
			cin >> command1;
			soundAlarm = false;
			deleteAlarm();
		}
		else
		{

			cout << "1 - View Next Alarm" << endl;
			cout << "2 - Add New Alarm" << endl;
			cout << "3 - Delete Alarm" << endl;
			cout << "4 - Exit" << endl;
			cout << "5 - Test Dummy Alarm" << endl;
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
	Node *viewer = new Node;
	Node *viewed = new Node;
	viewer = &alarms.front();
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
		if (!alarms.empty() && (mktime((&viewer->time)) <= tt))
			soundAlarm = true;
		/*
		while (viewer)
		{
			if (viewer && (mktime((&viewer->time)) <= tt))
			{
				soundAlarm = true;
				viewed->next = viewer->next;
				viewer->next = head;
				head = viewer;
				break;
			}

		}
		*/
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
	delete viewer;
	delete viewed;
};

//runs the threads
void Alarmer::mainLoop()
{
	t1 = thread(&Alarmer::userLoop, this);
	t2 = thread(&Alarmer::alarmLoop, this);
	t1.join();
	t2.join();
};
// FUNCTIONS : Alarmer
// NONE

#endif
