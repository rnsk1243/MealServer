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

enum ProtocolInfo       // ��з�
{
	None,               // �ʱ�ȭ ��
	ServerCommend,      // �������� ���
	ChattingMessage,    // ä�� �޼���
	ClientCommend,		// Ŭ���̾�Ʈ ���
	RequestResult,		// ��û ���
	SceneChange         // �� ����
	//PlayerInfo          // �÷��̾� ����( ĳ����, �̸� )
};

enum ProtocolDetail     // �Һз�
{
	NoneDetail,               // �ʱ�ȭ ��
	Message,            // �޼���
	ImageChange,              // �̹���
	NameChange,               // �̸�
	EnterRoom,
	EnterSpecialRoom,
	EnterChanel,
	MakeRoom,
	OutRoom,
	OutMakeRoom,
	SetReadyGame,           // �����غ�
	FrontMenu,              // �޴�(�α���, ȸ������, �Խ�Ʈ �α��� )
	ChangeCharacter,        // ĳ���� ����
	NotReadyGame,           // ���� �غ� ���
	StartGame,
	GetHostIP,
	SuccessRequest,       // ��û ����
	FailRequest,            // ��û ����
	RemovePanel             // ���� ��� �г� �����
};

enum ProtocolCharacterImageNameIndex
{
	Tofu, Mandu, Tangsuyuk
};

enum ProtocolTagNull
{
	Null = -1
};

enum ProtocolCharacterTagIndex   // CharacterImageTag �迭�� CharacterNameTag �迭�� �ε���
{
	//NoneCharacter, Red01, Red02, Red03, Blue01, Blue02, Blue03
	Red01, Blue01, Red02, Blue02, Red03, Blue03
};

enum ProtocolMessageTag
{
	Text // MessageTag�迭�� "TextView"��� �ױ�(����Ƽ���� ä��â�� �ش���)
};

