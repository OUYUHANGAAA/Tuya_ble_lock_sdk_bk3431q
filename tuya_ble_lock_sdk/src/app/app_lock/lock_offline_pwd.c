#include "lock_offline_pwd.h"
#include "fpe_decrypt.h"


/*  暂未支持  */

/*********************************************************************
 * LOCAL CONSTANTS
 */
#define STORAGE_OFFLINE_PWD_MAX_ITEMS           ( 500 ) 

#define CEIL_DIV(A, B)      (((A) + (B) - 1) / (B))

/*********************************************************************
 * LOCAL STRUCT
 */
#pragma pack(1)
typedef union _offline_pwd_info_t {
	uint8_t data[5];
    struct {
		uint8_t pwd_stu		:4;
		uint8_t pwd_type	:4;
		uint32_t pwd		:32;
    } bits1;
} offline_pwd_info_t;
#pragma pack()

typedef union {
    offline_pwd_info_t  info;
    uint32_t    padding[CEIL_DIV(sizeof(offline_pwd_info_t), 4)];
} OfflinePwdStoreType;

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint32_t t0 = 1556695723;
static bool already_export_t0 = false;

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
int fstorage_offline_pwd_read_data(uint32_t addr, uint8_t *data, uint16_t len) {
    return 0;
}

int fstorage_offline_pwd_write_data(uint32_t addr, uint8_t *data, uint16_t len) {
    return 0;
}

void import_offline_pwd_reference_time_t0(uint32_t time) {
//	t0 = time;
//    fstorage_offline_pwd_t0_write_data(0, (uint8_t *)&t0, 4);
//    APP_DEBUG_PRINTF("import t0->[%d]\r\n", t0);
}
    
uint32_t export_offline_pwd_reference_time_t0(void) {
//    if (!already_export_t0) {
//    	fstorage_offline_pwd_t0_read_data(0, (uint8_t *)&t0, 4);
//        APP_DEBUG_PRINTF("export t0->[%d]\r\n", t0);
//        
//        already_export_t0 = true;
//    }
    return t0;
}

// 计算离线密码组成，T2=开始时间T2 T3=结束时间/业务流水号
static void calc_offline_pwd_timeliness(offline_pwd_format_t *format, uint32_t *t2, uint32_t *t3) {
	int pos;
	uint32_t start_time = 0, end_time = 0;
	uint32_t tmp_t0, tmp_t2 = 0, tmp_t3 = 0;
    tuya_ble_time_struct_data_t datatime;

	for (pos=0; pos<5; pos++) {
		start_time = (start_time*10) + format->start_time[pos];
	}
	
	for (pos=0; pos<4; pos++) {
		end_time = (end_time*10) + format->end_time[pos];
	}

    /* T2 开始时间 T3结束时间或业务序号 */
    // t0 = t0 - (t0 % 3600)
    // x = (t2 - t0)/3600
	// y = (t3 - t2)/3600 
    tmp_t0 = t0 - (t0 % 3600); 
    tmp_t2 = (start_time*OFFLINE_PWD_TIME_ACCURACY_DIV + tmp_t0);
    if (format->pwd_type == TIMELINESS_OFFLINE_PWD || format->pwd_type == CLEAR_SINGLE_OFFLINE_PWD) 
        tmp_t3 = (end_time*OFFLINE_PWD_TIME_ACCURACY_DIV + tmp_t2); 
    else if (format->pwd_type == SINGLE_OFFLINE_PWD || format->pwd_type == CLEAR_ALL_OFFLINE_PWD) 
        tmp_t3 = end_time;
        
	*t2 = tmp_t2;
	*t3 = tmp_t3;
    
    /* printf t2 t3 now local time */
    tuya_ble_utc_sec_2_mytime(tmp_t2, &datatime, 0);
	APP_DEBUG_PRINTF("T2->[%02d-%02d-%02d %02d:xx:xx]\r\n", datatime.nYear, datatime.nMonth, datatime.nDay, datatime.nHour);
    
    if (format->pwd_type == TIMELINESS_OFFLINE_PWD || format->pwd_type == CLEAR_SINGLE_OFFLINE_PWD) {
        tuya_ble_utc_sec_2_mytime(tmp_t3, &datatime, 0);
        APP_DEBUG_PRINTF("T3->[%02d-%02d-%02d %02d:xx:xx]\r\n", datatime.nYear, datatime.nMonth, datatime.nDay, datatime.nHour);
    } else
        APP_DEBUG_PRINTF("C3->[%d]\r\n",tmp_t3);
    
    tuya_ble_utc_sec_2_mytime(app_port_get_timestamp(), &datatime, 0);
	APP_DEBUG_PRINTF("NOW->[%02d-%02d-%02d %02d:xx:xx]\r\n", datatime.nYear, datatime.nMonth, datatime.nDay, datatime.nHour);
}

