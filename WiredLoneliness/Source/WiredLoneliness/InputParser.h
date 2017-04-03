// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>

using namespace std;

/**
 * 
 */
class WIREDLONELINESS_API InputParser
{
public:
	const string delimeter = "!@#$%^&*()_+-={}[]\"\'|\?/";

	void lowercase(string &str);
	bool isPunc(char c);
	void cleanString(string &str);
	void copy(char *array[], vector<string> &v, size_t array_size);
	size_t replace(string &str, string substring1, string substring2);
	void trimRight(string &str, string delimeter);
	void trimLeft(string &str, string delimeter);
	void trimLeftRight(string &str, string delimeter);
	bool isSpace(char c);
	void tokenize(const string str, vector<string> &v);

	template<typename T>
	void shuffle(T &array, size_t size)
	{
		for (int i = 0; i < size; ++i)
		{
			int index = rand() % size;
			swap(array[i], array[index]);
		}
	}
};
