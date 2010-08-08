#ifndef _SMPP_H
#define _SMPP_H
/*///////////////////////////////////////////////////*/
//			以下是命令代码的定义					///
/*///////////////////////////////////////////////////*/
#define GENERIC_NACK			0x80000000
#define BIND_RECEIVER			0x00000001
#define BIND_RECEIVER_RESP		0x80000001
#define BIND_TRANSMITTER		0x00000002
#define BIND_TRANSMITTER_RESP	0x80000002
#define QUERY_SM				0x00000003
#define QUERY_SM_RESP			0x80000003
#define SUBMIT_SM				0x00000004
#define SUBMIT_SM_RESP			0x80000004
#define DELIVER_SM				0x00000005
#define DELIVER_SM_RESP			0x80000005
#define UNBIND					0x00000006
#define UNBIND_RESP				0x80000006
#define REPLACE_SM				0x00000007
#define REPLACE_SM_RESP			0x80000007
#define CANCEL_SM				0x00000008
#define CANCEL_SM_RESP			0x80000008
#define BIND_TRANSCEIVER		0x00000009
#define BIND_TRANSCEIVER_RESP	0x80000009
//Reserved  0x0000000A 
//			0x8000000A
#define OUTBIND					0x0000000B
//Reserved	0x0000000C-0x00000014
//			0x8000000C-0x80000014
#define ENQUIRE_LINK			0x00000015
#define ENQUIRE_LINK_RESP		0x80000015
//Reserved	0x00000016-0x00000020
//			0x80000016-0x80000020
#define SUBMIT_MULTI			0x00000021
#define SUBMIT_MULTI_RESP		0x80000021
//Reserved	0x00000022-0x000000FF
//			0x80000022-0x800000FF
//Reserved	0x00000100 
//Reserved	0x80000100
//Reserved	0x00000101
//			0x80000101
#define ALERT_NOTIFICATION		0x00000102
//Reserved	0x80000102
#define DATA_SM					0x00000103
#define DATA_SM_RESP			0x80000103
//Reserved for SMPP extension	0x00000104-0x0000FFFF
//								0x80000104-0x8000FFFF
//Reserver						0x00010000-0x000101FF
//								0x80010000-0x800101FF
//Reserved for SMPP Vendor		0x00010200-0x000102FF
//								0x80010200-0x800102FF
//Reserver						0x00010300-0xFFFFFFFF

