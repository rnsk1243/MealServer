#include "ReadHandler.h"
#include"Listener.h"
#include"Util.h"
#include"ErrorHandler.h"

CReadHandler::CReadHandler()
{
}

CReadHandler * CReadHandler::GetInstance()
{
	if (nullptr == ReadHandlerStatic)
	{
		ReadHandlerStatic = new CReadHandler();
	}
	return ReadHandlerStatic;
}


CReadHandler::~CReadHandler()
{
}

int CReadHandler::Search(const char * textFileName, vector<string>& tempUserInfo, int count, ...)
{
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "파일이 없습니다." << endl;
		return 0;
	}
	va_list Marker;
	va_start(Marker, count);

	vector<string> searchTarget;
	for (int i = 0; i < count; i++)
	{
		searchTarget.push_back(va_arg(Marker, string));
	}

	int sameAmount = 0;
	int line = 0;
	while (!inFile.eof())
	{
		line++;
		sameAmount = 0;
		char temp[IdPwSize];
		inFile.getline(temp, IdPwSize);
		string tempString = temp;
		vector<string> userInfoVec = Parse(tempString, '|');

		vector<string>::iterator iterUserInfoBegin = userInfoVec.begin();
		vector<string>::iterator iterTargetBegin = searchTarget.begin();
		for (; iterUserInfoBegin != userInfoVec.end(); ++iterUserInfoBegin)
		{
			for (; iterTargetBegin != searchTarget.end(); ++iterTargetBegin)
			{
				int compareResult = iterTargetBegin->compare(*iterUserInfoBegin);
				if (0 == compareResult)
				{
					sameAmount++;
				}
			}
			iterTargetBegin = searchTarget.begin();
		}
		if (sameAmount == count)
		{
			tempUserInfo = userInfoVec;
			break;
		}
	}
	va_end(Marker);
	inFile.close();
	if (sameAmount == count)
	{
		return line;
	}
	return 0;
}

vector<string> CReadHandler::Parse(const string & str, const char & ch)
{
	string next;
	vector<string> result;

	for (string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == ch)
		{
			if (!next.empty())
			{
				result.push_back(next);
				next.clear();
			}
		}
		else
		{
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}

const string CReadHandler::GetLastLine(const string & textFileName)
{
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "파일이 없습니다." << endl;
	}
	inFile.seekg(-2, ios::end);
	char checkLine = ' ';
	while (checkLine != '\n')
	{
		inFile.seekg(-1, ios::cur);
		checkLine = (char)inFile.peek();
		//		cout << checkLine << endl;
	}
	inFile.seekg(2, ios::cur);
	string lastLine;
	getline(inFile, lastLine);

	inFile.close();
	return lastLine;
}


bool CReadHandler::ReadUserObjectLine(const string& textFileName, const int& userPKNum, vector<string>& targetTemp)
{
	string strUserName = IntToString(userPKNum);
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "파일이 없습니다." << endl;
		return false;
	}

	while (!inFile.eof())
	{
		char temp[IdPwSize];
		inFile.getline(temp, IdPwSize);
		string tempString = temp;
		vector<string> tempVec = Parse(tempString, '|');

		for (int i = 0; i < (int)tempVec.size(); i++)
		{
			int compareResult = strUserName.compare(tempVec[i]);
			if (0 == compareResult)
			{
				targetTemp = tempVec;
				return true;
			}
			else
			{
				break;
			}
		}
	}
	inFile.close();
	return false;
}
//
//bool CReadHandler::ReadUserGoods(const LinkPtr& client, const string & textFileName)
//{
//	typedef vector<string> goodsInfoVec;
//	typedef goodsInfoVec::iterator goodsInfoVecIter;
//
//	goodsInfoVec userGoodsInfo;
//	if (!ReadUserObjectLine(textFileName, client->GetMyPKNumber(), userGoodsInfo))
//	{
//		ErrorHandStatic->ErrorHandler(ERROR_INIT_USER_GOODS, client);
//		return false;
//	}
//	if (!client->IsZeroMoney())
//	{
//		client->SetZeroMoney();
//	}
//	
//	if (IndexGoodsInfoTxtMoney < userGoodsInfo.size())
//	{
//		int myMoney = stoi(userGoodsInfo[IndexGoodsInfoTxtMoney]);
//		cout << "읽어 온 돈 = " << myMoney << endl;
//		if (client->InitGoods(myMoney))
//		{
//			client->SetInitGoods();
//			return true;
//		}
//		else
//		{
//			ErrorHandStatic->ErrorHandler(ERROR_INIT_GOODS, client);
//			return false;
//		}
//	}
//	else
//	{
//		ErrorHandStatic->ErrorHandler(ERROR_READ_GOODS_TXT_INDEX_OUTOFLANGE, client);
//		return false;
//	}
//	return false;
//}

const string CReadHandler::GetNextUserNum(const string & textFileName)
{
	//string lastLine = GetLastLine(textFileName);
	//vector<string> parseStr = Parse(lastLine, '|');
	//return parseStr[0];
	cout << "=============userNumFile Read!===========" << endl;
	ifstream inFile(textFileName);
	if (!inFile)
	{
		cout << "파일이 없습니다." << endl;
	}
	string nextUserNum;
	getline(inFile, nextUserNum);
	inFile.close();
	return nextUserNum;
}
