#include "ChannelManager.h"
#include"ErrorHandler.h"
#include"CommandController.h"

void CChannelManager::PushChannel(const ChannelPtr& shared_newChannel)
{
	if (0 >= shared_newChannel.use_count())
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_CHANNEL_COUNT_ZORO);
		return;
	}
	//{
	// ä���� ���� �ǰų� ���� �����Ƿ� lock ���ʿ�� ����.
	//ScopeLock<MUTEX> MU(RAII_ChannelManagerMUTEX); // lock
	//Channels.push_back(newChannel);
	//}// unlock
	mChannels.push_back(shared_newChannel);
}

CChannelManager::CChannelManager()
{
	mChannels.reserve(ChannelAmount);
	for (int i = StartEnterChannelNum; i <= ChannelAmount; i++)
	{
		shared_ptr<CChannel> newChannel(new CChannel(i));
		PushChannel(newChannel);
	}
}

CChannel * CChannelManager::GetMyChannel(int ChannelNum)
{
	ChannelVecIt iterBegin = mChannels.begin();
	ChannelVecIt iterEnd = mChannels.end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		if ((*iterBegin)->GetChannelNum() == ChannelNum)
			return (*iterBegin).get();
	}
//	cout << ChannelNum << "�� ä���� �����ϴ�." << endl;
	ErrorHandStatic->ErrorHandler(ERROR_GET_CHANNEL);
	return nullptr;
}

CChannelManager::~CChannelManager()
{
//	cout << "ChannelManager �Ҹ��� ȣ��" << endl;
}

bool CChannelManager::MoveChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber)
{
	if (nullptr == shared_clientInfo.get())
		return false;
	CChannel* nextChannel = GetMyChannel(moveChannelNumber);	
	if (nullptr == nextChannel)
		return false;
	return nextChannel->PushClient(shared_clientInfo, moveChannelNumber);
}

bool CChannelManager::ExitChannel(const LinkPtr& shared_clientInfo)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return false;
	int curChannelNumber = client->GetMyChannelNum();
	CChannel* nextChannel = GetMyChannel(curChannelNumber);
	if (nullptr == nextChannel)
		return false;
	if (true == nextChannel->EraseClient(shared_clientInfo))
	{
		//client->SendnMine(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "�� ä�ο��� ���Խ��ϴ�."));
		return true;
	}	
	return false;
}

bool CChannelManager::EnterMyChannel(const LinkPtr & shared_clientInfo)
{
	if (nullptr == shared_clientInfo.get())
		return false;
	int myChannelNumber = shared_clientInfo.get()->GetMyChannelNum();
	CChannel* nextChannel = GetMyChannel(myChannelNumber);
	if (nullptr == nextChannel)
		return false;
	return nextChannel->PushClient(shared_clientInfo, myChannelNumber);
}

void CChannelManager::Talk(const LinkPtr & shared_clientInfo, const Packet & packet, int flags)
{
	CLink* client = shared_clientInfo.get();
	if (nullptr == client)
		return;
	CChannel* myChannel = GetMyChannel(client->GetMyChannelNum());
	if (nullptr != myChannel)
	{
		myChannel->Talk(shared_clientInfo, packet, flags);
	}
}

