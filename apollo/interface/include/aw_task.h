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
 * \brief  多任务接口
 *
 * 使用本服务需要包含头文件 aw_task.h
 *
 * \note 当前平台优先级个数为8, 最低优先级为7，最高优先级为0
 *
 * \par 简单示例
 * \code
 *  #include "apollo.h"
 *  #include "aw_task.h"
 *
 *  AW_TASK_DECL(my_task, 256);                     // 定义任务实体 my_task
 *
 *  // 任务入口函数
 *  void taska (void *p_arg)
 *  {
 *      while (1) {
 *          // ...
 *          aw_task_delay(aw_sys_clkrate_get());    // 延时 1S
 *      }
 *  }
 *
 *  int main() {
 *
 *      // 初始化任务：
 *      AW_TASK_INIT(my_task,                       // 任务实体
 *                   "taskname",                    // 任务名字
 *                   5,                             // 任务优先级
 *                   taska,                         // 任务入口函数
 *                   (void*)1);                     // 任务入口参数
 *
 *      // 启动任务
 *      AW_TASK_STARTUP(my_task);
 *
 *      // 延时 20S.
 *      aw_task_delay(aw_sys_clkrate_get() * 20);
 *
 *      // 终止任务
 *      AW_TASK_TERMINATE(my_task);
 *
 *      return 0;
 *  }
 * \endcode
 *
 * // 更多内容待添加。。。
 *
 * \internal
 * \par modification history:
 * - 1.10 13-02-28  zen, refine the description
 * - 1.00 12-10-23  liangyaozhan, first implementation
 * \endinternal
 */

#ifndef __AW_TASK_H
#define __AW_TASK_H

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus	*/

/**
 * \addtogroup grp_aw_if_task
 * \copydoc aw_task.h
 * @{
 */

#include "aw_psp_task.h"

typedef aw_psp_task_id_t    aw_task_id_t;           /**< 任务ID     */
#define AW_TASK_ID_INVALID  AW_PSP_TASK_ID_INVALID  /**< 无效任务ID  */

