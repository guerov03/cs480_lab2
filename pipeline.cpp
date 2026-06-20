#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
 * Author: Chris Palomares and Eduardo Valdovinos
 * Assignment 2 - XSH
 *
 * This file contains the code needed to execute commands
 * connected by pipes.
 */

/*
 * executePipeline()
 *
 * Executes multiple commands connected by pipes.
 *
 * Example:
 * ls -la | sort -fi | grep .bashrc
 *
 * The output from one command becomes the input for
 * the next command in the pipeline.
 */
void executePipeline(const vector<vector<string>>& commands)
{
    int numberOfCommands = commands.size();

    // A pipeline needs at least two commands.
    if (numberOfCommands < 2)
    {
        return;
    }

    int previousPipeRead = -1;
    vector<pid_t> children;

    for (int i = 0; i < numberOfCommands; i++)
    {
        int pipeFileDescriptors[2];

        // Create a pipe for every command except the last one.
        if (i < numberOfCommands - 1)
        {
            if (pipe(pipeFileDescriptors) < 0)
            {
                perror("xsh: pipe");
                return;
            }
        }

        pid_t childPid = fork();

        if (childPid < 0)
        {
            perror("xsh: fork");
            return;
        }

        if (childPid == 0)
        {
            // If this is not the first command, read input from the previous pipe.
            if (previousPipeRead != -1)
            {
                dup2(previousPipeRead, STDIN_FILENO);
                close(previousPipeRead);
            }

            // If this is not the last command, send output into the current pipe.
            if (i < numberOfCommands - 1)
            {
                close(pipeFileDescriptors[0]);
                dup2(pipeFileDescriptors[1], STDOUT_FILENO);
                close(pipeFileDescriptors[1]);
            }

            vector<char*> arguments;

            // Convert C++ strings into the char* format needed by execvp.
            for (const string& token : commands[i])
            {
                arguments.push_back(const_cast<char*>(token.c_str()));
            }

            arguments.push_back(nullptr);

            execvp(arguments[0], arguments.data());

            // If execvp returns, something went wrong.
            perror("xsh: execvp");
            _exit(EXIT_FAILURE);
        }

        children.push_back(childPid);

        // Parent no longer needs the previous pipe read end.
        if (previousPipeRead != -1)
        {
            close(previousPipeRead);
        }

        // Parent keeps the read end of the current pipe for the next command.
        if (i < numberOfCommands - 1)
        {
            close(pipeFileDescriptors[1]);
            previousPipeRead = pipeFileDescriptors[0];
        }
    }

    // Wait for every child process in the pipeline.
    for (pid_t child : children)
    {
        waitpid(child, nullptr, 0);
    }
}
