#include "ErrorHandler.h"
#include"Link.h"
#include"CommandController.h"
#include"WriteHandler.h"
#include"Util.h"

EnumErrorCode CErrorHandler::CriticalError(EnumErrorCode code, LinkPtr client)
{
	//cout << "[�ɰ��� ���� �߻�] �����ڵ� = [ " << EnumErrorCode(code) << " ]" << endl;
	//vector<string> errorMessageVec;
	//GetErrorCurTime(errorMessageVec);
	//GetErrorLevel(ErrorLevel::Serious, errorMessageVec);
	//GetErrorCode(code, errorMessageVec);
	//GetErrorMemberInfo(client, errorMessageVec);
	//WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	/*if (nullptr != client)
	{
		
	}*/
	client.get()->SetSocketError();
	CCommandController::GetInstance()->DeleteClientSocket(client);
	_endthreadex(0);
	return code;
}

EnumErrorCode CErrorHandler::TakeNullLinkError(EnumErrorCode code)
{
	vector<string> errorMessageVec;
	GetErrorCurTime(errorMessageVec);
	GetErrorLevel(ErrorLevel::Normal, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	vector<string> temp; temp.reserve(1);
	temp.push_back("client�� ���� �����ڵ�");
	errorMessageVec.insert(errorMessageVec.end(), temp.begin(), temp.end());
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	//cout << "client�� ���� �����ڵ� = " << EnumErrorCode(code) << endl;
	//_endthreadex(0);
	return code;
}

EnumErrorCode CErrorHandler::TakeSucces(EnumErrorCode code, LinkPtr client)
{
	vector<string> errorMessageVec;
	GetErrorCurTime(errorMessageVec);
	GetErrorLevel(ErrorLevel::Succes, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	if (nullptr != client)
	{
		GetErrorMemberInfo(client, errorMessageVec);
	}
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	//cout << "���� �ڵ� = " << EnumErrorCode(code) << endl;
	return code;
}

EnumErrorCode CErrorHandler::TakeError(EnumErrorCode code, LinkPtr client)
{
//	cout << "�����ڵ� = " << EnumErrorCode(code) << endl;
	//_endthreadex(0);
	if (nullptr == client)
	{
		return code;
	}
	vector<string> errorMessageVec;
	GetErrorCurTime(errorMessageVec);
	GetErrorLevel(ErrorLevel::Normal, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	GetErrorMemberInfo(client, errorMessageVec);
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	return code;
}

CErrorHandler::CErrorHandler()
{
	cout << "ErrorHandler ������ ȣ��" << endl;
}


CErrorHandler::~CErrorHandler()
{
}

bool CErrorHandler::GetErrorMemberInfo(LinkPtr client, vector<string>& memberInfoVec)
{
	vector<string> localStrVector;
	bool returnVal = false; // ���� ����
	if (nullptr == client)
	{
		localStrVector.reserve(1);
		localStrVector.push_back("client�� �ʱ�ȭ ���� �ʾ� �ɰ��� ������ ó���� �� �����ϴ�.");
	}
	else
	{
		localStrVector.reserve(2);
		localStrVector.push_back(IntToString(client->GetMyPKNumber()));
		localStrVector.push_back(string(client->GetMyName()));
	}
	memberInfoVec.insert(memberInfoVec.end(), localStrVector.begin(), localStrVector.end()); // ���� ����
	returnVal = true;
	return returnVal;
}

void CErrorHandler::GetErrorLevel(ErrorLevel level, vector<string>& errorLevelStringVec)
{
	string strLevel;
	switch (level)
	{
	case ErrorLevel::Serious:
		strLevel = "[����]";
		break;
	case ErrorLevel::Normal:
		strLevel = "[����]";
		break;
	case ErrorLevel::Low:
		strLevel = "[����]";
		break;
	case ErrorLevel::Succes:
		strLevel = "[����]";
		break;
	default:
		strLevel = "[�˼�����]";
		break;
	}
	vector<string> localStrVector;
	localStrVector.reserve(1);
	localStrVector.push_back(strLevel);
	errorLevelStringVec.insert(errorLevelStringVec.end(), localStrVector.begin(), localStrVector.end()); // ���� ����
}

void CErrorHandler::GetErrorCurTime(vector<string>& timeStringVec)
{
	const time_t curTime(time(NULL));	// localtime�Լ��� ���� ���� ���� // 1970��1��1�Ϻ��� ���ʰ� �������� ���
	tm timeStruct;
	localtime_s(&timeStruct, &curTime); // ���� �ð� �������� ��ȯ
	vector<string> localStrVector;
	localStrVector.reserve(timeKind); // ���� �Ҵ�
	localStrVector.push_back(IntToString(timeStruct.tm_year - 100) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_mon + 1) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_mday) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_hour) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_min) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_sec) + "��");
	timeStringVec.insert(timeStringVec.end(), localStrVector.begin(), localStrVector.end()); // ���� ����
}

void CErrorHandler::GetErrorCode(EnumErrorCode code, vector<string>& errorCodeStringVec)
{
	vector<string> localStrVector;
	localStrVector.reserve(1); // ���� �Ҵ�
	localStrVector.push_back(IntToString((int)code));
	errorCodeStringVec.insert(errorCodeStringVec.end(), localStrVector.begin(), localStrVector.end()); // ���� ����
}

CErrorHandler * CErrorHandler::GetInstance()
{
	if (nullptr == ErrorHandStatic)
	{
		ErrorHandStatic = new CErrorHandler();
	}
	return ErrorHandStatic;
}

EnumErrorCode CErrorHandler::ErrorHandler(EnumErrorCode code, LinkPtr client)
{
	if (0 == code % 2)
	{
		return code; //TakeSucces(code, client);
	}
	cout << "���� �ڵ� = " << code << endl;

	if (nullptr == client)
	{
		return code;
		//return TakeNullLinkError(code);
	}

	switch (code)
	{
	//case ERROR_INIT_MONEY:
	case ERROR_RECV:
	case ERROR_SEND:
		return CriticalError(code, client);
	default:
		return code; //TakeError(code, client);
	}
}