static void inverse_calc_offline_pwd_timeliness(OfflinePwdStoreType *offline_pwd, uint32_t *t2, uint32_t *t3) {
	uint32_t start_time = 0, end_time = 0;
	uint32_t tmp_t0, tmp_t2 = 0, tmp_t3 = 0;
   
    /* T2-开始时间 T3-结束时间 */
    // t0 = t0 - (t0 % 3600)
    // x = (t2 - t0)/3600
	// y = (t3 - t2)/3600 
    tmp_t0 = t0 - (t0 % 3600); 
    start_time = (offline_pwd->info.bits1.pwd / 10000);
    end_time = (offline_pwd->info.bits1.pwd % 10000);
    
    tmp_t2 = (start_time*OFFLINE_PWD_TIME_ACCURACY_DIV + tmp_t0);
    if (TIMELINESS_OFFLINE_PWD == offline_pwd->info.bits1.pwd_type) 
        tmp_t3 = (end_time*OFFLINE_PWD_TIME_ACCURACY_DIV + tmp_t2); 
    else if (SINGLE_OFFLINE_PWD == offline_pwd->info.bits1.pwd_type) 
        tmp_t3 = SINGLE_OFFLINE_PWD_ACTIVE_PERIOD + tmp_t2;
    
	*t2 = tmp_t2;
	*t3 = tmp_t3;
}

static uint32_t offline_pwd_number_convert_to_integer(uint8_t *pwd_num, uint8_t pwd_len) {
	uint32_t input_pwd = 0;
	
	for (int pos=1; pos<pwd_len; pos++) {
		input_pwd = (input_pwd*10) + pwd_num[pos];
	}
	
	return input_pwd;
}