enum ProtocolFrontManuTag
{
	LoginManu, JoinManu, GuestManu, CancleManu // Ŭ���̾�Ʈ ��û ��
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

enum State	// Ŭ���̾�Ʈ ����
{
	ClientNone, ClientFrontMenu/*front�������� �⺻ ����*/,
	ClientLogin/*login�� ������ ����*/, ClientJoin/*ȸ�������� ������ ����*/, ClientGuest/*guest �α��� ����*/,
	ClientChannelMenu/*ä�ξ������� �⺻ ����*/,
	ClientMakeRoom, ClientOption,
	ClientGame,/*0926�߰���*/
	ClientRequestGaemReady, ClientReady,       // ������ ��Ʈ��
	ClientRequestGaemNotReady, ClientNotReady/*�뿡���� �⺻ ����*/, // 1001�߰���
	ClientRequestMatching, ClientMatching,
	ClientRequestCancleMactching, /*ä�� �⺻*/
	ClientRequestCharacterChange, /*��⺻*/
	ClientRequestBackExit, /*�� �⺻*/
	ClientRequestMakeRoom /*room����� ��û �Ϸ�*/
};

const string ProtocolFrontManuTag[4] = { "LoginManu", "JoinManu", "GuestManu", "CancleManu" };
const string ProtocolCharacterImageName[3] = { "Tofu", "Mandu", "Tangsuyuk" };
const ProtocolCharacterImageNameIndex InitCharacter = ProtocolCharacterImageNameIndex::Tofu;	// �濡 ����� ����Ǵ� �⺻ ĳ����
//const string ProtocolCharacterTagIndexImage[6] = { "RedImage01", "RedImage02", "RedImage03", "BlueImage01", "BlueImage02", "BlueImage03" };
//const string ProtocolCharacterTagIndexName[6] = { "RedName01", "RedName02", "RedName03", "BlueName01", "BlueName02", "BlueName03" };
const string ProtocolCharacterTagIndexImage[6] = { "RedImage01", "BlueImage01", "RedImage02", "BlueImage02", "RedImage03", "BlueImage03" };
const string ProtocolCharacterTagIndexName[6] = { "RedName01", "BlueName01", "RedName02", "BlueName02", "RedName03", "BlueName03" };
const string MessageTag[1] = { "TextView" };
const string ProtocolSceneNameString[4] = { "FrontScene", "ChannelScene", "RoomScene", "Main" };
const string CommandEnter = "e"; // �� ����
const string CommandChannal = "c"; // ä�� ����
const string CommandMakeRoom = "m"; // �� �����
const string CommandOutRoom = "o"; // �� ������
const string CommandGameStart = "ready"; // ���� �غ�
const string StartGoodsInventory("0000100"); // ȸ�� ���� ���� �Ӵ�
const int JoinMemberTxtCapacity = 9;
const string NameMemberInfoTxt = "MemberInfo.txt";
const string MakeNextJoinNumberTxt = "MakeNextJoinNumber.txt";
const string NameMemberGoodsTxt = "MemberGoods.txt";
const string ErrorLogTxt = "ErrorLog.txt";
const string GuestName = "Guest";
const string GuestPKNumber = "0";
const string RoomInitName = "Room";
const string RoomPWNone = "None";
// ���� �޼���
const string EnterRoomMoneyLack = "���� �����Ͽ� �濡 ���� �Ͻ� �� �����ϴ�.";
const string DialogEnterRoomPeopleLimit = "�濡 �ο����� �ʰ��Ͽ� �� �̻� �����Ͻ� �� �����ϴ�.";

// ��
const int CharacterNumberCols = 0; // �׼����� ī�� ��ȣ ����ȣ
const int CharacterNameCols = 1;
const int CharacterLVCols = 2;
const int Used = 1; // �����
const int NotUsed = 0; // �̻����

const int Port = 9000;
const int BufSizeRecv = 1024; // recv�ϴµ� ����ϴ� ����
const int BufSizeSend = 1024; // send�ϴµ� ����ϴ� ����
const int BufSizeTag = 64;	// ������Ʈ Tag��
const int BufSizeValue = 128; // ä�� �޼���, Ȥ�� ��
const int ExcelBufSize = 4096;
const int NameSize = 64;
const int TimeSize = 10;
const int IdPwSize = 256;
const int IntSize = 4;
const int MaxMoney = 9999999; // �� �ִ�ġ
const int MaxMoneyCipher = 7; // �� �ִ�ġ �ڸ���
const int MinMoney = 0;		// �� �ּ�ġ
const int MakeThreadNum = 3;
const int timeKind = 6; // �ð� ���� ���� (��, ��, ��, ��, ��, ��) 6��
const int StartEnterChannelNum = 1; // ó�� ���� ä��
const int StartRoomNum = 1; // ó�� ��������� �� ��ȣ
<<<<<<< HEAD
const ProtocolTeamAmount EnterAutoMatchingRoomPeopleLimit = ProtocolTeamAmount::ThreeTeam; // �濡 ���� ������ �ִ� �ο�
=======
const ProtocolTeamAmount EnterAutoMatchingRoomPeopleLimit = ProtocolTeamAmount::OneTeam; // �濡 ���� ������ �ִ� �ο�
>>>>>>> origin/WOOHEE
const int FineGamePlayingOutMoney = 20; // ���ӵ��� ���� ����
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1; // �濡 ���� ���� ���� Ȥ�� ����� ����
const int NonePosition = -1; // �濡 ���� ���� �ʾƼ� �ڸ��� �������� �ʾ�����
const int IndexUserPK = 0; // .txt���Ͽ��� �� ������ 0 | ��Ÿ�������� 1�� ����
const int IndexUserID = 1; 
const int IndexUserPW = 2;
const int IndexGoodsInfoTxtPK = 0;
const int IndexGoodsInfoTxtMoney = 1;
const int MoneyInfoSize = MaxMoneyCipher + 1; // +1 �� ������ '|' ����
const int GoodsTemplateSize = MoneyInfoSize + 3 ;
static const int PacketSize = 140;//sizeof(Packet);
const int PacketAmount = 1;


struct Packet
{
	// Packet������ PacketSize ������ ��.
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
		//	cout << "Packet ������" << endl;
		if (nullptr != infoValue)
		{
			strcpy_s(InfoValue, BufSizeValue, infoValue);
			//	cout << "�ʱ�ȭ�� �� Value == " << InfoValue << endl;
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