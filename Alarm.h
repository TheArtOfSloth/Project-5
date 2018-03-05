#pragma once
#ifndef ALARM_H
#define ALARM_H 1
#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <list>

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
	void save(Node alarm) {
		std::fstream file;
		file.open(filename, std::ios::out | std::ios::app | std::ios::binary);
		if (!file.is_open()) {
			std::cout << "Error saving to file\n";
		}
		else {
			file << alarm;
		}
	}
	//Menu interface & user interface loop
	void menu() {
		std::string line;
		std::cout << "1: Add new alarm\n"
			<< "2: View alarms\n"
			<< "3: Delete an alarm\n"
			<< "4: Quit\n";
		std::cout << ">";
		std::getline(std::cin, line);
		
		line == "1" ? addAlarm() :
		line == "2" ? getAlarms() :
		line == "3" ? removeAlarm() :
		line == "4" ? quit() : error();
	}

	void error() {
		std::cout << "Invalid entry selected\n";
	}

	void addAlarm() {
		std::string datetime;
		std::string date;
		std::string time;
		std::cout << "Enter date for alarm: ";
		std::cin >> date;
		std::cout << "Enter time to schedule alarm for: ";
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

		int i = 0;
		for (auto it : alarms) {
			std::cout << "#" << i << ": " << it;

			//NEED TO FIX THIS.
			/*if (it->i == num) {
				alarms.erase(it);
			}
			*/
			i++;
		}
	}

	void quit() {
		exit(0);
	}

	void setFilename() {
		std::string name;
		std::cout << "Enter filename: ";
		std::cin >> name;
		if (!name.find(".")) {
			filename = name + ".txt";
		}
		else {
			filename = name;
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

		menu();
	}
	~Alarm() { }
};

#endif
