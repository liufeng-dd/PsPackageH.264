#include "Struct.h"
#include<string.h>

/**
 * @remark：  Ps的头部封装
 * @param ：  pData   [in] 要填充ps头的数据缓存地址
 *            nSrcLen [in] 时间戳
 */
int make_ps_Header(char *pData,int nSrc)
{
    if(NULL == pData)
    {
        return 0;
    }

    bt_s bitBuffer;
    bitBuffer.nSize = PS_HEAD_LEN;
    bitBuffer.nData = 0;
    bitBuffer.nMask = 0x80;
    bitBuffer.pData = (unsigned char *)pData;

    memset(bitBuffer.pData, 0, PS_HEAD_LEN);
    bits_write( &bitBuffer, 32, 0x000001BA);			/* start codes */
	bits_write( &bitBuffer, 2, 	1);						/* marker bits '01b' */
    bits_write( &bitBuffer, 3, 	(nSrc>>30)&0x07);		/* System clock [32..30] */
	bits_write( &bitBuffer, 1, 	1);						/* marker bit */
    bits_write( &bitBuffer, 15, (nSrc>>15)&0x7FFF);		/* System clock [29..15] */
	bits_write( &bitBuffer, 1, 	1);						/* marker bit */
    bits_write( &bitBuffer, 15, nSrc&0x7fff);			/* System clock [29..15] */
	bits_write( &bitBuffer, 1, 	1);						/* marker bit */
	bits_write( &bitBuffer, 9, 	1);			            /* System clock [14..0] */
	bits_write( &bitBuffer, 1, 	1);						/* marker bit */
	bits_write( &bitBuffer, 22, (MAX_RATE)&0x3fffff);	/* bit rate(n units of 50 bytes per second.) */
	bits_write( &bitBuffer, 2, 	3);						/* marker bits '11' */
	bits_write( &bitBuffer, 5, 	0x1f);					/* reserved(reserved for future use) */
	bits_write( &bitBuffer, 3, 	0);
   
    return 1;

}
/**
 * @remark：  psm头部封装
 * @param ：  pData  [in]要填充psm的数据缓存地址
 * 
 */

int make_psm_Header(char *pData)
{
    if(NULL == pData)
    {
        return 0;
    }

    bt_s bitBuffer;
    bitBuffer.nSize = PSM_HEAD_LEN;
    bitBuffer.nData = 0;
    bitBuffer.nMask = 0x80;
    bitBuffer.pData = (unsigned char *)pData;

    memset(bitBuffer.pData,0,PSM_HEAD_LEN);
    bits_write( &bitBuffer, 24,0x000001);	/* start code */
	bits_write( &bitBuffer, 8, 0xBC);		/* map stream id */
	bits_write( &bitBuffer, 16,18);			/* program stream map length */ 
	bits_write( &bitBuffer, 1, 1);			/* current next indicator */
	bits_write( &bitBuffer, 2, 3);			/* reserved */
	bits_write( &bitBuffer, 5, 0); 			/* program stream map version */
    bits_write( &bitBuffer, 7, 0x7F);		/* reserved */
	bits_write( &bitBuffer, 1, 1);			/* marker bit */
	bits_write( &bitBuffer, 16,0); 			/* programe stream info length */
	bits_write( &bitBuffer, 16, 8); 		/* elementary stream map length	is */
	/*audio*/
    bits_write( &bitBuffer, 8, 0x90);       /* stream_type */
    bits_write( &bitBuffer, 8, 0xC0);		/* elementary_stream_id */
	bits_write( &bitBuffer, 16, 0); 		/* elementary_stream_info_length is */
	/*video*/
    bits_write( &bitBuffer, 8, 0x1B);       /* stream_type */
    bits_write( &bitBuffer, 8, 0xE0);		/* elementary_stream_id */
	bits_write( &bitBuffer, 16, 0); 		/* elementary_stream_info_length  */
	/*crc (2e b9 0f 3d)*/
	bits_write( &bitBuffer, 8, 0x2E);		/* crc (24~31) bits */
	bits_write( &bitBuffer, 8, 0xB9);		/* crc (16~23) bits */
	bits_write( &bitBuffer, 8, 0x0F);		/* crc (8~15) bits */
	bits_write( &bitBuffer, 8, 0x3D);		/* crc (0~7) bits */

    return 1;

}

