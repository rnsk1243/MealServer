//#pragma comment(lib,"version.lib")
#include<iostream>
#include"ReadyNetWork.h"
#include"CommandController.h"
#include"Link.h"
#include"RoomManager.h"
#include"ChannelManager.h"
#include"Channel.h"
#include"Listener.h"
#include"Lobby.h"
#include<process.h>
#include<thread>
#include"ErrorHandler.h"
#include"resource.h"
#include"BasicExcel.hpp"
#include<Windows.h>
#include"MinidumpHelp.h"
#include"ReadHandler.h"
#include"ConstEnumInfo.h"
using namespace std;
using namespace YExcel;

void printVersionInfo()
{
	// 버전정보를 담을 버퍼
	char* buffer = NULL;
	// 버전을 확인할 파일
	char* name = "RoomChatServer.exe";

	DWORD infoSize = 0;

	// 파일로부터 버전정보데이터의 크기가 얼마인지를 구합니다.
	infoSize = GetFileVersionInfoSize(name, 0);
	if (infoSize == 0) return;
	
	// 버퍼할당
	buffer = new char[infoSize];
	if (buffer)
	{
		// 버전정보데이터를 가져옵니다.
		if (GetFileVersionInfo(name, 0, infoSize, buffer) != 0)
		{
			VS_FIXEDFILEINFO* pFineInfo = NULL;
			UINT bufLen = 0;
			// buffer로 부터 VS_FIXEDFILEINFO 정보를 가져옵니다.
			if (VerQueryValue(buffer, "\\", (LPVOID*)&pFineInfo, &bufLen) != 0)
			{
				WORD majorVer, minorVer, buildNum, revisionNum;
				majorVer = HIWORD(pFineInfo->dwFileVersionMS);
				minorVer = LOWORD(pFineInfo->dwFileVersionMS);
				buildNum = HIWORD(pFineInfo->dwFileVersionLS);
				revisionNum = LOWORD(pFineInfo->dwFileVersionLS);

				// 파일버전 출력
				printf("version : %d,%d,%d,%d\n", majorVer, minorVer, buildNum, revisionNum);
			}
		}
		delete[] buffer;
	}
}


int thSendRecv(void* v_clientSocket, void* v_ip)
{
	const SOCKET* clientSocket = ((SOCKET*)v_clientSocket);
	string ip = *(string*)v_ip;
	cout << "붙은 클라이언트 ip = " << ip;
	CGuestLink guest(clientSocket);
	int isLogin = 0;
	vector<string> userInfo;
	while (SUCCES_LOGIN != isLogin)
	{
		isLogin = LobbyStatic->ActionServiceLobby(&guest, userInfo);
	}
	LinkPtr shared_clientInfo(new CLink(clientSocket, userInfo[IndexUserPK], userInfo[IndexUserID].c_str(), ip));

	// EnterChannelNum 채널에 입장
	//vector<string> commandChannel;
	CommandControllerStatic->SetEnterChannel(shared_clientInfo, StartEnterChannelNum);
	/*if (false == ReadHandlerStatic->ReadUserGoods(shared_clientInfo, NameMemberGoodsTxt))
		return 0;*/

	//cout << "보유 재화 = " << shared_clientInfo->GetMyMoney() << endl;
	string welcomeMessage = "환영합니다. " + shared_clientInfo.get()->GetMyName() + " 님";
	shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, welcomeMessage.c_str()));
	//shared_clientInfo.get()->SendnMine("로그인 성공");
	ErrorHandStatic->ErrorHandler(SUCCES_LOGIN, shared_clientInfo);
	while (true)
	{
		Packet packet;
		ListenerStatic->RecvnLink(shared_clientInfo, packet);

		switch (packet.InfoProtocol)
		{
		case ProtocolInfo::ChattingMessage:
			CommandControllerStatic->ChattingMessage(shared_clientInfo, packet);
			break;
		case ProtocolInfo::ServerCommend:
			CommandControllerStatic->CommandHandling(shared_clientInfo, packet);
			break;
		default:
			break;
		}
	}
}

void main()
{
	//MinidumpHelp dump;
	//dump.install_self_mini_dump();
	/////////// 버전 정보 출력 ///////////
	printVersionInfo();
	//////////////////////////////////////
	BasicExcel excel;
	excel.ReadExcel("CharacterDB.xls");
	

	while (true)
	{
		SOCKET* clientSocket = new SOCKET();
		string ip;
		ReadyNetWorkStatic->Accept(clientSocket, ip);
		
		thread clientThread(thSendRecv, clientSocket, &ip);
		clientThread.detach();
	}
	getchar();
}