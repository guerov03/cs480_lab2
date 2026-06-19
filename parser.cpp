#include "shell.h"
#include <sstream>
#include <string>
#include <vector>

/*
 * Author: Chris Palomares
 * Assignment 2 - XSH
 *
 * This file contains functions used to process
 * and validate user input before commands are executed
 */

/*
 *
 * splitInput()
 *
 * Takes a line of input entered by the user and
 * separates it into individual tokens using spaces
 * as delimiters
 *
 * Example:
 * Input: "ls - la | sort -fi"
 *
 * Output:
 * ["ls" , "-la", "|", "sort", "-fi"]
 */
std::vector<std::string> splitInput(const std::string& input)
{
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string token;

	// Reads each word or pipe symbol from the input line.
	while (ss >> token)
	{
		// Add the token to the list so the shell can process it later.
		tokens.push_back(token);
	}

	return tokens;
}

