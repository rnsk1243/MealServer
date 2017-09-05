#pragma once
#include<iostream>
#include<vector>
#include<stdarg.h>
#include<fstream>
#include"Link.h"
using namespace std;


class CReadHandler
{
	CReadHandler();
	// ���� ��ü ���� ��������
	bool ReadUserObjectLine(const string& textFileName, const int& userPKNum, vector<string>& targetTemp);
	// ������ ���� ��������
	const string GetLastLine(const string& textFileName);
public:
	static CReadHandler* GetInstance();
	CReadHandler(const CReadHandler&) = delete;
	CReadHandler& operator=(const CReadHandler&) = delete;
	~CReadHandler();
	// ���ϸ�, ������, ���ڸ, ��Ʈ��(ã������ ������ �ټ��� ����)
	int Search(const char* textFileName, vector<string>& tempUserInfo, int count, ...);
	// ���ڿ���Ʈ��, �����±��ع���
	vector<string> Parse(const string& str, const char& ch);
	// ���� ��ȭ �б�
	bool ReadUserGoods(const LinkPtr& client, const string& textFileName);
	// ���� ���� ��ȣ ��������
	const string GetNextUserNum(const string& textFileName);
};
static CReadHandler* ReadHandlerStatic = CReadHandler::GetInstance();
