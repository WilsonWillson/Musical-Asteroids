#include "Engine/Core/Console.hpp"
#include "Engine/Core/MemoryManager.hpp"
#include "Engine/Core/CallStack.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/Profiler.hpp"
#include "Engine/Networking/NetSystem.hpp"
#include "Engine/Networking/net_address_t.hpp"
#include "Engine/Networking/RemoteCommandService.hpp"
#include "Game/Game.hpp"
#include <string>
#include <vector>
Console::Console()
{
	m_currentConsoleCommand = "";
	m_CommandHistory;
	m_caratCounter = 0;
     m_consoleCounter = 0 ;
}

Console::~Console()
{

}

void Console::SaveConsoleToFile()
{
	SaveBufferToBinaryFile("data/consolefile.txt", m_CommandHistory);
}


//-----------------------------Console Logic---------------------------


void Console::ConsoleCommand(int asKey)
{
	if (asKey == KEY_ESCAPE)
	{
		if (m_currentConsoleCommand == "")
			g_theApp->DeleteConsoleWindow();
		else
			m_currentConsoleCommand = "";
		
		return;
	}

	if (asKey == KEY_ENTER)
	{
		m_currentConsoleCommand = m_currentConsoleCommand.substr(0, m_currentConsoleCommand.size());
		m_CommandHistory.push_back(m_currentConsoleCommand);
		RunConsoleCommands();
		return;
	}

	if (asKey == KEY_BACKSPACE)
	{
		m_currentConsoleCommand = m_currentConsoleCommand.substr(0, m_currentConsoleCommand.size() - 1);
		return;
	}

	if (asKey == KEY_LEFTARROW)
	{
		m_caratCounter--;
		if (m_currentConsoleCommand.length() < (unsigned)abs(m_caratCounter))
			m_caratCounter++;
		return;
	}

	if (asKey == KEY_RIGHTARROW)
	{
		m_caratCounter++;
		if (m_caratCounter > 0)
			m_caratCounter = 0;
		return;
	}

	if (asKey == KEY_UPARROW)
	{
		m_consoleCounter--;
		return;
	}

	if (asKey == KEY_DOWNARROW)
	{
		m_consoleCounter++;
		if (m_consoleCounter > 0)
			m_consoleCounter = 0;
		return;
	}

	SafeKeyUserPushed(asKey);
}


bool Console::IsThisValidKey(int asKey)
{
	//IGNORE f1 - f12 keys
	if (asKey == 46)
		return true;
	if (asKey >= 0x70 && asKey <= 0x7B)
		return false;
	//Ignore Keys lower than Space
	if (asKey < 0x20)
		return false;
	//Ignore keys lower than 0 and higher than space
	if (asKey > 0x20 && asKey < 0x30)
		return false;
	//Ignore non letter and number keys
	if (asKey > 0x5A)
		return false;

	return true;
}

void Console::SafeKeyUserPushed(int asKey)
{
	if (!IsThisValidKey(asKey))
		return;

	tolower(asKey);
	std::string s1;
	s1 = (char)asKey;

	if (m_caratCounter == 0)
	{
		int insertLocation = m_currentConsoleCommand.length();
		m_currentConsoleCommand.insert(insertLocation, s1);
	}
	else
	{
		int insertLocation = 0;
		insertLocation = m_currentConsoleCommand.length() + m_caratCounter - 1;
		m_currentConsoleCommand.insert(insertLocation, s1);
	}
}



//-------------------------Console Commands--------------------------------------------
void Console::RunConsoleCommands()
{
	std::vector<std::string> parsedConsoleCommands; 
	parsedConsoleCommands = ParseStringIntoPiecesByDelimiter(m_currentConsoleCommand, ' ');

	bool validCommand = false;

	if(validCommand == false)
		validCommand = BasicConsoleCommands(parsedConsoleCommands);
	if (validCommand == false)
		validCommand = MemoryConsoleCommands(parsedConsoleCommands);
	if (validCommand == false)
		validCommand = LoggerConsoleCommands(parsedConsoleCommands);
	if (validCommand == false)
		validCommand = NetworkingConsoleCommands(parsedConsoleCommands);

	if (validCommand == false)
		PrintOutInvalidMessage();

	m_currentConsoleCommand = "";
}

