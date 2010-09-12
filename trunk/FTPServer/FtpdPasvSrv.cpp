#include "stdafx.h"
#include "FtpdPasvSrv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char sadr[32];

CFtpPasvSrv::CFtpPasvSrv()
{
	m_port_from = 1524;
	m_port_to = 65530;
}

CFtpPasvSrv::CFtpPasvSrv(CSocket* dataSkt,UINT from, UINT to)
{
	AfxSocketInit(NULL);
	m_port_from = from;
	m_port_to = to;
	strcpy(sadr,"");
	Create(SOCK_STREAM);
	for(UINT i=m_port_from;i<=m_port_to;i++)
	{
		CSockAddr tSockAdr(INADDR_ANY,i);
		if(Bind(tSockAdr))
		{
			m_current_port = i;
			i = 65536;
		} 
	}
}

CFtpPasvSrv::~CFtpPasvSrv()
{
}

UINT CFtpPasvSrv::launchSocket()
{
		return 0;
}

void CFtpPasvSrv::AttachSocket(CSocket *dataSkt)
{
}
