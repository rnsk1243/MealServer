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
	// 현재 내가 속한 방 번호
	int mMyRoomNum;
	// 현재 내가 속한 채널 번호
	int mMyChannelNum;
	// 클라이언트 소켓
	const SOCKET* mClientSocket;
	MUTEX mRAII_LinkMUTEX;
	// 내 회원번호
	const int mMyPKNumber;
	// 게임할 준비 됐나?
	bool mIsGameOK;
	ProtocolCharacterTagIndex mMyPosition; // 방에서 나의 위치 또는 팀
	ProtocolCharacterImageNameIndex mSelectCharacter;	// 내가 선택한 캐릭터
	bool mIsSocketErrorState;
	ProtocolSceneName mCurMySceneState;		// 현재 나의 씬 상태
	bool mIsGuest;						// 손님 로그인 인가?
	// 나의 재화
	//CGoods mMyGoods;
	//int mDebtMoney;	// 앞으로 없어질 돈.
	//int mPayBackMoney; // 앞으로 받을 돈.
	//bool mIsInitGoods; // 재화 초기화 했나?
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
#pragma region get, set 함수
	const SOCKET* GetClientSocket();
	int GetMyRoomNum();
	void SetMyRoomNum(const int& myRoomNum);
	int GetMyChannelNum();
	void SetMyChannelNum(const int& myChannelNum);
	string GetMyName();
	const int GetMyPKNumber()const;
	void SetReadyGame(); //준비완료
	//void SetReadyGame(const int& battingMoney);
	void SetNoReadyGame(State isAll);
	bool GetReadyGame();
	ProtocolCharacterTagIndex GetMyPosition();
	void SetMyPosition(ProtocolCharacterTagIndex newPosition);
	void SetMyCharacter(ProtocolCharacterImageNameIndex newCharacter); // 캐릭터 변경
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
	//bool GetPrizeBattingMoney(const int& bettingMoney); // 베팅머니 받기
	//bool SaveCalculateMoney();
	//bool InitGoods(int initMoney);
	////bool InitBetting();
	//void LostWillMoney(const int& fine); // 곧 없어질돈 추가
};

