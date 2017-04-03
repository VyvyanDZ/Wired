#pragma once
#include "InputParser.h"
#include "Response.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <ctime>
#include <sapi.h>
#include <map>

using namespace std;

typedef struct {
	char *first;
	char *second;
} charStruct;

typedef map<string, vector<Response>> stringMap;

/**
 * 
 */
class WIREDLONELINESS_API AIChatBot
{
public:
	AIChatBot(string str);

	void seedRandomGenerator() {
		srand((unsigned)time(NULL));
	}

	void loadDatabase();

	~AIChatBot() {
		releaseTTS_Engine();
	}

	void initializeTTS_Engine();
	void releaseTTS_Engine();

	string getInput(string input);
	string respond();
	void speak(const string text);

	void signOn();

	bool quit() const {
		return bQuit;
	}

	void findMatch();
	void handleRepetition();
	void handleUserRepetition();
	void handleEvent(string str);
	void selectResponse();

	void savePrevInput() {
		prevUserInput = userInput;
	}

	void savePrevResponse() {
		prevBotResponse = botResponse;
	}

	void savePrevEvent() {
		prevBotEvent = botEvent;
	}

	void setEvent(string newEvent) {
		botEvent = newEvent;
	}

	void saveInput() {
		inputBackup = userInput;
	}

	void setInput(string newInput) {
		userInput = newInput;
	}

	void restoreInput() {
		userInput = inputBackup;
	}

	string printResponse() const {
		if (botResponse.length() > 0) {
			return botResponse;
		}
		return "";
	}

	void saveBotResponse() {
		if (!botResponse.empty()) {
			responseLog.push(botResponse);
		}
	}

	string getSubPhrase(vector<string> str, size_t start, size_t end);

	bool similarInput() const;
	void transpose(string &str);
	void extractRespList(vector<Response> responseList);

	void preprocessInput();
	void preprocessResponse();
	void findSubject();
	bool botRepeat();
	bool userRepeat();

	bool botUnderstand() const {
		return listOfResponse.size() > 0;
	}

	bool emptyInput() const {
		return (userInput.length() == 0) && (prevUserInput.length() != 0);
	}

	bool emptyInputRepetition() const {
		return (userInput.length() == 0) && (prevUserInput.length() == 0);
	}

	bool endConversation() const {
		return userInput.find("BYE") != string::npos;
	}

	bool sameEvent() const {
		return (botEvent.length() > 0) && (prevBotEvent == botEvent);
	}

	bool noResponse() const {
		return listOfResponse.size() == 0;
	}

	bool sameInput() const {
		return (userInput.length() > 0) && (userInput == prevUserInput);
	}

	string findBestKey(vector<string> listOfKeys);
	void preprocessKeyWord(string &str, size_t startPost, size_t endPos, size_t size);

	void addXmlTag(string &str);
	int findRespPos(string str);
	void updateUnknowInputList();

	void saveLog();
	void saveLog(string str);
	void createUnknowFile();

	private:
		string botName;
		string userName;
		string userInput;
		string botResponse;
		string prevUserInput;
		string prevBotResponse;
		string botEvent;
		string prevBotEvent;
		string inputBackup;
		string subject;
		string keyWord;
		string context;
		string prevContext;
		bool bQuit;
		bool bTTSEngineInitialised;
		ISpVoice *voice;

		vector<string> listOfResponse;
		vector<string> listOfUnknownInput;
		stack<string> responseLog;

		fstream logFile;
		fstream unknownFile;

		stringMap knowledgeBase;

		InputParser* inputParser = new InputParser();
};
