#include "GuestLink.h"
#include<atlstr.h>


CGuestLink::CGuestLink(const SOCKET * clientSocket):mClientSocket(clientSocket)
{
}

CGuestLink::~CGuestLink()
{
}

const SOCKET* CGuestLink::GetClientSocket() const
{
	return mClientSocket;
}

void CGuestLink::Sendn(const string & message, int flags) const
{
	int isSuccess = 0;

	/// UTF8 인코딩
	wstring strUni = CA2W(message.c_str());
	//string strMulti = CW2A(strUni.c_str());
	string strUTF8(CW2A(strUni.c_str(), CP_UTF8));
	///
	
	//const wchar_t* chMessage = strUni.c_str();
	const char* chMessage = strUTF8.c_str();
	//size_t size = wcslen(chMessage);// strlen(chMessage);
	size_t size = strlen(chMessage);
	isSuccess = send(*mClientSocket, (char*)&size, IntSize, flags); // 사이즈 보내기
	if (isSuccess == SOCKET_ERROR)
	{
		//return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND);
	}
	int temp = 0;
	while (true)
	{
		temp += send(*mClientSocket, chMessage, (int)size, flags);
		if (temp == SOCKET_ERROR)
		{
			//return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_SEND);
		}
		if (temp >= (int)size)
			break;
	}
}
