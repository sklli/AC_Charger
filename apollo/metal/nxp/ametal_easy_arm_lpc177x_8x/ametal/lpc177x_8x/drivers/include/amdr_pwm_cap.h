/*******************************************************************************
*                                 Apollo
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief PWM驱动，服务CAP标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-20  snk, first implementation.
 * \endinternal
 */

#ifndef __AMDR_PWM_CAP_H
#define __AMDR_PWM_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_cap.h"
#include "amhw_pwm.h"

/**
 * \addtogroup amdr_if_pwm_cap
 * \copydoc amdr_pwm_cap.h
 * @{
 */

/**
 * \brief CAP功能相关的GPIO信息
 */
typedef struct amdr_pwm_cap_ioinfo {
    uint32_t gpio;                          /**< \brief 对应的GPIO管脚  */
    uint32_t func;                          /**< \brief 为PWM功能时的GPIO功能设置   */
    uint32_t dfunc;                         /**< \brief 禁能管脚PWM功能时的默认GPIO功能设置 */
} amdr_pwm_cap_ioinfo_t;



/**
 * \brief pwm捕获功能相关的设备信息
 */
typedef struct amdr_pwm_cap_devinfo {
    amhw_pwm_t                *p_hw_pwm;   /**< \brief 指向pwm寄存器块的指针 */
    uint8_t                    inum;       /**< \brief  中断号          */
    amdr_pwm_cap_ioinfo_t     *p_ioinfo;      /**< \brief 指向PWM管脚信息结构体            */
    uint8_t                    channels_num;  /**< \brief 使用的通道数，最大为4 */
} amdr_pwm_cap_devinfo_t;
 
/**
 * \brief pwm捕获功能设备结构体
 */
typedef struct amdr_pwm_cap_dev {
    am_cap_serv_t           cap_serv;       /**< \brief 标准CAP服务  */
    
    struct {   
        am_cap_callback_t   callback_func;  /**< \brief 回调函数 */
        void               *p_arg;          /**< \brief 回调函数的用户参数   */
    }callback_info[2];                      /**< \brief 存储最大2通道的回调函数信息 */
    
    /** \brief 指向pwm捕获功能设备信息常量的指针 */
    const amdr_pwm_cap_devinfo_t  *p_devinfo; 

} amdr_pwm_cap_dev_t;

/**
 * \brief 初始化pwm为捕获功能
 *
 * \param[in] p_dev     : 指向pwm捕获功能设备的指针
 * \param[in] p_devinfo : 指向pwm捕获功能设备信息常量的指针
 *
 * \return CAP标准服务操作句柄，值为NULL时表明初始化失败
 */
am_cap_handle_t amdr_pwm_cap_init(amdr_pwm_cap_dev_t           *p_dev,
                                  const amdr_pwm_cap_devinfo_t *p_devinfo);

/**
 * \brief 不使用mcpwm捕获功能时，解初始化mcpwm捕获功能，释放相关资源
 * \param[in] p_dev : 指向mcpwm捕获功能设备的指针
 * \return 无
 */
void amdr_pwm_cap_deinit(amdr_pwm_cap_dev_t *p_dev);


/** 
 * @} 
 */

 
#ifdef __cplusplus
}
#endif

#endif /* __AMDR_PWM_CAP_H */

/* end of file */
