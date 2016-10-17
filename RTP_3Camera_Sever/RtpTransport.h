
//#pragma once

#include "stdafx.h"
/*
#include "rtpsession.h" 
#include "rtpudpv4transmitter.h"
#include "rtpsessionparams.h"    //这两个头文件定义了创建RTPSession所需的两个参数

#include "rtpipv4address.h" 

#include "rtperrors.h"
*/
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
//
#include <iostream>
#include <string>
//
using namespace std;

//
class CRtpTransport
{
public:
	CRtpTransport(void);
	~CRtpTransport(void);

private:
	void checkerror(int err);

public:
	void SetRtpInfo(string strIp,int nPort);
	void SendRtpPacket(unsigned char *buf,int nBufLen);
private:
	//
	jrtplib::RTPSession m_rtpSess;
	jrtplib::RTPUDPv4TransmissionParams transparams;
	jrtplib::RTPSessionParams sessparams;
	jrtplib::RTPIPv4Address addr;

	unsigned long m_destIp;
	string m_strDestIp;
	int m_nDestPort;
	int m_nPortBase;


};

