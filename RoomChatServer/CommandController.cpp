#include "CommandController.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
class CErrorHandler;

CCommandController::CCommandController()
{
}


CCommandController::~CCommandController()
{
	//cout << "CNetWork 객체 소멸자 호출" << endl;
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

void CCommandController::EnterRoom(const LinkPtr & shared_clientInfo)
{
	bool isSpecialRoom = false;
	int roomNumber = AutoMakeORSearchRoom(shared_clientInfo, isSpecialRoom);

	if (isSpecialRoom)
	{
<<<<<<< HEAD
		//cout << "room 새로 만듬" << endl;
		enterRoomNumber = MakeRoom(shared_clientInfo, RoomInitName, EnterAutoMatchingRoomPeopleLimit);
=======
		EnterSpecialRoomFunc(shared_clientInfo, roomNumber);
>>>>>>> origin/WOOHEE
	}
	else
	{
		//cout << enterRoomNumber << " 번 방으로 들어가기" << endl;
		if (mRoomManager.EnterRoom(shared_clientInfo, roomNumber))
		{
			mChannelManager.ExitChannel(shared_clientInfo);
			return;
		}
	}

}

void CCommandController::EnterSpecialRoomFunc(const LinkPtr& shared_clientInfo, const int & roomNumber, const string & pw)
{
	if (NoneRoom != roomNumber)
	{
		//cout << enterRoomNumber << " 번 방으로 들어가기" << endl
		if (mRoomManager.EnterRoomSpecial(shared_clientInfo, roomNumber, pw))
		{
			mChannelManager.ExitChannel(shared_clientInfo);
		}
	}
}

void CCommandController::EnterSpecialRoomFunc(const LinkPtr& shared_clientInfo, const int & roomNumber, const string & pw)
{
	if (NoneRoom != roomNumber)
	{
		//cout << enterRoomNumber << " 번 방으로 들어가기" << endl
		if (mRoomManager.EnterRoomSpecial(shared_clientInfo, roomNumber, pw))
		{
			mChannelManager.ExitChannel(shared_clientInfo);
		}
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

<<<<<<< HEAD
int CCommandController::MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName, const ProtocolTeamAmount& teamAmount, const string & roomPW)
=======
int CCommandController::MakeRoom(const LinkPtr & shared_clientInfo, const string & roomName, const ProtocolTeamAmount& teamAmount, const bool & isSpecialRoom, const string & roomPW)
>>>>>>> origin/WOOHEE
{
	CLink* client = shared_clientInfo.get();
	if (nullptr != client && (false == client->IsRoomEnterState()))
	{
		// 룸을 만들고
<<<<<<< HEAD
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName, teamAmount, roomPW);
=======
		int newRoomNumber = mRoomManager.MakeRoom(shared_clientInfo, roomName, teamAmount, isSpecialRoom, roomPW);
>>>>>>> origin/WOOHEE
		if (NoneRoom != newRoomNumber)
		{
			return newRoomNumber;
		}
	}
	shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientMakeRoom, nullptr)); // makeRoom 기본 상태로 돌아가라
	return NoneRoom;
}

void CCommandController::OutRoom(const LinkPtr & shared_clientInfo)
{
	//cout << "outRoom 호출" << endl;
	ProtocolCharacterTagIndex targetPos = shared_clientInfo.get()->GetMyPosition();
	ProtocolSceneName oldSceneState = shared_clientInfo.get()->GetMySceneState();
	RoomListIt roomIter;
	bool isSucces = false;
	{
		// 프로그램을 한번에 죽이면 스레드가 꼬이는지.. 반복자를 잘못 가져오게되어 프로그램 죽음 .. 어쩔 수 없이 락걸어버림.
		ScopeLock<MUTEX> MU(mRAII_CommandMUTEX);
		roomIter = mRoomManager.ExitRoom(shared_clientInfo, isSucces);	// 룸에서 나가기
	}
	if (!isSucces)
		return;
	try
	{
		if (true == (*roomIter).get()->IsRoomEmpty())							// 룸에 아무도 없나 확인
		{
			mRoomManager.EraseRoom(roomIter);								// 아무도 없으면 룸 삭제
		}
		else
		{
			if (ProtocolSceneName::RoomScene == oldSceneState)	// RoomScene에서 나갔으면 모두에게 나갔음을 알리고 패널에서 자신을 지우게 시킴
			{
				cout << "나간 사실을 모두에게 알림" << endl;
				Packet packet(ProtocolInfo::ClientCommend, ProtocolDetail::RemovePanel, targetPos, nullptr);
				(*roomIter)->Broadcast(packet);
				(*roomIter)->NotReadyTogether(); // 누군가 한사람이라도 나가면 Ready 풀림
			}
		}
	}
	catch (const std::exception&)
	{
		return;
	}
}
void CCommandController::DeleteClientSocket(const LinkPtr & shared_clientInfo)
{
	int myChannelNum = shared_clientInfo.get()->GetMyChannelNum();
	int myRoomNum = shared_clientInfo.get()->GetMyRoomNum();
	//cout << "DeleteClientSocket여기 myRoomNum = " << myRoomNum << endl;
	//방에 있나 채널에 있나 확인
	if (NoneRoom == myRoomNum)
	{
		// 채널일때
		if (mChannelManager.ExitChannel(shared_clientInfo))
		{
			//cout << "channel에서 나갔습니다." << endl;
		}
	}
	else
	{
		// 방일때
		OutRoom(shared_clientInfo);
		//cout << "room에서 나갑니다." << endl;
	}
}

