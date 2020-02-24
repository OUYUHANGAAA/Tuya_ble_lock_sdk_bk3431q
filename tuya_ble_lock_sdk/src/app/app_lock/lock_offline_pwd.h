/**
****************************************************************************
* @file      lock_offline_pwd.h
* @brief     lock_offline_pwd
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_OFFLINE_PWD_H__
#define __LOCK_OFFLINE_PWD_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "lock_common.h"

/*********************************************************************
 * CONSTANTS
 */
#define OFFLINE_PWD_TOKEN_LEN			( 10 )      // ������������
#define OFFLINE_PWD_TIME_ACCURACY_DIV	( 3600u )   // ��������ʱ�侫��-Сʱ
    
#define MAX_OFFLINE_PWD_LIST_NUM		( STORAGE_OFFLINE_PWD_MAX_ITEMS )		

// ҵ����ز���
#define OFFLINE_PWD_PER_PERIOD_MAX_SN           ( 10 )          // ��������ÿ����������ҵ����ˮ��
#define SINGLE_OFFLINE_PWD_ACTIVE_PERIOD        ( 6 * 3600 )    // �������뼤��� 6Сʱ
#define TIMELINESS_OFFLINE_PWD_ACTIVE_PERIOD    ( 24 * 3600 )   // ��ʱ���뼤��� 24Сʱ
#define CLEAR_ALL_OFFLINE_PWD_ACTIVE_PERIOD     ( 24 * 3600 )   // ������뼤��� 24Сʱ
    
typedef enum {
	TIMELINESS_OFFLINE_PWD 	    = 0, // ʱЧ��������
	SINGLE_OFFLINE_PWD 	        = 1, // ������������
    CLEAR_SINGLE_OFFLINE_PWD    = 8, // ���������������-SINGLE
	CLEAR_ALL_OFFLINE_PWD       = 9, // �����������-ALL
} ENUM_OFFLINE_PWD_TYPE;

typedef enum {
	OFFLINE_PWD_STU_EMPTY = 0, 		// δʹ�� 
	OFFLINE_PWD_STU_UNAVAILABLE,	// ��Ч[�ѽ���/��ʹ�ô���]
	OFFLINE_PWD_STU_AVAILABLE,		// ��Ч
} ENUM_OFFLINE_PWD_STU;

typedef enum {
	OFFLINE_PWD_VERIFY_SUCCESS		= ( 0 ),  // ��֤�ɹ�
	OFFLINE_PWD_CLEAR_SUCCESS		= ( 1 ),  // ����ɹ�
	OFFLINE_PWD_ERR_PARAS			= ( -1 ), // ��������
	OFFLINE_PWD_ERR_DECRYPT			= ( -2 ), // ���ܴ���
	OFFLINE_PWD_ERR_TYPE 			= ( -3 ), // �������ʹ���
	OFFLINE_PWD_ERR_UNAVAILABLE		= ( -4 ), // ����״̬�����á�ʧЧ ���á�
	OFFLINE_PWD_ERR_START_TIME		= ( -5 ), // ��ǰʱ��<��Чʱ��
	OFFLINE_PWD_ERR_END_TIME		= ( -6 ), // ��ǰʱ��>ʧЧʱ��
	OFFLINE_PWD_ERR_ACTIVE_TIME		= ( -7 ), // δ�ڼ�����ڼ���
    OFFLINE_PWD_ERR_BSS_SN          = ( -8 ), // �����ҵ����ˮ��
	OFFLINE_PWD_ERR_NO_SPACE		= ( -9 ), // �޴洢�ռ�
	OFFLINE_PWD_ERR_NO_EXIST		= ( -10 ),// ���벻����
	OFFLINE_PWD_ERR_UNKNOW			= ( -11 ),// δ֪����
} OFFLINE_PWD_VERIFY_ERR_CODE;


/*********************************************************************
 * STRUCT
 */
#pragma pack(1)
typedef struct {
	uint8_t pwd_type;			// ������������
	uint8_t start_time[5];		// ��ʼʱ�� (T2-T0)/3600
	uint8_t end_time[4];		// ����ʱ�� (T3-T2)/3600
} offline_pwd_format_t;
#pragma pack() 

#pragma pack(1)
typedef struct {
    int matched;
    uint32_t earliest_t;
    uint16_t pwdsn;
} traverse_info_t;
#pragma pack() 

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
/*  ��δ֧��  */
extern void import_offline_pwd_reference_time_t0(uint32_t time);
extern uint32_t export_offline_pwd_reference_time_t0(void);

extern int verify_offline_pwd_token(uint8_t *key, uint8_t key_len, uint8_t *cipher_pwd, uint8_t cipher_pwd_len, uint32_t timestamp);



#ifdef __cplusplus
}
#endif

#endif //__LOCK_OFFLINE_PWD_H__