bool Console::BasicConsoleCommands(std::vector<std::string> parsedConsoleCommands)
{
    if (m_currentConsoleCommand == "help" || m_currentConsoleCommand == "HELP")
	{
		PrintOutHelpMessage();
		return true;
	}
	else if (m_currentConsoleCommand == "quit" || m_currentConsoleCommand == "QUIT")
	{
		g_theApp->OnExitRequested();
		return true;
	}
	else if (m_currentConsoleCommand == "clear" || m_currentConsoleCommand == "CLEAR")
	{
		ClearCommandHistory();
		return true;
	}
	else if (m_currentConsoleCommand == "loghelp" || m_currentConsoleCommand == "LOGHELP")
	{ 
		PrintOutLogHelpMessage();
		return true;
	}
	else if (m_currentConsoleCommand == "memoryhelp" || m_currentConsoleCommand == "MEMORYHELP")
	{ 
		PrintOutMemoryHelpMessage();
		return true;
	}
	else if (m_currentConsoleCommand.compare(0, 5, "COLOR") == 0)
	{
		ChangeClearColor();
		return true;
	}
	else if (parsedConsoleCommands[0] == "DebugRender" || parsedConsoleCommands[0] == "DEBUGRENDER")
	{ 
		g_theApp->RenderDebugHelper();
	return true;
	}

	return false;
}

bool Console::LoggerConsoleCommands(std::vector<std::string> parsedConsoleCommands)
{
	if (parsedConsoleCommands[0] == "THREADTEST" || parsedConsoleCommands[0] == "threadtest")
		{ 
			ThreadTest(std::stoi(parsedConsoleCommands[1], nullptr, 10));
			return true;
		}
		else if (parsedConsoleCommands[0] == "LOGTEST" || parsedConsoleCommands[0] == "logtest")
		{
			LogTest(std::stoi(parsedConsoleCommands[1], nullptr, 10), std::stoi(parsedConsoleCommands[2], nullptr, 10)); 
			return true;
		}
		else if (parsedConsoleCommands[0] == "LOGENABLE" || parsedConsoleCommands[0] == "logenable")
		{
			LogEnable(parsedConsoleCommands[1].c_str());
			return true;
		}
		else if (parsedConsoleCommands[0] == "LOGENABLEALL" || parsedConsoleCommands[0] == "logenableall")
		{
			LogEnableAll();
			return true;
		}
		else if (parsedConsoleCommands[0] == "LOGDISABLE" || parsedConsoleCommands[0] == "logdisable")
		{
			LogDisable(parsedConsoleCommands[1].c_str());
			return true;
		}
		else if (parsedConsoleCommands[0] == "LOGDISABLEALL" || parsedConsoleCommands[0] == "logdisable")
		{
			LogDisableAll(parsedConsoleCommands);
			return true;
		}
		else if (m_currentConsoleCommand == "log flush" || m_currentConsoleCommand == "LOG FLUSH")
		{
			LogFlush();
			return true;
		}

	return false;
}

bool Console::MemoryConsoleCommands(std::vector<std::string> parsedConsoleCommands)
{
	if (m_currentConsoleCommand == "Print Callstack" || m_currentConsoleCommand == "PRINT CALLSTACK")
	{
		PrintCallstackInOutputWindow();
		return true;
	}
	else if (m_currentConsoleCommand == "consolecount" || m_currentConsoleCommand == "CONSOLECOUNT")
	{
		MemoryProfilerLogLiveAllocationsForFrameAmount(1);
		return true;
	}
	else if (m_currentConsoleCommand == "consolecount" || m_currentConsoleCommand == "CONSOLECOUNT")
	{
		MemoryProfilerLogLiveAllocationsForFrameAmount(1);
		return true;
	}
	else if (m_currentConsoleCommand == "profile" || m_currentConsoleCommand == "PROFILE")
	{
		setProfilerEnabled(true);
		//g_theApp->m_isConsoleCommandOpen = false;
		return true;
	}
	else if (m_currentConsoleCommand == "profilepause" || m_currentConsoleCommand == "PROFILEPAUSE")
	{
		Profiler::ProfilerPause();
		return true;
	}
	else if (m_currentConsoleCommand == "profileresume" || m_currentConsoleCommand == "PROFILERESUME")
	{
		Profiler::ProfilerResume();
		return true;
	}
	else if (m_currentConsoleCommand == "profilesnapshot" || m_currentConsoleCommand == "PROFILESNAPSHOT")
	{
		Profiler::ProfilerSnapshot();
		return true;
	}
	else if (m_currentConsoleCommand == "treeview" || m_currentConsoleCommand == "TREEVIEW")
	{
		Profiler::setTreeView();
		return true;
	}
	else if (m_currentConsoleCommand == "flatview" || m_currentConsoleCommand == "FLATVIEW")
	{
		Profiler::setFlatView();
		return true;
	}
	else if (m_currentConsoleCommand == "sorttotaltime" || m_currentConsoleCommand == "SORTTOTALTIME")
	{
		Profiler::setSortByTotalTime();
		return true;
	}
	else if (m_currentConsoleCommand == "sortselftime" || m_currentConsoleCommand == "SORTSELFTIME")
	{
		Profiler::setSortBySelfTime();
		return true;
	}
	else if (m_currentConsoleCommand == "profileoff" || m_currentConsoleCommand == "PROFILEOFF")
	{
		setProfilerEnabled(false);
		return true;
	}

	return false;
}

