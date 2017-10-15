#include "Room.h"
#include"ErrorHandler.h"
#include"ConstEnumInfo.h"
#include"Util.h"

CRoom::CRoom(int roomNum, int channelNum, const string& roomName, const ProtocolTeamAmount& teamAmount, const bool& isSpecialRoom, const string & roomPW) :
	mRoomNum(roomNum),
	mChannelNum(channelNum),
	mRoomName(roomName),
	mAmountPeople(0),
	//mBettingMoney(battingMoney),
	//mPlayingGame(false),
	mIsNewRoom(true),
	mEnterRoomPeopleLimit(teamAmount),
	mRoomPW(roomPW),
	mIsSpecialRoom(isSpecialRoom),
	mPlayingGame(false)
{
	mIsPublicRoom = (0 == RoomPWNone.compare(mRoomPW));
	cout << "mIsPublicRoom = " << mIsPublicRoom << endl;
	//InitializeCriticalSection(&CS_MyInfoList);
	//cout << "sizeof(ProtocolCharacterTagIndex) = " << sizeof(ProtocolCharacterTagIndex) << endl;
	mUsePosition.reserve(GetLimitEnterRoomPeople());
	mUsePosition.assign(mUsePosition.capacity(), NotUsed);
}


CRoom::~CRoom()
{
//	cout << mRoomNum << " �� ���� ���� �˴ϴ�." << endl;
}


LinkListIt CRoom::EraseClient(const LinkPtr& shared_client)
{
	LinkListIt delLinkIter = find(mClientInfos.begin(), mClientInfos.end(), shared_client);
	if (mClientInfos.end() != delLinkIter)
	{
		{
			ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
			delLinkIter = mClientInfos.erase(delLinkIter);
		}
		DecreasePeople();
		CLink* client = shared_client.get();


		client->SetMyRoomNum(NoneRoom);		
		if (ProtocolSceneName::ChannelScene == client->GetMySceneState())
		{
			client->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::SuccessRequest, State::ClientChannelMenu, nullptr));
		}
		else if (ProtocolSceneName::RoomScene == client->GetMySceneState())
		{
			if (NonePosition == client->GetMyPosition())
				return mClientInfos.end();
			mUsePosition[client->GetMyPosition()] = NotUsed; // �̻���� ǥ��
		}

		client->SetMySceneState(ProtocolSceneName::ChannelScene);
	}
	return delLinkIter;
}

int CRoom::GetRoomNum()
{
	return mRoomNum;
}

int CRoom::GetChannelNum()
{
	return mChannelNum;
}

const string CRoom::GetRoomName()
{
	return mRoomName;
}

int CRoom::GetAmountPeople()
{
	return mAmountPeople;
}