/**
 * @remark：  sys头部的封装
 * @param ：  pData  [in]要填充sys头的数据缓存地址
 * 
 */
int make_Sys_Header(char *pData)
{
    if(NULL == pData)
    {
        return 0;
    }

    bt_s bitBuffer;
    bitBuffer.nSize = SYS_HEAD_LEN;
    bitBuffer.nData = 0;
    bitBuffer.nMask = 0x80;
    bitBuffer.pData = (unsigned char *)pData;

    memset(bitBuffer.pData,0,SYS_HEAD_LEN);

    bits_write( &bitBuffer, 32, 0x000001BB);	/*start code*/
    bits_write( &bitBuffer, 16, SYS_HEAD_LEN-6);/* header_length */
    bits_write( &bitBuffer, 1,	 1);            /* marker_bit */
	bits_write( &bitBuffer, 22, 50000);		    /* rate_bound */
    bits_write( &bitBuffer, 1,  1);				/* marker_bit */
    bits_write( &bitBuffer, 6,  1);				/* audio_bound */
    bits_write( &bitBuffer, 1,  0);				/* fixed_flag */
    bits_write( &bitBuffer, 1,  0);        		/* CSPS_flag  */
    bits_write( &bitBuffer, 1,  0);        		/* system_audio_lock_flag */
    bits_write( &bitBuffer, 1,  0);        		/* system_video_lock_flag */
    bits_write( &bitBuffer, 1,  1);        		/* marker_bit */
    bits_write( &bitBuffer, 5,  1);        		/* video_bound */
    bits_write( &bitBuffer, 1,  1);        		/* dif from mpeg1 */
    bits_write( &bitBuffer, 7,  0x7F);     		/* reserver */
	/*audio stream bound*/
    bits_write( &bitBuffer, 8,  0xC0);			/* stream_id */
    bits_write( &bitBuffer, 2,  3);        		/* marker_bit */
    bits_write( &bitBuffer, 1,  0);				/* PSTD_buffer_bound_scale */
    bits_write( &bitBuffer, 13, 512);			/* PSTD_buffer_size_bound */
	/*video stream bound*/
    bits_write( &bitBuffer, 8,  0xE0);			/* stream_id */
    bits_write( &bitBuffer, 2,  3);        		/* marker_bit */
    bits_write( &bitBuffer, 1,  1);        		/* PSTD_buffer_bound_scale */
    bits_write( &bitBuffer, 13, 2048);     		/* PSTD_buffer_size_bound */

    return 0;
}

/**
 * @remark：    pes头部封装
 * @param ：    pData    [in]要填充pes头的数据缓存地址
 *              nSteamId [in]流类型，0XE0为视频流，0xC0为音频流
 *              nSrcLen  [in]流的数据长度
 *              nPts     [in]展现时间戳
 *              nDts     [in]解码时间戳
 */