static int find_unused_offline_pwdsn(void) {
	int find_pwdsn = -1;
    bool find_result = false;
    uint32_t t2, t3, t_now;
    OfflinePwdStoreType *ram_offline_pwd_data = NULL;
    traverse_info_t valid_pwd_traverse_info, invalid_pwd_traverse_info, single_pwd_traverse_info;
    
	ram_offline_pwd_data = (OfflinePwdStoreType *)app_port_malloc(sizeof(OfflinePwdStoreType)*MAX_OFFLINE_PWD_LIST_NUM);
	if (ram_offline_pwd_data == NULL) 
		return -2;
    
    // 1. 找是否有未使用的 
    // 2. 过期的t3<t_now 直接覆盖
    // 3. 多次密码中密码状态无效的 找失效时间最早的
    // 4. 多次密码中密码状态有效的 找生效时间最早的
    // 5. 如都是单次的 单次中失效时间最早的 
    t_now = app_port_get_timestamp();
    valid_pwd_traverse_info.matched     = false;
    invalid_pwd_traverse_info.matched   = false;
    single_pwd_traverse_info.matched    = false;
	fstorage_offline_pwd_read_data(0, (uint8_t *)ram_offline_pwd_data, sizeof(OfflinePwdStoreType)*MAX_OFFLINE_PWD_LIST_NUM);
    for (int i=0; i<MAX_OFFLINE_PWD_LIST_NUM; i++) {
		if (ram_offline_pwd_data[i].info.bits1.pwd_stu == OFFLINE_PWD_STU_EMPTY) {
			find_pwdsn = i;
			find_result = true;
            break;
		} else {
            inverse_calc_offline_pwd_timeliness(&ram_offline_pwd_data[i], &t2, &t3);
            if (t3 < t_now) {
                find_pwdsn = i;
			    find_result = true;
                break;
            } else if (ram_offline_pwd_data[i].info.bits1.pwd_type == TIMELINESS_OFFLINE_PWD) {
                if (ram_offline_pwd_data[i].info.bits1.pwd_stu == OFFLINE_PWD_STU_UNAVAILABLE) {
                    // 多次密码，被标记为'禁用'，找失效时间最早的
                    if (!invalid_pwd_traverse_info.matched) {
                        invalid_pwd_traverse_info.matched = true;
                        invalid_pwd_traverse_info.earliest_t = t3;
                        invalid_pwd_traverse_info.pwdsn = i;
                    } else if (t3 < invalid_pwd_traverse_info.earliest_t) {
                        invalid_pwd_traverse_info.earliest_t = t3;
                        invalid_pwd_traverse_info.pwdsn = i;
                    }
                } else {
                    // 多次密码，未被标记为‘禁用’，找生效时间最早的
                    if (!valid_pwd_traverse_info.matched) {
                        valid_pwd_traverse_info.matched = true;
                        valid_pwd_traverse_info.earliest_t = t2;
                        valid_pwd_traverse_info.pwdsn = i;
                    } else if (t2 < valid_pwd_traverse_info.earliest_t) {
                        valid_pwd_traverse_info.earliest_t = t2;
                        valid_pwd_traverse_info.pwdsn = i;
                    }
                }
            } else if (ram_offline_pwd_data[i].info.bits1.pwd_type == SINGLE_OFFLINE_PWD) {
                // 单次密码，找失效时间最早的
                if (!single_pwd_traverse_info.matched) {
                    single_pwd_traverse_info.matched = true;
                    single_pwd_traverse_info.earliest_t = t3;
                    single_pwd_traverse_info.pwdsn = i;
                } else if (t3 < single_pwd_traverse_info.earliest_t) {
                    single_pwd_traverse_info.earliest_t = t3;
                    single_pwd_traverse_info.pwdsn = i;
                }
            }
        }      
	}

    if (!find_result) {
        if (invalid_pwd_traverse_info.matched)
            find_pwdsn = invalid_pwd_traverse_info.pwdsn;
        else if (valid_pwd_traverse_info.matched)
            find_pwdsn = valid_pwd_traverse_info.pwdsn; 
        else if (single_pwd_traverse_info.matched)
            find_pwdsn = single_pwd_traverse_info.pwdsn;
        
        APP_DEBUG_PRINTF("all pwdsn full, cover sn->[%d]\r\n", find_pwdsn);
    }
    
	app_port_free(ram_offline_pwd_data);
	return find_pwdsn;
}

// ret >= 0 exist
// ret -1	
// ret -2
static int is_offline_pwd_exist(ENUM_OFFLINE_PWD_TYPE type, uint32_t pwd_integer, OfflinePwdStoreType *data) {
	int pwdsn = -1;
	OfflinePwdStoreType *ram_offline_pwd_data = NULL;

	ram_offline_pwd_data = (OfflinePwdStoreType *)app_port_malloc(sizeof(OfflinePwdStoreType)*MAX_OFFLINE_PWD_LIST_NUM);
	if (ram_offline_pwd_data == NULL) 
		return -2;

	fstorage_offline_pwd_read_data(0, (uint8_t *)ram_offline_pwd_data, sizeof(OfflinePwdStoreType)*MAX_OFFLINE_PWD_LIST_NUM);
	for (int i=0; i<MAX_OFFLINE_PWD_LIST_NUM; i++) {
		if (ram_offline_pwd_data[i].info.bits1.pwd_stu != OFFLINE_PWD_STU_EMPTY && \
			ram_offline_pwd_data[i].info.bits1.pwd_type == type && \
		    ram_offline_pwd_data[i].info.bits1.pwd == pwd_integer) {
			pwdsn = i;
			memcpy(data, &ram_offline_pwd_data[i].info.bits1, sizeof(OfflinePwdStoreType));
			break;
		}
	}

	app_port_free(ram_offline_pwd_data);
	return pwdsn;
}
	