bool CRoom::IsRoomEmpty()
{
	if (0 >= mAmountPeople)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

//int CRoom::GetBattingMoney()
//{
//	return mBettingMoney;
//}

//bool CRoom::MergeRoom(CRoom * targetRoom)
//{
//	{
//		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX); // rock0
//		{
//			ScopeLock<MUTEX> MU(targetRoom->mRAII_RoomMUTEX); // rock1
//															  // ���� �ű�� ���� �غ��۾����� room���� ����
//			LinkListIt linkBegin = targetRoom->GetIterMyInfoBegin();
//			LinkListIt linkEnd = targetRoom->GetIterMyInfoEnd();
//			for (; linkBegin != linkEnd; ++linkBegin)
//			{
//				CLink* targetClient = (*linkBegin).get();
//				targetClient->SetMyRoomNum(mRoomNum);
//				IncreasePeople(); // �� �ο��� ����
//			}
//			mClientInfos.merge(targetRoom->mClientInfos); // ���� �ű�
//		} // rock1 unlock
//	} // rock0 unlock
//	return true;
//}

bool CRoom::IsAllReady()
{
	// �ο����� ���ڸ��� ������ false;
	if (GetLimitEnterRoomPeople() > mAmountPeople)
	{
		return false;
	}
	LinkListIt iterBegin = mClientInfos.begin();
	for (; iterBegin != mClientInfos.end(); ++iterBegin)
	{
		CLink* client = (*iterBegin).get();
		if (false == client->GetReadyGame())
		{			
			return false;
		}
	}
	iterBegin = mClientInfos.begin();
	for (; iterBegin != mClientInfos.end(); ++iterBegin)
	{
		(*iterBegin).get()->SetMySceneState(ProtocolSceneName::MainScene);
	}
	SetGame(true);
	return true;
}


//bool CRoom::AllCalculateMoney()
//{
//	LinkListIt linkBegin = mClientInfos.begin();
//	bool isSaveResult = true; // ��� .txt�� ���� �Ǿ���? �� ���̶� �ȵǸ� false ��ȯ
//	for (; linkBegin != mClientInfos.end(); ++linkBegin)
//	{
//		if (false == (*linkBegin).get()->SaveCalculateMoney())
//		{
//			ErrorHandStatic->ErrorHandler(ERROR_SAVE_MONEY, (*linkBegin));
//			isSaveResult = false;
//			continue;
//		}
//	}
//	return isSaveResult;
//}

bool CRoom::PushClient(const LinkPtr& shared_client, const int& enterRoomNumber)
{
	{
		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
		mClientInfos.push_back(shared_client);
	}
	shared_client.get()->SetMyRoomNum(enterRoomNumber);
	IncreasePeople();
	//cout << "�濡 ������ ���� �ο� = " << mAmountPeople << endl;
	shared_client.get()->SendnMine(Packet(ProtocolInfo::RequestResult, ProtocolDetail::SuccessRequest, State::ClientMatching, nullptr));

	if (mIsNewRoom && mAmountPeople >= GetLimitEnterRoomPeople())
	{
		ProtocolCharacterTagIndex tagIndex = ProtocolCharacterTagIndex::Red01;
		for (LinkListIt clientBegin = mClientInfos.begin(); clientBegin != mClientInfos.end(); ++clientBegin)
		{
			EnterBroadcast(*clientBegin, tagIndex);
			tagIndex = (ProtocolCharacterTagIndex)(tagIndex + 1);
		}
		for (LinkListIt clientBegin = mClientInfos.begin(); clientBegin != mClientInfos.end(); ++clientBegin)
		{
			NoticRoomIn(*clientBegin);
		}
		mIsNewRoom = false;
		return true;
	}
	else if (!mIsNewRoom && mAmountPeople <= GetLimitEnterRoomPeople())
	{
		NoticSoloEnterRoomIn(shared_client);
		return true;
	}
	return false;
}

bool CRoom::PushClientSpecialRoom(const LinkPtr & shared_client, const int & enterRoomNumber, const string & pw)
{
	if (GetLimitEnterRoomPeople() <= mAmountPeople || mPlayingGame)
	{
		return false;
	}
	if (0 != mRoomPW.compare(pw))
	{
		cout << "���� ��� Ʋ��" << endl;
		return false;
	}
	{
		ScopeLock<MUTEX> MU(mRAII_RoomMUTEX);
		mClientInfos.push_back(shared_client);
	}
	shared_client.get()->SetMyRoomNum(enterRoomNumber);
	IncreasePeople();
	//cout << "�濡 ������ ���� �ο� = " << mAmountPeople << endl;
	NoticSoloEnterRoomIn(shared_client);
	string message("[" + IntToString(mRoomNum) + "]�� �濡 ���� �ϼ̽��ϴ�.");
	Packet roomNumberPacket(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, message.c_str());
	shared_client.get()->SendnMine(roomNumberPacket);
	return true;
}

void CRoom::BackRoomScene()
{
	LinkListIt iterBegin = mClientInfos.begin();
	for (; iterBegin != mClientInfos.end(); ++iterBegin)
	{
		EnterBroadcast(*iterBegin, (*iterBegin).get()->GetMyPosition(), true); // ������ �濡 �ִ� ��(client)���� ��� ����鿡�� �˷���
	}
	for (LinkListIt clientBegin = mClientInfos.begin(); clientBegin != mClientInfos.end(); ++clientBegin)
	{
		NoticRoomIn(*clientBegin);
	}
	NotReadyTogether();
	SetGame(false);
}

void CRoom::NoticRoomIn(const LinkPtr & shared_client)
{
	shared_client.get()->SetMySceneState(ProtocolSceneName::RoomScene);
	string message(shared_client.get()->GetMyName() + "���� �濡 ���� �ϼ̽��ϴ�.");
	Packet welcomePacket(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, message.c_str());
	Talk(shared_client, welcomePacket);
}

// ���, ��ġ
void CRoom::EnterBroadcast(const LinkPtr& shared_client, ProtocolCharacterTagIndex tagIndex, const bool& isBackRoom)
{
	if (Used == mUsePosition[tagIndex] && isBackRoom == false)
	{
		ErrorHandStatic->ErrorHandler(ERROR_ENTER_ROOM, shared_client);
		return;
	}
	mUsePosition[tagIndex] = Used; // ����� ǥ��
	Packet packetName(ProtocolInfo::ClientCommend, ProtocolDetail::NameChange, tagIndex, shared_client.get()->GetMyName().c_str());
	Packet* packetImage = nullptr;
	if (isBackRoom)
	{
		packetImage = &Packet(ProtocolInfo::ClientCommend, ProtocolDetail::ImageChange, tagIndex, ProtocolCharacterImageName[shared_client.get()->GetMyCharacter()].c_str());
	}
	else
	{
		packetImage = &Packet(ProtocolInfo::ClientCommend, ProtocolDetail::ImageChange, tagIndex, ProtocolCharacterImageName[InitCharacter].c_str());
	}
	shared_client.get()->SetMyPosition(tagIndex);
	Talk(shared_client, packetName);
	Talk(shared_client, *packetImage);
	packetName.InfoProtocolDetail = ProtocolDetail::MyInfoName;
	packetImage->InfoProtocolDetail = ProtocolDetail::MyInfoImage;
	shared_client.get()->SendnMine(packetName);
	shared_client.get()->SendnMine(*packetImage);
}

void CRoom::TeachNewPeople(const LinkPtr & shared_client)
{
	LinkListIt peopleBegin = mClientInfos.begin();
	for (; peopleBegin != mClientInfos.end(); ++peopleBegin)
	{
		if (shared_client != (*peopleBegin))
		{
			ProtocolCharacterTagIndex tagIndex = (*peopleBegin).get()->GetMyPosition();
			Packet packetName(ProtocolInfo::ClientCommend, ProtocolDetail::NameChange, tagIndex, (*peopleBegin).get()->GetMyName().c_str());
			Packet packetImage(ProtocolInfo::ClientCommend, ProtocolDetail::ImageChange, tagIndex, ProtocolCharacterImageName[(*peopleBegin).get()->GetMyCharacter()].c_str());
			shared_client.get()->SendnMine(packetName);
			shared_client.get()->SendnMine(packetImage);
		}
	}
}

void CRoom::NoticSoloEnterRoomIn(const LinkPtr & shared_client)
{
	TeachNewPeople(shared_client); // �濡 �ִ� ������� ���Կ��� �˷���
	int index = ProtocolCharacterTagIndex::Red01;
	vector<int>::iterator useBegin = mUsePosition.begin();
	for (; useBegin != mUsePosition.end(); ++useBegin)
	{
		if (Used != (*useBegin)) // ������� �ƴϸ�
		{
			EnterBroadcast(shared_client, ProtocolCharacterTagIndex(index)); // ������ �濡 �ִ� ��(client)���� ��� ����鿡�� �˷���
			NoticRoomIn(shared_client);
			return;
		}
		++index; 
	}
}

void CRoom::ChangeCharacterBroadcast(const LinkPtr & shared_client, const ProtocolCharacterImageNameIndex& characterImageIndex)
{
	CLink* client = shared_client.get();
	if (client->GetReadyGame())
	{
		Packet packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "�غ� �����̹Ƿ� ĳ���͸� ������ �� �����ϴ�.");
		client->SendnMine(packet);
		return;
	}
	if (NonePosition == client->GetMyPosition())
		return;
	if (mUsePosition[client->GetMyPosition()] == Used)
	{
		Packet packet(ProtocolInfo::ClientCommend, ProtocolDetail::ImageChange, client->GetMyPosition(), ProtocolCharacterImageName[characterImageIndex].c_str());
		Talk(shared_client, packet);
		packet.InfoProtocolDetail = ProtocolDetail::MyInfoImage;
		shared_client.get()->SendnMine(packet);
		client->SetMyCharacter(characterImageIndex);
		client->SetNoReadyGame(State::ClientNotReady);
	}
	else
	{
		ErrorHandStatic->ErrorHandler(ERROR_ROOM_CHANGET_CHARACTER, shared_client);
		return;
	}
}