int make_pes_Header(char *pData,int nStreamId, int nSrcLen, int nPts, int nDts)
{
    if(NULL == pData)
    {
        return 0;
    }

    bt_s bitBuffer;
    bitBuffer.nSize = PES_HEAD_LEN;
    bitBuffer.nData = 0;
    bitBuffer.nMask = 0x80;
    bitBuffer.pData = (unsigned char *)pData;

    memset(bitBuffer.pData,0,PES_HEAD_LEN);
    bits_write( &bitBuffer, 24,0x000001);	    /* start code */
	bits_write( &bitBuffer, 8, (nStreamId));	/* streamID */
	bits_write( &bitBuffer, 16,(nSrcLen)+13);	/* packet_len */ 
	bits_write( &bitBuffer, 2, 2 );		/* '10' */
	bits_write( &bitBuffer, 2, 0 );		/* scrambling_control */
	bits_write( &bitBuffer, 1, 0 );		/* priority */
	bits_write( &bitBuffer, 1, 0 );		/* data_alignment_indicator */
	bits_write( &bitBuffer, 1, 0 );		/* copyright */
	bits_write( &bitBuffer, 1, 0 );		/* original_or_copy */
	bits_write( &bitBuffer, 1, 1 );		/* PTS_flag */
	bits_write( &bitBuffer, 1, 1 );		/* DTS_flag */
	bits_write( &bitBuffer, 1, 0 );		/* ESCR_flag */
	bits_write( &bitBuffer, 1, 0 );		/* ES_rate_flag */
	bits_write( &bitBuffer, 1, 0 );		/* DSM_trick_mode_flag */
	bits_write( &bitBuffer, 1, 0 );		/* additional_copy_info_flag */
	bits_write( &bitBuffer, 1, 0 );		/* PES_CRC_flag */
	bits_write( &bitBuffer, 1, 0 );		/* PES_extension_flag */
	bits_write( &bitBuffer, 8, 10);		/* header_data_length */ 
	
	/*PTS,DTS*/	
    bits_write( &bitBuffer, 4, 3 );						/* "001" */
    bits_write( &bitBuffer, 3, ((nPts)>>30)&0x07 );     /* PTS[32..30] */
	bits_write( &bitBuffer, 1, 1 );
    bits_write( &bitBuffer, 15,((nPts)>>15)&0x7FFF);    /* PTS[29..15] */
	bits_write( &bitBuffer, 1, 1 );
    bits_write( &bitBuffer, 15,(nPts)&0x7FFF);          /* PTS[14..0] */
	bits_write( &bitBuffer, 1, 1 );
    bits_write( &bitBuffer, 4, 1 );						/* "0001" */
    bits_write( &bitBuffer, 3, ((nDts)>>30)&0x07 );     /* DTS[32..30] */
	bits_write( &bitBuffer, 1, 1 );
    bits_write( &bitBuffer, 15,((nDts)>>15)&0x7FFF);    /* DTS[29..15] */
	bits_write( &bitBuffer, 1, 1 );
    bits_write( &bitBuffer, 15,(nDts)&0x7FFF);          /* DTS[14..0] */
	bits_write( &bitBuffer, 1, 1 );
    return 1;

}

/**
 * @remark：    pes头部的封装
 * @param ：    pData     [in]要填充pes头的数据缓存地址
 *              nStreanId [in]流类型，0XE0为视频流，0xC0为音频流
 *              nSrcLen   [in]流的数据长度
 */

