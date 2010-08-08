extern "C" bool Bind_Receiver(struct USSD_SMPP_STRUCT *);
extern "C" bool Bind_Transmitter(struct USSD_SMPP_STRUCT *);
extern "C" void Get_Sequence_Number(unsigned long *Sequence_Numbers);
extern "C" bool On_Bind_Receiver_Resp(char *,struct USSD_SMPP_HEAD *);
extern "C" bool On_Bind_Transceiver_Resp(char *,struct USSD_SMPP_HEAD *);
extern "C" bool On_Bind_Transmitter_Resp(char *,struct USSD_SMPP_HEAD *);
extern "C" bool On_Cancel_Sm_Resp(char *);		
extern "C" bool On_Data_Sm(char *);
extern "C" bool On_Data_Sm_Resp(char *);
extern "C" bool On_Deliver_Sm(char *Buf,char *,struct USSD_SMPP_STRUCT *);
extern "C" bool On_Enquire_Link(char *);
extern "C" bool On_Enquire_Link_Resp(char *);
extern "C" bool On_Generic_Nack(char *,struct USSD_SMPP_HEAD *);
extern "C" bool On_Outbind(char *);
extern "C" bool On_Query_Sm_Resp(char *);
extern "C" bool On_Replace_Sm_Resp(char *);		
extern "C" bool On_Submit_Multi_Resp(char *);	
extern "C" bool On_Submit_Sm_Resp(char *,struct USSD_SMPP_STRUCT *);			
extern "C" bool On_Unbind_Resp(char *);
extern "C" int RecvMsg(unsigned char *,int);
extern "C" bool Submit_Sm(struct USSD_SMPP_STRUCT *);
extern "C" void USSDCloseConnect(void);
extern "C" int USSDConnect(char *,unsigned short);
extern "C" long Unix_Format1(long);
extern "C" short Unix_Format2(short);



extern "C" /*
extern "C" void InitVar(void);

extern "C" bool OptionLoad(char *buf,long TotalLen,long *pos);
extern "C" bool OptionSave(char *buf,long *pos);
extern "C" bool Bind_Mode(struct USSD_SMPP_STRUCT *,long Mode);

extern "C" bool Get_bind_mode_resp(char *,struct USSD_SMPP_HEAD *);
extern "C" bool Get_Basic_PDU1(char *);
extern "C" bool Get_Basic_PDU(char *,struct USSD_SMPP_HEAD *);

extern "C" bool Get_enquire_link(char *);

extern "C" bool Get_submit_sm_resp(char *,struct USSD_SMPP_STRUCT *);
extern "C" bool Get_submit_multi_resp(char *);
extern "C" bool Get_data_sm_resp(char *);
extern "C" bool Get_query_sm_resp(char *);
extern "C" bool Get_cancel_sm_resp(char *);
extern "C" bool Get_replace_sm_resp(char *);
extern "C" bool Get_outbind(char *);
extern "C" //绑定部分
extern "C" bool Unbind(unsigned long);






extern "C" //连接部分
extern "C" bool On_Enquire_Link(char *);
extern "C" bool resp_enquire_link(void);
extern "C" bool Enquire_Link(unsigned long);
extern "C" //SMPP server 送到用户部分

extern "C" bool Get_deliver_sm(char *Buf);
extern "C" bool resp_deliver_sm(void);
extern "C" //命令部分

extern "C" bool submit_sm1(unsigned long);			
extern "C" bool submit_multi(void);		
extern "C" bool Data_Sm(void);	

extern "C" bool Get_data_sm(char *);
extern "C" bool resp_data_sm(void);
extern "C" bool Query_Sm(void);


extern "C" bool Cancel_Sm(void);
extern "C" bool Replace_Sm(void);

extern "C" bool On_Cancel_Sm_Resp(char *);		
extern "C" bool On_Submit_Multi_Resp(char *);	


extern "C" void SendMsg(unsigned char *,unsigned short);

extern "C" */