//bool CRoom::AllInitBetting()
//{
//	LinkListIt linkBegin = mClientInfos.begin();
//	for (; linkBegin != mClientInfos.end(); ++linkBegin)
//	{
//		(*linkBegin).get()->InitBetting();
//	}
//	SetGameOver();
//	return true;
//}

bool CRoom::IsGame()
{
	return mPlayingGame;
}

void CRoom::SetGame(bool isGame)
{
	mPlayingGame = isGame;
}

void CRoom::SendMyReadyInfo(const LinkPtr& myClient)
{
	//cout << "SendAllReadyInfo ȣ��" << endl;
	Packet* readyInfoPacket = nullptr;
	if (myClient.get()->GetReadyGame())
	{
		readyInfoPacket = &Packet(ProtocolInfo::ClientCommend, ProtocolDetail::ReadyInfo, myClient.get()->GetMyPosition(), "Ready");
	}
	else
	{
		readyInfoPacket = &Packet(ProtocolInfo::ClientCommend, ProtocolDetail::ReadyInfo, myClient.get()->GetMyPosition(), "NotReady");
	}
	Broadcast(*readyInfoPacket);
}

void CRoom::Broadcast(const Packet& packet, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		(*clientIterBegin).get()->SendnMine(packet, flags);
	}
}

