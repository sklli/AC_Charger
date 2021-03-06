/*******************************************************************************
*                                 Apollo
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2013 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      anywhere.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief Ethernet NE2000 driver.
 *
 * \internal
 * \par modification history:
 * - 1.00 15-12-01, cod, first implementation.
 * \endinternal
 */

#ifndef __AWBL_EMAC_NE2000_H
#define __AWBL_EMAC_NE2000_H

#include "aw_spinlock.h"
#include "aw_assert.h"
#include "aw_task.h"                    /* 多任务服务 */
#include "aw_msgq.h"
#include "aw_sem.h"

#include "awbl_miibus.h"

/* Sitara DriverLib Header Files required for this interface driver. */
#include "netif/ethernetif.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus  */

#define AWBL_NE2000_NAME          "awbl_emac_ne2000"

#define __AX88796_EN

/* Support internal phy */
#define __AX88796_PHY_SUPPORT
/*****************************************************************************/
/*
 * The NE2000 register space is broken up into pages. There are
 * three available pages, selectable via two bits in the command register,
 * which is always mapped in the same place in all three pages.
 * Page 0 contains the main runtime registers. Page 1 contains the
 * physical address and multicast hash table registers. Page 2
 * contains diagnostic registers which aren't needed in normal operation.
 * Page 3 is invalid and should never be modified.
 */

/* Page 0 registers, read */

#define __ENE_CR    0x00    /* Command register */
//#define __ENE_CLDA0    0x01    /* Current local DMA address 0 */
//#define __ENE_CLDA1    0x02    /* Current local DMA address 1 */
#define __ENE_BNRY   0x03    /* Boundary pointer */
#define __ENE_TSR    0x04    /* TX status register */
#define __ENE_NCR    0x05    /* Collision count */
//#define __ENE_FIFO    0x06    /* FIFO */
#define __ENE_ISR    0x07    /* Interrupt status register */
#define __ENE_CRDA0  0x08    /* Current remote DMA address 0 */
#define __ENE_CRDA1  0x09    /* Current remode DMA address 1 */
#define __ENE_RSVD0  0x0A
#define __ENE_RSVD1  0x0B
#define __ENE_RSR    0x0C    /* RX status register */
#define __ENE_CNTR0  0x0D    /* Frame alignment error counter */
#define __ENE_CNTR1  0x0E    /* CRC error counter */
#define __ENE_CNTR2  0x0F    /* Missed packet counter */

/* Page 0 registers, write */

#define __ENE_PSTART    0x01    /* Page start */
#define __ENE_PSTOP     0x02    /* Page stop */
/* #define __ENE_BNRY    0x03       Boundary pointer */
#define __ENE_TPSR      0x04    /* TX page start address */
#define __ENE_TBCR0     0x05    /* TX byte count 0 */
#define __ENE_TBCR1     0x06    /* TX byte count 1 */
/* #define __ENE_ISR    0x07       Interrupt status register */
#define __ENE_RSAR0     0x08    /* Remote start address 0 */
#define __ENE_RSAR1     0x09    /* Remote start address 1 */
#define __ENE_RBCR0     0x0A    /* Remote byte count 0 */
#define __ENE_RBCR1     0x0B    /* Remote byte count 1 */
#define __ENE_RCR       0x0C    /* RX configuration register */
#define __ENE_TCR       0x0D    /* TX configuration register */
#define __ENE_DCR       0x0E    /* data configuration register */
#define __ENE_IMR       0x0F    /* Interrupt mask register */


/* Command register */

#define __ENE_CR_STOP       0x01    /* Software reset */
#define __ENE_CR_START      0x02    /* Start ST-NIC */
#define __ENE_CR_TXP        0x04    /* Initiate packet transmission */
#define __ENE_CR_RDMA_CMD   0x38    /* RDMA command */
#define __ENE_CR_PAGESEL    0xC0    /* Page select */

#define __ENE_RDMA_READ     0x08    /* Remote read */
#define __ENE_RDMA_WRITE    0x10    /* Remote write */
#define __ENE_RDMA_SEND     0x18    /* Send packet */
#define __ENE_RDMA_ABORT    0x20    /* Abort/complete RDMA */

#define __ENE_PAGESEL_0     0x00    /* Select page 0 */
#define __ENE_PAGESEL_1     0x40    /* Select page 1 */
#define __ENE_PAGESEL_2     0x80    /* Select page 2 */
#define __ENE_PAGESEL_3     0xC0    /* reserved */


/* ISR register bit */