static int clear_offline_pwd(ENUM_OFFLINE_PWD_TYPE pwd_type, uint32_t pwd) {
	OfflinePwdStoreType *ram_offline_pwd_data = NULL;

    if (pwd_type != CLEAR_SINGLE_OFFLINE_PWD && \
        pwd_type != CLEAR_ALL_OFFLINE_PWD) {
        APP_DEBUG_PRINTF("Warning! clear offline pwd, but type err\r\n");
        return -1;
    }
    
	ram_offline_pwd_data = (OfflinePwdStoreType *)app_port_malloc(sizeof(OfflinePwdStoreType)*MAX_OFFLINE_PWD_LIST_NUM);
	if (ram_offline_pwd_data == NULL) 
		return -1;

	fstorage_offline_pwd_read_data(0, (uint8_t *)ram_offline_pwd_data, sizeof(OfflinePwdStoreType)*MAX_OFFLINE_PWD_LIST_NUM);
    for (int i=0; i<MAX_OFFLINE_PWD_LIST_NUM; i++) {
		if (ram_offline_pwd_data[i].info.bits1.pwd_stu != OFFLINE_PWD_STU_EMPTY) {
            if (pwd_type == CLEAR_SINGLE_OFFLINE_PWD && ram_offline_pwd_data[i].info.bits1.pwd == pwd) {
                ram_offline_pwd_data[i].info.bits1.pwd_stu = OFFLINE_PWD_STU_UNAVAILABLE;
            } else if (pwd_type == CLEAR_ALL_OFFLINE_PWD) {
                ram_offline_pwd_data[i].info.bits1.pwd_stu = OFFLINE_PWD_STU_UNAVAILABLE;
            }
		}
	}
    
    fstorage_offline_pwd_write_data(0, (uint8_t *)(ram_offline_pwd_data), sizeof(OfflinePwdStoreType)*MAX_OFFLINE_PWD_LIST_NUM);
	app_port_free(ram_offline_pwd_data);
	return 0;
}

