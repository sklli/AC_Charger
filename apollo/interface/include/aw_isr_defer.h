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
 * \brief isr deferral library
 *
 * 使用本服务需要包含头文件 aw_isr_defer.h
 *
 * \par 简单示例
 * \code
 *  #include "aw_isr_defer.h"
 *  #include "aw_vdebug.h"
 *
 *  struct aw_isr_defer_job g_myjob;                    // 中断延迟处理任务
 *
 *  // 中断延迟处理任务
 *  static void defer_job (void *param)
 *  {
 *      aw_kprintf("isr defer job is running...  (param = %d) \n", (int)param);
 *  }
 *
 *  // 软件定时器中断服务函数
 *  void timer_isr (void *p_arg)
 *  {
 *      aw_isr_defer_job_add(&g_myjob);                 // 添加中断延迟处理任务
 *      aw_timer_start(p_arg, aw_ms_to_ticks(500));     // 再次定时500ms
 *  }
 *
 *
 *  int main (void)
 *  {
 *      static aw_timer_t  timer;
 *
 *      // 初始化中断延迟处理任务
 *      aw_isr_defer_job_init(&g_myjob, defer_job, (void *)0x12345678);
 *
 *      // 初始化并启动软件定时器
 *      aw_timer_init(&timer, timer_isr, (void *)&timer);
 *      aw_timer_start(&timer, aw_ms_to_ticks(500));    // 定时500ms
 *  }
 * \endcode
 *
 * // 更多内容待添加。。。
 *
 * \internal
 * \par modification history:
 * - 1.10 13-02-26  zen, modify module name from aw_isr_differ_xxxx to
 *                  aw_isr_defer_xxxx
 * - 1.00 12-12-10  liangyaozhan, first implementation
 * \endinternal
 */

#ifndef __AW_ISR_DEFER_H
#define __AW_ISR_DEFER_H

/**
 * \addtogroup grp_aw_if_isr_defer
 * \copydoc aw_isr_defer.h
 * @{
 */

#include "aw_task.h"
#include "aw_list.h"

/** \brief 延迟处理任务 */
struct aw_isr_defer_job
{
	struct aw_list_head node; /*!< node in the job queue. Empty means done */
    void (*func)(void *);     /*!< function pointer                        */
    void *param;              /*!< parameter                               */
};

/**
 *  \brief 中断延迟处理库初始化
 */
void aw_isr_defer_lib_init( void );

/**
 * \brief 初始化一个 job
 *
 * \param[in] p_job     job 节点指针
 * \param[in] pfunc     job 回调函数
 * \param[in] p_arg     job 回调函数的参数
 *
 * \par 示例
 * \code
 *  #include "aw_isr_defer.h"
 *  #include "aw_vdebug.h"
 *
 *  struct aw_isr_defer_job g_myjob;                    // 中断延迟处理任务
 *
 *  // 中断延迟处理任务
 *  static void defer_job (void *param)
 *  {
 *      aw_kprintf("isr defer job is running...  (param = %d) \n", (int)param);
 *  }
 *
 *  // 软件定时器中断服务函数
 *  void timer_isr (void *p_arg)
 *  {
 *      aw_isr_defer_job_add(&g_myjob);                 // 添加中断延迟处理任务
 *      aw_timer_start(p_arg, aw_ms_to_ticks(500));     // 再次定时500ms
 *  }
 *
 *
 *  int main (void)
 *  {
 *      static aw_timer_t  timer;
 *
 *      // 初始化中断延迟处理任务
 *      aw_isr_defer_job_init(&g_myjob, defer_job, (void *)0x12345678);
 *
 *      // 初始化并启动软件定时器
 *      aw_timer_init(&timer, timer_isr, (void *)&timer);
 *      aw_timer_start(&timer, aw_ms_to_ticks(500));    // 定时500ms
 *  }
 * \endcode
 */
void aw_isr_defer_job_init (struct   aw_isr_defer_job *p_job,
                            void    (*pfunc) (void *),
                            void     *p_arg);

/**
 * \brief 把任务链接到工作队列
 *
 * \param[in] p_job     job 节点指针。
 *
 * \retval  AW_OK    当 list_empty(&p_job->node)为真或者
 *                  (p_job->node.next == NULL && p_job->node.prev == NULL)时，
 *                  成功加到队列中。
 *
 * \retval -EEXIST  加入队列失败。节点仍然在队列中。
 *
 * \attention       节点被处理完后。list_empty(&p_job->node)为真。
 *                  特别地，在使用list_empty(&p_job->node)时，要注意关中断，
 *                  或者使用 aw_isr_defer_job_is_usable()代替。
 *
 * \par 示例
 * 请参考 aw_isr_defer_job_init()
 */
aw_err_t aw_isr_defer_job_add(struct aw_isr_defer_job *p_job);

/**
 * \brief 判断节点是否可用
 *
 * \param[in] p_job  节点指针。
 *
 * \retval  true    节点可用
 * \retval  false   节点不可用, 已经被添加到队列中
 *
 * \par 示例
 * \code
 *  void timer_isr (void *param)
 *  {
 *      if (aw_isr_defer_job_is_usable(&g_myjob) == true) {
 *          // 节点可用，将其添加到队列中
 *          aw_isr_defer_job_add(&g_myjob);
 *      } else {
 *          // 节点不可用，已经被添加到队列中
 *      }
 *  }
 * \endcode
 */
bool_t aw_isr_defer_job_is_usable (struct aw_isr_defer_job *p_job);

/** @} grp_aw_if_isr_defer */

#ifdef __cplusplus
}
#endif  /* __cplusplus  */

#endif  /* __AW_ISR_DEFER_H */

/* end of file */