int make_pes_Header2(char *pData,int nStreamId, int nSrcLen)
{
        if(NULL == pData)
    {
        return 0;
    }

    bt_s bitBuffer;
    bitBuffer.nSize = PES_HEAD_LEN;
    bitBuffer.nData = 0;
    bitBuffer.nMask = 0x80;
    bitBuffer.pData = (unsigned char *)pData;

    memset(bitBuffer.pData,0,PES_HEAD_LEN - 7); //12个字节长度
    bits_write( &bitBuffer, 24,0x000001);		/* start code */
	bits_write( &bitBuffer, 8, (nStreamId));	/* streamID */
	bits_write( &bitBuffer, 16,(nSrcLen)+6);	/* packet_len */ 
	bits_write( &bitBuffer, 2, 2 );		/* "10" */
	bits_write( &bitBuffer, 2, 0 );		/* scrambling_control */
	bits_write( &bitBuffer, 1, 1 );		/* priority */
	bits_write( &bitBuffer, 1, 0 );		/* data_alignment_indicator */
	bits_write( &bitBuffer, 1, 0 );		/* copyright */
	bits_write( &bitBuffer, 1, 0 );		/* original_or_copy */
	bits_write( &bitBuffer, 1, 1 );		/* PTS_flag */
	bits_write( &bitBuffer, 1, 1 );		/* DTS_flag */
	bits_write( &bitBuffer, 1, 0 );		/* ESCR_flag */
	bits_write( &bitBuffer, 1, 0 );		/* ES_rate_flag */
	bits_write( &bitBuffer, 1, 0 );		/* DSM_trick_mode_flag */
	bits_write( &bitBuffer, 1, 0 );		/* additional_copy_info_flag */
	bits_write( &bitBuffer, 1, 0 );		/* PES_CRC_flag */
	bits_write( &bitBuffer, 1, 0 );		/* PES_extension_flag */
	bits_write( &bitBuffer, 8, 3);		/* header_data_length */ 

    bits_write( &bitBuffer, 8, 0xFF);
    bits_write( &bitBuffer, 8, 0xFF);
    bits_write( &bitBuffer, 8, 0xFC);

    return 1;
}

int PackStream::ExcludeHeader(char *pData, int nLen)
{
    if(NULL == pData || nLen <= 0)
    {
        return 0;
    }

    if(pData[0] == 0x44 && pData[1] == 0x48 && pData[2] == 0x41 && pData[3] == 0x56)
    {
        int nCount = 0;
        nCount = GetHeaderLength(pData + 4, nLen);
        return nCount + 4;
    }
    else if(pData[0] == 0x49 && pData[1] == 0x4D && pData[2] == 0x4B && pData[3] == 0x48)
    {
        return HK_HEADER_LEN;
    }

    return -1;
}

int PackStream::GetHeaderLength(char *pData, int nLen)
{
    if(NULL == pData || nLen <= 0)
    {
        return 0;
    }

    int nCount = 0;
    while(nLen)
    {
        if(pData[0] == 0x00 && pData[1] == 0x00 && pData[2] == 0x00 && pData[3] == 0x01) 
        {
            break;
        }
        else
        {
            nCount++;
            pData++;
            nLen--;
        }

    }

    return nCount;
}

int PackStream::GetStreamType(char *pData)
{
    if(NULL == pData)
    {
        return 0;
    }
    if(pData[0] == 0x00 && pData[1] == 0x00 && pData[2] == 0x01 && (pData[3] == 0xBA || pData[3] == 0xE0))
    {
        return PS_STREAM;
    }
    else if(pData[0] == 0x00 && pData[1] == 0x00 && pData[2] == 0x01 && (pData[3] == 0xBD || pData[3] == 0xBF))
    {
        return HK_PRIVATE_STREAM;
    }
    else if(pData[0] == 0x00 && pData[1] == 0x00 && pData[2] == 0x01 && pData[3] == 0xC0)
    {
        return AUDIO_STREAM;
    }
    else if(GetFrameType(pData))
    {
        return H264_STREAM;
    }

    return -1;
}

int PackStream::GetFrameType(char *pData)
{
    if(NULL == pData)
    {
        return 0;
    }

    int nNaluType = -1;
    if(pData[0] == 0x00 && pData[1] == 0x00 && pData[2] == 0x00 && pData[3] == 0x01)
    {
        nNaluType = (int)(pData[4] & 0x1F);
    }

    return nNaluType;
    
}

