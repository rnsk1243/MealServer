#pragma once
#include<string>
#include<iostream>
#include<vector>
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
	ClientCommend,		// 클라이언트 명령
	RequestResult,		// 요청 결과
	SceneChange         // 씬 변경
	//PlayerInfo          // 플레이어 정보( 캐릭터, 이름 )
};

enum ProtocolDetail     // 소분류
{
	NoneDetail,               // 초기화 값
	Message,            // 메세지
	ImageChange,              // 이미지
	NameChange,               // 이름
	EnterRoom,
	EnterSpecialRoom,
	EnterChanel,
	MakeRoom,
	OutRoom,
	OutMakeRoom,
	SetReadyGame,           // 게임준비
	FrontMenu,              // 메뉴(로그인, 회원가입, 게스트 로그인 )
	ChangeCharacter,        // 캐릭터 변경
	NotReadyGame,           // 게임 준비 취소
	StartGame,
	GetHostIP,
	SuccessRequest,       // 요청 성공
	FailRequest,            // 요청 실패
	RemovePanel             // 나간 사람 패널 지우기
};

enum ProtocolCharacterImageNameIndex
{
	Tofu, Mandu, Tangsuyuk
};

enum ProtocolTagNull
{
	Null = -1
};

enum ProtocolCharacterTagIndex   // CharacterImageTag 배열과 CharacterNameTag 배열의 인덱스
{
	//NoneCharacter, Red01, Red02, Red03, Blue01, Blue02, Blue03
	Red01, Blue01, Red02, Blue02, Red03, Blue03
};

enum ProtocolMessageTag
{
	Text // MessageTag배열의 "TextView"라는 테그(유니티에서 채팅창에 해당함)
};

enum ProtocolFrontManuTag
{
	LoginManu, JoinManu, GuestManu, CancleManu // 클라이언트 요청 상세
};

enum ProtocolChannelMenuTag
{
	MatchingStart, MatchingCancel
};

enum ProtocolSceneName
{
	FrontScene, ChannelScene, RoomMakeScene, RoomScene, MainScene
};

enum ProtocolTeamAmount
{
	OneTeam, TwoTeam, ThreeTeam
};

enum ProtocolTeamAmount
{
	OneTeam, TwoTeam, ThreeTeam
};

enum State	// 클라이언트 상태
{
	ClientNone, ClientFrontMenu/*front씬에서의 기본 상태*/,
	ClientLogin/*login을 선택한 상태*/, ClientJoin/*회원가입을 선택한 상태*/, ClientGuest/*guest 로그인 상태*/,
	ClientChannelMenu/*채널씬에서의 기본 상태*/,
	ClientMakeRoom, ClientOption,
	ClientGame,/*0926추가됨*/
	ClientRequestGaemReady, ClientReady,       // 쌍으로 세트임
	ClientRequestGaemNotReady, ClientNotReady/*룸에서의 기본 상태*/, // 1001추가됨
	ClientRequestMatching, ClientMatching,
	ClientRequestCancleMactching, /*채널 기본*/
	ClientRequestCharacterChange, /*룸기본*/
	ClientRequestBackExit, /*씬 기본*/
	ClientRequestMakeRoom /*room만들기 신청 완료*/
};

const string ProtocolFrontManuTag[4] = { "LoginManu", "JoinManu", "GuestManu", "CancleManu" };
const string ProtocolCharacterImageName[3] = { "Tofu", "Mandu", "Tangsuyuk" };
const ProtocolCharacterImageNameIndex InitCharacter = ProtocolCharacterImageNameIndex::Tofu;	// 방에 입장시 적용되는 기본 캐릭터
//const string ProtocolCharacterTagIndexImage[6] = { "RedImage01", "RedImage02", "RedImage03", "BlueImage01", "BlueImage02", "BlueImage03" };
//const string ProtocolCharacterTagIndexName[6] = { "RedName01", "RedName02", "RedName03", "BlueName01", "BlueName02", "BlueName03" };
const string ProtocolCharacterTagIndexImage[6] = { "RedImage01", "BlueImage01", "RedImage02", "BlueImage02", "RedImage03", "BlueImage03" };
const string ProtocolCharacterTagIndexName[6] = { "RedName01", "BlueName01", "RedName02", "BlueName02", "RedName03", "BlueName03" };
const string MessageTag[1] = { "TextView" };
const string ProtocolSceneNameString[4] = { "FrontScene", "ChannelScene", "RoomScene", "Main" };
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
const string GuestName = "Guest";
const string GuestPKNumber = "0";
const string RoomInitName = "Room";
const string RoomPWNone = "None";
// 보낼 메세지
const string EnterRoomMoneyLack = "돈이 부족하여 방에 입장 하실 수 없습니다.";
const string DialogEnterRoomPeopleLimit = "방에 인원수가 초과하여 더 이상 입장하실 수 없습니다.";

// 값
const int CharacterNumberCols = 0; // 액셀파일 카드 번호 열번호
const int CharacterNameCols = 1;
const int CharacterLVCols = 2;
const int Used = 1; // 사용중
const int NotUsed = 0; // 미사용중

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
<<<<<<< HEAD
const ProtocolTeamAmount EnterAutoMatchingRoomPeopleLimit = ProtocolTeamAmount::ThreeTeam; // 방에 입장 가능한 최대 인원
=======
const ProtocolTeamAmount EnterAutoMatchingRoomPeopleLimit = ProtocolTeamAmount::OneTeam; // 방에 입장 가능한 최대 인원
>>>>>>> origin/WOOHEE
const int FineGamePlayingOutMoney = 20; // 게임도중 도주 벌금
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1; // 방에 들어와 있지 않음 혹은 빈방이 없음
const int NonePosition = -1; // 방에 들어와 있지 않아서 자리가 정해지지 않아있음
const int IndexUserPK = 0; // .txt파일에서 맨 왼쪽을 0 | 나타날때마나 1씩 증가
const int IndexUserID = 1; 
const int IndexUserPW = 2;
const int IndexGoodsInfoTxtPK = 0;
const int IndexGoodsInfoTxtMoney = 1;
const int MoneyInfoSize = MaxMoneyCipher + 1; // +1 한 이유는 '|' 때문
const int GoodsTemplateSize = MoneyInfoSize + 3 ;
static const int PacketSize = 140;//sizeof(Packet);
const int PacketAmount = 1;


struct Packet
{
	// Packet수정시 PacketSize 변경할 것.
	int InfoProtocol;
	int InfoProtocolDetail;
	int InfoTagIndex;
	char InfoValue[BufSizeValue];

	Packet();

	Packet(int infoProtocol, int infoProtocolDetail, int infoTagIndex, const char* infoValue) :
		InfoProtocol(infoProtocol),
		InfoProtocolDetail(infoProtocolDetail),
		InfoTagIndex(infoTagIndex)
	{
		//	cout << "Packet 생성자" << endl;
		if (nullptr != infoValue)
		{
			strcpy_s(InfoValue, BufSizeValue, infoValue);
			//	cout << "초기화된 값 Value == " << InfoValue << endl;
		}
		else
		{
			infoValue = nullptr;
		}
	}

	void AddName(string name);
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