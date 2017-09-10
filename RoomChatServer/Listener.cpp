#define _CRT_SECURE_NO_WARNINGS
#include "Listener.h"
#include"ErrorHandler.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"
#include"Util.h"

CListener::CListener()
{
}

CListener* CListener::GetInstance()
{
	if (nullptr == ListenerStatic)
	{
		return new CListener();
	}
	return ListenerStatic;
}

CListener::~CListener()
{
}

int CListener::Recvn(const SOCKET* socket, string& strMessage, int flags)
{
	char temp[IntSize];
	size_t isSuccess = recv(*socket, temp, IntSize, flags);

	if (isSuccess == SOCKET_ERROR)
	{
		return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
	}
	size_t sendRecvSize = *(int*)temp;

	// �ӽ÷� ���� temp �޸� ��ȯ
#pragma endregion
#pragma region �޼��� �ޱ�
	isSuccess = 0;
	char recvedMessage[BufSize];
	while (sendRecvSize > 0)
	{
		isSuccess += recv(*socket, recvedMessage, (int)sendRecvSize, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			return ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
		}
		else if (isSuccess >= sendRecvSize)
			break;
	}
	recvedMessage[sendRecvSize] = '\0';
#pragma endregion

	string messageStr = ANSIToUTF8(recvedMessage); // ANSI ���ڿ��� UTF-8�� ��ȯ

	//cout << "���� idPw�޽��� = " << MS.message << endl;
	//strMessage = messageStr;
	strMessage.assign(messageStr.begin(), messageStr.end());
	cout << "���� �޼��� = " << strMessage.c_str() << endl;
	return SUCCES_RECV;
}

