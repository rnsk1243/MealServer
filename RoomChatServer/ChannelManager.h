#pragma once
#include<vector>
#include<iostream>
#include"Channel.h"

using namespace std;
class CChannel;
class CLink;
class CErrorHandler;


typedef shared_ptr<CChannel> ChannelPtr;
typedef vector<ChannelPtr> ChannelVector;
typedef ChannelVector::iterator ChannelVecIt;

class CChannelManager
{
	ChannelVector mChannels;
	MUTEX mRAII_ChannelManagerMUTEX;
	//CRITICAL_SECTION CS_Channel;
	void PushChannel(const ChannelPtr& shared_newChannel);
	CChannel * GetMyChannel(int ChannelNum);
public:
	CChannelManager();
	~CChannelManager();
	CChannelManager& operator=(const CChannelManager&) = delete;
	CChannelManager(const CChannelManager&) = delete;
	// ä�οű� �Լ�(�ű�� ��ũ, �ű����� ä�� ��ȣ)
	bool MoveChannel(const LinkPtr& shared_clientInfo, const int & moveChannelNumber);
	// ä�� ���� ������ (���������ϴ� ����)
	bool ExitChannel(const LinkPtr& shared_clientInfo);
	// ���� �ִ� ä�η� ���ư���
	bool EnterMyChannel(const LinkPtr& shared_clientInfo);
	// �ش� ��ũ�� ����ִ� ����
	void Talk(const LinkPtr& shared_clientInfo, const string& message, int flags = 0);
};