/**
 * \brief 定义任务
 *
 * 本宏定义一个任务，在编译时完成任务所需内存的分配; 在运行时，需要分别调用
 * AW_TASK_INIT()和 AW_TASK_STARTUP()完成任务的初始化和启动；调用
 * AW_TASK_TERMINATE()终止任务。
 *
 * 参数 \a task 只能作为宏 AW_TASK_INIT()、 AW_TASK_STARTUP()和
 * AW_TASK_TERMINATE()的参数。
 *
 * AW_TASK_INIT()返回任务的\a task_id,\a task_id 作为函数 aw_task_startup() 、
 * aw_task_terminate()的参数; 这些函数的功能与对应的宏的功能相同，区别在于，函数
 * 使用\a task_id 作为参数，\a task_id 可以作为任务的句柄在函数调用之间传递。
 *
 * 可以使用本宏将任务实体嵌入到另一个数据结构中，这样的话，分配大的数据对象的时
 * 候就自动完成了任务实体的内存分配，如下面的范例所示。
 *
 * \attention 若任务没有跨模块(文件)使用的需求，推荐使用 AW_TASK_DECL_STATIC()
 *            代替本宏，这样可避免模块之间任务命名污染。
 *
 * \attention 除特殊用法外不, 切莫在函数内使用此宏来定义任务，因为在函数内此宏分
 *            配的内存来自于栈上，一旦函数返回，分配的内存被自动回收。在函数内部
 *            使用 AW_TASK_DECL_STATIC()来定义任务。
 *
 * \param task          任务实体，与 AW_TASK_INIT() 、 AW_TASK_STARTUP()和
 *                      AW_TASK_TERMINATE() 一起使用。
 *
 * \param stack_size    任务堆栈大小，此参数必须和 AW_TASK_INIT() 一致!
 *
 * \par 范例：常规用法
 * \code
 *  #include "aw_task.h"
 *
 *  AW_TASK_DECL(my_task, 512);         // 定义任务实体 my_task
 *
 *  // 任务入口函数
 *  void taska (void *p_arg)
 *  {
 *      while (1) {
 *          // ...
 *          aw_task_delay(aw_sys_clkrate_get());    // 延时 1S
 *      }
 *  }
 *
 *  int main() {
 *
 *      // 初始化任务：
 *      AW_TASK_INIT(my_task,       // 任务实体
 *                   "taskname",    // 任务名字
 *                   5,             // 任务优先级
 *                   512,           // 任务堆栈大小(和 AW_TASK_DECL()一致)
 *                   taska,         // 任务入口函数
 *                   (void*)1);     // 任务入口参数
 *
 *      // 启动任务
 *      AW_TASK_STARTUP(my_task);
 *
 *      // 延时 20S.
 *      aw_task_delay(aw_sys_clkrate_get() * 20);
 *
 *      // 终止任务
 *      AW_TASK_TERMINATE(my_task);
 *
 *      return 0;
 *  }
 * \endcode
 *
 * \par 范例：将任务实体嵌入另一个数据结构
 * \code
 *  struct my_struct {
 *      int my_data1;
 *      int my_data2;
 *      AW_TASK_DECL(my_task, 512);     // 任务实体my_task, 堆栈512
 *  };
 *  struct my_struct   my_object;       // 定义大的数据对象
 *
 *  int main() {
 *
 *      //...
 *
 *      // 初始化任务：
 *      AW_TASK_INIT(my_object.my_task, // 任务实体
 *                   "taskname",        // 任务名字
 *                   5,                 // 任务优先级
 *                   512,               // 任务堆栈大小
 *                   taska,             // 任务入口函数
 *                   (void*)1);         // 任务入口参数
 *
 *      // 启动任务
 *      AW_TASK_STARTUP(my_object.my_task);
 *
 *      // 延时 20S.
 *      aw_task_delay(aw_sys_clkrate_get() * 20);
 *
 *      // 终止任务
 *      AW_TASK_TERMINATE(my_object.my_task);
 *
 *      return 0;
 *  }
 * \endcode
 *
 * \sa AW_TASK_DECL_STATIC(), AW_TASK_INIT(), AW_TASK_STARTUP(),
 *     AW_TASK_TERMINATE()
 * \hideinitializer
 */
#define AW_TASK_DECL(task, stack_size)  \
            AW_PSP_TASK_DECL(task, stack_size)

/**
 * \brief 定义任务(静态)
 *
 * 本宏与 AW_TASK_DECL() 的功能相同，区别在于，AW_TASK_DECL_STATIC() 在定义
 * 任务所需内存时，使用关键字 \b static ，如此一来，便可以将任务实体的作用域限制在
 * 模块内(文件内)从而避免模块之间的任务命名冲突;还可以在函数内使用本宏定义任务。
 *
 * \param task          任务实体，与 AW_TASK_INIT() 、 AW_TASK_STARTUP()和
 *                      AW_TASK_TERMINATE() 一起使用。
 *
 * \param stack_size    任务栈大小, 此参数必须和 AW_TASK_INIT() 一致!
 *
 * \par 范例
 * \code
 *  #include "aw_task.h"
 *
 *  // 任务入口函数
 *  void taska (void *p_arg)
 *  {
 *      while (1) {
 *          // ...
 *          aw_task_delay(aw_sys_clkrate_get());    // 延时 1S
 *      }
 *  }
 *
 *  int main() {
 *
 *      // 定义任务实体 my_task
 *      AW_TASK_DECL_STATIC(my_task, 512);
 *
 *      // 初始化任务：
 *      AW_TASK_INIT(my_task,       // 任务实体
 *                   "taskname",    // 任务名字
 *                   5,             // 任务优先级
 *                   512,           // 任务堆栈大小
 *                   taska,         // 任务入口函数
 *                   (void*)1);     // 任务入口参数
 *
 *      // 启动任务
 *      AW_TASK_STARTUP(my_task);
 *
 *      // 延时 20S.
 *      aw_task_delay(aw_sys_clkrate_get() * 20);
 *
 *      // 终止任务
 *      AW_TASK_TERMINATE(my_task);
 *
 *      return 0;
 *  }
 *  \endcode
 *
 * \sa AW_TASK_DECL(), AW_TASK_INIT(), AW_TASK_STARTUP(), AW_TASK_TERMINATE()
 * \hideinitializer
 */
