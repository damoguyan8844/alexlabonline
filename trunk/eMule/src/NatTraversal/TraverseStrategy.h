#pragma once

/** @file TraverseStrategy.h @brief ��͸�������
 <pre>
 *	Copyright (c) 2007��Emule
 *	All rights reserved.
 *
 *	��ǰ�汾��
 *	��    �ߣ�yunchenn.chen
 *	������ڣ�2007-01-10
 *
 *	ȡ���汾��none
 *	��    �ߣ�none
 *	������ڣ�none
 </pre>*/

class CTraverseStrategy
{
public:
	CTraverseStrategy(const uchar * pUserhash, CTraverseStrategy * pNext);
	virtual ~CTraverseStrategy(void);

	virtual bool Initialize() { return false; }
	virtual void SendPacket() { Failed(); }

	//  return true if the strategy process the packet
	virtual bool ProcessPacket(const uchar * /*data*/, int /*len*/, DWORD /*ip*/, WORD /*port*/) { return false; }

	CTraverseStrategy * GetNextStrategy() const
	{
		return m_pNext;
	}
	const uchar * GetUserHash() const
	{
		return m_UserHash;
	}
	void SetNextStrategy(CTraverseStrategy * pNext)
	{
		m_pNext = pNext;
	}
	bool IsFailed () const
	{
		return m_bFailed;
	}
	bool IsFinish() const
	{
		return m_bFinish;
	}
	void Failed()
	{
		m_bFailed = true;
	}
	void Finish()
	{
		m_bFinish = true;
	}
protected:
	CTraverseStrategy * m_pNext;
	uchar m_UserHash[16];
	bool m_bFailed, m_bFinish;
};

class CTraverseFactory
{
public:
	virtual CTraverseStrategy * CreateStrategy(const uchar * pUserhash) =0;
};

//  ��͸˳��: server -> buddy
class CTraverseBySvrFac : public CTraverseFactory
{
public:
	virtual CTraverseStrategy * CreateStrategy(const uchar * pUserhash);
};

class CTraverseBySEFac : public CTraverseFactory
{
public:
	virtual CTraverseStrategy * CreateStrategy(const uchar * pUserhash);
};