#define __ENE_ISR_PRX    0x01    /* Packet received */
#define __ENE_ISR_PTX    0x02    /* Packet sent */
#define __ENE_ISR_RXE    0x04    /* Receive error */
#define __ENE_ISR_TXE    0x08    /* Transmit error */
#define __ENE_ISR_OVW    0x10    /* RX overrun */
#define __ENE_ISR_CNT    0x20    /* Counter overflow */
#define __ENE_ISR_RDC    0x40    /* Remote DMA complete */
#define __ENE_ISR_RST    0x80    /* Reset complete */

/* Data configuration register */

#define __ENE_DCR_WTS    0x01    /* 0 = byte transfers, 1 = word transfers */
#define __ENE_DCR_BOS    0x02    /* 0 = little endian, 1 = big endian */
#define __ENE_DCR_LAS    0x04    /* 0 = dual 16 bit DMA, 1 = single 32 bit DMA*/
#define __ENE_DCR_LS     0x08    /* 0 = loopback, 1 = normal */
#define __ENE_DCR_ARM    0x10    /* autoinit remote DMA */
#define __ENE_DCR_FT     0x60    /* FIFO threshold select */

#define __ENE_FIFOTHR_1WORD     0x00
#define __ENE_FIFOTHR_2WORDS    0x20
#define __ENE_FIFOTHR_4WORDS    0x40
#define __ENE_FIFOTHR_6WORDS    0x60


/* TX configuration register */

#define __ENE_TCR_CRC   0x01    /* 1 = inhibit CRC generation */
#define __ENE_TCR_LB    0x06    /* loopback control */
#define __ENE_TCR_ATD   0x08    /* Auto transmit disable (flow control) */
#define __ENE_TCR_OFST  0x10    /* Collision offset enable */

#define __ENE_LOOP_OFF    0x00    /* Normal operation, loopback off */
#define __ENE_LOOP_NIC    0x02    /* NIC/MAC loopback */
#define __ENE_LOOP_ENDEC  0x04    /* ENDEC loopback */
#define __ENE_LOOP_EXT    0x06    /* External loopback */


/* TX status register */

#define __ENE_TSR_PTX    0x01    /* Frame transmitted */
#define __ENE_TSR_COL    0x04    /* Collision detected */
#define __ENE_TSR_ABT    0x08    /* Abort due to excess collisions */
#define __ENE_TSR_CRS    0x10    /* Carrier sense lost */
#define __ENE_TSR_FU     0x20    /* FIFO underrun */
#define __ENE_TSR_CDH    0x40    /* CD heartbeat failure */
#define __ENE_TSR_OWC    0x80    /* Out of window (late) collision */


/* RX configuration register */

#define __ENE_RCR_SEP    0x01    /* Save bad frames */
#define __ENE_RCR_AR     0x02    /* Accept runt frames */
#define __ENE_RCR_AB     0x04    /* Accept broadcast frames */
#define __ENE_RCR_AM     0x08    /* Accept multicast frames */
#define __ENE_RCR_PRO    0x10    /* Accept all unicasts */
#define __ENE_RCR_MON    0x20    /* Monitor mode */


/* RX status register */

#define __ENE_RSR_PRX    0x01    /* Frame received */
#define __ENE_RSR_CRC    0x02    /* CRC error */
#define __ENE_RSR_FAE    0x04    /* Frame alignment error */
#define __ENE_RSR_FO     0x08    /* FIFO overrun */
#define __ENE_RSR_MPA    0x10    /* Missed frame */
#define __ENE_RSR_PHY    0x20    /* 0 = unicast, 1 = multicast */
#define __ENE_RSR_DIS    0x40    /* Receiver disabled (in monitor mode) */
#define __ENE_RSR_DFR    0x80    /* Deferring, jabber */


/* Page 1 registers, read/write */

#define __ENE_PAR0    0x01    /* Station address 0 */
#define __ENE_PAR1    0x02    /* Station address 1 */
#define __ENE_PAR2    0x03    /* Station address 2 */
#define __ENE_PAR3    0x04    /* Station address 3 */
#define __ENE_PAR4    0x05    /* Station address 4 */
#define __ENE_PAR5    0x06    /* Station address 5 */
#define __ENE_CURR    0x07    /* Current page */
#define __ENE_MAR0    0x08    /* Multicast hash table 0 */
#define __ENE_MAR1    0x09    /* Multicast hash table 1 */
#define __ENE_MAR2    0x0A    /* Multicast hash table 2 */
#define __ENE_MAR3    0x0B    /* Multicast hash table 3 */
#define __ENE_MAR4    0x0C    /* Multicast hash table 4 */
#define __ENE_MAR5    0x0D    /* Multicast hash table 5 */
#define __ENE_MAR6    0x0E    /* Multicast hash table 6 */
#define __ENE_MAR7    0x0F    /* Multicast hash table 7 */

