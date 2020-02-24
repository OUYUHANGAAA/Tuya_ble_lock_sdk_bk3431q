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
#define OFFLINE_PWD_TOKEN_LEN			( 10 )      // 离线密码口令长度
#define OFFLINE_PWD_TIME_ACCURACY_DIV	( 3600u )   // 离线密码时间精度-小时
    
#define MAX_OFFLINE_PWD_LIST_NUM		( STORAGE_OFFLINE_PWD_MAX_ITEMS )		

// 业务相关参数
#define OFFLINE_PWD_PER_PERIOD_MAX_SN           ( 10 )          // 离线密码每周期内最大的业务流水号
#define SINGLE_OFFLINE_PWD_ACTIVE_PERIOD        ( 6 * 3600 )    // 单次密码激活窗口 6小时
#define TIMELINESS_OFFLINE_PWD_ACTIVE_PERIOD    ( 24 * 3600 )   // 限时密码激活窗口 24小时
#define CLEAR_ALL_OFFLINE_PWD_ACTIVE_PERIOD     ( 24 * 3600 )   // 清除密码激活窗口 24小时
    
typedef enum {
	TIMELINESS_OFFLINE_PWD 	    = 0, // 时效离线密码
	SINGLE_OFFLINE_PWD 	        = 1, // 单次离线密码
    CLEAR_SINGLE_OFFLINE_PWD    = 8, // 清除单条离线密码-SINGLE
	CLEAR_ALL_OFFLINE_PWD       = 9, // 清除离线密码-ALL
} ENUM_OFFLINE_PWD_TYPE;

typedef enum {
	OFFLINE_PWD_STU_EMPTY = 0, 		// 未使用 
	OFFLINE_PWD_STU_UNAVAILABLE,	// 无效[已禁用/无使用次数]
	OFFLINE_PWD_STU_AVAILABLE,		// 有效
} ENUM_OFFLINE_PWD_STU;

typedef enum {
	OFFLINE_PWD_VERIFY_SUCCESS		= ( 0 ),  // 验证成功
	OFFLINE_PWD_CLEAR_SUCCESS		= ( 1 ),  // 清除成功
	OFFLINE_PWD_ERR_PARAS			= ( -1 ), // 参数错误
	OFFLINE_PWD_ERR_DECRYPT			= ( -2 ), // 解密错误
	OFFLINE_PWD_ERR_TYPE 			= ( -3 ), // 密码类型错误
	OFFLINE_PWD_ERR_UNAVAILABLE		= ( -4 ), // 密码状态不可用【失效 禁用】
	OFFLINE_PWD_ERR_START_TIME		= ( -5 ), // 当前时间<生效时间
	OFFLINE_PWD_ERR_END_TIME		= ( -6 ), // 当前时间>失效时间
	OFFLINE_PWD_ERR_ACTIVE_TIME		= ( -7 ), // 未在激活窗口内激活
    OFFLINE_PWD_ERR_BSS_SN          = ( -8 ), // 错误的业务流水号
	OFFLINE_PWD_ERR_NO_SPACE		= ( -9 ), // 无存储空间
	OFFLINE_PWD_ERR_NO_EXIST		= ( -10 ),// 密码不存在
	OFFLINE_PWD_ERR_UNKNOW			= ( -11 ),// 未知错误
} OFFLINE_PWD_VERIFY_ERR_CODE;


/*********************************************************************
 * STRUCT
 */
#pragma pack(1)
typedef struct {
	uint8_t pwd_type;			// 离线密码类型
	uint8_t start_time[5];		// 起始时间 (T2-T0)/3600
	uint8_t end_time[4];		// 结束时间 (T3-T2)/3600
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
/*  暂未支持  */
extern void import_offline_pwd_reference_time_t0(uint32_t time);
extern uint32_t export_offline_pwd_reference_time_t0(void);

extern int verify_offline_pwd_token(uint8_t *key, uint8_t key_len, uint8_t *cipher_pwd, uint8_t cipher_pwd_len, uint32_t timestamp);



#ifdef __cplusplus
}
#endif

#endif //__LOCK_OFFLINE_PWD_H__
