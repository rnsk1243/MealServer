#pragma once
#include"ChannelManager.h"
#include"RoomManager.h"

typedef vector<SOCKET> SocketVec;
typedef SocketVec::iterator SocketVecIt;

class CCommandController
{
	CChannelManager mChannelManager;
	CRoomManager mRoomManager;
	CCommandController();
	void EnterRoom(const LinkPtr& shared_clientInfo);
	void EnterSpecialRoomFunc(const LinkPtr& shared_clientInfo, const int & roomNumber, const string & pw = nullptr);
	void ChangeChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber);
	int MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName, const ProtocolTeamAmount& teamAmount, const string & roomPW = nullptr);
	void OutRoom(const LinkPtr& shared_clientInfo);
	void SendAllReadyGameNotice(const LinkPtr & shared_clientInfo);
	void ChangeCharacter(const LinkPtr & shared_clientInfo, Packet& packet);
	void GetHostIP(const LinkPtr & shared_clientInfo);
	// ��� ó�� �Լ�(�� ����� ��)
public:
	static CCommandController* GetInstance();
	CCommandController(const CCommandController&) = delete;
	CCommandController& operator=(const CCommandController&) = delete;
	~CCommandController();
	void SetEnterChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber);
	void CommandHandling(const LinkPtr& shared_clientInfo, Packet& packet);
	void ChattingMessage(const LinkPtr& shared_clientInfo, Packet& packet);
	void DeleteClientSocket(const LinkPtr& shared_clientInfo); // �ι� ���� ȣ�� �Ǹ� ����
};
static CCommandController* CommandControllerStatic = CCommandController::GetInstance();