/*///////////////////////////////////////////////////*/
//			以下是错误状态的定义					///
/*///////////////////////////////////////////////////*/
#define ESME_ROK				0x00000000 /*No Error*/
#define ESME_RINVMSGLEN			0x00000001 /*Message Length is invalid*/
#define ESME_RINVCMDLEN			0x00000002 /*Command Length is invalid*/
#define ESME_RINVCMDID			0x00000003 /*Invalid Command ID*/
#define ESME_RINVBNDSTS			0x00000004 /*Incorrect BIND Status for given com-mand*/
#define ESME_RALYBND			0x00000005 /*ESME Already in Bound State*/
#define ESME_RINVPRTFLG			0x00000006 /*Invalid Priority Flag*/
#define ESME_RINVREGDLVFLG		0x00000007 /*Invalid Registered Delivery Flag*/
#define ESME_RSYSERR			0x00000008 /*System Error*/
//Reserved 0x00000009 Reserved
#define ESME_RINVSRCADR			0x0000000A /*Invalid Source Address*/
#define ESME_RINVDSTADR			0x0000000B /*Invalid Dest Addr*/
#define ESME_RINVMSGID			0x0000000C /*Message ID is invalid*/
#define ESME_RBINDFAIL			0x0000000D /*Bind Failed*/
#define	ESME_RINVPASWD			0x0000000E /*Invalid Password*/
#define ESME_RINVSYSID			0x0000000F /*Invalid System ID*/
//Reserved 0x00000010 Reserved
#define ESME_RCANCELFAIL	 	0x00000011 /*Cancel SM Failed*/
//Reserved 0x00000012 Reserved
#define ESME_RREPLACEFAIL		0x00000013 /*Replace SM Failed*/
#define ESME_RMSGQFUL			0x00000014 /*Message Queue Full*/
#define ESME_RINVSERTYP			0x00000015 /*Invalid Service Type*/
//Reserved 0x00000016-0x00000032 Reserved
#define ESME_RINVNUMDESTS		0x00000033 /*Invalid number of destinations*/
#define ESME_RINVDLNAME			0x00000034 /*Invalid Distribution List name*/
//Reserved 0x00000035-0x0000003F Reserved
#define ESME_RINVDESTFLAG		0x00000040 /*Destination flag is invalid (submit_multi)
//Reserved 0x00000041 Reserved*/
#define ESME_RINVSUBREP			0x00000042 /*Invalid submit with replace request*/
//								(i.e. submit_sm withreplace_if_present_flag set)*/
#define ESME_RINVESMCLASS		0x00000043 /*Invalid esm_class field data*/
#define ESME_RCNTSUBDL			0x00000044 /*Cannot Submit to Distribution List*/
#define ESME_RSUBMITFAIL		0x00000045 /*submit_sm or submit_multi failed*/
//Reserved 0x00000046-0x00000047 Reserved
#define ESME_RINVSRCTON		 	0x00000048 /*Invalid Source address TON*/
#define ESME_RINVSRCNPI			0x00000049 /*Invalid Source address NPI*/
#define ESME_RINVDSTTON			0x00000050 /*Invalid Destination address TON*/
#define ESME_RINVDSTNPI			0x00000051 /*Invalid Destination address NPI*/
//Reserved 0x00000052 Reserved
#define ESME_RINVSYSTYP			0x00000053 /*Invalid system_type field*/
#define ESME_RINVREPFLAG		0x00000054 /*Invalid replace_if_present flag*/
#define ESME_RINVNUMMSGS		0x00000055 /*Invalid number of messages*/
//Reserved 0x00000056-0x00000057 Reserved
#define ESME_RTHROTTLED			0x00000058 /*Throttling error (ESME has exceeded allowed message limits)*/
//Reserved 0x00000059-0x00000060 Reserved
#define ESME_RINVSCHED			0x00000061 /*Invalid Scheduled Delivery Time*/
#define ESME_RINVEXPIRY			0x00000062 /*Invalid message validity period (Expiry time)*/
#define ESME_RINVDFTMSGID		0x00000063 /*Predefined Message Invalid or Not Found*/
#define ESME_RX_T_APPN			0x00000064 /*ESME Receiver Temporary App Error Code*/
#define ESME_RX_P_APPN			0x00000065 /*ESME Receiver Permanent App Error Code*/
#define ESME_RX_R_APPN			0x00000066 /*ESME Receiver Reject Message Error Code*/
#define ESME_RQUERYFAIL			0x00000067 /*query_sm request failed*/
//Reserved 0x00000068-0x000000BF Reserved
#define ESME_RINVOPTPARSTREAM	0x000000C0 /*Error in the optional part of the PDU Body.*/
#define ESME_ROPTPARNOTALLWD	0x000000C1 /*Optional Parameter not allowed*/
#define ESME_RINVPARLEN			0x000000C2 /*Invalid Parameter Length.*/
#define ESME_RMISSINGOPTPARAM	0x000000C3 /*Expected Optional Parameter missing*/
#define ESME_RINVOPTPARAMVAL	0x000000C4 /*Invalid Optional Parameter Value*/
//Reserved 0x000000C5-0x000000FD Reserved
#define ESME_RDELIVERYFAILURE	0x000000FE /*Delivery Failure (used for data_sm_resp)*/
#define ESME_RUNKNOWNERR		0x000000FF /*Unknown Error*/
//Reserved for SMPP extension 0x00000100-0x000003FF Reserved for SMPP extension
//Reserved for SMSC vendor specific errors 0x00000400-0x000004FF Reserved for SMSC vendor specific errors
//Reserved 0x00000500-0xFFFFFFFF Reserved
/*///////////////////////////////////////////////////*/
//			以下是选项名称的定义					///
/*///////////////////////////////////////////////////*/
#define DEST_ADDR_SUBUNIT			0x0005 /*GSM*/
#define DEST_NETWORK_TYPE			0x0006 /*Generic*/
#define DEST_BEARER_TYPE			0x0007 /*Generic*/
#define DEST_TELEMATICS_ID 			0x0008 /*GSM*/
#define SOURCE_ADDR_SUBUNIT			0x000D /*GSM*/
#define SOURCE_NETWORK_TYPE			0x000E /*Generic*/
#define SOURCE_BEARER_TYPE			0x000F /*Generic*/
#define SOURCE_TELEMATICS_ID		0x0010 /*GSM*/
#define QOS_TIME_TO_LIVE			0x0017 /*Generic*/
#define PAYLOAD_TYPE				0x0019 /*Generic*/
#define ADDITIONAL_STATUS_INFO_TEXT	0x001D /*Generic*/
#define RECEIPTED_MESSAGE_ID		0x001E /*Generic*/
#define MS_MSG_WAIT_FACILITIES		0x0030 /*GSM*/
#define PRIVACY_INDICATOR			0x0201 /*CDMA,TDMA*/
#define SOURCE_SUBADDRESS			0x0202 /*CDMA,TDMA*/
#define DEST_SUBADDRESS				0x0203 /*CDMA,TDMA*/
#define USER_MESSAGE_REFERENCE		0x0204 /*Generic*/
#define USER_RESPONSE_CODE			0x0205 /*CDMA,TDMA*/
#define SOURCE_PORT					0x020A /*Generic*/
#define DESTINATION_PORT			0x020B /*Generic*/
#define SAR_MSG_REF_NUM				0x020C /*Generic*/
#define LANGUAGE_INDICATOR			0x020D /*CDMA,TDMA*/
#define SAR_TOTAL_SEGMENTS			0x020E /*Generic*/
#define SAR_SEGMENT_SEQNUM			0x020F /*Generic*/
#define SC_INTERFACE_VERSION		0x0210 /*Generic*/
#define CALLBACK_NUM_PRES_IND		0x0302 /*TDMA*/
#define CALLBACK_NUM_ATAG			0x0303 /*TDMA*/
#define NUMBER_OF_MESSAGES			0x0304 /*CDMA*/
#define CALLBACK_NUM				0x0381 /*CDMA,TDMA,GSM,iDEN*/
#define DPF_RESULT					0x0420 /*Generic*/
#define SET_DPF						0x0421 /*Generic*/
#define MS_AVAILABILITY_STATUS		0x0422 /*Generic*/
#define NETWORK_ERROR_CODE			0x0423 /*Generic*/
#define MESSAGE_PAYLOAD				0x0424 /*Generic*/
#define DELIVERY_FAILURE_REASON		0x0425 /*Generic*/
#define MORE_MESSAGES_TO_SEND		0x0426 /*GSM*/
#define MESSAGE_STATE				0x0427 /*Generic*/
#define USSD_SERVICE_OP				0x0501 /*GSM(USSD)*/
#define DISPLAY_TIME				0x1201 /*CDMA,TDMA*/
#define SMS_SIGNAL					0x1203 /*TDMA*/
#define MS_VALIDITY					0x1204 /*CDMA,TDMA*/
#define ALERT_ON_MESSAGE_DELIVERY	0x130C /*CDMA*/
#define ITS_REPLY_TYPE				0x1380 /*CDMA*/
#define ITS_SESSION_INFO			0x1383 /*CDMA*/
/*///////////////////////////////////////////////////*/
//			以下是连接状态部分						///
/*///////////////////////////////////////////////////*/
#define SOCKET_OPEN					0x01
#define BOUND_TX					0x02
#define BOUND_RX					0x04
#define BOUND_TRX					0x08
#define SOCKET_CLOSE				0x00
#define RECEIVE_BUFFER_SIZE			(64*1024)
#define DATA_BUF_SIZE				(64*1024)
#define SMPP_RECEIVE_DATA		((WM_USER)+1)
#define SMPP_BIND_COMMAND       ((WM_USER)+2)
#define SMPP_SUBMIT_DATA		((WM_USER)+3)
#define SMPP_ERROR_DATA			((WM_USER)+4)
#endif
