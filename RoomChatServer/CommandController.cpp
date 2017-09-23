#include "CommandController.h"
#include"ErrorHandler.h"
class CErrorHandler;

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	cout << "CNetWork 객체 소멸자 호출" << endl;
}

void CCommandController::SetEnterChannel(const LinkPtr & shared_clientInfo, const int & moveChannelNumber)
{
	if (nullptr != shared_clientInfo.get())
	{
		if (ChannelAmount >= moveChannelNumber && StartEnterChannelNum <= moveChannelNumber)
		{
			if (0 == shared_clientInfo.get()->GetMyChannelNum())
			{
				mChannelManager.MoveChannel(shared_clientInfo, moveChannelNumber);
			}
		}
	}
}

void CCommandController::EnterRoom(const LinkPtr & shared_clientInfo, const int& roomNumber)
{
	if (mRoomManager.EnterRoom(shared_clientInfo, roomNumber))
	{
		mChannelManager.ExitChannel(shared_clientInfo);
	}
}

void CCommandController::ChangeChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber)
{
	if (nullptr != shared_clientInfo.get())
	{
		if (false == shared_clientInfo.get()->IsRoomEnterState())
		{
			if (ChannelAmount >= moveChannelNumber && StartEnterChannelNum <= moveChannelNumber)
			{
				if (mChannelManager.ExitChannel(shared_clientInfo))
				{
					mChannelManager.MoveChannel(shared_clientInfo, moveChannelNumber);
				}
			}
		}	
	}
}

void CCommandController::MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr != client && (false == client->IsRoomEnterState()))
	{
		// 룸을 만들고
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName);
		if (-1 != newRoomNumber)
		{
			// 룸에 들어가고
			EnterRoom(shared_clientInfo, newRoomNumber);
		}
	}
}

void CCommandController::OutRoom(const LinkPtr & shared_clientInfo)
{
	 RoomListIt roomIter = mRoomManager.ExitRoom(shared_clientInfo);	// 룸에서 나가기
	 if (true == (*roomIter)->IsRoomEmpty())			// 룸에 아무도 없나 확인
	 {
		 mRoomManager.EraseRoom(roomIter);			// 아무도 없으면 룸 삭제
	 }
}
void CCommandController::DeleteClientSocket(const LinkPtr & shared_clientInfo)
{

	int myChannelNum = shared_clientInfo.get()->GetMyChannelNum();
	int myRoomNum = shared_clientInfo.get()->GetMyRoomNum();
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
		if (mChannelManager.ExitChannel(shared_clientInfo))
		{
			cout << "channel에서 나갔습니다." << endl;
		}
	}
	else
	{
		// 방일때
		OutRoom(shared_clientInfo);
		cout << "room에서 나갑니다." << endl;
	}
}

void CCommandController::SendAllReadyGameNotice(const LinkPtr & shared_clientInfo)
{
	if (mRoomManager.IsAllReadyGame(shared_clientInfo))
	{
		// 룸메니저를 통해 방 멤버 함수 호출 할 것.
		mRoomManager.Broadcast(shared_clientInfo, Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "모든 플레이어가 준비 되었습니다."));
	}
	else
	{
		shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::ChattingMessage,ProtocolDetail::Message, ProtocolMessageTag::Text, "다른 모든 플레이어가 준비 되어야 합니다."));
	}
	
}

CCommandController * CCommandController::GetInstance()
{
	if (nullptr == CommandControllerStatic)
	{
		CommandControllerStatic = new CCommandController();
	}
	return CommandControllerStatic;
}



void CCommandController::CommandHandling(const LinkPtr& shared_clientInfo, Packet& packet)
{
	try
	{
		if (ProtocolDetail::EnterRoom == packet.InfoProtocolDetail) // 방에 입장
		{
			EnterRoom(shared_clientInfo, stoi(packet.InfoValue/*commandString.at(1)*/));
		}
		else if (ProtocolDetail::EnterChanel == packet.InfoProtocolDetail)
		{
			ChangeChannel(shared_clientInfo, stoi(packet.InfoValue));
		}
		else if (ProtocolDetail::MakeRoom == packet.InfoProtocolDetail)
		{
			//int battingMoney = stoi(commandString.at(2));
			MakeRoom(shared_clientInfo, packet.InfoValue);
		}
		else if (ProtocolDetail::OutRoom == packet.InfoProtocolDetail)
		{
			if (true == mChannelManager.EnterMyChannel(shared_clientInfo)) // 채널에 들어가기
			{
				OutRoom(shared_clientInfo);
			}
		}
		else if (ProtocolDetail::ReadyGame == packet.InfoProtocolDetail)
		{
			SendAllReadyGameNotice(shared_clientInfo);
		}
		else if(ProtocolDetail::Message == packet.InfoProtocolDetail)
		{
			if (shared_clientInfo.get()->IsRoomEnterState())
			{
				mRoomManager.Talk(shared_clientInfo, packet);
			}
			else
			{
				mChannelManager.Talk(shared_clientInfo, packet);
			}
		}
	}
	catch (const std::exception&)
	{
		int channelNum = 0;
		cout << "명령처리 오류" << endl;
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, shared_clientInfo);
	}
}