int verify_offline_pwd_token(uint8_t *key, uint8_t key_len, uint8_t *cipher_pwd, uint8_t cipher_pwd_len, uint32_t timestamp) {
	int exist_pwd_sn, find_pwd_sn;
	OfflinePwdStoreType ram_offline_pwd_data;
	uint8_t plaintext_pwd[OFFLINE_PWD_TOKEN_LEN] = {0}, plaintext_pwd_len;
	uint32_t pwd_integer, t2, t3, t_now = timestamp;
	uint32_t active_period;
    
    /* input paras check */
	if ((key == NULL) || \
		(key_len != 16 && key_len != 24 && key_len != 32) || \
		(cipher_pwd == NULL) || \
		(cipher_pwd_len != OFFLINE_PWD_TOKEN_LEN)) {
		APP_DEBUG_PRINTF("offline pwd, paras err\r\n");
		return OFFLINE_PWD_ERR_PARAS;
	}

    /* fpe decrypt */
	if (fpe_decrypt(key, key_len, cipher_pwd, cipher_pwd_len, plaintext_pwd, &plaintext_pwd_len)) {
		APP_DEBUG_PRINTF("offline pwd, decrypt err\r\n");
		return OFFLINE_PWD_ERR_DECRYPT;
	} 

    /* offline pwd type check */
	offline_pwd_format_t *format = (offline_pwd_format_t *)(&plaintext_pwd);
	if (format->pwd_type != TIMELINESS_OFFLINE_PWD && \
		format->pwd_type != SINGLE_OFFLINE_PWD && \
        format->pwd_type != CLEAR_SINGLE_OFFLINE_PWD && \
		format->pwd_type != CLEAR_ALL_OFFLINE_PWD) {
		APP_DEBUG_PRINTF("offline pwd, type failed\r\n");
		return OFFLINE_PWD_ERR_TYPE;
	}
	
    /* pending logic */
    export_offline_pwd_reference_time_t0();
	calc_offline_pwd_timeliness(format, &t2, &t3);
    
	pwd_integer = offline_pwd_number_convert_to_integer(plaintext_pwd, OFFLINE_PWD_TOKEN_LEN);
	APP_DEBUG_PRINTF("offline pwd info, type->[%d] pwd->[%09d]\r\n", format->pwd_type, pwd_integer);
    if (format->pwd_type == TIMELINESS_OFFLINE_PWD || format->pwd_type == SINGLE_OFFLINE_PWD) {
        // 限时/单次离线密码
        if (format->pwd_type == TIMELINESS_OFFLINE_PWD) {
            if (t2 > t_now) 
        	    return OFFLINE_PWD_ERR_START_TIME;
        	else if (t3 < t_now) 
        		return OFFLINE_PWD_ERR_END_TIME;	
            
            active_period = TIMELINESS_OFFLINE_PWD_ACTIVE_PERIOD;
        } else if (format->pwd_type == SINGLE_OFFLINE_PWD) {
            if (t2 > t_now) 
        		return OFFLINE_PWD_ERR_START_TIME;
            else if (t3 > OFFLINE_PWD_PER_PERIOD_MAX_SN) 
                return OFFLINE_PWD_ERR_BSS_SN;   
             
            active_period = SINGLE_OFFLINE_PWD_ACTIVE_PERIOD;
        }

        exist_pwd_sn = is_offline_pwd_exist((ENUM_OFFLINE_PWD_TYPE)format->pwd_type, pwd_integer, &ram_offline_pwd_data);
        if (exist_pwd_sn >= 0) 
		    return (ram_offline_pwd_data.info.bits1.pwd_stu == OFFLINE_PWD_STU_UNAVAILABLE) ? (OFFLINE_PWD_ERR_UNAVAILABLE) : (OFFLINE_PWD_VERIFY_SUCCESS);
		else if ((t_now - t2) > active_period) 
			return OFFLINE_PWD_ERR_ACTIVE_TIME;
		else {
            find_pwd_sn = find_unused_offline_pwdsn();
			if (find_pwd_sn < 0) {
				APP_DEBUG_PRINTF("offline pwd, storage space err\r\n");
				return OFFLINE_PWD_ERR_NO_SPACE;
			}
			
			ram_offline_pwd_data.info.bits1.pwd_stu = (format->pwd_type == TIMELINESS_OFFLINE_PWD) ? (OFFLINE_PWD_STU_AVAILABLE) : (OFFLINE_PWD_STU_UNAVAILABLE);
			ram_offline_pwd_data.info.bits1.pwd_type = format->pwd_type;
			ram_offline_pwd_data.info.bits1.pwd 	 = pwd_integer;
			fstorage_offline_pwd_write_data(sizeof(OfflinePwdStoreType)*find_pwd_sn, (uint8_t *)(&ram_offline_pwd_data), sizeof(OfflinePwdStoreType));	
			return OFFLINE_PWD_VERIFY_SUCCESS;
        }
    } else if (format->pwd_type == CLEAR_SINGLE_OFFLINE_PWD) {
        // 清除单个离线密码
        APP_DEBUG_PRINTF("clear single offline pwd\r\n");
        
        clear_offline_pwd(CLEAR_SINGLE_OFFLINE_PWD, pwd_integer);
        return OFFLINE_PWD_CLEAR_SUCCESS;
    } else if (format->pwd_type == CLEAR_ALL_OFFLINE_PWD) {
        // 清除所有离线密码 
        if ((t_now - t2) > CLEAR_ALL_OFFLINE_PWD_ACTIVE_PERIOD) 
			return OFFLINE_PWD_ERR_ACTIVE_TIME;
		else if (t3 > OFFLINE_PWD_PER_PERIOD_MAX_SN) 
            return OFFLINE_PWD_ERR_BSS_SN;   
        
        APP_DEBUG_PRINTF("clear all offline pwd\r\n");
		clear_offline_pwd(CLEAR_ALL_OFFLINE_PWD, 0);
		return OFFLINE_PWD_CLEAR_SUCCESS;
    }
    
    /* Can't reach here */
	return OFFLINE_PWD_ERR_UNKNOW;
}
