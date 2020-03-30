#include "bk_test.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */
#define TUYA_TEST_LEN  32
#define TUYA_TEST_ADDR 0x70000

uint8_t tmpBuf1[TUYA_TEST_LEN] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x00};
uint8_t tmpBuf2[TUYA_TEST_LEN] = {0};




/*********************************************************
FN: 
*/
//static void bk_timers_timeout_handler(void * p_context)
//{
//    BK_PRINTF("Start!");
//}

void rtc_handler(void)
{
    BK_PRINTF("Start!");
}

//static void test_env(void)
//{
//    uint32_t i_boot_times = NULL;
//    char *c_old_boot_times, c_new_boot_times[11] = {0};

//    /* get the boot count number from Env */
//    c_old_boot_times = ef_get_env("boot_times");
////    assert_param(c_old_boot_times);
//    i_boot_times = atol(c_old_boot_times);
//    /* boot count +1 */
//    i_boot_times ++;
//    NRFS_PRINTF("The system now boot %d times\n\r", i_boot_times);
//    /* interger to string */
//    sprintf(c_new_boot_times,"%ld", (long)i_boot_times);
//    /* set and store the boot count number to Env */
//    ef_set_env("boot_times", c_new_boot_times);
//    ef_save_env();
//}
void bk_test(void)
{
//    bk_gpio_init();
    
//    bk_local_timer_start();
//    bk_timer_start_0(BK_TIMER0, 1000, 100);
    
//    bk_timer_create(&app_timer_id, 1000, APP_TIMER_MODE_REPEATED, nrfs_timers_timeout_handler);
//    bk_timer_start_0(app_timer_id);
    
//    bk_flash_erase(TUYA_TEST_ADDR, 1);
//    BK_PRINTF("bk_flash_read1");
//    for(uint16_t idx=0; idx<1000; idx++)
//    bk_flash_read(TUYA_TEST_ADDR, tmpBuf2, 8);
//    BK_PRINTF("bk_flash_read2");
//    BK_HEXDUMP("tmpBuf2-1", tmpBuf2, 8);
//    bk_flash_write(TUYA_TEST_ADDR, tmpBuf1, 8);
//    bk_flash_read(TUYA_TEST_ADDR, tmpBuf2, 5);
//    BK_HEXDUMP("tmpBuf2-2", tmpBuf2, 8);
//    bk_flash_read(TUYA_TEST_ADDR, tmpBuf2, 8);
//    BK_HEXDUMP("tmpBuf2-3", tmpBuf2, 8);
    
//    test_env();
//    ef_set_env_blob("test", tmpBuf1, 5);
//    ef_del_env("test");
//    ef_get_env_blob("test", tmpBuf2, 5, NULL);
//    ef_set_env_blob("test", tmpBuf1, 5);
//    ef_del_env("test");
//    ef_get_env_blob("test", tmpBuf2, 5, NULL);
//    ef_set_env_blob("test", tmpBuf1, 5);
//    ef_del_env("test");
//    ef_get_env_blob("test", tmpBuf2, 5, NULL);
//    ef_set_env_blob("test", tmpBuf1, 5);
//    ef_del_env("test");
//    ef_get_env_blob("test", tmpBuf2, 5, NULL);
    
//    uint8_t tmp[300];
//    for(int idx=0; idx<300; idx++)
//    {
//        tmp[idx] = idx;
//    }
//    BK_HEXDUMP("tmp", tmp, 200);
}









