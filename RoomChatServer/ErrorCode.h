#pragma once

enum EnumErrorCode
{
	ERROR_VALUE_RETURN = -1,
	// ���
	CANCLE = 4444,

	// recv���� 10~19 return
	SUCCES_RECV = 10,
	// recv ����
	ERROR_RECV = 11,
	ERROR_NULL_LINK_RECV = 13,
	// enum���� ������

	// send���� 20~29 return
	SUCCES_SEND = 20,
	// send ����
	ERROR_SEND = 21,
	ERROR_NULL_LINK_SEND = 23, // link ���� ������ ����
	SUCCES_MULTI_SEND = 24,
	ERROR_MULTI_SEND = 25,

	///////////// ��Ÿ ���� 60~89 return////////////
	// �߸� �Է�
	ERROR_WRONG_INPUT = 69,
	// Ŭ���̾�Ʈ ���� ����/
	SUCCES_ASKCLIENT = 70,
	// Ŭ���̾�Ʈ �뺸 ����
	SUCCES_NOTIFICATION = 72,
	// �α��� ����
	SUCCES_LOGIN = 74,
	// �α��� ����
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
	ERROR_CURSER_SIZE = 121, // Ŀ�� �̵� ũ�� �� �� ������
	ERROR_CIPHER_OUT_RANGE = 137, // ���� �ڸ��� ���� �ʰ�
	ERROR_INT_TO_ALPHABET_OUT_RANGE = 139, // ���ڸ� ���ڷ� �ٲٴ� ���� �ʰ�
	ERROR_SET_EXP = 141,
	SUCCES_SET_EXP = 142, // set exp ����
	ERROR_SAVE = 143, // save ����
	ERROR_SHARED_ROOM_COUNT_ZORO = 147, // roomList�� 
	ERROR_SHARED_CHANNEL_COUNT_ZORO = 153,
	ERROR_INIT_USER_GOODS = 155, // ���� ��ȭ �ʱ�ȭ ����
	ERROR_ADD_MONEY_MAX = 157, // ���� �Ӵ� �ִ�ġ�� �߰� �Ұ�
	ERROR_MINUS_MONEY_MIN = 159, // ���� �Ӵ� �ּ�ġ�� ���� �Ұ�
	ERROR_READ_GOODS_TXT_INDEX_OUTOFLANGE = 161, // ��ȭ txt�о����� �ε����� ���
	ERROR_INIT_GOODS_TRUE = 165, // �̹� ��ȭ �ʱ�ȭ�� �Ǿ��־ �� �̻� �ʱ�ȭ�� �� ����.
	ERROR_SAVE_TXT_CAPACITY_CIPHER = 167, // �����Ϸ��� ���� �ڸ����� txt�� �ڸ��� ���� Ʋ��
	ERROR_INT_TO_ALPHABET_OUT_RANGE_MONEY = 169, // ���� ���ڷ� �ٲٴ� ���� �ʰ�
	ERROR_CALC_CIPHER_MINUS_NUMBER = 171, // �ڸ��� ��꿡 ���� ����.
	ERROR_MAXCIPHER_OVER = 173, // �ִ� �ڸ��� �ʰ�
	ERROR_VALID_NUMBER_OUT_OF_RANGE = 175, // ��ȿ ���� ���� �ʰ�
	ERROR_SAVE_MONEY = 177, // �� ���� ����
	ERROR_INIT_GOODS = 179, // ��ȭ �ʱ�ȭ ����
	ERROR_ROOM_ENTRER_BATTING_MONEY = 181, // �뿡 ���µ� ���ñݾ� ����
	ERROR_ROOM_NULL = 183, // ���� ��
	ERROR_MAKE_ROOM_BATTING_MONEY_SHORTAGE = 185, // �� ����µ� ���� ���� �� ���� ���ñݾ��� �� ŭ
	ERROR_INIT_MONEY = 187, // �� �ʱ�ȭ ����
	ERROR_MESSAGESTRUCT_INIT = 189, // �޼��� �ʱ�ȭ �ȵ�.
	ERROR_GETROOMSOCKET_MYCLIENT_NULLPTR = 191, // ���� ������ room ���� �������� ����
	ERROR_GET_CHANNEL_SOCKET_MYCLIENT_NULLPTR = 193, // ���� ������ channel ���� �������� ����
	ERROR_MULTI_SEND_EMPTY = 195, // ��Ƽ ���� ���� ��� ����
	ERROR_BATTING_RESULT_ALONE = 197, // ���� ��� ȥ����.
	ERROR_SAVE_MONEY_ZERO = 199, //  .txt���� �ϴµ� 0���� ���ϰų� ��������.
	ERROR_ROOM_CHANGET_CHARACTER = 201, // �߸��� ĳ���� ���� ��û
	//////////////////////////////////////////////
	// ����ó���� �ȵ� ����
	ERROR_EXCEPTION = 9876
};

enum EnumInfoCode
{
	//INFO_NEW_EVOLUTION = 1
};