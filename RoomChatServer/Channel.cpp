#include "Channel.h"
#include"ErrorHandler.h"
#include"Util.h"

CChannel::CChannel(int channelNum):
	mChannelNum(channelNum),
	mPeopleAmount(0)
{
	//InitializeCriticalSection(&CS_MyInfoList);
}

CChannel::~CChannel()
{
//	cout << "ä�� ����" << endl;
}

int CChannel::GetChannelNum()
{
	return mChannelNum;
}

bool CChannel::PushClient(const LinkPtr& shared_client, const int& channelNumber)
{
	if (nullptr == shared_client.get())
	{
		ErrorHandStatic->ErrorHandler(ERROR_SHARED_LINK_COUNT_ZORO);
		return false;
	}
	if (shared_client.get()->GetMyChannelNum() == NoneRoom)
	{
		return false;
	}
	ScopeLock<MUTEX> MU(mRAII_ChannelMUTEX);
	mClientInfos.push_back(shared_client);
	shared_client.get()->SetMyChannelNum(channelNumber);
	mPeopleAmount++;
	//string channelNum(IntToString(mChannelNum));
	//shared_client.get()->SendnMine(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, (channelNum + "�� ä�ο� ���Խ��ϴ�.").c_str()));

	//Packet p1(ProtocolInfo::PlayerInfo, ProtocolDetail::Image, ProtocolCharacterTagIndex::Red01, CharacterImageName[0].c_str());
	//shared_client.get()->SendnMine((p1));

	return true;
}

bool CChannel::EraseClient(const LinkPtr& shared_clientInfo)
{
	LinkListIt eraseClientIter = find(mClientInfos.begin(), mClientInfos.end(), shared_clientInfo);
	if (mClientInfos.end() == eraseClientIter)
	{
		return false;
	}
	{
		ScopeLock<MUTEX> MU(mRAII_ChannelMUTEX);
		eraseClientIter = mClientInfos.erase(eraseClientIter);
		if (0 < mPeopleAmount)
		{
			--mPeopleAmount;
		}
	}
	return true;
}

void CChannel::Broadcast(const Packet & packet, int flags)
{
	LinkListIt clientIterBegin = mClientInfos.begin();
	for (; clientIterBegin != mClientInfos.end(); ++clientIterBegin)
	{
		(*clientIterBegin).get()->SendnMine(packet, flags);
	}
}

void CChannel::Talk(const LinkPtr & myClient, const Packet & packet, int flags)
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