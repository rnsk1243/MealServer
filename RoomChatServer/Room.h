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
	const int mChannelNum;
	const int mRoomNum;
	// ���� ����ִ� �� �ο�
	int mAmountPeople;
	MUTEX mRAII_RoomMUTEX;
	//CRITICALSECTION CT;
	//int mBettingMoney;
	//bool mPlayingGame; // ������?
	bool mIsNewRoom; // ���ο� ���ΰ�?
	vector<int> mUsePosition; // ĳ���� â ������� ĭ�� ����ִ� ĭ Ȯ�ο�
	void IncreasePeople();
	void DecreasePeople();
	//void SetGame();
	//void SetGameOver();
	//void SendBattingResult(const LinkPtr& winner, int flags = 0);
	bool AllCalculateMoney(); // �濡 �ִ� ��� ��� ����(.txt ����)
	// �濡 ������ �˷��ֱ�
	void NoticRoomIn(const LinkPtr& shared_client);
	void EnterBroadcast(const LinkPtr& shared_client, ProtocolCharacterTagIndex tagIndex); // �� ó�� ���� �Լ� ��ο��� �� ǥ����ġ�� �̸� ������
public:
	CRoom(const CRoom&) = delete;
	CRoom& operator=(const CRoom&) = delete;
	CRoom(int roomNum,int channelNum, const string& roomName,const int& battingMoney);
	~CRoom();
	void PushClient(const LinkPtr& shared_client, const int& enterRoomNumber);
	LinkListIt EraseClient(const LinkPtr& shared_client);
	int GetRoomNum();
	int GetChannelNum();
	const string GetRoomName();
	int GetAmountPeople();
	bool IsRoomEmpty();
	//int GetBattingMoney();
	bool IsAllReady();
	//void GameResult();	// ���� ���
	//bool IsGame();
	//bool AllInitBetting();
	void Broadcast(const Packet& packet, int flags = 0);
	void ChangetCharacterBroadcast(const LinkPtr & shared_client, const ProtocolCharacterImageNameIndex& characterImageIndex); // ��ο��� ĳ���� ���� ������
	void Talk(const LinkPtr& myClient, const Packet& packet, int flags = 0);

};

