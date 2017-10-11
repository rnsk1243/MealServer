#define _CRT_SECURE_NO_WARNINGS
#include "Lobby.h"
#include"ErrorHandler.h"
#include"ReadHandler.h"
#include"WriteHandler.h"
#include"GuestLink.h"
#include"Util.h"

CLobby::CLobby(int NextUserNum_) :mNextUserNum(NextUserNum_) {}

//void CLobby::AskedID(const CGuestLink * guest, Packet & packetID)
//{
//	guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "ID : "));
//	ListenerStatic->Recvn((guest->GetClientSocket()), packetID);
//}
//
//void CLobby::AskedPW(const CGuestLink * guest, Packet & packetPW)
//{
//	guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "PW : "));
//	ListenerStatic->Recvn(guest->GetClientSocket(), packetPW);
//}

int CLobby::Login(const CGuestLink * guest , vector<string>& tempUserInfo, string idpw)
{
	
	vector<string> idpwVector = ReadHandlerStatic->Parse(idpw, '/');
	try
	{
		if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 2, idpwVector.at(0), idpwVector.at(1)))
		{
			//cout << "�α��� ����" << endl;
			//guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "�α��� ���� �ϼ̽��ϴ�. ��ſ� ��ȭ �Ǽ���."));
			return ErrorHandStatic->ErrorHandler(SUCCES_LOGIN);
		}
		//guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "���̵� Ȥ�� ��й�ȣ�� Ʋ���ϴ�."));
		return ErrorHandStatic->ErrorHandler(ERROR_LOGIN);
	}
	catch (const std::exception&)
	{
		cout << "CLobby::Login Error" << endl;
<<<<<<< HEAD
=======
		return ErrorHandStatic->ErrorHandler(ERROR_LOGIN);
>>>>>>> origin/WOOHEE
	}

}
//
//int CLobby::JoinMember(const CGuestLink * guest, vector<string>& tempUserInfo)
//{
//	//Packet packetID, packetPW;
//	SetNextUserPKNumber();
//	string nextUserNum;
//	nextUserNum = IntToString(mNextUserNum);
//	// ID ����
//	AskedID(guest, packetID);
//	string id(packetID.InfoValue);
//	if (0 == id.compare("9"))
//		return ErrorHandStatic->ErrorHandler(CANCLE);
//
//	AskedPW(guest, packetPW);
//	string pw(packetPW.InfoValue);
//	if (0 == pw.compare("9"))
//		return ErrorHandStatic->ErrorHandler(CANCLE);
//
//	if (ReadHandlerStatic->Search(NameMemberInfoTxt.c_str(), tempUserInfo, 1, id))
//	{
//		cout << "id �ߺ� �Դϴ�." << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "id �ߺ� �Դϴ�."));
//		return ErrorHandStatic->ErrorHandler(OVERLAPID);
//	}
//
//	if (WriteHandlerStatic->Write(NameMemberInfoTxt.c_str(), 3, nextUserNum, id, pw) /*&& WriteHandlerStatic->write(NameMemberCardInfoTxt.c_str(), 2, nextUserNum, StartCardInventory)*/)
//	{
//		if (mNextUserNum > JoinMemberTxtCapacity)
//		{
//			WriteHandlerStatic->Write(NameMemberGoodsTxt.c_str(), 2, nextUserNum, StartGoodsInventory);
//		}
//		cout << "ȸ������ ����" << endl;
//		WriteHandlerStatic->WriteNextJoinUserNum(MakeNextJoinNumberTxt, mNextUserNum);
//		++mNextUserNum;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "ȸ������ ���� �߽��ϴ�. �α��� ���ּ���."));
//		return SUCCES_JOIN;
//	}
//	else
//	{
//		cout << "ȸ������ ����" << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "ȸ������ ���� �Դϴ�."));
//		return ErrorHandStatic->ErrorHandler(ERROR_JOIN);
//	}
//	return ErrorHandStatic->ErrorHandler(ERROR_EXCEPTION);
//}


