/*******************************************************************************
*                                 Apollo
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LED设备操作接口
 *
 * 使用本服务需要包含头文件 aw_led.h
 *
 * \par 使用示例
 * \code
 * #include "aw_led.h"
 *
 * aw_led_set(0, true);     // 点亮LED0
 * aw_led_set(0, false);    // 熄灭LED0
 *
 * aw_led_on(1);            // 点亮LED1
 * aw_led_off(1);           // 熄灭LED1
 * \endcode
 *
 * \internal
 * \par modification history
 * - 1.01 14-11-25  ops, redefine the led framework by service.
 * - 1.00 13-03-11  orz, first implementation
 * \endinternal
 */

#ifndef __AW_LED_H
#define __AW_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup grp_aw_if_led
 * \copydoc aw_led.h
 * @{
 */

#include "aw_types.h"

/**
 * \brief 设置LED状态
 * \param id    LED编号, 可用编号由具体平台决定
 * \param on    LED的点亮状态, true为点亮, false为熄灭
 *
 * \return AW_OK   操作成功
 *        -ENODEV  设备不存在
 */
aw_err_t aw_led_set (int id, bool_t on);

/**
 * \brief 点亮LED
 * \param id    LED编号, 可用编号由具体平台决定.
 *
 * \return AW_OK   操作成功
 *        -ENODEV  设备不存在
 */
aw_err_t aw_led_on  (int id);

/**
 * \brief 熄灭LED
 * \param id    LED编号, 可用编号由具体平台决定
 *
 * \return AW_OK   操作成功
 *        -ENODEV  设备不存在
 */
aw_err_t aw_led_off (int id);

/**
 * \brief 翻转LED
 * \param id    LED编号, 可用编号由具体平台决定
 *
 * \return AW_OK   操作成功
 *        -ENODEV  设备不存在
 */
aw_err_t aw_led_toggle (int id);

/** @} grp_aw_if_led */

#ifdef __cplusplus
}
#endif

#endif /* __AW_LED_H */

/* end of file */
