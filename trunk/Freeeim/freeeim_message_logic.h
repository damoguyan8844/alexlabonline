#ifndef _FreeEIM_Message_Logic
#define _FreeEIM_Message_Logic

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ������Ϣ
#define FREEEIM_NONE		0x00
#define FREEEIM_TEXT		0x01


class CEIM02Dlg;
class FreeEIM_Message_Logic
{

public:
	CEIM02Dlg *m_pMainTop;

	// ����ֻ�ǰ���Ϣ������У�������Ҫ�Ŷӣ�������������Ϣ�Ƿ�ɹ�����
	void PostTo(const char *szUser);
	unsigned int m_nType; // Message type.
	char *m_szData; // Message buffer.

	FreeEIM_Message_Logic();
	~FreeEIM_Message_Logic();
private:
protected:
};

#endif // _FreeEIM_Message_Logic