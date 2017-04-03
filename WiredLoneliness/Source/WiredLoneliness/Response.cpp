// Fill out your copyright notice in the Description page of Project Settings.

#include "WiredLoneliness.h"
#include "Response.h"

Response::Response()
{
}

Response::~Response()
{
}

void Response::addContext(string str)
{
	listOfContext.push_back(str);
}

void Response::addResp(string str)
{
	listOfResp.push_back(str);
}

vector<string> Response::getContextList() const
{
	return listOfContext;
}

vector<string> Response::getRespList() const
{
	return listOfResp;
}

void Response::clear()
{
	listOfContext.clear();
	listOfResp.clear();
}