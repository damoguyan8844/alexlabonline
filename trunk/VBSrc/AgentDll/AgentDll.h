#include "smpplib.h"

#define AGENTAPI __stdcall

void AGENTAPI APIInitVar(void);
void AGENTAPI APIGet_Sequence_Number(unsigned long *Sequence_Numbers);
bool AGENTAPI APIOptionLoad(char *buf,long TotalLen,long *pos);
bool AGENTAPI APIOptionSave(char *buf,long *pos);
bool AGENTAPI APIBind_Mode(struct USSD_SMPP_STRUCT *,long Mode);
bool AGENTAPI APIOn_Outbind(char *);
bool AGENTAPI APIGet_bind_mode_resp(char *,struct USSD_SMPP_HEAD *);
bool AGENTAPI APIGet_Basic_PDU1(char *);
bool AGENTAPI APIGet_Basic_PDU(char *,struct USSD_SMPP_HEAD *);
bool AGENTAPI APIOn_Generic_Nack(char *,struct USSD_SMPP_HEAD *);
bool AGENTAPI APIGet_enquire_link(char *);
bool AGENTAPI APIOn_Enquire_Link_Resp(char *);
bool AGENTAPI APIGet_submit_sm_resp(char *,struct USSD_SMPP_STRUCT *);
bool AGENTAPI APIGet_submit_multi_resp(char *);
bool AGENTAPI APIGet_data_sm_resp(char *);
bool AGENTAPI APIGet_query_sm_resp(char *);
bool AGENTAPI APIGet_cancel_sm_resp(char *);
bool AGENTAPI APIGet_replace_sm_resp(char *);
bool AGENTAPI APIGet_outbind(char *);
//绑定部分
bool AGENTAPI APIUnbind(unsigned long);
bool AGENTAPI APIOn_Unbind_Resp(char *);
bool AGENTAPI APIBind_Transmitter(struct USSD_SMPP_STRUCT *);
bool AGENTAPI APIOn_Bind_Receiver_Resp(char *,struct USSD_SMPP_HEAD *);
bool AGENTAPI APIBind_Receiver(struct USSD_SMPP_STRUCT *);
bool AGENTAPI APIOn_Bind_Transmitter_Resp(char *,struct USSD_SMPP_HEAD *);
bool AGENTAPI APIOn_Bind_Transceiver_Resp(char *,struct USSD_SMPP_HEAD *);
//连接部分
bool AGENTAPI APIOn_Enquire_Link(char *);
bool AGENTAPI APIresp_enquire_link(void);
bool AGENTAPI APIEnquire_Link(unsigned long);
//SMPP server 送到用户部分
bool AGENTAPI APIOn_Deliver_Sm(char *Buf,char *,struct USSD_SMPP_STRUCT *);
bool AGENTAPI APIGet_deliver_sm(char *Buf);
bool AGENTAPI APIresp_deliver_sm(void);
//命令部分
bool AGENTAPI APISubmit_Sm(struct USSD_SMPP_STRUCT *);
bool AGENTAPI APIsubmit_sm1(unsigned long);			
bool AGENTAPI APIsubmit_multi(void);		
bool AGENTAPI APIData_Sm(void);	
bool AGENTAPI APIOn_Data_Sm(char *);
bool AGENTAPI APIGet_data_sm(char *);
bool AGENTAPI APIresp_data_sm(void);
bool AGENTAPI APIQuery_Sm(void);
bool AGENTAPI APIOn_Query_Sm_Resp(char *);
bool AGENTAPI APIOn_Submit_Sm_Resp(char *,struct USSD_SMPP_STRUCT *);			
bool AGENTAPI APICancel_Sm(void);
bool AGENTAPI APIReplace_Sm(void);
bool AGENTAPI APIOn_Replace_Sm_Resp(char *);		
bool AGENTAPI APIOn_Cancel_Sm_Resp(char *);		
bool AGENTAPI APIOn_Submit_Multi_Resp(char *);	
//bool AGENTAPI APIOn_Data_Sm(char *);			
bool AGENTAPI APIOn_Data_Sm_Resp(char *);

long AGENTAPI APIUnix_Format1(long);
short AGENTAPI APIUnix_Format2(short);
void AGENTAPI APISendMsg(unsigned char *,unsigned short);
int AGENTAPI APIRecvMsg(unsigned char *,int);
int AGENTAPI APIUSSDConnect(char *,unsigned short);
void AGENTAPI APIUSSDCloseConnect(void);