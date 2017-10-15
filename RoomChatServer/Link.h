//#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<WinSock2.h>
#include<iostream>
#include<vector>
using namespace std;
#include"ConstEnumInfo.h"
#include"RAII.h"
#include"Goods.h"

typedef shared_ptr<CLink> LinkPtr;
class CLink
{
private:
	string mIP;
	string mName;
	// ���� ���� ���� �� ��ȣ
	int mMyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int mMyChannelNum;
	// Ŭ���̾�Ʈ ����
	const SOCKET* mClientSocket;
	MUTEX mRAII_LinkMUTEX;
	// �� ȸ����ȣ
	const int mMyPKNumber;
	// ������ �غ� �Ƴ�?
	bool mIsGameOK;
	ProtocolCharacterTagIndex mMyPosition; // �濡�� ���� ��ġ �Ǵ� ��
	ProtocolCharacterImageNameIndex mSelectCharacter;	// ���� ������ ĳ����
	bool mIsSocketErrorState;
	ProtocolSceneName mCurMySceneState;		// ���� ���� �� ����
	bool mIsGuest;						// �մ� �α��� �ΰ�?
	// ���� ��ȭ
	//CGoods mMyGoods;
	//int mDebtMoney;	// ������ ������ ��.
	//int mPayBackMoney; // ������ ���� ��.
	//bool mIsInitGoods; // ��ȭ �ʱ�ȭ �߳�?
	//bool InitMoney(int money);
	//bool AddMoney(const int& addMoney);
	//bool MinusMyMoney(const int& minusMoney);
	//bool PayBackMoney(const int& payBack);
public:
	CLink(const SOCKET* clientSocket, const string& strPKNumber,const char* name, const string& ip, const bool& isGuest);
	CLink(const CLink&) = delete;
	CLink& operator=(const CLink&) = delete;
	~CLink();
	bool IsRoomEnterState();
#pragma region get, set �Լ�
	const SOCKET* GetClientSocket();
	int GetMyRoomNum();
	void SetMyRoomNum(const int& myRoomNum);
	int GetMyChannelNum();
	void SetMyChannelNum(const int& myChannelNum);
	string GetMyName();
	const int GetMyPKNumber()const;
	void SetReadyGame(); //�غ�Ϸ�
	//void SetReadyGame(const int& battingMoney);
	void SetNoReadyGame(State isAll);
	bool GetReadyGame();
	ProtocolCharacterTagIndex GetMyPosition();
	void SetMyPosition(ProtocolCharacterTagIndex newPosition);
	void SetMyCharacter(ProtocolCharacterImageNameIndex newCharacter); // ĳ���� ����
	ProtocolCharacterImageNameIndex GetMyCharacter();
	bool GetIsGuest();
#pragma endregion
	string GetMyIP();
	////////////////////
	void SendnMine(const Packet & packet, int flags = 0);
	void SetSocketError();
	void SetMySceneState(ProtocolSceneName curMySceneState);
	ProtocolSceneName GetMySceneState();
	//bool IsZeroMoney();
	//void SetZeroMoney();
	//const int GetMyMoney();
	//void SetInitGoods();
	//bool GetPrizeBattingMoney(const int& bettingMoney); // ���øӴ� �ޱ�
	//bool SaveCalculateMoney();
	//bool InitGoods(int initMoney);
	////bool InitBetting();
	//void LostWillMoney(const int& fine); // �� �������� �߰�
};