#define AW_TASK_DECL_STATIC(task, stack_size) \
            AW_PSP_TASK_DECL_STATIC(task, stack_size)

/**
 * \brief 引用任务
 *
 * 本宏引入一个任务，相当于使用关键 \b extern 声明一个外部符号。如果要使用另一个
 * 模块(文件)中定义的任务，首先使用本宏引入该任务，然后就可以操作该任务。被引入
 * 任务应当是使用 AW_TASK_DECL() 定义的。
 *
 * \param task  任务实体
 *
 * \par 范例
 * \code
 *  #include "aw_task.h"
 *
 *  AW_TASK_IMPORT(his_task);    // 引入任务 his_task
 *
 *  int func () {
 *      AW_TASK_TERMINATE(his_task); // 终止任务 his_task
 *  }
 *  \endcode
 *
 * \sa AW_TASK_DECL(), AW_TASK_INIT(), AW_TASK_STARTUP(), AW_TASK_TERMINATE()
 * \hideinitializer
 */
#define AW_TASK_IMPORT(task)    AW_PSP_TASK_IMPORT(task)

/**
 * \brief 初始化任务
 *
 * \param task      任务实体，由 AW_TASK_DECL() 或 AW_TASK_DECL_STATIC() 定义
 * \param name      任务名称
 * \param priority  任务优先级, 0 优先级最高, 最低优先级可通过
 *                  aw_task_lowest_priority() 获取。
 *                  当前平台优先级个数为8, 最低优先级为7, 最高优先级为0。
 *
 * \param stack_size    任务堆栈大小，此参数必须和 AW_TASK_DECL() 一致!
 * \param func      入口函数
 * \param arg       入口参数
 *
 * \return          成功返回任务的ID，失败则返回NULL
 *
 * \note 参数 \a priority 是用户任务的优先级。系统保留了若干优先级供内部系统服务
 * 和设备驱动使用。对于这些系统服务和设备驱动，使用宏 AW_TASK_SYS_PRIORITY(pri)
 * 表示系统任务优先级。
 *
 * \par 范例
 * 见 AW_TASK_DECL() 和 AW_TASK_DECL_STATIC()
 *
 * \sa AW_TASK_DECL(), AW_TASK_DECL_STATIC(), AW_TASK_INIT(), AW_TASK_STARTUP(),
 *  AW_TASK_TERMINATE()
 * \hideinitializer
 */
#define AW_TASK_INIT(task, name, priority, stack_size, func, arg)    \
            AW_PSP_TASK_INIT(task, name, priority, stack_size, func, arg)


/**
 * \brief 系统任务优先级
 *
 * 系统保留了若干优先级供内部系统服务和设备驱动使用,对于这些系统服务和设备驱动,
 * 凡是涉及优先级的API，都必须使用宏 AW_TASK_SYS_PRIORITY(pri) 来表示系统任务的
 * 优先级,\a pri 为 0 时优先级最高,\a pri 值越大优先级越低。
 * 所有系统任务优先级都高于用户任务优先级。
 *
 * \par 示例
 * \code
 *  #include "aw_task.h"
 *
 *  AW_TASK_DECL_STATIC(sys_task, 512);
 *
 *  AW_TASK_INIT(sys_task,
 *               "sys_task",
 *               AW_TASK_SYS_PRIORITY(0),   // 最高系统优先级
 *               512,               // 任务堆栈大小
 *               sys_task_entry,
 *               NULL;
 * \endcode
 * \hideinitializer
 */
#define AW_TASK_SYS_PRIORITY(priority)  \
            AW_PSP_TASK_SYS_PRIORITY(priority)

