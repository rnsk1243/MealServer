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
	string mName;
	// ���� ���� ���� �� ��ȣ
	int mMyRoomNum;
	// ���� ���� ���� ä�� ��ȣ
	int mMyChannelNum;
	// Ŭ���̾�Ʈ ����
	const SOCKET* mClientSocket;
	//MessageStruct mMS;
	// ���� ��ȭ
	CGoods mMyGoods;
	MUTEX mRAII_LinkMUTEX;
	// �� ȸ����ȣ
	const int mMyPKNumber;
	// ������ �غ� �Ƴ�?
	bool mIsGameOK;
	int mDebtMoney;	// ������ ������ ��.
	int mPayBackMoney; // ������ ���� ��.
	ProtocolCharacterTagIndex mMyPosition; // �濡�� ���� ��ġ �Ǵ� ��
	bool mIsInitGoods; // ��ȭ �ʱ�ȭ �߳�?
	ProtocolCharacterImageNameIndex mSelectCharacter;	// ���� ������ ĳ����
	bool InitMoney(int money);
	bool AddMoney(const int& addMoney);
	bool MinusMyMoney(const int& minusMoney);
	bool PayBackMoney(const int& payBack);
public:
	CLink(const SOCKET* clientSocket, const string& strPKNumber,const char* name);
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
	bool IsZeroMoney();
	void SetZeroMoney();
	const int GetMyMoney();
	const int GetMyPKNumber()const;
	void SetInitGoods();
	void SetReadyGame(); //�غ�Ϸ�
	//void SetReadyGame(const int& battingMoney);
	void SetNoReadyGame();
	bool GetReadyGame();
	bool GetPrizeBattingMoney(const int& bettingMoney); // ���øӴ� �ޱ�
	bool SaveCalculateMoney();
	ProtocolCharacterTagIndex GetMyPosition();
	void SetMyPosition(ProtocolCharacterTagIndex newPosition);
	void SetMyCharacter(ProtocolCharacterImageNameIndex newCharacter); // ĳ���� ����
#pragma endregion
	bool InitGoods(int initMoney);
	bool InitBetting();
	void LostWillMoney(const int& fine); // �� �������� �߰�
	////////////////////
	void SendnMine(const Packet & packet, int flags = 0);
};

