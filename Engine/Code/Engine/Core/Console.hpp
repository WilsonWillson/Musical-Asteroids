#pragma once
#include "Game/App.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/File/FileUtils.hpp"

#include <string>
#include <vector>

class Console
{
public:
		Console();
		~Console();
		void SaveConsoleToFile();
		
		void ConsoleCommand(int asKey);
		void SafeKeyUserPushed(int asKey);
		bool IsThisValidKey(int asKey);

		void RunConsoleCommands();
		void ClearCommandHistory();
		void PrintOutHelpMessage();
		void PrintOutLogHelpMessage();
		void PrintOutMemoryHelpMessage();
		void PrintOutInvalidMessage();
		void PrintOutGivenMessage(std::string message);
		void ChangeClearColor();
		void PrintCallstackInOutputWindow();

private:
	void RenderProfiler(bool enableProfiler);
	bool BasicConsoleCommands(std::vector<std::string> parsedConsoleCommands);
	bool LoggerConsoleCommands(std::vector<std::string> parsedConsoleCommands);
	bool MemoryConsoleCommands(std::vector<std::string> parsedConsoleCommands);

public:
		std::string m_currentConsoleCommand;
		std::vector<std::string> m_CommandHistory;
		int m_caratCounter;
		int m_consoleCounter;
private:
	bool NetworkingConsoleCommands(std::vector<std::string> parsedConsoleCommands);
};