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
#include<cctype>

using namespace std;

// GLOBAL CONSTANTS / VARIABLES
// NONE

// CLASSES / STRUCTS

struct Node					        // Struct to hold each individual alarm
{
	string label;		    // Alarm label
	struct tm time;			// ctime object to hold time
};


/*
//for comparing two nodes
bool compareTwoNodes(Node &a, Node &b)
{
	{
		struct tm nothingBefore;
		nothingBefore.tm_year = 0;
		nothingBefore.tm_mon = 0;
		nothingBefore.tm_mday = 1;
		nothingBefore.tm_hour = 0;
		nothingBefore.tm_min = 0;
		nothingBefore.tm_sec = 0;
		return difftime(mktime(&a.time), mktime(&nothingBefore)) < difftime(mktime(&b.time), mktime(&nothingBefore));
	};
};
*/

//for comparing two nodes. true=sorted, false=unsorted
bool compareTwoNodes(Node &a, Node &b)
{
	if (a.time.tm_year < b.time.tm_year)
		return true;
	else
	{
		if (a.time.tm_mon < b.time.tm_mon)
			return true;
		else
		{
			if (a.time.tm_mday < b.time.tm_mday)
				return true;
			else
			{
				if (a.time.tm_hour < b.time.tm_hour)
					return true;
				else
				{
					if (a.time.tm_min <= b.time.tm_min)
						return true;
				}
			}
		}

	}
	return false;
};


//for sorting a list of nodes
void sortList(list<Node> a)
{
	if (!a.empty())
	{
		Node holder = Node();	//holds contents during swapping
		Node* b = NULL;	//dynamic Node pointer
		int c;			//int for holding list size
		c = a.size();	//set c equal to list size
		b = new Node[c];	//dynamic array for easier sorting
		//transfer list contents to the array
		for (int i = 0; i < c; i++)
		{
			b[i] = a.front();
			a.pop_front();
		}
		//sort the array
		//---------------------------------------------------------------
		for (int i = 0; i < c; i++)
		{
			for (int j = i; j < c; j++)
			{
				if (!compareTwoNodes(b[i], b[j]))
				{
					holder.label = b[i].label;
					holder.time = b[i].time;
					b[i].label = b[j].label;
					b[i].time = b[j].time;
					b[j].label = holder.label;
					b[j].time = holder.time;
				}
			}
		}
		//---------------------------------------------------------------
		//transfer sorted array contents to list
		for (int i = 0; i < c; i++)
		{
			a.push_back(b[i]);
		}
		//deallocate dynamic array
		delete[] b;
		b = NULL;
	}
};


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
	list<Node> alarms;
	bool isRunning, soundAlarm, dummy, dummy2;	// Semaphore booleans for threads
	string filename;		    // String to hold file name
	thread t1, t2;			    // Threads to run loops
	void addAlarm();			      // Function to add a new alarm
	void alarmLoop();			      // Thread function to handle alarm checking
	void deleteAlarm();			    // Function to remove alarm from list
	void saveFile();			      // Function to sort alarms and save to filename
	void userLoop();			      // Thread function to handle user input
	void viewAlarms();			    // Function to display all alarms
	void sortAlarms();				//Function to sort list of alarms
	void deleteAlarmOnCall();		//deletes any alarm with a time earlier than the present
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

//saves the file
void Alarmer::saveFile()
{

};

//sorts the list of Alarms
void Alarmer::sortAlarms()
{
	alarms.sort(compareTwoNodes);
};

//delete an Alarm
void Alarmer::deleteAlarm()
{
	viewAlarms();
	if (!alarms.empty())
	{
		int num;
		cout << "Enter alarm number to delete: ";
		cin >> num;
		list<Node>::iterator it1;
		it1 = alarms.begin();
		advance(it1, num);
		alarms.erase(it1);
	}
};

//deletes alarms in thread
void Alarmer::deleteAlarmOnCall()
{
	if (!alarms.empty())
	{
		using chrono::system_clock;
		time_t tt = system_clock::to_time_t(system_clock::now());
		struct tm * ptm = localtime(&tt);
		int i = 0;
		list<Node>::iterator it1;
		it1 = alarms.begin();
		for (auto it : alarms)
		{
			if ((mktime((&it.time)) <= tt))
			{
				advance(it1, i);
				alarms.erase(it1);
				break;
			}
			i++;
		}
	}
}

//view alarms
void Alarmer::viewAlarms()
{
	if (!alarms.empty())
	{
		int i = 0;
		for (auto it : alarms) {
			cout << "#" << i << ": " << it.time.tm_hour << ":" << it.time.tm_min << " on " << (it.time.tm_mon) + 1 << "/" << it.time.tm_mday << "/" << (it.time.tm_year) + 1900 << "     " << it.label << endl;
			i++;
		}
	}

	else
	{
		cout << "There are no scheduled events." << endl;
	}
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
	alarms.push_back(temp);
	//sortList();//-----------------------------------------------------------------------------------------
	//sortAlarms();
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
			if (!dummy2)
				deleteAlarmOnCall();
				//alarms.pop_front();
			dummy2 = false;
		}
		else
		{

			cout << "1 - View Alarms" << endl;
			cout << "2 - Add New Alarm" << endl;
			cout << "3 - Delete an Alarm" << endl;
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
				dummy = true;
				dummy2 = true;
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
	Node viewer = Node();
	Node *viewed = new Node;
	if (!alarms.empty())
	{
		viewer = alarms.front();
		viewed = &viewer;
	}
	else
	{
		viewed = NULL;
	}
	using chrono::system_clock;
	time_t tt = system_clock::to_time_t(system_clock::now());
	struct tm * ptm = localtime(&tt);
	while (isRunning)
	{
		tt = system_clock::to_time_t(system_clock::now());
		ptm = localtime(&tt);
		if (!alarms.empty())
		{
			viewer = alarms.front();
			viewed = &viewer;
		}
		else
		{
			viewed = NULL;
		};
		if (dummy)
		{
			dummy = false;
			soundAlarm = true;
		};
		//-------------------------------------------------------
		if (!alarms.empty())
		{
			for (auto it : alarms)
			{
				if ((mktime((&it.time)) <= tt))
					soundAlarm = true;
			}
		}
		//-------------------------------------------------------
		/*
		if (!alarms.empty() && (mktime((&viewed->time)) <= tt))
			soundAlarm = true;
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
};

//runs the threads
void Alarmer::mainLoop()
{
	isRunning = true;
	soundAlarm = false;
	dummy = false;
	t1 = thread(&Alarmer::userLoop, this);
	t2 = thread(&Alarmer::alarmLoop, this);
	t1.join();
	t2.join();
};
// FUNCTIONS : Alarmer
// NONE

#endif
