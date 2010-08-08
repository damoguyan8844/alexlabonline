#include "AgentDll.h"

void AGENTAPI APIGet_Sequence_Number(unsigned long *Sequence_Numbers){
	Get_Sequence_Number(Sequence_Numbers);
}

bool AGENTAPI APIOn_Outbind(char *s){
	return On_Outbind(s);
}

bool AGENTAPI APIOn_Generic_Nack(char *s,struct USSD_SMPP_HEAD *t){
	return On_Generic_Nack(s,t);
}

/*
void AGENTAPI APIInitVar(void){
	InitVar();
}

bool AGENTAPI APIOptionLoad(char *buf,long TotalLen,long *pos){
	return OptionLoad(buf,TotalLen,pos);
}


bool AGENTAPI APIOptionSave(char *buf,long *pos){
	return OptionSave(buf,pos);
}

bool AGENTAPI APIBind_Mode(struct USSD_SMPP_STRUCT * t,long Mode){
	return Bind_Mode(t,Mode);
}

bool AGENTAPI APIGet_bind_mode_resp(char * s,struct USSD_SMPP_HEAD * t)
{
	return Get_bind_mode_resp(s,t);
}

bool AGENTAPI APIGet_Basic_PDU1(char *s){
	return Get_Basic_PDU1(s);
}

bool AGENTAPI APIGet_Basic_PDU(char *s,struct USSD_SMPP_HEAD *t){
	return Get_Basic_PDU(s,t);
}

bool AGENTAPI APIGet_enquire_link(char *s){
	return Get_enquire_link(s);
}
*/

bool AGENTAPI APIOn_Enquire_Link_Resp(char *s){
	return On_Enquire_Link_Resp(s);
}

/*
bool AGENTAPI APIGet_submit_sm_resp(char *s,struct USSD_SMPP_STRUCT *t){
	return Get_submit_sm_resp(s,t);
}

bool AGENTAPI APIGet_submit_multi_resp(char *s){
	return Get_submit_multi_resp(s);
}

bool AGENTAPI APIGet_data_sm_resp(char *s){
	return Get_data_sm_resp(s);
}

bool AGENTAPI APIGet_query_sm_resp(char *s){
	return Get_query_sm_resp(s);
}

bool AGENTAPI APIGet_cancel_sm_resp(char *s){
	return Get_cancel_sm_resp(s);
}

bool AGENTAPI APIGet_replace_sm_resp(char *s){
	return Get_replace_sm_resp(s);
}

bool AGENTAPI APIGet_outbind(char *s){
	return Get_outbind(s);
}

//绑定部分
bool AGENTAPI APIUnbind(unsigned long i){
	return Unbind(i);
}
*/

bool AGENTAPI APIOn_Unbind_Resp(char *s){
	return On_Unbind_Resp(s);
}

bool AGENTAPI APIBind_Transmitter(struct USSD_SMPP_STRUCT *t){
	return Bind_Transmitter(t);
}

bool AGENTAPI APIOn_Bind_Receiver_Resp(char *s,struct USSD_SMPP_HEAD *t){
	return On_Bind_Receiver_Resp(s,t);
}

bool AGENTAPI APIBind_Receiver(struct USSD_SMPP_STRUCT *t){
	return Bind_Receiver(t);
}

bool AGENTAPI APIOn_Bind_Transmitter_Resp(char *s,struct USSD_SMPP_HEAD *t){
	return On_Bind_Transmitter_Resp(s,t);
}

bool AGENTAPI APIOn_Bind_Transceiver_Resp(char *s,struct USSD_SMPP_HEAD *t){
	return On_Bind_Transceiver_Resp(s,t);
}

//连接部分
bool AGENTAPI APIOn_Enquire_Link(char *s){
	return On_Enquire_Link(s);
}

/*
bool AGENTAPI APIresp_enquire_link(void){
	return resp_enquire_link();
}

bool AGENTAPI APIEnquire_Link(unsigned long i){
	return Enquire_Link(i);
}
*/

//SMPP server 送到用户部分
bool AGENTAPI APIOn_Deliver_Sm(char *Buf,char *s,struct USSD_SMPP_STRUCT *t){
	return On_Deliver_Sm(Buf,s,t);
}

/*
bool AGENTAPI APIGet_deliver_sm(char *Buf){
	return Get_deliver_sm(Buf);
}

bool AGENTAPI APIresp_deliver_sm(void){
	return resp_deliver_sm();
}
*/
//命令部分
bool AGENTAPI APISubmit_Sm(struct USSD_SMPP_STRUCT *t){
	return Submit_Sm(t);
}
/*
bool AGENTAPI APIsubmit_sm1(unsigned long i){
	return submit_sm1(i);
}

bool AGENTAPI APIsubmit_multi(void){
	return submit_multi();
}

bool AGENTAPI APIData_Sm(void){
	return Data_Sm();
}
*/

bool AGENTAPI APIOn_Data_Sm(char *s){
	return On_Data_Sm(s);
}

/*
bool AGENTAPI APIGet_data_sm(char *s){
	return Get_data_sm(s);
}

bool AGENTAPI APIresp_data_sm(void){
	return resp_data_sm();
}

bool AGENTAPI APIQuery_Sm(void){
	return Query_Sm();
}
*/

bool AGENTAPI APIOn_Query_Sm_Resp(char *s){
	return On_Query_Sm_Resp(s);
}

bool AGENTAPI APIOn_Submit_Sm_Resp(char *s,struct USSD_SMPP_STRUCT *t){
	return On_Submit_Sm_Resp(s,t);
}

/*
bool AGENTAPI APICancel_Sm(void){
	return Cancel_Sm();
}

bool AGENTAPI APIReplace_Sm(void){
	return Replace_Sm();
}
*/

bool AGENTAPI APIOn_Replace_Sm_Resp(char *s){
	return On_Replace_Sm_Resp(s);
}

bool AGENTAPI APIOn_Cancel_Sm_Resp(char *s){
	return On_Cancel_Sm_Resp(s);
}

bool AGENTAPI APIOn_Submit_Multi_Resp(char *s){
	return On_Submit_Multi_Resp(s);
}

//bool AGENTAPI APIOn_Data_Sm(char *s);
	
bool AGENTAPI APIOn_Data_Sm_Resp(char *s){
	return On_Data_Sm_Resp(s);
}

long AGENTAPI APIUnix_Format1(long i){
	return Unix_Format1(i);
}

short AGENTAPI APIUnix_Format2(short i){
	return Unix_Format2(i);
}

/*
void AGENTAPI APISendMsg(unsigned char *s,unsigned short i){
	SendMsg(s,i);
}
*/

int AGENTAPI APIRecvMsg(unsigned char *s,int i){
	return RecvMsg(s,i);
}

int AGENTAPI APIUSSDConnect(char *s,unsigned short i){
	return USSDConnect(s,i); 
}

void AGENTAPI APIUSSDCloseConnect(void){
	USSDCloseConnect();
}
