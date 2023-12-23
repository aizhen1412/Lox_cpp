/*
 * File: lox.h
 * ------------
 * This file defines the Lox class, which is the main entry point for the Lox interpreter.
 * The Lox class provides static methods for running a Lox script from a file or from an interactive prompt.
 *
 * The RunFile method takes a file path as input, reads the Lox script from the file, and executes it.
 *
 * The RunPrompt method starts an interactive prompt where the user can enter Lox commands, which are executed immediately.
 *
 * The Run method is a private helper method that takes a Lox script as a string and executes it. This method is used by both RunFile and RunPrompt.
 */
#ifndef LOX_H
#define LOX_H

#include <string>

class Lox
{
public:
    static void RunFile(const std::string &filePath);
    static void RunPrompt();

private:
    static void Run(const std::string &source);
};

#endif // LOX_H