bool Console::NetworkingConsoleCommands(std::vector<std::string> parsedConsoleCommands)
{
	if (parsedConsoleCommands[0] == "ipport" || parsedConsoleCommands[0] == "IPPORT")
	{
		NetSystemConnectToHost(parsedConsoleCommands[1].c_str(), std::stoi(parsedConsoleCommands[2]));
		return true;
	}

	if (parsedConsoleCommands[0] == "iphost" || parsedConsoleCommands[0] == "IPHOST")
	{
		NetSystemHostConnection();
		return true;
	}
	if (parsedConsoleCommands[0] == "ipping" || parsedConsoleCommands[0] == "IPPING")
	{
		//g_theGame->PingHost();
		return true;
	}
	if (parsedConsoleCommands[0] == "ipquit" || parsedConsoleCommands[0] == "IPQUIT")
	{
		//g_theGame->PingQuit();
		return true;
	}
	if (parsedConsoleCommands[0] == "iphostsession" || parsedConsoleCommands[0] == "IPHOSTSESSION")
	{
		//g_theGame->HostSession();
		return true;
	}

	if (parsedConsoleCommands[0] == "ipjoin" || parsedConsoleCommands[0] == "IPJOIN")
	{
		char const* hostname = parsedConsoleCommands[1].c_str();
		uint16_t port = std::stoi(parsedConsoleCommands[2]);
		std::vector<net_address_t> addr = GetAddressFromHostName(hostname, port, false);
		//if (args.get_net_address(&addr)) {
			RemoteCommandService::GetInstance()->Join(addr[0]);
		//}

		return true;
	}

	if (parsedConsoleCommands[0] == "rc" || parsedConsoleCommands[0] == "RC")
	{
		RemoteCommandService::GetInstance()->send_command_to_others(parsedConsoleCommands);
		return true;
	}

	if (parsedConsoleCommands[0] == "rcecho" || parsedConsoleCommands[0] == "RCECHO")
	{
		RemoteCommandService::GetInstance()->disableEcho = !RemoteCommandService::GetInstance()->disableEcho;
		return true;
	}

	if (parsedConsoleCommands[0] == "rm" || parsedConsoleCommands[0] == "RM")
	{
		RemoteCommandService::GetInstance()->send_message(parsedConsoleCommands);
		return true;
	}

	return false;
}

void Console::ClearCommandHistory()
{
	m_CommandHistory.clear();
}

void Console::PrintOutHelpMessage()
{
	m_CommandHistory.push_back("Valid Commands are as followed: help, clear, quit, loghelp, memoryhelp ");
}

void Console::PrintOutLogHelpMessage()
{
	m_CommandHistory.push_back("Valid Log Commands are as followed: threadtest ####, logtest, logenable, logdisable ###, logdisableall ### ### ###, log flush");
}

void Console::PrintOutMemoryHelpMessage()
{
	m_CommandHistory.push_back("Valid Commands are as followed: print callstack, consolecount, profile, profileoff, profilesnapshot, profilepause, profileresume, treeview, flat view,");
	m_CommandHistory.push_back("sorttotaltime, sortselftime, profileoff");
}
void Console::PrintOutInvalidMessage()
{
	m_CommandHistory.push_back("Invalid message Detected");
}
void Console::PrintOutGivenMessage(std::string message)
{
	m_CommandHistory.push_back(message);
}

void Console::RenderProfiler(bool enableProfiler)
{
	g_ProfilerEnabled = enableProfiler;
}



void Console::PrintCallstackInOutputWindow()
{
	PrintCallStackListinOutput();
}


void Console::ChangeClearColor()
{
	if (m_currentConsoleCommand.length() < 16)
	{
		m_CommandHistory.push_back("Please Enter an R G B value next time");
		return;
	}

	char red[] = { m_currentConsoleCommand.at(6) , m_currentConsoleCommand.at(7) , m_currentConsoleCommand.at(8) };
	char green[] = { m_currentConsoleCommand.at(10) , m_currentConsoleCommand.at(11) , m_currentConsoleCommand.at(12) };
	char blue[] = { m_currentConsoleCommand.at(14) , m_currentConsoleCommand.at(15) , m_currentConsoleCommand.at(16) };

	float redColor = (float)atoi(red) / 255.f;
	float blueColor = (float)atoi(blue) / 255.f;
	float greenColor = (float)atoi(green) / 255.f;

	if (redColor > 1)
		redColor = 1;
	if (blueColor > 1)
		blueColor = 1;
	if (greenColor > 1)
		greenColor = 1;
}