#pragma once
#include<list>
#include<iostream>
#include"Room.h"

class CLink;
class CRoom;
using namespace std;
#pragma region 타입재정의

typedef shared_ptr<CRoom> RoomPtr;
typedef list<RoomPtr> RoomList;
typedef RoomList::const_iterator RoomListIt;
#pragma endregion

class CRoomManager
{
	RoomList mRooms;
//	CRITICAL_SECTION CS_Room;
	MUTEX mRAII_RoomManagerMUTEX;
	RoomListIt GetMyRoomIter(int ChannelNum, int roomNum);
	int newRoomNumber;
	void PushRoom(const RoomPtr& shared_newRoom);
public:
	CRoomManager();
	CRoomManager(const CRoomManager&) = delete;
	CRoomManager& operator=(const CRoomManager&) = delete;
	~CRoomManager();

	RoomListIt EraseRoom(RoomListIt deleteTargetRoomIter);

	////////////
	// room에서 나가기 (나가고자하는 소켓)
	RoomListIt ExitRoom(const LinkPtr & shared_clientInfo, bool & isSucces);
	// 현재 자신이 속한 방이 없을때 방 만들기(들어갈 소켓, 만드는 방 번호) 
	int MakeRoom(const LinkPtr & shared_clientInfo, const string& roomName, const ProtocolTeamAmount& teamAmount, const string & roomPW = nullptr);
	// 현재 자신이 속한 방이 없을때 방 입장 하기
	bool EnterRoom(const LinkPtr& shared_clientInfo, int targetRoomNumBer);
	bool EnterRoomSpecial(const LinkPtr& shared_clientInfo, int targetRoomNumBer, const string & pw);
	// 방에 있는 사람 전부 배팅 준비 됐나?
	bool IsAllReadyGame(const LinkPtr& shared_clientInfo);
	// 게임 결과 판별
	//void ResultGame(const LinkPtr& shared_clientInfo);
	// 해당 링크가 들어있는 방 방송
	void Broadcast(const LinkPtr& shared_clientInfo, const Packet& packet, int flags = 0);
	// 해당 링크가 들어있는 수다
	void Talk(const LinkPtr & shared_clientInfo, const Packet & packet, int flags = 0);
	// 빈방 찾기
	int SearchRoom();
	// 캐릭터 변경
	void ChangeMyCharacter(const LinkPtr & shared_clientInfo, const Packet & packet);
	// 호스트 ip 가져오기
	void GetHostIP(const LinkPtr & shared_clientInfo);
};
 