int PackStream::PackH264(char *pSrc, int nSrcLen, char *pDst, int &nDstLen)
{
    if(NULL == pSrc || NULL == pDst)
    {
        return 0;
    }

    int nStreamType = GetStreamType(pSrc);
    int nNaluType = 0;

    if(nStreamType == PS_STREAM)
    {
        return PS_STREAM;
    }
    else if(nStreamType == HK_PRIVATE_STREAM)
    {
        return HK_PRIVATE_STREAM;
    }
    else if(nStreamType == H264_STREAM)
    {
        nNaluType = GetFrameType(pSrc);
        switch(nNaluType)
        {
            case NALU_TYPE_SLICE:
                Packet_P_Frame(pSrc,nSrcLen,pDst,nDstLen);
                break;
            case NALU_TYPE_DPA:
                break;
            case NALU_TYPE_DPB:
                break;
            case NALU_TYPE_DPC:
                break;
            case NALU_TYPE_IDR:
                Packet_Pes(pSrc,nSrcLen,pDst,nDstLen);
                break;
            case NALU_TYPE_SEI:
                Packet_Pes(pSrc,nSrcLen,pDst,nDstLen);
                break;
            case NALU_TYPE_SPS:
                Packet_I_Frame(pSrc,nSrcLen,pDst,nDstLen);
                break;
            case NALU_TYPE_PPS:
                Packet_Pes(pSrc,nSrcLen,pDst,nDstLen);
                break;
            case NALU_TYPE_AUD:
                break;
            case NALU_TYPE_EOSEQ:
                break;
            case NALU_TYPE_EOSTREAM:
                break;
            case NALU_TYPE_FILL:
                break;
            default:
                break;
        }
    } 
    else if(nStreamType == -1)
    {
        /* code */
        return -1;
    }
    return 1;
}

int PackStream::Packet_I_Frame(char *pData, int nLen, char *pDst, int &nDstLen)
{
    if(NULL == pData || NULL == pDst)
    {
        return 0;
    }
    int nTotalLen = 0;

    //m_nDTS = m_VideoPTS + OFFSET(m_nFrameRate);
    //m_VideoPTS += OFFSET(m_nFrameRate);
    //m_Src = m_VideoPTS;

    make_ps_Header(pDst,m_Src);
    nTotalLen += PS_HEAD_LEN;

    make_Sys_Header(pDst + nTotalLen);
    nTotalLen += SYS_HEAD_LEN;

    make_psm_Header(pDst + nTotalLen);
    nTotalLen += PSM_HEAD_LEN;

    Packet_Video(nLen,pDst + nTotalLen);
    //make_pes_Header(pDst + nTotalLen,0xE0,nLen,m_nVideoPTS,m_nDTS);
    nTotalLen += PES_HEAD_LEN;

    memcpy(pDst + nTotalLen,pData,nLen);
    nDstLen = nTotalLen + nLen;

    return 1;
}

int PackStream::Packet_P_Frame(char *pData, int nLen, char *pDst, int &nDstLen)
{
    if(NULL == pData || NULL == pDst)
    {
        return 0;
    }

    int nTotalLen = 0;
    make_ps_Header(pDst,m_Src);
    nTotalLen += PS_HEAD_LEN;

    Packet_Video(nLen,pDst + nTotalLen);
    //make_pes_Header(pDst + nTotalLen,0xE0,nLen,m_VideoPTS,m_nDTS);
    nTotalLen += PES_HEAD_LEN;

    memcpy(pDst + nTotalLen, pData, nLen);
    nDstLen = nTotalLen + nLen;

    return 1;
}

int PackStream::Packet_Pes(char *pData, int nLen, char *pDst, int &nDstLen)
{
    if(NULL == pData || NULL == pDst)
    {
        return 0;
    }

    make_pes_Header2(pDst,0xE0,nLen);
    memcpy(pDst + (PES_HEAD_LEN - 7), pData,nLen);
    nDstLen = PES_HEAD_LEN + nLen;

    return 1;
}

int PackStream::Packet_Video(int nLen, char *pDst)
{
    if(NULL == pDst)
    {
        return 0;
    }

    
	m_nDTS = m_nVideoPTS + OFFSET(m_nFrameRate);
	m_nVideoPTS += OFFSET(m_nFrameRate);
	m_Src = m_nVideoPTS;

	make_pes_Header(pDst,0xE0,nLen,m_nVideoPTS,m_nDTS);
}