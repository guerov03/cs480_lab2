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

    // Reject a pipe as the first token.
    if (tokens.front() == "|")
    {
        return false;
    }

    // Reject a pipe as the last token.
    if (tokens.back() == "|")
    {
        return false;
    }

    // Reject invalid pipe usage.
    for (size_t i = 1; i < tokens.size(); i++)
    {
    // Reject tokens like "||" because only a single pipe is valid.
    if (tokens[i].find('|') != std::string::npos && tokens[i] != "|")
    {
        return false;
    }
}
    // Reject consecutive pipes such as "ls || sort".
    for (size_t i = 1; i < tokens.size(); i++)
    {
        if (tokens[i] == "|" && tokens[i - 1] == "|")
        {
            return false;
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