/**
 * \brief 启动任务
 *
 * \param task  任务实体，由 AW_TASK_DECL() 或 AW_TASK_DECL_STATIC() 定义
 *
 * \par 范例
 * \code
 *  #include "aw_task.h"
 *
 *  // 任务入口函数
 *  void taska (void *p_arg)
 *  {
 *      while (1) {
 *          // ...
 *          aw_task_delay(aw_sys_clkrate_get());    // 延时 1S
 *      }
 *  }
 *
 *  int main() {
 *
 *      aw_task_id_t task_id;   // 任务ID
 *
 *      // 定义任务实体 my_task
 *      AW_TASK_DECL_STATIC(my_task, 512);
 *
 *      // 初始化任务：
 *      task_id = AW_TASK_INIT(my_task,       // 任务实体
 *                             "taskname",    // 任务名字
 *                             5,             // 任务优先级
 *                             512,           // 任务堆栈大小
 *                             taska,         // 任务入口函数
 *                             (void*)1);     // 任务入口参数
 *
 *      if (task_id != NULL) {
 *
 *          // 启动任务
 *          aw_task_startup(task_id);
 *
 *          // 延时 20S.
 *          aw_task_delay(aw_sys_clkrate_get() * 20);
 *
 *          // 终止任务
 *          aw_task_terminate(task_id);
 *      }
 *
 *      return 0;
 *  }
 * \endcode
 *
 * \sa AW_TASK_DECL(), AW_TASK_DECL_STATIC(), AW_TASK_INIT(), AW_TASK_STARTUP(),
 *  AW_TASK_TERMINATE()
 * \hideinitializer
 */
#define AW_TASK_STARTUP(task)           AW_PSP_TASK_STARTUP(task)

/**
 * \brief 终止任务
 *
 * \param task  任务实体，由 AW_TASK_DECL() 或 AW_TASK_DECL_STATIC() 定义
 *
 * \retval  AW_OK   成功终止
 * \retval  -EPERM  终止操作被拒绝
 * \hideinitializer
 */
#define AW_TASK_TERMINATE(task)         AW_PSP_TASK_TERMINATE(task)

#define AW_TASK_VALID(task)             AW_PSP_TASK_VALID(task)
#define AW_TASK_SET_INVALID(task)       AW_PSP_TASK_SET_INVALID(task)

#define AW_TASK_DELAY(tick)             AW_PSP_TASK_DELAY(tick)

#define AW_TASK_SUSPEND(task)           AW_PSP_TASK_SUSPEND(task)
#define AW_TASK_RESUME(task)            AW_PSP_TASK_RESUME(task)

/**
 * \brief 使调用者任务进入删除安全模式
 *
 * 尝试删除调用者任务的其它任务将会阻塞，直到调用者任务调用 AW_TASK_UNSAFE()
 * 退出删除安全模式。
 *
 * \return  AW_OK   总是成功
 * \hideinitializer
 */
#define AW_TASK_SAFE()                  AW_PSP_TASK_SAFE()

/**
 * \brief 使调用者任务退出删除安全模式
 *
 * 尝试删除调用者任务的其它任务将会阻塞，直到调用者任务调用 AW_TASK_UNSAFE()
 * 退出删除安全模式。
 *
 * \return  AW_OK   总是成功
 * \hideinitializer
 */
#define AW_TASK_UNSAFE()            AW_PSP_TASK_UNSAFE()

