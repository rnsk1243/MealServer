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
	void EnterRoom(const LinkPtr& shared_clientInfo, const int& roomNumber);
	void ChangeChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber);
	void MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName, const int & battingMoney);
	void OutRoom(const LinkPtr& shared_clientInfo);
	void SendAllReadyGameNotice(const LinkPtr & shared_clientInfo);
	// 명령 처리 함수(방 만들기 등)
public:
	static CCommandController* GetInstance();
	CCommandController(const CCommandController&) = delete;
	CCommandController& operator=(const CCommandController&) = delete;
	~CCommandController();
	void SetEnterChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber);
	void CommandHandling(const LinkPtr& shared_clientInfo, vector<string>& commandString);
	void DeleteClientSocket(const LinkPtr& shared_clientInfo); // 두번 연속 호출 되면 오류
};
static CCommandController* CommandControllerStatic = CCommandController::GetInstance();