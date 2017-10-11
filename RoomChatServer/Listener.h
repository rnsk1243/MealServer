#pragma once
#include"Link.h"
#include"CommandController.h"

class CListener
{
	CListener();
	void Translate(char * infoValue);
public:
	static CListener* GetInstance();
	CListener(const CListener&) = delete;
	CListener& operator=(const CListener&) = delete;
	~CListener();
	// 클라이언트에게 답장 받기
	void Recvn(const SOCKET* socket, Packet& packet, int flags = 0);
	void RecvnLink(const LinkPtr& link, Packet & packet, int flags = 0);
};
static CListener* ListenerStatic = CListener::GetInstance();