//int CLobby::ChooseMenu(const char * message, const CGuestLink* guest)
//{
//	switch (message[0])
//	{
//	case '1':
//		cout << "�α���" << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "�α��� ȭ�� �Դϴ�. 9���Է� : ���"));
//		return 1;
//	case '2':
//		cout << "ȸ������" << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "ȸ������ ȭ�� �Դϴ�. 9���Է� : ���"));
//		return 2;
//	case '9':
//		cout << "���� �޴��� ���ư���" << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "ȯ���մϴ�. 1���Է� : �α��� / 2���Է� : ȸ������ / 9���Է� : ���"));
//		return 9;
//	default:
//		cout << "�� �� �Է�" << endl;
//		break;
//	}
//	return ErrorHandStatic->ErrorHandler(ERROR_MENUOUT);
//}

void CLobby::SetNextUserPKNumber()
{
	if (0 == mNextUserNum)
	{
		string strNextUserPKNumber = ReadHandlerStatic->GetNextUserNum(MakeNextJoinNumberTxt);
		mNextUserNum = stoi(strNextUserPKNumber);
	}
}

CLobby * CLobby::GetInstance(const int& NextUserNum)
{
	if (nullptr == LobbyStatic)
	{
		return new CLobby(NextUserNum);
	}
	return LobbyStatic;
}


int CLobby::ActionServiceLobby(const CGuestLink* guest, vector<string>& tempUserInfo, bool& isGuest)
{
	int resultLoginFunc = 0;
	//guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "ȯ���մϴ�. 1���Է� : �α��� / 2���Է� : ȸ������ / 9���Է� : ���"));
	
	Packet packet;
	ListenerStatic->Recvn(guest->GetClientSocket(), packet); //(guest->GetClientSocket(), strMessage);

	if (ProtocolDetail::FrontMenu == packet.InfoProtocolDetail)
	{
		switch (packet.InfoTagIndex)
		{
		case ProtocolFrontManuTag::LoginManu:
			resultLoginFunc = Login(guest, tempUserInfo, packet.InfoValue);
			if (SUCCES_LOGIN == resultLoginFunc)
			{
				isGuest = false;
				return SUCCES_LOGIN;
			}
			else if (CANCLE == resultLoginFunc)
			{
				return CANCLE;
			}
			else if (ERROR_LOGIN == resultLoginFunc)
			{
				return ERROR_LOGIN;
			}
			return ERROR_NULL_LINK_RECV;
		case ProtocolFrontManuTag::JoinManu:
			//return JoinMember(guest, tempUserInfo);
		case ProtocolFrontManuTag::GuestManu:
			tempUserInfo.reserve(2);
			tempUserInfo.push_back(GuestPKNumber);// guest pk number
			tempUserInfo.push_back(packet.InfoValue);// guest name(ID)e);
			isGuest = true;
			return SUCCES_LOGIN;
		case ProtocolFrontManuTag::CancleManu:
			return CANCLE;
		default:
			return ERROR_WRONG_INPUT;
		}
	}
	return ERROR_WRONG_INPUT;
	//int choose = ChooseMenu(packet.InfoValue, guest);
	//switch (packet.InfoProtocolDetail)
	//{
	//case 1:
	//	resultLoginFunc = Login(guest, tempUserInfo);
	//	if (SUCCES_LOGIN == resultLoginFunc)
	//	{
	//		return SUCCES_LOGIN;
	//	}
	//	else if (CANCLE == resultLoginFunc)
	//	{
	//		return CANCLE;
	//	}
	//	else if (ERROR_LOGIN == resultLoginFunc)
	//	{
	//		return ERROR_LOGIN;
	//	}
	//	return ERROR_NULL_LINK_RECV;
	//case 2:
	//	return JoinMember(guest, tempUserInfo);
	//case 9:
	//	return CANCLE;
	//default:
	//	return ERROR_WRONG_INPUT;
	//}
	//return ERROR_NULL_LINK_SEND;
}

//void CLobby::PushGuest(const GuestLinkPtr & newGuest)
//{
//	ScopeLock<MUTEX> lock(mRAII_GuestMUTEX);
//	mGuestList.push_back(newGuest);
//}
//
//void CLobby::EraseGuest(const GuestLinkPtr & targetGuest)
//{
//	ScopeLock<MUTEX> lock(mRAII_GuestMUTEX);
//	GuestListIt targetIter = find(mGuestList.begin(), mGuestList.end(), targetGuest);
//	if (mGuestList.end() != targetIter)
//	{
//		mGuestList.erase(targetIter);
//	}
//}
