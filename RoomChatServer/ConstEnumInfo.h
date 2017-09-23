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

enum ProtocolInfo       // ��з�
{
	None,               // �ʱ�ȭ ��
	ServerCommend,      // �������� ���
	ChattingMessage,    // ä�� �޼���
	ClientCommend
	//PlayerInfo          // �÷��̾� ����( ĳ����, �̸� )
};

enum ProtocolDetail     // �Һз�
{
	NoneDetail,               // �ʱ�ȭ ��
	Message,            // �޼���
	ImageChange,              // �̹���
	NameChange,               // �̸�
	EnterRoom,
	EnterChanel,
	MakeRoom,
	OutRoom,
	ReadyGame,           // �����غ�
	FrontMenu,				// �޴�(�α���, ȸ������, �Խ�Ʈ �α��� )
	MatchingSuccess, // ��Ī ���� �� UI ǥ��
	LoginSuccess         // �α��� ���� ä�� ����
};

enum ProtocolCharacterImage
{
	Tofu, Mandu, Tangsuyuk
};

enum ProtocolCharacterTagIndex   // CharacterImageTag �迭�� CharacterNameTag �迭�� �ε���
{
	Red01, Red02, Red03, Blue01, Blue02, Blue03
};

enum ProtocolMessageTag
{
	Text // MessageTag�迭�� "TextView"��� �ױ�(����Ƽ���� ä��â�� �ش���)
};

enum ProtocolFrontManuTag
{
	LoginManu, JoinManu, GuestManu, CancleManu // Ŭ���̾�Ʈ ��û ��
};

enum ProtocolSceneName
{
	FrontScene, ChannelScene, RoomScene
};

enum State	// Ŭ���̾�Ʈ ����
{
	ClientNone, ClientFrontMenu, ClientJoin, ClientChannelMenu, ClientMatching,
	ClientMakeRoom, ClientOption, ClientRoomIn
};

const string ProtocolFrontManuTag[4] = { "LoginManu", "JoinManu", "GuestManu", "CancleManu" };
const string ProtocolCharacterImageName[3] = { "Tofu", "Mandu", "Tangsuyuk" };
const string ProtocolCharacterTagIndexImage[6] = { "RedImage01", "RedImage02", "RedImage03", "BlueImage01", "BlueImage02", "BlueImage03" };
const string ProtocolCharacterTagIndexName[6] = { "RedName01", "RedName02", "RedName03", "BlueName01", "BlueName02", "BlueName03" };
const string MessageTag[1] = { "TextView" };
const string ProtocolSceneName[3] = { "FrontScene", "ChannelScene", "RoomScene" };
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

// ���� �޼���
const string EnterRoomMoneyLack = "���� �����Ͽ� �濡 ���� �Ͻ� �� �����ϴ�.";
const string DialogEnterRoomPeopleLimit = "�濡 �ο����� �ʰ��Ͽ� �� �̻� �����Ͻ� �� �����ϴ�.";

// ��
const int CharacterNumberCols = 0; // �׼����� ī�� ��ȣ ����ȣ
const int CharacterNameCols = 1;
const int CharacterLVCols = 2;

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
const int EnterRoomPeopleLimit = 6; // �濡 ���� ������ �ִ� �ο�
const int FineGamePlayingOutMoney = 20; // ���ӵ��� ���� ����
const int MaxChannelNum = 5;
const int ChannelAmount = 5;
const int NoneRoom = -1; // �濡 ���� ���� ����
const int IndexUserPK = 0; // .txt���Ͽ��� �� ������ 0 | ��Ÿ�������� 1�� ����
const int IndexUserID = 1; 
const int IndexUserPW = 2;
const int IndexGoodsInfoTxtPK = 0;
const int IndexGoodsInfoTxtMoney = 1;
const int MoneyInfoSize = MaxMoneyCipher + 1; // +1 �� ������ '|' ����
const int GoodsTemplateSize = MoneyInfoSize + 3 ;
static const int PacketSize = 140;//sizeof(Packet);

struct Packet
{
	// Packet������ PacketSize ������ ��.
	int InfoProtocol;
	int InfoProtocolDetail;
	int InfoTagIndex;
	char InfoValue[BufSizeValue];

	Packet() {}

	Packet(int infoProtocol, int infoProtocolDetail, int infoTagIndex, const char* infoValue) :
		InfoProtocol(infoProtocol),
		InfoProtocolDetail(infoProtocolDetail),
		InfoTagIndex(infoTagIndex)
	{
		cout << "Packet ������" << endl;
		if (nullptr != infoValue)
		{
			strcpy_s(InfoValue, BufSizeValue, infoValue);
			cout << "�ʱ�ȭ�� �� Value == " << InfoValue << endl;
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