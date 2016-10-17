#include "StdAfx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "RtpTransport.h"

// rpt 最大可发送包的大小
#define  MAX_RTP_PACKETSIZE 1300

CRtpTransport::CRtpTransport(void)
:m_destIp(0)
,m_strDestIp("")
,m_nDestPort(0)
,m_nPortBase(6666)
{

	WSADATA dat;
	WSAStartup(MAKEWORD(2,2),&dat);
}

CRtpTransport::~CRtpTransport(void)
{
	  WSACleanup();
}

void CRtpTransport::checkerror( int err )
{
	if (err < 0) 
	{   
		string errstr = jrtplib::RTPGetErrorString(err);    
		printf("Error:%s\\n", errstr);    
		exit(-1);  
	}   
}

void CRtpTransport::SetRtpInfo( string strIp,int ndestPort )
{
    //用于保存用inet_addr转换后的ip地址
	
    unsigned long destIp =  inet_addr(strIp.c_str());
	
	if (destIp == INADDR_NONE)//INADDR_NONE表示255.255.255.255
	{
		std::cerr << "Bad IP address specified" << std::endl;
		return ;
	}

	//inet_addr返回的是网络字节的地址，这里必须转换为主机字节
    destIp = ntohl(destIp);
   
	sessparams.SetOwnTimestampUnit(1.0/10.0);
	//设置时间戳，单位是秒，当使用RTP会话传输8000Hz采样的音频数据时，由于时戳
	//每秒钟将递增8000，所以时戳单元相应地应该被设置成1/8000
	sessparams.SetAcceptOwnPackets(true);
	transparams.SetPortbase(m_nPortBase);
	int status = m_rtpSess.Create(sessparams,&transparams);	
	checkerror(status);

	jrtplib::RTPIPv4Address addr(destIp,ndestPort);

	status = m_rtpSess.AddDestination(addr);
	checkerror(status);
}

void CRtpTransport::SendRtpPacket( unsigned char *buf,int nBufLen )
{
    int status;
    int nLeftPacket = nBufLen/MAX_RTP_PACKETSIZE + 1;
    int nPos = 0;
	int nPacketLen = 0;
	// 代表是否为最后一个包
	bool bMark = false;
    if (1 == nLeftPacket)
    {   
		// 最后一个包
		bMark = true;
        status = m_rtpSess.SendPacket((void *)buf,nBufLen,0,bMark,10);
    }
    else
	{
		for (int i=0;i<nLeftPacket;++i)
		{   
			nPacketLen = MAX_RTP_PACKETSIZE;
			// 代表发送最后一包
			if (i == nLeftPacket - 1)
			{   
				bMark = true;
				nPacketLen = nBufLen%MAX_RTP_PACKETSIZE;
			}
			 status = m_rtpSess.SendPacket((void *)(buf + nPos),nPacketLen,0,bMark,10);

			nPos += MAX_RTP_PACKETSIZE;
		}

	}
	
	checkerror(status);
	Sleep(10);
}