void CCommandController::SendAllReadyGameNotice(const LinkPtr & shared_clientInfo)
{
	if (mRoomManager.IsAllReadyGame(shared_clientInfo))
	{
		cout << "모두 레디 함" << endl;
		// 룸메니저를 통해 방 멤버 함수 호출 할 것.
		//mRoomManager.Broadcast(shared_clientInfo, Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "모든 플레이어가 준비 되었습니다."));
		//mRoomManager.Broadcast(shared_clientInfo, Packet(ProtocolInfo::SceneChange, ProtocolDetail::NoneDetail, ProtocolSceneName::MainScene, nullptr));
	}
}

void CCommandController::ChangeCharacter(const LinkPtr & shared_clientInfo, Packet& packet)
{
	mRoomManager.ChangeMyCharacter(shared_clientInfo, packet);
}

void CCommandController::GetHostIP(const LinkPtr & shared_clientInfo)
{
	mRoomManager.GetHostIP(shared_clientInfo);
}

int CCommandController::AutoMakeORSearchRoom(const LinkPtr & shared_clientInfo, bool & isSpecialRoom)
{
	int enterRoomNumber = mRoomManager.SearchRoom(isSpecialRoom);
	// 빈방이 없는 경우 만듬
	if (NoneRoom == enterRoomNumber)
	{
		//cout << "room 새로 만듬" << endl;
		enterRoomNumber = MakeRoom(shared_clientInfo, RoomInitName, EnterAutoMatchingRoomPeopleLimit, false); 
		isSpecialRoom = false;
	}
	return enterRoomNumber;
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
			//cout << "입장 명령 시작" << endl;
			EnterRoom(shared_clientInfo); // 자동매칭 입장
		}
		else if (ProtocolDetail::EnterSpecialRoom == packet.InfoProtocolDetail)
		{
			vector<string> roomNumberPWVector = ReadHandlerStatic->Parse(packet.InfoValue, '/');
			EnterSpecialRoomFunc(shared_clientInfo, stoi(roomNumberPWVector.at(0)), roomNumberPWVector.at(1));
		}
		else if (ProtocolDetail::EnterChanel == packet.InfoProtocolDetail)
		{
			ChangeChannel(shared_clientInfo, stoi(packet.InfoValue));
		}
		else if (ProtocolDetail::MakeRoom == packet.InfoProtocolDetail)
		{
<<<<<<< HEAD
			int roomNumber = MakeRoom(shared_clientInfo, RoomInitName, (ProtocolTeamAmount)packet.InfoTagIndex, packet.InfoValue/*pw*/);
=======
			int roomNumber = MakeRoom(shared_clientInfo, RoomInitName, (ProtocolTeamAmount)packet.InfoTagIndex, true, packet.InfoValue/*pw*/);
>>>>>>> origin/WOOHEE
			EnterSpecialRoomFunc(shared_clientInfo, roomNumber, packet.InfoValue);
			//int battingMoney = stoi(commandString.at(2));
			//MakeRoom(shared_clientInfo, packet.InfoValue);
		}
		else if (ProtocolDetail::OutRoom == packet.InfoProtocolDetail)
		{
			if (true == mChannelManager.EnterMyChannel(shared_clientInfo)) // 채널에 들어가기
			{
				OutRoom(shared_clientInfo);
			}
		}
		else if (ProtocolDetail::SetReadyGame == packet.InfoProtocolDetail)
		{
			shared_clientInfo.get()->SetReadyGame();
			SendAllReadyGameNotice(shared_clientInfo);
		}
		else if (ProtocolDetail::NotReadyGame == packet.InfoProtocolDetail)
		{
			shared_clientInfo.get()->SetNoReadyGame();
		}
		else if (ProtocolDetail::ChangeCharacter == packet.InfoProtocolDetail)
		{
			ChangeCharacter(shared_clientInfo, packet);
		}
		else if (ProtocolDetail::GetHostIP == packet.InfoProtocolDetail)
		{
			GetHostIP(shared_clientInfo);
		}
	}
	catch (const std::exception&)
	{
		int channelNum = 0;
		//cout << "명령처리 오류" << endl;
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, shared_clientInfo);
	}
}

void CCommandController::ChattingMessage(const LinkPtr & shared_clientInfo, Packet & packet)
{
	if (ProtocolDetail::Message != packet.InfoProtocolDetail || ProtocolMessageTag::Text != packet.InfoTagIndex)
	{
		return;
	}

	try
	{
		packet.AddName(shared_clientInfo.get()->GetMyName());
		if (shared_clientInfo.get()->IsRoomEnterState())
		{
			mRoomManager.Talk(shared_clientInfo, packet);
		}
		else
		{
			//mChannelManager.Talk(shared_clientInfo, packet);
		}
	}
	catch (const std::exception&)
	{
		int channelNum = 0;
		//cout << "명령처리 오류" << endl;
		ErrorHandStatic->ErrorHandler(ERROR_COMMAND, shared_clientInfo);
	}
}