/* NE2000 data port offset */
#define __ENE_IOPORT    0x10

/* NE2000 reset port */
#define __ENE_RESET    0x1F
/******************************************************************************
  寄存器读写宏定义
******************************************************************************/
#define __ENE_SHIFT(offset)  (p_this->reg_offset[offset])

#define __ENEREG_BYTE_READ(base, offset) \
        (*((volatile uint8_t *)((base) + __ENE_SHIFT(offset))))

#define __ENEREG_SHORT_READ(base, offset)  \
        (*((volatile uint16_t *)((base) + __ENE_SHIFT(offset))))

#define __ENEREG_BYTE_WRITE(base, offset, data)  \
        (*((volatile uint8_t *)((base) + __ENE_SHIFT(offset))))  = \
            (uint8_t)(data)

#define __ENEREG_SHORT_WRITE(base, offset, data)  \
        (*((volatile uint16_t *)((base) + __ENE_SHIFT(offset)))) = \
            (uint16_t)(data)

/*****************************************************************************/
struct __ene_rxpkt_hdr {
    uint8_t  status;       /* rx packet status */
    uint8_t  next_page;    /* page next pkt starts at */
    uint16_t len;          /* frame length */
};

/**
 * Helper struct to hold private data used to operate the ethernet interface.
 */
struct awbl_ne2000_emacif {
    struct ethernet_if ethif;        /**< Ethernet struct  */

    /* emac output mutex */
    AW_MUTEX_DECL(ene_out_mutex);    /**< output mutex  */

};

/**
 * \brief NE2000 设备信息
 */
struct awbl_ne2000_devinfo {

    uint32_t  emac_unit;                   /**< MAC address nvram unit */
    char     *p_hwaddr_name;              /**< MAC address nvram name*/

    uint32_t  regbase;                    /**< register base  */

    uint32_t  eint_gpio;                  /**< EINT_GPIO   */

    bool_t    byte_access;                /**< Byte Acess  */

    int       create_mii_bus_id;          /**< Create MII Bus ID */
    int       phy_use_mii_bus_id;         /**< Use MII Bus ID */

    uint32_t *reg_offset;                 /**< NE2000 register offset */

    void (*pfunc_plfm_init)(void);        /**< platform init */

};

/**
 * \brief NE2000 设备实例
 */
struct awbl_ne2000_dev {
    struct awbl_dev             ene_dev;        /**< ne2000 dev super  */
    struct awbl_ne2000_emacif   emac_if;        /**< ne2000 ethernet interface  */

    struct awbl_mii_master      mii_master;     /**< MII master  */
    struct awbl_dev            *p_ene_mii_dev;  /**<  Get MII device */

    awbl_miibus_read_t          mii_phy_read;   /**<  PHY read function */
    awbl_miibus_write_t         mii_phy_write;  /**<  PHY write function */
    AW_SEMB_DECL(               mii_dev_semb);  /**<  PHY device lock */

    uint8_t                     tx_start_page;  /**<  ENE TX start page  */
    uint8_t                     rx_start_page;  /**<  ENE RX start page  */
    uint8_t                     rx_curr_page;   /**<  ENE RX current page  */
    uint8_t                     stop_page;      /**<  ENE stop page  */

    uint16_t                    tx1, tx2;       /**<  current tx len  */
    uint8_t                     txing;          /**<  indicate current tx is tx1 or tx2, or no txing   */

    bool_t                      rx_handling;    /**<  current rx handling, TRUE : handling   */
    bool_t                      rx_overrun;     /**< rx overrun handling, TRUE : handling   */

    uint8_t                     int_mask;       /**< save current interrupt mask   */

    aw_spinlock_isr_t           lock;           /**< device lock */

    uint32_t                    phy_mode;       /**< PHY mode */
    uint32_t                    phy_stat;       /**< PHY stat */

    uint32_t                   *reg_offset;     /**< NE2000 register offset */

    uint8_t                     ene_init_flg;   /* init flag */

};

#ifdef __cplusplus
}
#endif  /* __cplusplus  */

#endif  /* __AWBL_EMAC_NE2000_H */


