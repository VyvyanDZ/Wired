// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include<iostream>
#include<map>
#include<vector>

using namespace std;

/**
 * 
 */
class WIREDLONELINESS_API Response
{
public:
	Response();
	~Response();

	void addContext(string str);
	void addResp(string str);
	vector<string> getContextList() const;
	vector<string> getRespList() const;
	void clear();

private:
	vector<string> listOfContext;
	vector<string> listOfResp;
};