/**
 * \brief 获取指定任务的栈空间的使用情况
 *
 * \param task  任务实体，由 AW_TASK_DECL() 或 AW_TASK_DECL_STATIC() 定义
 * \par 范例
 * \code
 *  #include "aw_task_stkchk.h"
 *  #include "aw_vdebug.h"
 *   
 *  AW_TASK_DECL_STATIC(task1,512);
 *  AW_TASK_DECL_STATIC(task2,512);
 *
 *  task1_entry () 
 *  {
 *      unsigned int total;
 *      unsigned int free;
 *
 *      AW_FOREVER {
 *          //... do something
 *          AW_TASK_STACK_CHECK(task2,&total,&free);
 *          aw_kprintf("task2 stack total: %d bytes, free：%d bytes",total,free);
 *      }
 *  }
 *
 *  task2_entry () 
 *  {
 *      AW_FOREVER {
 *       
 *          //... do something
 *       
 *      }
 *  }
 *
 *  void aw_main()
 *  {
 *      
 *      AW_TASK_INIT(task1,   "task1",   5, 512, task1_entry,   0); 
 *      AW_TASK_INIT(task1,   "task1",   6, 512, task2_entry,   0); 
 *      AW_TASK_STARTUP(task1);
 *      AW_TASK_STARTUP(task2);
 *      
 *      AW_FOREVER {
 *          //... do something
 *          aw_mdelay(10);
 *      }
 *  }
 * \endcode
 *
 * \return  0:  stack check success.
 *
 * \note  结果仅供堆栈空间大小分配时的参考，时间越长，检测结果越准确。 
 */     
#define AW_TASK_STACK_CHECK(task,p_total,p_free)  \
                                        AW_PSP_TASK_STACK_CHECK(task,p_total,p_free)

/**
 * \brief 获取当前任务的栈空间的使用情况
 *
 * \param task  任务实体，由 AW_TASK_DECL() 或 AW_TASK_DECL_STATIC() 定义
 * \par 范例
 * \code
 *  #include "aw_task_stkchk.h"
 *  #include "aw_vdebug.h"
 *   
 *  AW_TASK_DECL_STATIC(task1,512);
 *
 *
 *  task1_entry () 
 *  {
 *      unsigned int total;
 *      unsigned int free;
 *
 *      AW_FOREVER {
 *          //... do something
 *          AW_TASK_STACK_CHECK_SELF(&total,&free);
 *          aw_kprintf("task1 stack total: %d bytes, free：%d bytes",total,free);
 *          aw_mdelay(10);
 *      }
 *  }
 *
 *  void aw_main()
 *  {
 *      
 *      AW_TASK_INIT(task1,   "task1",   5, 512, task1_entry,   0); 
 *
 *      AW_TASK_STARTUP(task1);
 *
 *      AW_FOREVER {
 *          //... do something
 *          aw_mdelay(10);
 *      }
 *  }
 * \endcode
 *
 * \return  0:  stack check success.
 *
 * \note  结果仅供堆栈空间大小分配时的参考，时间越长，检测结果越准确。 
 */  
#define AW_TASK_STACK_CHECK_SELF(p_total,p_free)  \
                                 AW_PSP_TASK_STACK_CHECK_SELF(p_total,p_free)
 
/**
 * \brief 创建任务
 *
 * \param name      任务名称
 * \param priority  任务优先级, 0 优先级最高, 最低优先级可通过
 *                  aw_task_lowest_priority() 获取。
 *
 * \param stack_size  任务堆栈大小
 * \param func        入口函数
 * \param arg         入口参数
 *
 * \return          成功返回任务的ID，失败则返回NULL
 *
 * \note 参数 \a priority 是用户任务的优先级。系统保留了若干优先级供内部系统服务
 * 和设备驱动使用。对于这些系统服务和设备驱动，使用宏 AW_TASK_SYS_PRIORITY(pri)
 * 表示系统任务优先级。
 */
aw_task_id_t aw_task_create (const char     *name,
                             int             priority,
                             size_t          stack_size,
                             aw_pfuncvoid_t  func,
                             void           *arg);

/**
 * \brief 终止任务并删除相关的内存
 *
 * \param   task_id 任务ID
 *
 * \return  AW_OK   成功
 */
aw_err_t aw_task_delete (aw_task_id_t task_id);

/**
 * \brief 设置/获取任务用户数据
 *
 * tls: thread local storage.
 *
 * \param   task_id 任务ID
 *
 * \return  旧的/当前的tls
 */
void *aw_task_set_tls(aw_task_id_t task_id, void *data );
void *aw_task_get_tls(aw_task_id_t task_id );


void aw_task_yield( void );

