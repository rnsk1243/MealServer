#pragma once
#include<WinSock2.h>
#include<iostream>
#include<list>
#include"Channel.h"
#include"RAII.h"
class CChannel;
class CLink;
using namespace std;

class CRoom
{
	LinkList mClientInfos;
	string mRoomName;
	string mRoomPW; // room 입장 비번
	const int mChannelNum;
	const int mRoomNum;
	// 현재 들어있는 방 인원
	int mAmountPeople;
	const ProtocolTeamAmount mEnterRoomPeopleLimit;
	MUTEX mRAII_RoomMUTEX;
	//CRITICALSECTION CT;
	//int mBettingMoney;
	bool mPlayingGame; // 게임중?
	bool mIsNewRoom; // 새로운 방인가?
	bool mIsSpecialRoom; // 직접 만든 방인가?
	bool mIsPublicRoom; // 공개방인가?
	vector<int> mUsePosition; // 캐릭터 창 사용중인 칸과 비어있는 칸 확인용
	void IncreasePeople();
	void DecreasePeople();
	//void SetGame();
	//void SetGameOver();
	//void SendBattingResult(const LinkPtr& winner, int flags = 0);
	//bool AllCalculateMoney(); // 방에 있는 사람 모두 정산(.txt 저장)
	// 방에 들어왔음 알려주기
	void NoticRoomIn(const LinkPtr& shared_client);
	void EnterBroadcast(const LinkPtr& shared_client, ProtocolCharacterTagIndex tagIndex, const bool& isBackRoom = false); // 방 처음 입장 함수 // 모두에게 내 표시위치와 이름 보내기
	void TeachNewPeople(const LinkPtr& shared_client); // 새로 들어온 사람에게 현재 방얘들 이름이랑 고른 캐릭터 알려주기
	void NoticSoloEnterRoomIn(const LinkPtr & shared_client);
	void SetGame(bool isGame);
public:
	CRoom(const CRoom&) = delete;
	CRoom& operator=(const CRoom&) = delete;
	CRoom(int roomNum,int channelNum, const string& roomName, const ProtocolTeamAmount& teamAmount, const bool& isSpecialRoom, const string & roomPW = RoomPWNone);
	~CRoom();
	bool PushClient(const LinkPtr& shared_client, const int& enterRoomNumber);
	bool PushClientSpecialRoom(const LinkPtr& shared_client, const int& enterRoomNumber, const string & pw);
	void BackRoomScene();
	LinkListIt EraseClient(const LinkPtr& shared_client);
	int GetRoomNum();
	int GetChannelNum();
	const string GetRoomName();
	int GetAmountPeople();
	bool IsRoomEmpty();
	//int GetBattingMoney();
	bool IsAllReady();
	//void GameResult();	// 게임 결과
	bool IsGame();
	//bool AllInitBetting();
	void Broadcast(const Packet& packet, int flags = 0);
	void ChangeCharacterBroadcast(const LinkPtr & shared_client, const ProtocolCharacterImageNameIndex& characterImageIndex); // 모두에게 캐릭터 변경 보내기
	void Talk(const LinkPtr& myClient, const Packet& packet, int flags = 0);
	void GetHostIP(const LinkPtr& requestClient);
	void NotReadyTogether();
	int GetLimitEnterRoomPeople();
	bool IsSpecialRoom();
	bool IsPublicroom();
	void SendMyReadyInfo(const LinkPtr& myClient);
};

