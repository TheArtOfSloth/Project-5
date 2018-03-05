#pragma once
#ifndef ALARM_H
#define ALARM_H 1
#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>

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
			//std::copy(alarms.begin(), alarms.end(), ostream_iterator<std::string>(out, " "));
			return out;
		}

		~Node() {}
	};

	//List of alarms. Parsed from filename.
	std::vector<Node> alarms;
	//Filename - TESTING : Change later.
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
		
    //NOTE: Currently some issues with removeAlarm. FIX.
    
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
		//message = message + '\n';
		Node alarm = Node(datetime, message);
		alarms.push_back(alarm);
		save(alarm);
	}

	void getAlarms() {
		std::fstream file;
		std::string line;
		file.open(filename, std::fstream::in);
		if (!file.is_open()) {
			std::cout << "Error retrieving alarms. File may be corrupted.\n";
		}
		else {
			int i = 0;
			while (!file.eof()) {
				std::cout << "Alarm #" << i << ": ";
				std::getline(file, line);
				std::cout << line << "\n";
				i++;
			}
		}
		file.close();
	}

	void removeAlarm() {
		getAlarms();
		int num;
		std::cout << "Enter alarm number to delete: ";
		std::cin >> num;
		std::fstream file;
		file.open(filename, std::fstream::in);
		if (file.is_open()) {
			int line_ctr = 0;
			while (!file.eof()) {
				if (line_ctr != num) {
					std::string line;
					std::getline(file, line, ' ');

					std::string date = line;
					std::cin.ignore();
					std::getline(file, line, ' ');

					std::string time = line;
					std::string datetime = date + " " + time;
					std::cin.ignore();

					std::getline(file, line, '\n');
					std::string message = line;
					Node alarm = Node(datetime, message);
					alarms.push_back(alarm);
				}
				else {
					file.ignore(256, '\n');
				}
			}
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
		menu();
	}
	~Alarm() { }
};

#endif
