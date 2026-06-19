#ifndef SHELL_H
#define SHELL_H
#include <string>
#include <vector>

void runShell();

void executeCommand(const std::vector<std::string>& command);

bool isExecutable(const std::string& filename); //checks if a program can be exexcuted

std::vector<std::string> splitInput(const std::string& input); // separates user input into tokens

bool validateInput(const std::vector<std::string>& tokens); //checks for invalid input

std::vector<std::vector<std::string>> //organizes tokens into separate commands
buildCommands(const std::vector<std::string>& tokens);

void executePipeline( //executes commands connected in pipeline
	const std::vector<std::vector<std::string>>& commands
);

#endif
