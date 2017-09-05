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
const int BufSize = 1024;
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
