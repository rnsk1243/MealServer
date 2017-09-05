#pragma once
#include<string>
#include<iostream>
#include<vector>
using namespace std;


enum WhatGoodsCursorSize
{
	Money
};

enum ErrorLevel
{
	Serious, Normal, Low, Succes
};

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
const int BufSize = 1024;
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
const int EnterRoomPeopleLimit = 4; // �濡 ���� ������ �ִ� �ο�
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