void CRoom::Talk(const LinkPtr& myClient, const Packet & packet, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	LinkListIt myIter = find(mClientInfos.begin(), mClientInfos.end(), myClient);
	if (mClientInfos.end() == myIter)
	{
		Broadcast(packet, flags);
		return;
	}
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		if (clientIterBegin != myIter)
		{
			(*clientIterBegin).get()->SendnMine(packet, flags);
		}
	}
}

void CRoom::GetHostIP(const LinkPtr& requestClient)
{
	LinkListIt linkBegin = mClientInfos.begin();
	for (; linkBegin != mClientInfos.end(); ++linkBegin)
	{
		if ((*linkBegin).get()->GetMyPosition() == ProtocolCharacterTagIndex::Red01)
		{
			Packet packet(ProtocolInfo::ClientCommend, ProtocolDetail::GetHostIP, 0, (*linkBegin).get()->GetMyIP().c_str());
			//(*linkBegin).get()->SendnMine(packet);
			requestClient.get()->SendnMine(packet);
		}
	}
}

void CRoom::NotReadyTogether()
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		(*clientIterBegin).get()->SetNoReadyGame(State::ClientNotAllReady);
	}
}

int CRoom::GetLimitEnterRoomPeople()
{
	switch (mEnterRoomPeopleLimit)
	{
	case ProtocolTeamAmount::OneTeam:
		return 2; // 2
	case ProtocolTeamAmount::TwoTeam:
		return 4;
	case ProtocolTeamAmount::ThreeTeam:
		return 6;
	default:
		break;
	}
	return 0;
}

bool CRoom::IsSpecialRoom()
{
	return mIsSpecialRoom;
}

bool CRoom::IsPublicroom()
{
	return mIsPublicRoom;
}

void CRoom::IncreasePeople()
{
	mAmountPeople++;
}

void CRoom::DecreasePeople()
{
	if (mAmountPeople > 0) mAmountPeople--;
}

//void CRoom::SetGame()
//{
//	mPlayingGame = true;
//}
//
//void CRoom::SetGameOver()
//{
//	mPlayingGame = false;
//}
//
//void CRoom::SendBattingResult(const LinkPtr& winner, int flags)
//{
//	LinkListIt clientIterBegin = mClientInfos.begin();
//	LinkListIt winnerIter = find(mClientInfos.begin(), mClientInfos.end(), winner);
//	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
//	{
//		if (clientIterBegin != winnerIter)
//		{
//			(*clientIterBegin).get()->SendnMine("����� �����ϴ�.", flags);
//		}
//		else
//		{
//			(*clientIterBegin).get()->SendnMine("����� �̰���ϴ�.", flags);
//		}
//	}
//}
//
//
