#include "WiredLoneliness.h"
#include "AIChatBot.h"
#include "InputParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <ctime>
#include <sapi.h>
#include <map>

AIChatBot::AIChatBot(string str) : botName(str), bQuit(0), userInput("null") {
	seedRandomGenerator();
	loadDatabase();
}

AIChatBot::AIChatBot(){}

charStruct transposList[] = {
	{ " MYSELF ", " YOURSELF " },
	{ " DREAMS ", " DREAM " },
	{ " WEREN'T ", " WASN'T " },
	{ " AREN'T ", " AM NOT " },
	{ " I'VE ", " YOU'VE " },
	{ " MINE ", " YOURS " },
	{ " MY ", " YOUR " },
	{ " WERE ", " WAS " },
	{ " MOM ", " MOTHER " },
	{ " I AM ", " YOU ARE " },
	{ " I'M ", " YOU'RE " },
	{ " DAD ", " FATHER " },
	{ " MY ", " YOUR " },
	{ " AM ", " ARE " },
	{ " I'D ", " YOU'D " },
	{ " I ", " YOU " },
	{ " ME ", " YOU " }
};

size_t charStructSize = sizeof(transposList) / sizeof(transposList[0]);

void AIChatBot::loadDatabase()
{
	fstream fin("C:\\Users\\Vyvyan\\Downloads\\script.txt", ios::in);
	if (fin.fail()) {
		throw string("Unable to load database");
	}

	Response response;
	vector<Response> listOfResponse;
	string buffer;
	vector<string> keyList;

	while (getline(fin, buffer)) {
		char symbol = buffer[0];
		buffer.erase(0, 1);

		switch (symbol) {
		case 'K': keyList.push_back(buffer); break;
		case 'C': response.addContext(buffer); break;
		case 'R': response.addResp(buffer); break;
		case '%': listOfResponse.push_back(response); response.clear();  break;
		case '#':
			if (listOfResponse.empty()) {
				listOfResponse.push_back(response);
			}

			vector<string>::const_iterator iter = keyList.begin();

			for (; iter != keyList.end(); ++iter) {
				knowledgeBase[*iter] = listOfResponse;
			}

			keyList.clear();
			listOfResponse.clear();
			response.clear();
			break;
		}
	}
	fin.close();
}

string AIChatBot::getInput(string input)
{
	savePrevInput();
	userInput = input;
	return "> " + input;
}

string AIChatBot::respond()
{
	savePrevResponse();
	setEvent("BOT UNDERSTAND**");

	if (emptyInput()) {
		handleEvent("NULL INPUT**");
	}
	else if (emptyInputRepetition()) {
		handleEvent("NULL INPUT REPETITION**");
	}
	else if (userRepeat()) {
		handleUserRepetition();
	}
	else {
		findMatch();
	}

	if (quit()) {
		bQuit = 1;
	}

	if (!botUnderstand()) {
		handleEvent("BOT DON'T UNDERSTAND**");
	}

	if (listOfResponse.size() > 0) {
		selectResponse();
		preprocessResponse();
		saveBotResponse();
		saveInput();

		if (botRepeat()) {
			handleRepetition();
		}
		return printResponse();
	}
	return "";
}


void AIChatBot::signOn()
{
	handleEvent("SIGNON**");
	selectResponse();
	printResponse();
}

void AIChatBot::findMatch()
{
	listOfResponse.clear();

	string bestKeyWord;
	vector<string> listOfWord;

	if (userInput.find("**") == string::npos) {
		inputParser->trimRight(userInput, ".");
		inputParser->lowercase(userInput);
		inputParser->tokenize(userInput, listOfWord);
		bestKeyWord = findBestKey(listOfWord);
		keyWord = bestKeyWord;
	}
	else {
		keyWord = userInput;
	}

	if (knowledgeBase.find(keyWord) != knowledgeBase.end()) {
		vector<Response> listOfResponse = knowledgeBase[keyWord];
		extractRespList(listOfResponse);
	}
}

void AIChatBot::handleRepetition()
{
	/*map<int, string> listOfPrevResponse;
	if (listOfResponse.size() > 1) {
	stack<string> s = responseLog;
	vector<string>::const_iterator iter = listOfResponse.begin();

	for (; iter != listOfResponse.end(); ++iter) {
	int pos = findRespPos(*iter);
	listOfPrevResponse[pos] = *iter;
	}
	}
	map<int, string>::const_iterator iter2 = listOfPrevResponse.end();
	--iter2;
	botResponse = iter2->second;
	*/
}

void AIChatBot::handleUserRepetition()
{
	if (sameInput()) {
		handleEvent("REPETITION T1**");
	}
	else if (similarInput()) {
		handleEvent("REPETITION T2**");
	}
}

