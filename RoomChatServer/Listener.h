#pragma once
#include"Link.h"
#include"CommandController.h"

class CListener
{
	CListener();
public:
	static CListener* GetInstance();
	CListener(const CListener&) = delete;
	CListener& operator=(const CListener&) = delete;
	~CListener();
	// Ŭ���̾�Ʈ���� ���� �ޱ�
	void Recvn(const SOCKET* socket, string& strMessage, int flags = 0);
	void RecvnLink(const LinkPtr& socket, string & strMessage, int flags = 0);
};
static CListener* ListenerStatic = CListener::GetInstance();
