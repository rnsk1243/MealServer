#include "ErrorHandler.h"
#include"Link.h"
#include"CommandController.h"
#include"WriteHandler.h"
#include"Util.h"

EnumErrorCode CErrorHandler::CriticalError(EnumErrorCode code, LinkPtr client)
{
	//cout << "[심각한 에러 발생] 에러코드 = [ " << EnumErrorCode(code) << " ]" << endl;
	vector<string> errorMessageVec;
	GetErrorCurTime(errorMessageVec);
	GetErrorLevel(ErrorLevel::Serious, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	GetErrorMemberInfo(client, errorMessageVec);
    
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	//_endthreadex(0);
	return code;
}

EnumErrorCode CErrorHandler::TakeNullLinkError(EnumErrorCode code)
{
	vector<string> errorMessageVec;
	GetErrorCurTime(errorMessageVec);
	GetErrorLevel(ErrorLevel::Normal, errorMessageVec);
	GetErrorCode(code, errorMessageVec);
	vector<string> temp; temp.reserve(1);
	temp.push_back("client가 없는 에러코드");
	errorMessageVec.insert(errorMessageVec.end(), temp.begin(), temp.end());
	WriteHandlerStatic->Write(ErrorLogTxt.c_str(), errorMessageVec);
	cout << "client가 없는 에러코드 = " << EnumErrorCode(code) << endl;
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
	cout << "성공 코드 = " << EnumErrorCode(code) << endl;
	return code;
}

EnumErrorCode CErrorHandler::TakeError(EnumErrorCode code, LinkPtr client)
{
	cout << "에러코드 = " << EnumErrorCode(code) << endl;
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
	cout << "ErrorHandler 생성자 호출" << endl;
}


CErrorHandler::~CErrorHandler()
{
}

bool CErrorHandler::GetErrorMemberInfo(LinkPtr client, vector<string>& memberInfoVec)
{
	vector<string> localStrVector;
	bool returnVal = false; // 성공 여부
	if (nullptr == client)
	{
		localStrVector.reserve(1);
		localStrVector.push_back("client가 초기화 되지 않아 심각한 오류를 처리할 수 없습니다.");
	}
	else
	{
		localStrVector.reserve(2);
		localStrVector.push_back(IntToString(client->GetMyPKNumber()));
		localStrVector.push_back(string(client->GetMyName()));
	}
	memberInfoVec.insert(memberInfoVec.end(), localStrVector.begin(), localStrVector.end()); // 범위 대입
	returnVal = true;
	return returnVal;
}

void CErrorHandler::GetErrorLevel(ErrorLevel level, vector<string>& errorLevelStringVec)
{
	string strLevel;
	switch (level)
	{
	case ErrorLevel::Serious:
		strLevel = "[높음]";
		break;
	case ErrorLevel::Normal:
		strLevel = "[보통]";
		break;
	case ErrorLevel::Low:
		strLevel = "[낮음]";
		break;
	case ErrorLevel::Succes:
		strLevel = "[성공]";
		break;
	default:
		strLevel = "[알수없음]";
		break;
	}
	vector<string> localStrVector;
	localStrVector.reserve(1);
	localStrVector.push_back(strLevel);
	errorLevelStringVec.insert(errorLevelStringVec.end(), localStrVector.begin(), localStrVector.end()); // 범위 대입
}

void CErrorHandler::GetErrorCurTime(vector<string>& timeStringVec)
{
	const time_t curTime(time(NULL));	// localtime함수에 넣을 인자 선언 // 1970년1월1일부터 몇초가 지났는지 계산
	tm timeStruct;
	localtime_s(&timeStruct, &curTime); // 지역 시간 기준으로 변환
	vector<string> localStrVector;
	localStrVector.reserve(timeKind); // 공간 할당
	localStrVector.push_back(IntToString(timeStruct.tm_year - 100) + "년");
	localStrVector.push_back(IntToString(timeStruct.tm_mon + 1) + "월");
	localStrVector.push_back(IntToString(timeStruct.tm_mday) + "일");
	localStrVector.push_back(IntToString(timeStruct.tm_hour) + "시");
	localStrVector.push_back(IntToString(timeStruct.tm_min) + "분");
	localStrVector.push_back(IntToString(timeStruct.tm_sec) + "초");
	timeStringVec.insert(timeStringVec.end(), localStrVector.begin(), localStrVector.end()); // 범위 대입
}

void CErrorHandler::GetErrorCode(EnumErrorCode code, vector<string>& errorCodeStringVec)
{
	vector<string> localStrVector;
	localStrVector.reserve(1); // 공간 할당
	localStrVector.push_back(IntToString((int)code));
	errorCodeStringVec.insert(errorCodeStringVec.end(), localStrVector.begin(), localStrVector.end()); // 범위 대입
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
	cout << "에러 코드 = " << code << endl;
	if (0 == code % 2)
	{
		return TakeSucces(code, client);
	}

	if (nullptr == client)
	{
		return TakeNullLinkError(code);
	}

	switch (code)
	{
	case ERROR_INIT_MONEY:
	case ERROR_RECV:
	case ERROR_SEND:
	case ERROR_NULL_LINK_SEND:
		return CriticalError(code, client);
	default:
		return TakeError(code, client);
	}
}
