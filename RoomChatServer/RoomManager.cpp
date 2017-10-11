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
		// ä���� ������ Ȯ��
		if (ChannelNum == (*iterBegin)->GetChannelNum())
		{
			// ä���� ���� ���ȣ ���� ������ Ȯ��
			if (roomNum == (*iterBegin)->GetRoomNum())
			{
				return iterBegin;
			}
		}
	}
	//cout << roomNum << "�� ���� �����ϴ�." << endl;
	ErrorHandStatic->ErrorHandler(ERROR_GET_ROOM);
	//RoomListIt* error = nullptr;
	return iterBegin; // iterBegin == iterEnd �̸� ���̾���.
}


void CRoomManager::PushRoom(const RoomPtr & shared_newRoom)
{
	ScopeLock<MUTEX> MU(mRAII_RoomManagerMUTEX);
	mRooms.push_back(shared_newRoom);
	++newRoomNumber;
}

int CRoomManager::SearchRoom(bool& isSpecialRoom)
{
	RoomListIt roomBegin = mRooms.begin();
	for (; roomBegin != mRooms.end(); ++roomBegin)
	{
<<<<<<< HEAD
=======
		if (false == (*roomBegin).get()->IsPublicroom())
			continue;
>>>>>>> origin/WOOHEE
		if ((*roomBegin).get()->GetLimitEnterRoomPeople() > (*roomBegin).get()->GetAmountPeople())
		{
			isSpecialRoom = (*roomBegin).get()->IsSpecialRoom();
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
		//string outClientName(shared_clientInfo.get()->GetMyName() + " ���� �濡�� �����̽��ϴ�.");
		//(*myRoomIter).get()->Talk(shared_clientInfo, Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, outClientName.c_str()));
		//client->InitBetting(); // ���� �ʱ�ȭ ��Ŵ
		//if (true == (*myRoomIter)->IsGame())					// �����߿� ������?
		//{
		//	client->LostWillMoney((*myRoomIter)->GetBattingMoney());	// ���� �ΰ�
		//	client->SaveCalculateMoney(); // ���� ������ ������..�ؾ���
		//	(*myRoomIter)->AllInitBetting();					// �뿡 ����ִ� ��� �غ� �ʱ�ȭ
		//}

		(*myRoomIter).get()->EraseClient(shared_clientInfo);
		isSucces = true;
		return myRoomIter;
	}
	isSucces = false;
	return mRooms.end();
}

<<<<<<< HEAD
int CRoomManager::MakeRoom(const LinkPtr & shared_clientInfo, const string& roomName, const ProtocolTeamAmount& teamAmount, const string & roomPW)
{
	CLink* client = shared_clientInfo.get();
	RoomPtr newRoom(new CRoom(newRoomNumber, client->GetMyChannelNum(), roomName, teamAmount, roomPW));
=======
int CRoomManager::MakeRoom(const LinkPtr & shared_clientInfo, const string& roomName, const ProtocolTeamAmount& teamAmount, const bool& isSpecialRoom, const string & roomPW)
{
	CLink* client = shared_clientInfo.get();
	RoomPtr newRoom(new CRoom(newRoomNumber, client->GetMyChannelNum(), roomName, teamAmount,isSpecialRoom, roomPW));
>>>>>>> origin/WOOHEE
	int makedRoomNumber = newRoomNumber; // PushRoom�� ȣ���ϸ� newRoomNumber�� 1 �����ϱ⶧����
	PushRoom(newRoom);
	return makedRoomNumber;

	//if (client->GetMyMoney() >= battingMoney && battingMoney >= 0)
	//{
	//	RoomPtr newRoom(new CRoom(newRoomNumber, client->GetMyChannelNum(), roomName, battingMoney));
	//	int makedRoomNumber = newRoomNumber; // PushRoom�� ȣ���ϸ� newRoomNumber�� 1 �����ϱ⶧����
	//	PushRoom(newRoom);
	//	return makedRoomNumber;
	//}
	//else
	//{
	//	client->SendnMine("���ÿ� �ʿ��� ���� �����Ͽ� ���� ���� �� �����ϴ�.");
	//	ErrorHandStatic->ErrorHandler(ERROR_ROOM_ENTRER_BATTING_MONEY);
	//	return -1;
	//}
}

bool CRoomManager::EnterRoom(const LinkPtr& shared_clientInfo, int targetRoomNumBer)
{
	CLink* client = shared_clientInfo.get(); 
	if (nullptr == client)
		return false;
	if (true == client->IsRoomEnterState()) // �̹� �濡 �ִ��� Ȯ��
	{
		//cout << "�̹� �濡 �ִµ�" << endl;
		return false;
	}
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), targetRoomNumBer);
	
	if (mRooms.end() != targetRoomIter)
	{
		if ((*targetRoomIter).get()->GetLimitEnterRoomPeople() <= (*targetRoomIter).get()->GetAmountPeople())
		{
			shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientChannelMenu, nullptr));
			//cout << "�� �� ���� �� ��" << endl;
			return false;
		}
		(*targetRoomIter)->PushClient(shared_clientInfo, targetRoomNumBer);
		return true;
	}
	shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientChannelMenu, nullptr));
	// ���� ���̶� �� ��
	return false;
}

bool CRoomManager::EnterRoomSpecial(const LinkPtr & shared_clientInfo, int targetRoomNumBer, const string & pw)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return false;
	if (true == client->IsRoomEnterState()) // �̹� �濡 �ִ��� Ȯ��
	{
		//cout << "�̹� �濡 �ִµ�" << endl;
		return false;
	}
	RoomListIt targetRoomIter = GetMyRoomIter(client->GetMyChannelNum(), targetRoomNumBer);

	if (mRooms.end() != targetRoomIter)
	{
		if ((*targetRoomIter).get()->GetLimitEnterRoomPeople() <= (*targetRoomIter).get()->GetAmountPeople())
		{
			//shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientChannelMenu, nullptr));
			shared_clientInfo.get()->SetMySceneState(ProtocolSceneName::ChannelScene);
			//cout << "�� �� ���� �� ��" << endl;
			return false;
		}
		if ((*targetRoomIter)->PushClientSpecialRoom(shared_clientInfo, targetRoomNumBer, pw))
		{
			return true;
		}
		//shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientChannelMenu, nullptr));
		shared_clientInfo.get()->SetMySceneState(ProtocolSceneName::ChannelScene);
		return false; // ����� Ʋ��
	}
	//shared_clientInfo.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::FailRequest, State::ClientChannelMenu, nullptr));
	shared_clientInfo.get()->SetMySceneState(ProtocolSceneName::ChannelScene);
	// ���� ���̶� �� ��
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