void AIChatBot::handleEvent(string str)
{
	savePrevEvent();
	setEvent(str);
	saveInput();
	setInput(str);

	if (!sameEvent()) {
		findMatch();
	}
	restoreInput();
}

void AIChatBot::selectResponse()
{
	if (botUnderstand()) {
		inputParser->shuffle(listOfResponse, listOfResponse.size());
		botResponse = listOfResponse[0];
	}
}

string AIChatBot::getSubPhrase(vector<string> str, size_t start, size_t end)
{
	string buffer;
	for (int i = start; i < end; ++i) {
		buffer += str[i];
		if (i != end - 1) {
			buffer += " ";
		}
	}
	return buffer;
}

bool AIChatBot::similarInput() const
{
	return (userInput.length() > 0 && ((userInput == prevUserInput) || (userInput.find(prevUserInput) != string::npos) ||
		(prevUserInput.find(userInput) != string::npos)));
}

void AIChatBot::transpose(string & str)
{
	string buffer = " " + str + " ";
	bool bTransponded = 0;

	for (int i = 0; i < charStructSize; ++i) {
		string first = transposList[i].first;
		string second = transposList[i].second;

		while (inputParser->replace(buffer, first, second) != string::npos) {
			bTransponded = 1;
		}
	}

	if (!bTransponded) {
		for (int i = 0; i < charStructSize; ++i) {
			string first = transposList[i].first;
			string second = transposList[i].second;

			while (inputParser->replace(buffer, second, first) != string::npos);
		}
	}

	inputParser->trimLeftRight(buffer, " ");
	str = buffer;
}

void AIChatBot::extractRespList(vector<Response> responseList)
{
	vector<Response>::const_iterator iter = responseList.begin();

	for (; iter != responseList.end(); ++iter) {
		vector<string> listOfContext = iter->getContextList();
		if (listOfContext.empty()) {
			listOfResponse = iter->getRespList();
		}
		else if (find(listOfContext.begin(), listOfContext.end(), prevBotResponse) != listOfContext.end()) {
			listOfResponse = iter->getRespList();
			break;
		}
	}
}

void AIChatBot::preprocessInput()
{
	inputParser->cleanString(userInput);
	inputParser->trimRight(userInput, ". ");
	inputParser->lowercase(userInput);
}

void AIChatBot::preprocessResponse()
{
	if (botResponse.find("*") != string::npos) {
		findSubject();
		transpose(subject);
		inputParser->replace(botResponse, "*", " " + subject);
	}
}

void AIChatBot::findSubject()
{
	subject.erase();
	inputParser->trimRight(userInput, " ");
	inputParser->trimLeftRight(keyWord, "_");
	size_t pos = userInput.find(keyWord);

	if (pos != string::npos) {
		subject = userInput.substr(pos + keyWord.length() + 1, userInput.length() - 1);
	}
}

bool AIChatBot::botRepeat()
{
	int pos = findRespPos(botResponse);
	if (pos > 0) {
		return pos + 1 < listOfResponse.size();
	}
	return 0;
}

bool AIChatBot::userRepeat()
{
	return (prevUserInput.length() > 0 && ((userInput == prevUserInput) || (userInput.find(prevUserInput) != string::npos)
		|| (prevUserInput.find(userInput) != string::npos)));
}

string AIChatBot::findBestKey(vector<string> listOfKeys)
{
	string buffer;
	int size = listOfKeys.size();
	bool bKeyFound = 0;

	for (int i = 0, j = size; i < size && j >= 1; ++i, --j) { //Po co mi to i?
		for (int k = 0; (k + j) <= size; ++k) {
			buffer = getSubPhrase(listOfKeys, k, k + j);
			if (knowledgeBase.find(buffer) != knowledgeBase.end()) {
				bKeyFound = 1;
			}
			else {
				preprocessKeyWord(buffer, k, k + j, size);
				if (knowledgeBase.find(buffer) != knowledgeBase.end()) {
					bKeyFound = 1;
				}
			}
			if (bKeyFound) {
				return buffer;
			}
		}
	}
	return buffer;
}

void AIChatBot::preprocessKeyWord(string & str, size_t startPost, size_t endPos, size_t size)
{
	if (startPost == 0) {
		str.insert(0, "_");
	}
	if (endPos == size - 1) {
		str.insert(str.length(), "_");
	}
}

int AIChatBot::findRespPos(string str)
{
	int pos = -1;
	stack<string> s = responseLog;

	while (s.empty()) {
		++pos;
		if (s.top() == str) {
			break;
		}
		s.pop();
	}
	return pos;
}