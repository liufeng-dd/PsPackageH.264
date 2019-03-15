#ifndef _STRUCT_H
#define _STRUCT_H

#define PS_HEAD_LEN   		14
#define PSM_HEAD_LEN  		24
#define SYS_HEAD_LEN  		18
#define PES_HEAD_LEN  		19 
#define MAX_RATE            6150
#define OFFSET(x)          	(90000/x)

#define HK_HEADER_LEN		40      //海康头部长度
#define PS_STREAM           2       //PS流
#define H264_STREAM         3       //H.264流
#define HK_PRIVATE_STREAM   4       //海康私有流
#define AUDIO_STREAM        5       //音频流

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#ifndef uint64_t
typedef unsigned long long uint64_t;
#endif

typedef struct 
{
    uint32_t nSize;
    uint32_t nData;
    uint16_t nMask;
    uint8_t *pData;
}bt_s;

typedef enum
{
	NALU_TYPE_SLICE = 1,
	NALU_TYPE_DPA = 2,  
	NALU_TYPE_DPB = 3,
	NALU_TYPE_DPC = 4,
	NALU_TYPE_IDR = 5, //IDR
	NALU_TYPE_SEI = 6, //SEI
	NALU_TYPE_SPS = 7, //SPS
	NALU_TYPE_PPS = 8, //PPS
	NALU_TYPE_AUD = 9,
	NALU_TYPE_EOSEQ = 10,
	NALU_TYPE_EOSTREAM = 11,
	NALU_TYPE_FILL = 12,
}NaluType;

#define bits_write(buffer, count, bits)\
{\
	bt_s *p_buffer = (buffer);\
	int i_count = (count);\
	uint64_t i_bits = (bits);\
	while( i_count > 0 )\
	{\
		i_count--;\
		if( ( i_bits >> i_count )&0x01 )\
		{\
			p_buffer->pData[p_buffer->nData] |= p_buffer->nMask;\
		}\
		else\
		{\
			p_buffer->pData[p_buffer->nData] &= ~p_buffer->nMask;\
		}\
		p_buffer->nMask >>= 1; \
		if( p_buffer->nMask == 0 )\
		{\
			p_buffer->nData++;\
			p_buffer->nMask = 0x80;\
		}\
	}\
}



class PackStream
{
	public:

	PackStream():m_nFrameRate(25)
				,m_nDTS(0)
				,m_nVideoPTS(0)
				,m_Src(0)
	{
	}
	~PackStream(){}

	int ExcludeHeader(char *pSrc, int nSrcLen);
	int GetHeaderLength(char *pData, int nLen);
	int GetStreamType(char *pData);
	int GetFrameType(char *pData);
	int PackH264(char *pSrc, int nSrcLen, char *pDst, int &nDstLen);
	int Packet_I_Frame(char *pData, int nLen, char *pDst, int &nDstLen);
	int Packet_P_Frame(char *pData, int nLen, char *pDst, int &nDstLen);
	int Packet_Video(int nLen, char *pDst);
	int Packet_Pes(char *pData, int nLen, char *pDst, int &nDstLen);

	private:
	unsigned int m_nFrameRate;    //帧数
	unsigned long m_nDTS;
	unsigned long m_nVideoPTS;
	unsigned int m_Src;


};

#endif