/**
 * \brief 启动任务
 *
 * \param   task_id     任务ID，由 AW_TASK_INIT() 返回
 *
 * \par 范例
 *  见 AW_TASK_DECL()
 *
 * \sa AW_TASK_DECL(), AW_TASK_DECL_STATIC(), AW_TASK_INIT(), AW_TASK_STARTUP(),
 *  AW_TASK_TERMINATE()
 */
aw_err_t aw_task_startup(aw_task_id_t task_id);

/**
 * \brief 终止任务
 *
 * \param   task_id     任务ID，由 AW_TASK_INIT() 返回
 *
 * \retval  AW_OK       成功终止
 * \retval  -EPERM      终止操作被拒绝
 */
aw_err_t aw_task_terminate(aw_task_id_t task_id);

/**
 * \brief 任务廷时
 *
 * 任务廷时，单位为时间节拍数，使用 aw_sys_clkrate_get() 获取系统时钟节拍频率，
 * 即1秒钟的系统时钟节拍数。
 *
 * - 秒级延时表示方式为       (aw_sys_clkrate_get() * delay_x_s);
 * - 毫秒级延时的表示方式为   (aw_sys_clkrate_get() * delay_x_ms / 1000);
 * - 可使用 aw_ms_to_ticks() 将毫秒数转换为节拍数
 *
 * \par 范例
 * \code
 *  #include "aw_task.h"
 *  void taska( int a, int b )
 *  {
 *      while (1){
 *          // ...
 *          aw_task_delay(aw_sys_clkrate_get() * 1);            // 延时1S
 *          aw_task_delay(aw_sys_clkrate_get() * 100 / 1000);   // 延时100ms
 *          aw_task_delay(aw_ms_to_ticks(100));                 // 延时100ms
 *      }
 *  }
 *
 *  \endcode
 *  \sa aw_sys_clkrate_get()
 *  \hideinitializer
 */
void aw_task_delay(int ticks);

/**
 * \brief 动态地设置任务的优先级
 *
 * \param[in] 任务ID，由 AW_TASK_INIT() 返回
 * \param[in] new_priority 新的优先级
 *                         当前平台优先级个数为8, 最低优先级为7, 最高优先级为0。
 *
 * \return    AW_OK    成功设置
 * \return   -EPERM   操作被拒绝
 */
aw_err_t aw_task_priority_set(aw_task_id_t task_id, unsigned int new_priority);

/**
 * \brief 获取优先级数目
 *
 * 优先级的范围是：0 (最高优先级) ~ aw_task_priority_count() - 1 (最低优先级)
 * \note 当前平台优先级个数为8, 最低优先级为7, 最高优先级为0
 *
 * \return  优先级数目
 * \hideinitializer
 */
unsigned int aw_task_priority_count(void);

/**
 * \brief 获取最低的优先级
 *
 * 最低优先级 = aw_task_priority_count() - 1
 * \note 当前平台优先级个数为8, 最低优先级为7, 最高优先级为0。
 *
 * \return  最低优先级
 * \hideinitializer
 */
unsigned int aw_task_lowest_priority(void);

/**
 * \brief 获取当前任务的ID
 *
 * \attention   不可在中断中调用本函数
 *
 * \return  当前任务的ID
 */
aw_task_id_t aw_task_id_self(void);

/**
 * \brief 获取当前任务的名字
 *
 * \attention   不可在中断中调用本函数
 *
 * \return  当前任务的名字
 * \hideinitializer
 */
const char * aw_task_name_get(aw_task_id_t task_id);

/**
 * \brief 获取当前任务的优先级
 *
 * \attention   不可在中断中调用本函数
 *
 * \return  当前任务的优先级
 * \hideinitializer
 */
unsigned int aw_task_priority_get(aw_task_id_t task_id);

bool_t aw_task_valid (aw_task_id_t task_id);

void aw_task_set_invalid (aw_task_id_t *p_task_id);

/** @} grp_aw_if_task */

#ifdef __cplusplus
}
#endif	/* __cplusplus 	*/

#endif    /* __AW_TASK_H */

/* end of file */
