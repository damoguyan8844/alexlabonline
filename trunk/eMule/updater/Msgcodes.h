//*********************************************************************
// ���³��򷵻�ֵ
//*********************************************************************

const int RESULT_NEWVERSION			=  100;

//*********************************************************************
// USER MESSAGE
//*********************************************************************

// WPARAM: �߳�����
// LPARAM: 
#define UM_THREADMESSAGE				WM_APP + 1

// WPARAM:
// LPARAM:
#define UM_ERROR						WM_APP + 2
#define UM_DOWNLOAD_COMPLETE			WM_APP + 3

//*********************************************************************
// �߳�����
//*********************************************************************
const int THREAD_START			= 0;
const int THREAD_PAUSE			= 1;
const int THREAD_RESUME			= 2;
const int THREAD_FINISH			= 3;
const int THREAD_CANCEL			= 4;

//*********************************************************************
// ERROR
//*********************************************************************

const int ERROR_UNKNOWN			= 0;	// δ֪����
const int ERROR_NOCONNECTION	= 1;	// ������
const int ERROR_SERVER			= 2;	// �޷����ӵ�������

const int ERROR_CHECKFAIL		= 3;	// У��ʧ��
const int ERROR_MEMNOTCREATE	= 4;	// �����ڴ洴��ʧ��
const int ERROR_MEMNOTOPEN		= 5;	// �����ڴ��ʧ��
const int ERROR_MEMNOTMAP		= 6;	// �����ڴ�ӳ��ʧ��
const int ERROR_WRITEMEM		= 7;	// �޷�д�빲���ڴ�

const int ERROR_NONEWVERSION	= 8;	// û���°汾����
const int ERROR_LOADFAIL		= 9;	// Loadʧ��



//*********************************************************************
// HTTP VERBS
//*********************************************************************

const HTTP_VERB_POST	= 0;
const HTTP_VERB_GET		= 1;
const HTTP_VERB_HEAD	= 2;
const HTTP_VERB_PUT		= 3;
const HTTP_VERB_LINK	= 4;
const HTTP_VERB_DELETE	= 5;
const HTTP_VERB_UNLINK	= 6;

//*********************************************************************
// PARAMETERS
//*********************************************************************
const CString PARAMETER_CHECKNEWVERSION		= _T("-checkforupdates");

//*********************************************************************
// HTTP ADDRES
//*********************************************************************
const CString HTTP_REMOTE_ADDRES_REALEASE_DOWNLOAD		=_T("http://update.emule.org.cn/updateinfo");
const CString HTTP_REMOTE_ADDRES_BEAT_DOWNLOAD			=_T("http://update.emule.org.cn/betaupdateinfo");
const CString HTTP_LOCAL_ADDRES_DEBUG_DOWNLOAD			=_T("http://update.emule.org.cn/updateinfo");
const CString HTTP_REMOTE_ADDRES_VCTEST_DOWNLOAD		=_T("http://update.emule.org.cn/updateinfotest");

//*********************************************************************
// BUFFERS
//*********************************************************************

const int BUFFER_DOWNLOADFILE	= 1024;