#pragma once
#include<string>
#include<iostream>
#include<vector>
#include"RecvRepository.h"
using namespace std;
struct Packet;

enum WhatGoodsCursorSize
{
	Money
};

enum ErrorLevel
{
	Serious, Normal, Low, Succes
};

enum ProtocolInfo       // 대분류
{
	None,               // 초기화 값
	ServerCommend,      // 서버에게 명령
	ChattingMessage,    // 채팅 메세지
	PlayerInfo          // 플레이어 정보( 캐릭터, 이름 )
};

enum ProtocolDetail     // 소분류
{
	NoneDetail,               // 초기화 값
	Message,            // 메세지
	Image,              // 이미지
	Name,               // 이름
	EnterRoom,
	EnterChanel,
	MakeRoom,
	OutRoom,
	ReadyGame           // 게임준비
};

enum ProtocolCharacter
{
	Tofu, Mandu, Tangsuyuk
};

enum ProtocolCharacterTagIndex   // CharacterImageTag 배열과 CharacterNameTag 배열의 인덱스
{
	Red01, Red02, Red03, Blue01, Blue02, Blue03
};

enum ProtocolMessageTag
{
	Text // MessageTag배열의 "TextView"라는 테그(유니티에서 채팅창에 해당함)
};

const string CharacterImageName[3] = { "Tofu", "Mandu", "Tangsuyuk" };
const string CharacterImageTag[6] = { "RedImage01", "RedImage02", "RedImage03", "BlueImage01", "BlueImage02", "BlueImage03" };
const string CharacterNameTag[6] = { "RedName01", "RedName02", "RedName03", "BlueName01", "BlueName02", "BlueName03" };
const string MessageTag[1] = { "TextView" };
const string CommandEnter = "e"; // 방 입장
const string CommandChannal = "c"; // 채널 변경
const string CommandMakeRoom = "m"; // 방 만들기
const string CommandOutRoom = "o"; // 방 나가기
const string CommandGameStart = "ready"; // 게임 준비
const string StartGoodsInventory("0000100"); // 회원 가입 시작 머니
const int JoinMemberTxtCapacity = 9;
const string NameMemberInfoTxt = "MemberInfo.txt";
const string MakeNextJoinNumberTxt = "MakeNextJoinNumber.txt";
const string NameMemberGoodsTxt = "MemberGoods.txt";
const string ErrorLogTxt = "ErrorLog.txt";

// 보낼 메세지
const string EnterRoomMoneyLack = "돈이 부족하여 방에 입장 하실 수 없습니다.";
const string DialogEnterRoomPeopleLimit = "방에 인원수가 초과하여 더 이상 입장하실 수 없습니다.";

// 값
const int CharacterNumberCols = 0; // 액셀파일 카드 번호 열번호
const int CharacterNameCols = 1;
const int CharacterLVCols = 2;

const int Port = 9000;
const int BufSizeRecv = 1024; // recv하는데 사용하는 버퍼
const int BufSizeSend = 1024; // send하는데 사용하는 버퍼
const int BufSizeTag = 64;	// 오브젝트 Tag값
const int BufSizeValue = 128; // 채팅 메세지, 혹은 값
const int ExcelBufSize = 4096;
const int NameSize = 64;
const int TimeSize = 10;
const int IdPwSize = 256;
const int IntSize = 4;
const int MaxMoney = 9999999; // 돈 최대치
const int MaxMoneyCipher = 7; // 돈 최대치 자릿수
const int MinMoney = 0;		// 돈 최소치
const int MakeThreadNum = 3;
const int timeKind = 6; // 시간 종류 갯수 (년, 월, 일, 시, 분, 초) 6개
const int StartEnterChannelNum = 1; // 처음 접속 채널
const int StartRoomNum = 1; // 처음 만들어지는 룸 번호
const int EnterRoomPeopleLimit = 4; // 방에 입장 가능한 최대 인원
const int FineGamePlayingOutMoney = 20; // 게임도중 도주 벌금
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1; // 방에 들어와 있지 않음
const int IndexUserPK = 0; // .txt파일에서 맨 왼쪽을 0 | 나타날때마나 1씩 증가
const int IndexUserID = 1; 
const int IndexUserPW = 2;
const int IndexGoodsInfoTxtPK = 0;
const int IndexGoodsInfoTxtMoney = 1;
const int MoneyInfoSize = MaxMoneyCipher + 1; // +1 한 이유는 '|' 때문
const int GoodsTemplateSize = MoneyInfoSize + 3 ;
static const int PacketSize = 140;//sizeof(Packet);

struct Packet
{
	// Packet수정시 PacketSize 변경할 것.
	int InfoProtocol;
	int InfoProtocolDetail;
	int InfoTagNumber;
	char InfoValue[BufSizeValue];

	Packet() {}

	Packet(int infoProtocol, int infoProtocolDetail, int infoTagNumber, const char* infoValue) :
		InfoProtocol(infoProtocol),
		InfoProtocolDetail(infoProtocolDetail),
		InfoTagNumber(infoTagNumber)
	{
		cout << "Packet 생성자" << endl;
		if (nullptr != infoValue)
		{
			strcpy_s(InfoValue, BufSizeValue, infoValue);
			cout << "초기화된 값 Value == " << InfoValue << endl;
		}
		else
		{
			infoValue = nullptr;
		}
	}
};

//struct OrderStructSocket
//{
//	const SOCKET* clientSocket;
//	PacketPtr packet;
//	OrderStructSocket(const SOCKET* clientSock, const PacketPtr& pack)
//	{
//		clientSocket = clientSock;
//		packet = pack;
//	}
//};
//
//struct OrderStructLink
//{
//	LinkPtr linkPtr;
//	PacketPtr packet;
//	OrderStructLink(const LinkPtr& link, const PacketPtr& pack)
//	{
//		linkPtr = link;
//		packet = pack;
//	}
//};