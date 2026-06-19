#include "shell.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

bool isExecutable(const string& filename) {
	if (filename.empty()) {
		return false; //blank input is not executable
	}
	if (filename.find('/') != string::npos) { //checks if user enters a path
		return access(filename.c_str(), X_OK) == 0; // checks if file is executable
	}

	const char* pathVariable = getenv("PATH"); //gets system PATH variable

	if (pathVariable == nullptr) {
		return false; //if no PATH, then cant do command search
	}

	stringstream pathStream(pathVariable); //read each PATH directory separately
	string directory; // 1 path directory at a time

	while (getline(pathStream, directory, ':')) { // reads directories separ. by :
		if (directory.empty()) {
			directory = "."; //empty entry = curr directory
		}
		string fullPath = directory + "/" + filename;
		if (access(fullPath.c_str(), X_OK) == 0) {
			return true; //if command exists and can be executed
		}
	}
	return false; //cmd not found in PATH
}
