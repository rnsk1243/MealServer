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
			//cout << "로그인 성공" << endl;
			//guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "로그인 성공 하셨습니다. 즐거운 대화 되세요."));
			return ErrorHandStatic->ErrorHandler(SUCCES_LOGIN);
		}
		//guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "아이디 혹은 비밀번호가 틀립니다."));
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
//	// ID 묻기
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
//		cout << "id 중복 입니다." << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "id 중복 입니다."));
//		return ErrorHandStatic->ErrorHandler(OVERLAPID);
//	}
//
//	if (WriteHandlerStatic->Write(NameMemberInfoTxt.c_str(), 3, nextUserNum, id, pw) /*&& WriteHandlerStatic->write(NameMemberCardInfoTxt.c_str(), 2, nextUserNum, StartCardInventory)*/)
//	{
//		if (mNextUserNum > JoinMemberTxtCapacity)
//		{
//			WriteHandlerStatic->Write(NameMemberGoodsTxt.c_str(), 2, nextUserNum, StartGoodsInventory);
//		}
//		cout << "회원가입 성공" << endl;
//		WriteHandlerStatic->WriteNextJoinUserNum(MakeNextJoinNumberTxt, mNextUserNum);
//		++mNextUserNum;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "회원가입 성공 했습니다. 로그인 해주세요."));
//		return SUCCES_JOIN;
//	}
//	else
//	{
//		cout << "회원가입 실패" << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "회원가입 실패 입니다."));
//		return ErrorHandStatic->ErrorHandler(ERROR_JOIN);
//	}
//	return ErrorHandStatic->ErrorHandler(ERROR_EXCEPTION);
//}


//int CLobby::ChooseMenu(const char * message, const CGuestLink* guest)
//{
//	switch (message[0])
//	{
//	case '1':
//		cout << "로그인" << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "로그인 화면 입니다. 9번입력 : 취소"));
//		return 1;
//	case '2':
//		cout << "회원가입" << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "회원가입 화면 입니다. 9번입력 : 취소"));
//		return 2;
//	case '9':
//		cout << "이전 메뉴로 돌아가기" << endl;
//		guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "환영합니다. 1번입력 : 로그인 / 2번입력 : 회원가입 / 9번입력 : 취소"));
//		return 9;
//	default:
//		cout << "잘 못 입력" << endl;
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
	//guest->Sendn(Packet(ProtocolInfo::ChattingMessage, ProtocolDetail::Message, ProtocolMessageTag::Text, "환영합니다. 1번입력 : 로그인 / 2번입력 : 회원가입 / 9번입력 : 취소"));
	
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
