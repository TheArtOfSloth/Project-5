#pragma once
#ifndef ALARM_H
#define ALARM_H 1
#define _CRT_SECURE_NO_WARNINGS 1
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>

class Alarm
{
	class Node {
		std::string timestamp;
		std::string message;
	public:
		Node(std::string timestamp, std::string message) {
			this->timestamp = timestamp;
			this->message = message;
		}

		friend std::ostream & operator<<(std::ostream & out, const Node & alarm) {
			out << alarm.timestamp << " " << alarm.message << "\n";
			return out;
		}

		~Node() {}
	};

	//List of alarms. Parsed from filename.
	std::list<Node> alarms;
	//Filename - CHANGE LATER.
	std::string filename = "C:\\Users\\John\\Desktop\\Schedule.txt";
	//Open file
	void open(std::string filename) {
		std::fstream file;
		std::string line;
		file.open(filename, std::ios::out | std::ios::app | std::ios::binary);
		if (!file.is_open())
			std::cout << "Error: file failed to open\n";

		while (!file.eof()) {
			std::getline(file, line, '\n');
			std::cout << line;
		}

		file.close();
	}
	//TODO: Change from append mode to truncate and overwrite file from std::list contents.
	void save(Node alarm) {
		std::fstream file;
		file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
		if (!file.is_open()) {
			std::cout << "Error saving to file\n";
		}
		else {
			file << alarm;
		}
		file.close();
	}

	void menu() {
		std::cout << "1: Add new alarm\n"
			<< "2: View alarms\n"
			<< "3: Delete an alarm\n"
			<< "4: Check current time\n"
			<< "5: Quit\n";
		std::cout << ">";
	}

	//Menu interface & user interface loop
	void polling() {
		//Tarin here, trying to set up a polling loop
		bool exitVar = false;

		while (!exitVar) {
			std::string line;
			menu();
			std::getline(std::cin, line);

			if (line == "1") {
				addAlarm();
			}
			else if (line == "2") {
				getAlarms();
			}
			else if (line == "3") {
				removeAlarm();
			}
			else if (line == "4") {
				time();
			}
			else {
				exitVar = true;
			}
		}
		quit();

	}

	void time() {
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		time_t tt = std::chrono::system_clock::to_time_t(now);
		tm local_tm = *localtime(&tt);

		//Capture timepoint for current time
		std::string year = local_tm.tm_year + 1900 + " ";
		std::string mon = local_tm.tm_mon + 1 + "";
		std::string day = local_tm.tm_mday + "";
		std::string hour = local_tm.tm_hour + "";
		std::string min = local_tm.tm_min + "";
		std::string sec = local_tm.tm_sec + "";
		//Convert entered date versus timepoint to see how long til alarm goes off.


	}

	void error() {
		std::cout << "Invalid entry selected\n";
	}

	void addAlarm() {
		std::string datetime;
		std::string date;
		std::string time;
		std::cout << "Enter date for alarm (YYYY/MM/DD): ";
		std::cin >> date;
		std::cout << "Enter time to schedule alarm for (00:00 - 23:59): ";
		std::cin >> time;
		datetime = date + " " + time;
		std::string message;
		std::cin.ignore();
		std::cout << "Enter message for alarm: ";
		std::getline(std::cin, message);
		std::cin.ignore();
		message = message + '\n';
		Node alarm = Node(datetime, message);
		alarms.push_back(alarm);
		save(alarm);
	}

	void getAlarms() {
		int i = 0;
		for (auto it : alarms) {
			std::cout << "#" << i << ": " << it;
			i++;
		}
	}

	void removeAlarm() {
		getAlarms();
		int num;
		std::cout << "Enter alarm number to delete: ";
		std::cin >> num;
		// Daniel's implementation
		if (!alarms.empty()) {
			std::list<Node>::iterator remove;
			remove = alarms.begin();
			advance(remove, num);
			alarms.erase(remove);

			std::cout << "Success?\n";
			for (auto it : alarms) {
				save(it);

				std::cout << it;
			}

		}
		//There are some issues with the old list (after removal) writing to the file properly.
		//As such, we can create a new list to store the values. Then write the new list into the file.
		/*std::list<Node> newAlarms;
		for (auto it : alarms) {
			newAlarms.push_back(it);
		}
		for (auto it : newAlarms) {
			save(it);
		}
		std::cout << "Success?\n";
		getAlarms();
		*/
		//This is to capture user input and prevent the program from closing.
		std::cin.get();
		getAlarms();
		
	}

	void quit() {
		exit(0);
	}


	//TODO: Create wizard to let user save file elsewhere.
	//For time being, save the file to the current directory?
	void setFilename() {
		std::string name;
		std::cout << "Enter filename: ";
		std::cin >> name;
		if (!name.find(".")) {
			filename = "./" + name + ".txt";
		}
		else {
			filename = "./" + name;
		}
	}

public:
	Alarm() {
		//setFilename();
		//Parse any contents of file into alarms list.
		std::fstream file;
		std::string datetime, message, line;
		file.open(filename, std::fstream::in);
		while (!file.eof()) {
			std::getline(file, line, '_');
			datetime = line;
			std::getline(file, line, '\n');
			message = line;
			Node alarm = Node(datetime, message);
			alarms.push_back(alarm);
		}
		file.close();
		int i = 0;
		for (auto it : alarms) {
			std::cout << "#" << i << ": " << it;
			i++;
		}

		polling();
	}
	~Alarm() { }
};

#endif
