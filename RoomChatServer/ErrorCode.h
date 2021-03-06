#pragma once

enum EnumErrorCode
{
	ERROR_VALUE_RETURN = -1,
	// 취소
	CANCLE = 4444,

	// recv관련 10~19 return
	SUCCES_RECV = 10,
	// recv 에러
	ERROR_RECV = 11,
	ERROR_NULL_LINK_RECV = 13,
	// enum으로 만들어라

	// send관련 20~29 return
	SUCCES_SEND = 20,
	// send 에러
	ERROR_SEND = 21,
	ERROR_NULL_LINK_SEND = 23, // link 없이 보내기 실패
	SUCCES_MULTI_SEND = 24,
	ERROR_MULTI_SEND = 25,

	///////////// 기타 관련 60~89 return////////////
	// 잘못 입력
	ERROR_WRONG_INPUT = 69,
	// 클라이언트 묻기 성공/
	SUCCES_ASKCLIENT = 70,
	// 클라이언트 통보 성공
	SUCCES_NOTIFICATION = 72,
	// 로그인 성공
	SUCCES_LOGIN = 74,
	// 로그인 실패
	ERROR_LOGIN = 75,
	OVERLAPID = 76,
	SUCCES_JOIN = 78,
	ERROR_JOIN = 79,
	SUCCES_MENUOUT = 80,
	ERROR_MENUOUT = 81,
	ERROR_GET_CHANNEL = 83,
	ERROR_GET_ROOM = 85,
	ERROR_ENTER_CHANNEL = 87,
	SUCCES_COMMAND = 88,
	ERROR_COMMAND = 89,
	ERROR_EXIT_CHANNEL = 91,
	ERROR_EXIT_ROOM = 93,
	ERROR_MAKE_ROOM = 95,
	ERROR_ENTER_ROOM = 97,
	ERROR_DELETE_SOCKET = 99,
	ERROR_SHARED_LINK_COUNT_ZORO = 101,
	ERROR_MONEY_FAIL = 103,
	SUCCES_COMMAND_MESSAGE = 104,
	ERROR_GACHAR = 105,
	SUCCES_GACHAR = 106,
	ERROR_SAVE_EXP = 111,
	ERROR_CURSER_SIZE = 121, // 커서 이동 크기 잘 못 가져옴
	ERROR_CIPHER_OUT_RANGE = 137, // 누적 자릿수 범위 초과
	ERROR_INT_TO_ALPHABET_OUT_RANGE = 139, // 숫자를 문자로 바꾸는 범위 초과
	ERROR_SET_EXP = 141,
	SUCCES_SET_EXP = 142, // set exp 성공
	ERROR_SAVE = 143, // save 에러
	ERROR_SHARED_ROOM_COUNT_ZORO = 147, // roomList에 
	ERROR_SHARED_CHANNEL_COUNT_ZORO = 153,
	ERROR_INIT_USER_GOODS = 155, // 유저 재화 초기화 실패
	ERROR_ADD_MONEY_MAX = 157, // 유저 머니 최대치라서 추가 불가
	ERROR_MINUS_MONEY_MIN = 159, // 유저 머니 최소치라서 감소 불가
	ERROR_READ_GOODS_TXT_INDEX_OUTOFLANGE = 161, // 재화 txt읽었지만 인덱스를 벗어남
	ERROR_INIT_GOODS_TRUE = 165, // 이미 재화 초기화가 되어있어서 더 이상 초기화할 수 없음.
	ERROR_SAVE_TXT_CAPACITY_CIPHER = 167, // 저장하려는 숫자 자릿수와 txt에 자릿수 공간 틀림
	ERROR_INT_TO_ALPHABET_OUT_RANGE_MONEY = 169, // 돈을 문자로 바꾸는 범위 초과
	ERROR_CALC_CIPHER_MINUS_NUMBER = 171, // 자릿수 계산에 음수 들어옴.
	ERROR_MAXCIPHER_OVER = 173, // 최대 자릿수 초과
	ERROR_VALID_NUMBER_OUT_OF_RANGE = 175, // 유효 숫자 범위 초과
	ERROR_SAVE_MONEY = 177, // 돈 저장 실패
	ERROR_INIT_GOODS = 179, // 재화 초기화 실패
	ERROR_ROOM_ENTRER_BATTING_MONEY = 181, // 룸에 들어가는데 배팅금액 부족
	ERROR_ROOM_NULL = 183, // 없는 방
	ERROR_MAKE_ROOM_BATTING_MONEY_SHORTAGE = 185, // 방 만드는데 내가 가진 돈 보다 배팅금액이 더 큼
	ERROR_INIT_MONEY = 187, // 돈 초기화 에러
	ERROR_MESSAGESTRUCT_INIT = 189, // 메세지 초기화 안됨.
	ERROR_GETROOMSOCKET_MYCLIENT_NULLPTR = 191, // 나를 제외한 room 소켓 가져오기 실패
	ERROR_GET_CHANNEL_SOCKET_MYCLIENT_NULLPTR = 193, // 나를 제외한 channel 소켓 가져오기 실패
	ERROR_MULTI_SEND_EMPTY = 195, // 멀티 샌드 보낼 사람 없음
	ERROR_BATTING_RESULT_ALONE = 197, // 배팅 결과 혼자임.
	ERROR_SAVE_MONEY_ZERO = 199, //  .txt저장 하는데 0원을 더하거나 빼려고함.
	//////////////////////////////////////////////
	// 예외처리가 안된 오류
	ERROR_EXCEPTION = 9876
};

enum EnumInfoCode
{
	//INFO_NEW_EVOLUTION = 1
};