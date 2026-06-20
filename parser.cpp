#include "shell.h"
#include <sstream>
#include <string>
#include <vector>

/*
 * Author: Chris Palomares
 * Assignment 2 - XSH
 *
 * This file contains functions used to process
 * and validate user input before commands are executed.
 */

/*
 * splitInput()
 *
 * Takes a line of input entered by the user and
 * separates it into individual tokens using spaces
 * as delimiters.
 *
 * Example:
 * Input: "ls -la | sort -fi"
 *
 * Output:
 * ["ls", "-la", "|", "sort", "-fi"]
 */
std::vector<std::string> splitInput(const std::string& input)
{
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    // Read each token from the user's command line.
    while (ss >> token)
    {
        // Store the token so it can be processed later.
        tokens.push_back(token);
    }

    return tokens;
}

/*
 * validateInput()
 *
 * Checks whether the user entered a command in one
 * of the valid formats required by the assignment.
 *
 * Invalid examples:
 * |
 * ls |
 * | sort
 * ls || sort
 *
 * Valid examples:
 * ls
 * ls -la
 * ls | sort
 * ls -la | sort -fi
 */
bool validateInput(const std::vector<std::string>& tokens)
{
    // Reject empty input.
    if (tokens.empty())
    {
        return false;
    }

    // The exit command is only valid by itself.
    if (tokens[0] == "exit")
    {
        return tokens.size() == 1;
    }

    bool hasPipe = false;

    // Reject bad pipe tokens like "||" or "ls|sort".
    for (const std::string& token : tokens)
    {
        if (token == "|")
        {
            hasPipe = true;
        }

        if (token.find('|') != std::string::npos && token != "|")
        {
            return false;
        }
    }

    // Without pipes, only these are valid:
    // executablefilename
    // executablefilename argument
    if (!hasPipe)
    {
        return tokens.size() == 1 || tokens.size() == 2;
    }

    // With pipes, only this pattern is valid:
    // executable | executable | executable
    if (tokens.front() == "|" || tokens.back() == "|")
    {
        return false;
    }

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (i % 2 == 0)
        {
            // Commands must be in even positions.
            if (tokens[i] == "|")
            {
                return false;
            }
        }
        else
        {
            // Pipes must be in odd positions.
            if (tokens[i] != "|")
            {
                return false;
            }
        }
    }

    return true;
}

/*
 * buildCommands()
 *
 * Organize the token list into separate commands.
 * Each pipe symbol marks the beginning of a new command.
 *
 * Example:
 * Input:
 * ["ls", "-la", "|", "sort", "-fi"]
 *
 * Output:
 * [
 *  ["ls", "-la"],
 *  ["sort", "-fi"]
 * ]
 */

std::vector<std::vector<std::string>>
buildCommands(const std::vector<std::string>& tokens)
{
    std::vector<std::vector<std::string>> commands;
    std::vector<std::string> currentCommand;

    // Process each token in order.
    for (const std::string& token : tokens)
    {
        // When a pipe is found, save the current command
        // and start building the next one.
        if (token == "|")
        {
            commands.push_back(currentCommand);
            currentCommand.clear();
        }
        else
        {
            currentCommand.push_back(token);
        }
    }

    // Add the final command after the loop finishes.
    if (!currentCommand.empty())
    {
        commands.push_back(currentCommand);
    }

    return commands;
}

