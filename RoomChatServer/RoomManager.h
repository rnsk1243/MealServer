#pragma once
#include<list>
#include<iostream>
#include"Room.h"

class CLink;
class CRoom;
using namespace std;
#pragma region Ÿ��������

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
	// room���� ������ (���������ϴ� ����)
	RoomListIt ExitRoom(const LinkPtr & shared_clientInfo, bool & isSucces);
	// ���� �ڽ��� ���� ���� ������ �� �����(�� ����, ����� �� ��ȣ) 
	int MakeRoom(const LinkPtr & shared_clientInfo, const string& roomName, const ProtocolTeamAmount& teamAmount, const string & roomPW = nullptr);
	// ���� �ڽ��� ���� ���� ������ �� ���� �ϱ�
	bool EnterRoom(const LinkPtr& shared_clientInfo, int targetRoomNumBer);
	bool EnterRoomSpecial(const LinkPtr& shared_clientInfo, int targetRoomNumBer, const string & pw);
	// �濡 �ִ� ��� ���� ���� �غ� �Ƴ�?
	bool IsAllReadyGame(const LinkPtr& shared_clientInfo);
	// ���� ��� �Ǻ�
	//void ResultGame(const LinkPtr& shared_clientInfo);
	// �ش� ��ũ�� ����ִ� �� ���
	void Broadcast(const LinkPtr& shared_clientInfo, const Packet& packet, int flags = 0);
	// �ش� ��ũ�� ����ִ� ����
	void Talk(const LinkPtr & shared_clientInfo, const Packet & packet, int flags = 0);
	// ��� ã��
	int SearchRoom();
	// ĳ���� ����
	void ChangeMyCharacter(const LinkPtr & shared_clientInfo, const Packet & packet);
	// ȣ��Ʈ ip ��������
	void GetHostIP(const LinkPtr & shared_clientInfo);
};
 
