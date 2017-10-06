#include "RoomManager.h"
#include"ErrorHandler.h"

CRoomManager::CRoomManager():newRoomNumber(StartRoomNum)
{
}


CRoomManager::~CRoomManager()
{
}

RoomListIt CRoomManager::GetMyRoomIter(int ChannelNum, int roomNum)
{
	RoomListIt iterBegin = mRooms.begin();
	RoomListIt iterEnd = mRooms.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		// 채널이 같은지 확인
		if (ChannelNum == (*iterBegin)->GetChannelNum())
		{
			// 채널이 같고 룸번호 까지 같은지 확인
			if (roomNum == (*iterBegin)->GetRoomNum())
			{
				return iterBegin;
			}
		}
	}
	//cout << roomNum << "번 방이 없습니다." << endl;
	ErrorHandStatic->ErrorHandler(ERROR_GET_ROOM);
	//RoomListIt* error = nullptr;
	return iterBegin; // iterBegin == iterEnd 이면 방이없다.
}


void CRoomManager::PushRoom(const RoomPtr & shared_newRoom)
{
	ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
	mRooms.push_back(shared_newRoom);
	++newRoomNumber;
}

int CRoomManager::SearchRoom()
{
	RoomListIt roomBegin = mRooms.begin();
	for (; roomBegin != mRooms.end(); ++roomBegin)
	{
		if (EnterRoomPeopleLimit > (*roomBegin).get()->GetAmountPeople())
		{
			return (*roomBegin).get()->GetRoomNum();
		}
	}
	return NoneRoom;
}

void CRoomManager::ChangeMyCharacter(const LinkPtr & shared_clientInfo, const Packet & packet)
{
	CLink* client = shared_clientInfo.get();
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		(*myRoomIter).get()->ChangeCharacterBroadcast(shared_clientInfo, ProtocolCharacterImageNameIndex(packet.InfoTagIndex));
	}
}

void CRoomManager::GetHostIP(const LinkPtr & shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		(*myRoomIter).get()->GetHostIP();
	}
}

RoomListIt CRoomManager::EraseRoom(RoomListIt deleteTargetRoomIter)
{
	ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
	RoomListIt delRoom = mRooms.erase(deleteTargetRoomIter);
	return delRoom;
}


RoomListIt CRoomManager::ExitRoom(const LinkPtr & shared_clientInfo, bool & isSucces)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return mRooms.end();
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		//string outClientName(shared_clientInfo.get()->GetMyName() + " 님이 방에서 나가셨습니다.");
		//(*myRoomIter).get()->Talk(shared_clientInfo, Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, outClientName.c_str()));
		//client->InitBetting(); // 베팅 초기화 시킴
		//if (true == (*myRoomIter)->IsGame())					// 게임중에 나갔나?
		//{
		//	client->LostWillMoney((*myRoomIter)->GetBattingMoney());	// 벌금 부과
		//	client->SaveCalculateMoney(); // 갈땐 가더라도 정산은..해야지
		//	(*myRoomIter)->AllInitBetting();					// 룸에 들어있는 사람 준비 초기화
		//}
		(*myRoomIter).get()->EraseClient(shared_clientInfo);
		isSucces = true;
		return myRoomIter;
	}
	isSucces = false;
	return mRooms.end();
}

int CRoomManager::MakeRoom(const LinkPtr & shared_clientInfo, const string& roomName)
{
	CLink* client = shared_clientInfo.get();
	RoomPtr newRoom(new CRoom(newRoomNumber, client->GetMyChannelNum(), roomName, 0));
	int makedRoomNumber = newRoomNumber; // PushRoom을 호출하면 newRoomNumber가 1 증가하기때문에
	PushRoom(newRoom);
	return makedRoomNumber;

	//if (client->GetMyMoney() >= battingMoney && battingMoney >= 0)
	//{
	//	RoomPtr newRoom(new CRoom(newRoomNumber, client->GetMyChannelNum(), roomName, battingMoney));
	//	int makedRoomNumber = newRoomNumber; // PushRoom을 호출하면 newRoomNumber가 1 증가하기때문에
	//	PushRoom(newRoom);
	//	return makedRoomNumber;
	//}
	//else
	//{
	//	client->SendnMine("베팅에 필요한 돈이 부족하여 방을 만들 수 없습니다.");
	//	ErrorHandStatic->ErrorHandler(ERROR_ROOM_ENTRER_BATTING_MONEY);
	//	return -1;
	//}
}

bool CRoomManager::EnterRoom(const LinkPtr & shared_clientInfo, int targetRoomNumBer)
{
	CLink* client = shared_clientInfo.get(); 
	if (nullptr == client)
		return false;
	if (true == client->IsRoomEnterState()) // 이미 방에 있는지 확인
	{
		//cout << "이미 방에 있는데" << endl;
		return false;
	}
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), targetRoomNumBer);
	
	if (mRooms.end() != targetRoomIter)
	{
		if (EnterRoomPeopleLimit <= (*targetRoomIter).get()->GetAmountPeople())
		{
			shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientChannelMenu, nullptr));
			//cout << "방 꽉 차서 못 들어감" << endl;
			return false;
		}
		(*targetRoomIter)->PushClient(shared_clientInfo, targetRoomNumBer);
		return true;
	}
	shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientChannelMenu, nullptr));
	return false;
}

bool CRoomManager::IsAllReadyGame(const LinkPtr & shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return false;
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != targetRoomIter)
	{
		//client->SetReadyGame((*targetRoomIter).get()->GetBattingMoney());
		return ((*targetRoomIter)->IsAllReady());
	}
	return false;
}

void CRoomManager::Broadcast(const LinkPtr & shared_clientInfo, const Packet & packet, int flags)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		(*myRoomIter).get()->Broadcast(packet, flags);
	}
}

void CRoomManager::Talk(const LinkPtr & shared_clientInfo, const Packet & packet, int flags)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	RoomListIt myRoomIter = GetMyRoomIter(client->GetMyChannelNum(), client->GetMyRoomNum());
	if (mRooms.end() != myRoomIter)
	{
		(*myRoomIter).get()->Talk(shared_clientInfo, packet, flags);
	}
}



