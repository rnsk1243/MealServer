#define _CRT_SECURE_NO_WARNINGS
#include "Listener.h"
#include"ErrorHandler.h"
#include "ChannelManager.h"
#include"RoomManager.h"
#include"ReadHandler.h"
#include"Util.h"
#include"RecvRepository.h"

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

void CListener::Recvn(const SOCKET* socket, Packet& packet, int flags)
{
	size_t isSuccess = 0;
	char recvPacket[BufSizeRecv];
	while (true)
	{
		isSuccess += recv(*socket, recvPacket, PacketSize, flags);
		//cout << "success = " << isSuccess << endl;
		if (isSuccess == SOCKET_ERROR)
		{
			closesocket(*socket);
			delete socket;
			ErrorHandStatic->ErrorHandler(ERROR_NULL_LINK_RECV);
			_endthreadex(0);
		}
		else if (isSuccess >= PacketSize)
			break;
	}

	//PacketPtr packetPtr(new Packet());

	memcpy_s(&packet, PacketSize, recvPacket, PacketSize);
	// 나중에 비동기 소켓 사용시 Recv박스에 넣어 처리
	//OrderStructSocketPtr order(new OrderStructSocket(socket, packetPtr));
	//RecvRepositoryStatic->PushPacket(order);

	//string messageStr = ANSIToUTF8(recvedMessage); // ANSI 문자열을 UTF-8로 변환

	//cout << "받은 idPw메시지 = " << MS.message << endl;
	//strMessage = messageStr;
	//strMessage.assign(messageStr.begin(), messageStr.end());
	//cout << "받은 메세지 = " << strMessage.c_str() << endl;
}

void CListener::RecvnLink(const LinkPtr& link, Packet& packet, int flags)
{
	size_t isSuccess = 0;
	char recvPacket[BufSizeRecv];
	while (true)
	{
		isSuccess += recv(*link.get()->GetClientSocket(), recvPacket, PacketSize, flags);
		if (isSuccess == SOCKET_ERROR)
		{
			ErrorHandStatic->ErrorHandler(ERROR_RECV, link);
		}
		else if (isSuccess >= PacketSize)
			break;
	}
	//PacketPtr packetPtr(new Packet());
	memcpy_s(&packet, PacketSize, recvPacket, PacketSize);

	char temp[BufSizeRecv];
	BSTR bstrWide;
	int length = 0;
	ReadyANSIToUTF8(packet.InfoValue, bstrWide, length);
	ANSIToUTF8(bstrWide, length, temp);

	for (int i = 0; i < length; ++i)
	{
		packet.InfoValue[i] = temp[i];
	}
	cout << "변환된 값 = " << packet.InfoValue << endl;
	//char buf[BufSizeValue];
	//cout << ANSIToUTF8(packet.Value, buf) << endl;
	// 나중에 비동기 소켓 사용시 Recv박스에 넣어 처리
	//OrderStructLinkPtr order(new OrderStructLink(link, packetPtr));
	//RecvRepositoryStatic->PushPacket(order);
}

