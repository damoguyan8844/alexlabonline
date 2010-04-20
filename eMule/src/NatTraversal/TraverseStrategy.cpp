#include "StdAfx.h"
#include ".\traversestrategy.h"
#include "TraverseBySvr.h"
#include "TraverseByBuddy.h"
#include "TraverseBySourceExchange.h"


CTraverseStrategy::CTraverseStrategy(const uchar * pUserhash, CTraverseStrategy * pNext)
{
	m_pNext = pNext;
	memcpy(m_UserHash, pUserhash, 16);

	m_bFinish = m_bFailed = false;
}

CTraverseStrategy::~CTraverseStrategy(void)
{
	if(m_pNext) delete m_pNext;
}

CTraverseStrategy * CTraverseBySvrFac::CreateStrategy(const uchar * pUserhash)
{
	//CTraverseStrategy * bybuddy = new CTraverseByBuddy(pUserhash, NULL);
	CTraverseStrategy * bysvr= new CTraverseBySvr(pUserhash, NULL);
	return bysvr;
}

CTraverseStrategy * CTraverseBySEFac::CreateStrategy(const uchar * pUserhash)
{
/*
	CTraverseStrategy * bysvr= new CTraverseBySvr(pUserhash, NULL);
	CTraverseBySourceExchange * bySourceExchange = new CTraverseBySourceExchange(pUserhash, bysvr);
	return bySourceExchange;
*/
	//[VC-Huby-080326] SourceExchage traverse is slow,so use CTraverseBySvr first(we will fix the CTraverseBySourceExchange slow bug in the future)
	CTraverseBySourceExchange * bySourceExchange = new CTraverseBySourceExchange(pUserhash, NULL);
	CTraverseStrategy * bysvr = new CTraverseBySvr(pUserhash, bySourceExchange);
	return bysvr;
}