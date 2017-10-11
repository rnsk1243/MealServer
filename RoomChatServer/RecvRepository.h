//#pragma once
//#include<queue>
//#include<memory>
//#include"ConstEnumInfo.h"
//#include"RAII.h"
//
//typedef shared_ptr<Packet> PacketPtr;
//
////typedef shared_ptr<OrderStructSocket> OrderStructSocketPtr;
////typedef shared_ptr<OrderStructLink> OrderStructLinkPtr;
//
//typedef queue<OrderStructSocketPtr> QueueStructOtherSocket;
//typedef queue<OrderStructLinkPtr> QueueStructOtherLink;
//
//class CRecvRepository
//{
//	QueueStructOtherSocket mQueueOtherSocket;
//	QueueStructOtherLink mQueueOtherLink;
//	MUTEX mRAII_RecvRepositoryMUTEX;
//	CRecvRepository();
//public:
//	static CRecvRepository* GetInstance();
//	CRecvRepository(const CRecvRepository&) = delete;
//	CRecvRepository& operator=(const CRecvRepository&) = delete;
//	~CRecvRepository();
//	void PushPacket(const OrderStructSocketPtr& packet);
//	void PopPacket(OrderStructSocketPtr& packet);
//
//	void PushPacket(const OrderStructLinkPtr& packet);
//	void PopPacket(OrderStructLinkPtr& packet);
//};
//static CRecvRepository* RecvRepositoryStatic = CRecvRepository::GetInstance();