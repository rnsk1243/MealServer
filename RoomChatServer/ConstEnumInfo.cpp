#include"ConstEnumInfo.h"
#include"Util.h"

Packet::Packet()
{
}

void Packet::AddName(string name)
{
	char resultTemp[BufSizeValue];
	resultTemp[0] = '\0'; // 첫번째 요소를 null문자로 해주어야 함.
	strcat_s(resultTemp, BufSizeValue, name.c_str());
	strcat_s(resultTemp, BufSizeValue, " : ");
	strcat_s(resultTemp, BufSizeValue, InfoValue);

	strcpy_s(InfoValue, BufSizeValue, resultTemp);
}
