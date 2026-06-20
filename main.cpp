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

void executeCommand(const vector<string>& command) { //runs a command in a child process
	if (command.empty()) { //makes sure the command is not blank
		return;
}

	if (!isExecutable(command[0])) { //checks if it can exec
		cerr << "xsh: " << command[0] << " file does not exist / is not executable" << endl;
		return;
	}

	pid_t childPid = fork(); //new child process
	if (childPid < 0) { //if fork fails, send error
		perror("xsh: fork");
		return;
	}
	if (childPid == 0) {
		vector<char*> arguments; //stores arguments in execvp format
		for (const string& token : command) { // copies command tokens
			arguments.push_back(const_cast<char*>(token.c_str()));
		}

		arguments.push_back(nullptr); //required for execvp
		execvp(arguments[0], arguments.data()); //swaps for requested program
		perror("xsh: execvp"); //if fails
		_exit(EXIT_FAILURE); //Ends if fails
	}

	if (waitpid(childPid, nullptr, 0) < 0) { //wait for child to finish
		perror("xsh: waitpid");
	}
}

void runShell() {
	const char* user = getenv("USER"); //get edoras user
	string username = "xsh"; //default

	if (user != nullptr) {
		username = user;
	}

	string input;
	while (true) { //keeps alive
		cout << username << "% "; //prints prompt
		cout.flush(); //neatness

		if(!getline(cin, input)) { //if input stops then break
			cout << endl;
			break;
		}

		vector<string> tokens = splitInput(input);

		if (tokens.empty()) { // ignore blanks
			continue;
		}

		if (!validateInput(tokens)) {
			cerr << "xsh: invalid input" << endl;
			continue;
		}
		if (tokens.size() == 1 && tokens[0] == "exit") {
			break; //exit
		}
		vector<vector<string>> commands = buildCommands(tokens);
		if (commands.size() == 1) {
			executeCommand(commands[0]); //runs command
		}
		else {
			executePipeline(commands);
		}
	}
}

int main() {
	runShell();
	return 0;
}
