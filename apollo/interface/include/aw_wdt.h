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
 * \brief Apollo WATCHDOG(看门狗) 标准接口
 *
 * 使用本服务需要包含以下头文件：
 * \code
 * #include "aw_wdt.h"
 * \endcode
 *
 * 本服务提供了对软件看门狗添加、喂狗的接口，如果系统未在规定时间内
 * 进行喂狗，那么就会导致系统复位。
 *
 * 注意：在系统中规定，硬件看门狗的时间必须大于1ms。
 *
 * \par 简单示例
 * \code
 * #include "aw_wdt.h"
 *
 * // 设置看门狗时间为1200ms
 * struct awbl_wdt   wdt_dev;
 * aw_wdt_add(&wdt_dev, 1200);
 *
 * // 喂狗
 * aw_wdt_feed(&wdt_dev);
 * \endcode
 *
 * // 更多内容待添加。。。
 *
 * \internal
 * \par modification history:
 * - 1.00 14-08-27  may, first implementation
 * \endinternal
 */


#ifndef __AW_WDT_H
#define __AW_WDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apollo.h"
#include "aw_timer.h"

/**
 * \brief AWBus watchdog 设备
 */
struct awbl_wdt {

    /** \brief watchdog 时间 */
    uint32_t   period_ms;

    /** \brief 用于看门狗定时的软件定时器 */
    aw_timer_t sftimer;
};

/**
 * \addtogroup grp_aw_if_watchdog
 * \copydoc aw_wdt.h
 * @{
 */
/**
 * \brief    添加看门狗，并设置看门狗时间
 *
 * \param    p_wdt:   watchdog设备
 * \param    t_ms:    watchdog时间(ms)
 *
 * \retval   AW_OK:   成功
 * \retval  -EINVAL   p_wdt值无效
 * \retval  -EPERM    操作不允许
 */
aw_err_t  aw_wdt_add (struct awbl_wdt  *p_wdt, uint32_t  t_ms);

/**
 * \brief   执行喂狗操作
 *
 * \param   p_wdt:    watchdog设备
 *
 * \retval  AW_OK:    成功
 * \retval  -EINVAL   p_wdt值无效
 * \retval  -EPERM    操作不允许
 */
aw_err_t  aw_wdt_feed (struct awbl_wdt  *p_wdt);

/** @} grp_aw_if_watchdog */

#ifdef __cplusplus
}
#endif


#endif /* __AW_WDT_H */
