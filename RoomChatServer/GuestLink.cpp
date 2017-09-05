#include "GuestLink.h"


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
	const char* chMessage = message.c_str();
	size_t size = strlen(chMessage);
	isSuccess = send(*mClientSocket, (char*)&size, IntSize, flags); // ������ ������
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
