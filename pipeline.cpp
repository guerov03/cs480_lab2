#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <iostream>

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
 * Example:
 * ls -la | sort -fi | grep .bashrc
 */
void executePipeline(const std::vector<std::vector<std::string>>& commands)
{
    std::cout << "Pipeline function not finished yet." << std::endl;
}
