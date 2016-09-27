#ifndef INCLUDED_CYFITTER_H
#define INCLUDED_CYFITTER_H
#include <cydevice_trm.h>

/* BLE_bless_isr */
#define BLE_bless_isr__INTC_CLR_EN_REG CYREG_CM0_ICER
#define BLE_bless_isr__INTC_CLR_PD_REG CYREG_CM0_ICPR
#define BLE_bless_isr__INTC_MASK 0x1000u
#define BLE_bless_isr__INTC_NUMBER 12u
#define BLE_bless_isr__INTC_PRIOR_MASK 0xC0u
#define BLE_bless_isr__INTC_PRIOR_NUM 3u
#define BLE_bless_isr__INTC_PRIOR_REG CYREG_CM0_IPR3
#define BLE_bless_isr__INTC_SET_EN_REG CYREG_CM0_ISER
#define BLE_bless_isr__INTC_SET_PD_REG CYREG_CM0_ISPR

/* BLE_cy_m0s8_ble */
#define BLE_cy_m0s8_ble__ADC_BUMP1 CYREG_BLE_BLERD_ADC_BUMP1
#define BLE_cy_m0s8_ble__ADC_BUMP2 CYREG_BLE_BLERD_ADC_BUMP2
#define BLE_cy_m0s8_ble__ADV_CH_TX_POWER CYREG_BLE_BLELL_ADV_CH_TX_POWER
#define BLE_cy_m0s8_ble__ADV_CONFIG CYREG_BLE_BLELL_ADV_CONFIG
#define BLE_cy_m0s8_ble__ADV_INTERVAL_TIMEOUT CYREG_BLE_BLELL_ADV_INTERVAL_TIMEOUT
#define BLE_cy_m0s8_ble__ADV_INTR CYREG_BLE_BLELL_ADV_INTR
#define BLE_cy_m0s8_ble__ADV_NEXT_INSTANT CYREG_BLE_BLELL_ADV_NEXT_INSTANT
#define BLE_cy_m0s8_ble__ADV_PARAMS CYREG_BLE_BLELL_ADV_PARAMS
#define BLE_cy_m0s8_ble__ADV_SCN_RSP_TX_FIFO CYREG_BLE_BLELL_ADV_SCN_RSP_TX_FIFO
#define BLE_cy_m0s8_ble__ADV_TX_DATA_FIFO CYREG_BLE_BLELL_ADV_TX_DATA_FIFO
#define BLE_cy_m0s8_ble__AGC CYREG_BLE_BLERD_AGC
#define BLE_cy_m0s8_ble__BALUN CYREG_BLE_BLERD_BALUN
#define BLE_cy_m0s8_ble__BB_BUMP1 CYREG_BLE_BLERD_BB_BUMP1
#define BLE_cy_m0s8_ble__BB_BUMP2 CYREG_BLE_BLERD_BB_BUMP2
#define BLE_cy_m0s8_ble__BB_XO CYREG_BLE_BLERD_BB_XO
#define BLE_cy_m0s8_ble__BB_XO_CAPTRIM CYREG_BLE_BLERD_BB_XO_CAPTRIM
#define BLE_cy_m0s8_ble__CE_CNFG_STS_REGISTER CYREG_BLE_BLELL_CE_CNFG_STS_REGISTER
#define BLE_cy_m0s8_ble__CE_LENGTH CYREG_BLE_BLELL_CE_LENGTH
#define BLE_cy_m0s8_ble__CFG_1_FCAL CYREG_BLE_BLERD_CFG_1_FCAL
#define BLE_cy_m0s8_ble__CFG_2_FCAL CYREG_BLE_BLERD_CFG_2_FCAL
#define BLE_cy_m0s8_ble__CFG_3_FCAL CYREG_BLE_BLERD_CFG_3_FCAL
#define BLE_cy_m0s8_ble__CFG_4_FCAL CYREG_BLE_BLERD_CFG_4_FCAL
#define BLE_cy_m0s8_ble__CFG_5_FCAL CYREG_BLE_BLERD_CFG_5_FCAL
#define BLE_cy_m0s8_ble__CFG_6_FCAL CYREG_BLE_BLERD_CFG_6_FCAL
#define BLE_cy_m0s8_ble__CFG1 CYREG_BLE_BLERD_CFG1
#define BLE_cy_m0s8_ble__CFG2 CYREG_BLE_BLERD_CFG2
#define BLE_cy_m0s8_ble__CFGCTRL CYREG_BLE_BLERD_CFGCTRL
#define BLE_cy_m0s8_ble__CLOCK_CONFIG CYREG_BLE_BLELL_CLOCK_CONFIG
#define BLE_cy_m0s8_ble__COMMAND_REGISTER CYREG_BLE_BLELL_COMMAND_REGISTER
#define BLE_cy_m0s8_ble__CONN_CE_COUNTER CYREG_BLE_BLELL_CONN_CE_COUNTER
#define BLE_cy_m0s8_ble__CONN_CE_INSTANT CYREG_BLE_BLELL_CONN_CE_INSTANT
#define BLE_cy_m0s8_ble__CONN_CH_TX_POWER CYREG_BLE_BLELL_CONN_CH_TX_POWER
#define BLE_cy_m0s8_ble__CONN_CONFIG CYREG_BLE_BLELL_CONN_CONFIG
#define BLE_cy_m0s8_ble__CONN_INDEX CYREG_BLE_BLELL_CONN_INDEX
#define BLE_cy_m0s8_ble__CONN_INTERVAL CYREG_BLE_BLELL_CONN_INTERVAL
#define BLE_cy_m0s8_ble__CONN_INTR CYREG_BLE_BLELL_CONN_INTR
#define BLE_cy_m0s8_ble__CONN_INTR_MASK CYREG_BLE_BLELL_CONN_INTR_MASK
#define BLE_cy_m0s8_ble__CONN_PARAM1 CYREG_BLE_BLELL_CONN_PARAM1
#define BLE_cy_m0s8_ble__CONN_PARAM2 CYREG_BLE_BLELL_CONN_PARAM2
#define BLE_cy_m0s8_ble__CONN_REQ_WORD0 CYREG_BLE_BLELL_CONN_REQ_WORD0
#define BLE_cy_m0s8_ble__CONN_REQ_WORD1 CYREG_BLE_BLELL_CONN_REQ_WORD1
#define BLE_cy_m0s8_ble__CONN_REQ_WORD10 CYREG_BLE_BLELL_CONN_REQ_WORD10
#define BLE_cy_m0s8_ble__CONN_REQ_WORD11 CYREG_BLE_BLELL_CONN_REQ_WORD11
#define BLE_cy_m0s8_ble__CONN_REQ_WORD2 CYREG_BLE_BLELL_CONN_REQ_WORD2
#define BLE_cy_m0s8_ble__CONN_REQ_WORD3 CYREG_BLE_BLELL_CONN_REQ_WORD3
#define BLE_cy_m0s8_ble__CONN_REQ_WORD4 CYREG_BLE_BLELL_CONN_REQ_WORD4
#define BLE_cy_m0s8_ble__CONN_REQ_WORD5 CYREG_BLE_BLELL_CONN_REQ_WORD5
#define BLE_cy_m0s8_ble__CONN_REQ_WORD6 CYREG_BLE_BLELL_CONN_REQ_WORD6
#define BLE_cy_m0s8_ble__CONN_REQ_WORD7 CYREG_BLE_BLELL_CONN_REQ_WORD7
#define BLE_cy_m0s8_ble__CONN_REQ_WORD8 CYREG_BLE_BLELL_CONN_REQ_WORD8
#define BLE_cy_m0s8_ble__CONN_REQ_WORD9 CYREG_BLE_BLELL_CONN_REQ_WORD9
#define BLE_cy_m0s8_ble__CONN_RXMEM_BASE_ADDR CYREG_BLE_BLELL_CONN_RXMEM_BASE_ADDR
#define BLE_cy_m0s8_ble__CONN_STATUS CYREG_BLE_BLELL_CONN_STATUS
#define BLE_cy_m0s8_ble__CONN_TXMEM_BASE_ADDR CYREG_BLE_BLELL_CONN_TXMEM_BASE_ADDR
#define BLE_cy_m0s8_ble__CONN_UPDATE_NEW_INTERVAL CYREG_BLE_BLELL_CONN_UPDATE_NEW_INTERVAL
#define BLE_cy_m0s8_ble__CONN_UPDATE_NEW_LATENCY CYREG_BLE_BLELL_CONN_UPDATE_NEW_LATENCY
#define BLE_cy_m0s8_ble__CONN_UPDATE_NEW_SL_INTERVAL CYREG_BLE_BLELL_CONN_UPDATE_NEW_SL_INTERVAL
#define BLE_cy_m0s8_ble__CONN_UPDATE_NEW_SUP_TO CYREG_BLE_BLELL_CONN_UPDATE_NEW_SUP_TO
#define BLE_cy_m0s8_ble__CTR1 CYREG_BLE_BLERD_CTR1
#define BLE_cy_m0s8_ble__DATA_CHANNELS_H0 CYREG_BLE_BLELL_DATA_CHANNELS_H0
#define BLE_cy_m0s8_ble__DATA_CHANNELS_H1 CYREG_BLE_BLELL_DATA_CHANNELS_H1
#define BLE_cy_m0s8_ble__DATA_CHANNELS_L0 CYREG_BLE_BLELL_DATA_CHANNELS_L0
#define BLE_cy_m0s8_ble__DATA_CHANNELS_L1 CYREG_BLE_BLELL_DATA_CHANNELS_L1
#define BLE_cy_m0s8_ble__DATA_CHANNELS_M0 CYREG_BLE_BLELL_DATA_CHANNELS_M0
#define BLE_cy_m0s8_ble__DATA_CHANNELS_M1 CYREG_BLE_BLELL_DATA_CHANNELS_M1
#define BLE_cy_m0s8_ble__DATA_LIST_ACK_UPDATE__STATUS CYREG_BLE_BLELL_DATA_LIST_ACK_UPDATE__STATUS
#define BLE_cy_m0s8_ble__DATA_LIST_SENT_UPDATE__STATUS CYREG_BLE_BLELL_DATA_LIST_SENT_UPDATE__STATUS
#define BLE_cy_m0s8_ble__DATA_MEM_DESCRIPTOR0 CYREG_BLE_BLELL_DATA_MEM_DESCRIPTOR0
#define BLE_cy_m0s8_ble__DATA_MEM_DESCRIPTOR1 CYREG_BLE_BLELL_DATA_MEM_DESCRIPTOR1
#define BLE_cy_m0s8_ble__DATA_MEM_DESCRIPTOR2 CYREG_BLE_BLELL_DATA_MEM_DESCRIPTOR2
#define BLE_cy_m0s8_ble__DATA_MEM_DESCRIPTOR3 CYREG_BLE_BLELL_DATA_MEM_DESCRIPTOR3
#define BLE_cy_m0s8_ble__DATA_MEM_DESCRIPTOR4 CYREG_BLE_BLELL_DATA_MEM_DESCRIPTOR4
#define BLE_cy_m0s8_ble__DATA0 CYREG_BLE_BLELL_DATA0
#define BLE_cy_m0s8_ble__DATA1 CYREG_BLE_BLELL_DATA1
#define BLE_cy_m0s8_ble__DATA10 CYREG_BLE_BLELL_DATA10
#define BLE_cy_m0s8_ble__DATA11 CYREG_BLE_BLELL_DATA11
#define BLE_cy_m0s8_ble__DATA12 CYREG_BLE_BLELL_DATA12
#define BLE_cy_m0s8_ble__DATA13 CYREG_BLE_BLELL_DATA13
#define BLE_cy_m0s8_ble__DATA2 CYREG_BLE_BLELL_DATA2
#define BLE_cy_m0s8_ble__DATA3 CYREG_BLE_BLELL_DATA3
#define BLE_cy_m0s8_ble__DATA4 CYREG_BLE_BLELL_DATA4
#define BLE_cy_m0s8_ble__DATA5 CYREG_BLE_BLELL_DATA5
#define BLE_cy_m0s8_ble__DATA6 CYREG_BLE_BLELL_DATA6
#define BLE_cy_m0s8_ble__DATA7 CYREG_BLE_BLELL_DATA7
#define BLE_cy_m0s8_ble__DATA8 CYREG_BLE_BLELL_DATA8
#define BLE_cy_m0s8_ble__DATA9 CYREG_BLE_BLELL_DATA9
#define BLE_cy_m0s8_ble__DBG_1 CYREG_BLE_BLERD_DBG_1
#define BLE_cy_m0s8_ble__DBG_2 CYREG_BLE_BLERD_DBG_2
#define BLE_cy_m0s8_ble__DBG_3 CYREG_BLE_BLERD_DBG_3
#define BLE_cy_m0s8_ble__DBG_BB CYREG_BLE_BLERD_DBG_BB
#define BLE_cy_m0s8_ble__DBUS CYREG_BLE_BLERD_DBUS
#define BLE_cy_m0s8_ble__DC CYREG_BLE_BLERD_DC
#define BLE_cy_m0s8_ble__DCCAL CYREG_BLE_BLERD_DCCAL
#define BLE_cy_m0s8_ble__DEV_PUB_ADDR_H CYREG_BLE_BLELL_DEV_PUB_ADDR_H
#define BLE_cy_m0s8_ble__DEV_PUB_ADDR_L CYREG_BLE_BLELL_DEV_PUB_ADDR_L
#define BLE_cy_m0s8_ble__DEV_PUB_ADDR_M CYREG_BLE_BLELL_DEV_PUB_ADDR_M
#define BLE_cy_m0s8_ble__DEVICE_RAND_ADDR_H CYREG_BLE_BLELL_DEVICE_RAND_ADDR_H
#define BLE_cy_m0s8_ble__DEVICE_RAND_ADDR_L CYREG_BLE_BLELL_DEVICE_RAND_ADDR_L
#define BLE_cy_m0s8_ble__DEVICE_RAND_ADDR_M CYREG_BLE_BLELL_DEVICE_RAND_ADDR_M
#define BLE_cy_m0s8_ble__DIAG1 CYREG_BLE_BLERD_DIAG1
#define BLE_cy_m0s8_ble__DPLL_CONFIG CYREG_BLE_BLELL_DPLL_CONFIG
#define BLE_cy_m0s8_ble__DSM1 CYREG_BLE_BLERD_DSM1
#define BLE_cy_m0s8_ble__DSM2 CYREG_BLE_BLERD_DSM2
#define BLE_cy_m0s8_ble__DSM3 CYREG_BLE_BLERD_DSM3
#define BLE_cy_m0s8_ble__DSM4 CYREG_BLE_BLERD_DSM4
#define BLE_cy_m0s8_ble__DSM5 CYREG_BLE_BLERD_DSM5
#define BLE_cy_m0s8_ble__DSM6 CYREG_BLE_BLERD_DSM6
#define BLE_cy_m0s8_ble__DTM_RX_PKT_COUNT CYREG_BLE_BLELL_DTM_RX_PKT_COUNT
#define BLE_cy_m0s8_ble__ENC_CONFIG CYREG_BLE_BLELL_ENC_CONFIG
#define BLE_cy_m0s8_ble__ENC_INTR CYREG_BLE_BLELL_ENC_INTR
#define BLE_cy_m0s8_ble__ENC_INTR_EN CYREG_BLE_BLELL_ENC_INTR_EN
#define BLE_cy_m0s8_ble__ENC_KEY0 CYREG_BLE_BLELL_ENC_KEY0
#define BLE_cy_m0s8_ble__ENC_KEY1 CYREG_BLE_BLELL_ENC_KEY1
#define BLE_cy_m0s8_ble__ENC_KEY2 CYREG_BLE_BLELL_ENC_KEY2
#define BLE_cy_m0s8_ble__ENC_KEY3 CYREG_BLE_BLELL_ENC_KEY3
#define BLE_cy_m0s8_ble__ENC_KEY4 CYREG_BLE_BLELL_ENC_KEY4
#define BLE_cy_m0s8_ble__ENC_KEY5 CYREG_BLE_BLELL_ENC_KEY5
#define BLE_cy_m0s8_ble__ENC_KEY6 CYREG_BLE_BLELL_ENC_KEY6
#define BLE_cy_m0s8_ble__ENC_KEY7 CYREG_BLE_BLELL_ENC_KEY7
#define BLE_cy_m0s8_ble__ENC_PARAMS CYREG_BLE_BLELL_ENC_PARAMS
#define BLE_cy_m0s8_ble__EVENT_ENABLE CYREG_BLE_BLELL_EVENT_ENABLE
#define BLE_cy_m0s8_ble__EVENT_INTR CYREG_BLE_BLELL_EVENT_INTR
#define BLE_cy_m0s8_ble__FCAL_TEST CYREG_BLE_BLERD_FCAL_TEST
#define BLE_cy_m0s8_ble__FPD_TEST CYREG_BLE_BLERD_FPD_TEST
#define BLE_cy_m0s8_ble__FSM CYREG_BLE_BLERD_FSM
#define BLE_cy_m0s8_ble__IM CYREG_BLE_BLERD_IM
#define BLE_cy_m0s8_ble__INIT_CONFIG CYREG_BLE_BLELL_INIT_CONFIG
#define BLE_cy_m0s8_ble__INIT_INTERVAL CYREG_BLE_BLELL_INIT_INTERVAL
#define BLE_cy_m0s8_ble__INIT_INTR CYREG_BLE_BLELL_INIT_INTR
#define BLE_cy_m0s8_ble__INIT_NEXT_INSTANT CYREG_BLE_BLELL_INIT_NEXT_INSTANT
#define BLE_cy_m0s8_ble__INIT_PARAM CYREG_BLE_BLELL_INIT_PARAM
#define BLE_cy_m0s8_ble__INIT_SCN_ADV_RX_FIFO CYREG_BLE_BLELL_INIT_SCN_ADV_RX_FIFO
#define BLE_cy_m0s8_ble__INIT_WINDOW CYREG_BLE_BLELL_INIT_WINDOW
#define BLE_cy_m0s8_ble__IQMIS CYREG_BLE_BLERD_IQMIS
#define BLE_cy_m0s8_ble__IV_MASTER0 CYREG_BLE_BLELL_IV_MASTER0
#define BLE_cy_m0s8_ble__IV_MASTER1 CYREG_BLE_BLELL_IV_MASTER1
#define BLE_cy_m0s8_ble__IV_SLAVE0 CYREG_BLE_BLELL_IV_SLAVE0
#define BLE_cy_m0s8_ble__IV_SLAVE1 CYREG_BLE_BLELL_IV_SLAVE1
#define BLE_cy_m0s8_ble__KVCAL CYREG_BLE_BLERD_KVCAL
#define BLE_cy_m0s8_ble__LDO CYREG_BLE_BLERD_LDO
#define BLE_cy_m0s8_ble__LDO_BYPASS CYREG_BLE_BLERD_LDO_BYPASS
#define BLE_cy_m0s8_ble__LE_PING_TIMER_ADDR CYREG_BLE_BLELL_LE_PING_TIMER_ADDR
#define BLE_cy_m0s8_ble__LE_PING_TIMER_NEXT_EXP CYREG_BLE_BLELL_LE_PING_TIMER_NEXT_EXP
#define BLE_cy_m0s8_ble__LE_PING_TIMER_OFFSET CYREG_BLE_BLELL_LE_PING_TIMER_OFFSET
#define BLE_cy_m0s8_ble__LE_PING_TIMER_WRAP_COUNT CYREG_BLE_BLELL_LE_PING_TIMER_WRAP_COUNT
#define BLE_cy_m0s8_ble__LE_RF_TEST_MODE CYREG_BLE_BLELL_LE_RF_TEST_MODE
#define BLE_cy_m0s8_ble__LL_CLK_EN CYREG_BLE_BLESS_LL_CLK_EN
#define BLE_cy_m0s8_ble__LL_DSM_CTRL CYREG_BLE_BLESS_LL_DSM_CTRL
#define BLE_cy_m0s8_ble__LL_DSM_INTR_STAT CYREG_BLE_BLESS_LL_DSM_INTR_STAT
#define BLE_cy_m0s8_ble__LLH_FEATURE_CONFIG CYREG_BLE_BLELL_LLH_FEATURE_CONFIG
#define BLE_cy_m0s8_ble__MIC_IN0 CYREG_BLE_BLELL_MIC_IN0
#define BLE_cy_m0s8_ble__MIC_IN1 CYREG_BLE_BLELL_MIC_IN1
#define BLE_cy_m0s8_ble__MIC_OUT0 CYREG_BLE_BLELL_MIC_OUT0
#define BLE_cy_m0s8_ble__MIC_OUT1 CYREG_BLE_BLELL_MIC_OUT1
#define BLE_cy_m0s8_ble__MODEM CYREG_BLE_BLERD_MODEM
#define BLE_cy_m0s8_ble__MONI CYREG_BLE_BLERD_MONI
#define BLE_cy_m0s8_ble__NEXT_CE_INSTANT CYREG_BLE_BLELL_NEXT_CE_INSTANT
#define BLE_cy_m0s8_ble__NEXT_RESP_TIMER_EXP CYREG_BLE_BLELL_NEXT_RESP_TIMER_EXP
#define BLE_cy_m0s8_ble__NEXT_SUP_TO CYREG_BLE_BLELL_NEXT_SUP_TO
#define BLE_cy_m0s8_ble__OFFSET_TO_FIRST_INSTANT CYREG_BLE_BLELL_OFFSET_TO_FIRST_INSTANT
#define BLE_cy_m0s8_ble__PACKET_COUNTER0 CYREG_BLE_BLELL_PACKET_COUNTER0
#define BLE_cy_m0s8_ble__PACKET_COUNTER1 CYREG_BLE_BLELL_PACKET_COUNTER1
#define BLE_cy_m0s8_ble__PACKET_COUNTER2 CYREG_BLE_BLELL_PACKET_COUNTER2
#define BLE_cy_m0s8_ble__PDU_ACCESS_ADDR_H_REGISTER CYREG_BLE_BLELL_PDU_ACCESS_ADDR_H_REGISTER
#define BLE_cy_m0s8_ble__PDU_ACCESS_ADDR_L_REGISTER CYREG_BLE_BLELL_PDU_ACCESS_ADDR_L_REGISTER
#define BLE_cy_m0s8_ble__PDU_RESP_TIMER CYREG_BLE_BLELL_PDU_RESP_TIMER
#define BLE_cy_m0s8_ble__PEER_ADDR_H CYREG_BLE_BLELL_PEER_ADDR_H
#define BLE_cy_m0s8_ble__PEER_ADDR_L CYREG_BLE_BLELL_PEER_ADDR_L
#define BLE_cy_m0s8_ble__PEER_ADDR_M CYREG_BLE_BLELL_PEER_ADDR_M
#define BLE_cy_m0s8_ble__POC_REG__TIM_CONTROL CYREG_BLE_BLELL_POC_REG__TIM_CONTROL
#define BLE_cy_m0s8_ble__RCCAL CYREG_BLE_BLERD_RCCAL
#define BLE_cy_m0s8_ble__READ_IQ_1 CYREG_BLE_BLERD_READ_IQ_1
#define BLE_cy_m0s8_ble__READ_IQ_2 CYREG_BLE_BLERD_READ_IQ_2
#define BLE_cy_m0s8_ble__READ_IQ_3 CYREG_BLE_BLERD_READ_IQ_3
#define BLE_cy_m0s8_ble__READ_IQ_4 CYREG_BLE_BLERD_READ_IQ_4
#define BLE_cy_m0s8_ble__RECEIVE_TRIG_CTRL CYREG_BLE_BLELL_RECEIVE_TRIG_CTRL
#define BLE_cy_m0s8_ble__RF_CONFIG CYREG_BLE_BLESS_RF_CONFIG
#define BLE_cy_m0s8_ble__RMAP CYREG_BLE_BLERD_RMAP
#define BLE_cy_m0s8_ble__RSSI CYREG_BLE_BLERD_RSSI
#define BLE_cy_m0s8_ble__RX CYREG_BLE_BLERD_RX
#define BLE_cy_m0s8_ble__RX_BUMP1 CYREG_BLE_BLERD_RX_BUMP1
#define BLE_cy_m0s8_ble__RX_BUMP2 CYREG_BLE_BLERD_RX_BUMP2
#define BLE_cy_m0s8_ble__SCAN_CONFIG CYREG_BLE_BLELL_SCAN_CONFIG
#define BLE_cy_m0s8_ble__SCAN_INTERVAL CYREG_BLE_BLELL_SCAN_INTERVAL
#define BLE_cy_m0s8_ble__SCAN_INTR CYREG_BLE_BLELL_SCAN_INTR
#define BLE_cy_m0s8_ble__SCAN_NEXT_INSTANT CYREG_BLE_BLELL_SCAN_NEXT_INSTANT
#define BLE_cy_m0s8_ble__SCAN_PARAM CYREG_BLE_BLELL_SCAN_PARAM
#define BLE_cy_m0s8_ble__SCAN_WINDOW CYREG_BLE_BLELL_SCAN_WINDOW
#define BLE_cy_m0s8_ble__SL_CONN_INTERVAL CYREG_BLE_BLELL_SL_CONN_INTERVAL
#define BLE_cy_m0s8_ble__SLAVE_LATENCY CYREG_BLE_BLELL_SLAVE_LATENCY
#define BLE_cy_m0s8_ble__SLAVE_TIMING_CONTROL CYREG_BLE_BLELL_SLAVE_TIMING_CONTROL
#define BLE_cy_m0s8_ble__SLV_WIN_ADJ CYREG_BLE_BLELL_SLV_WIN_ADJ
#define BLE_cy_m0s8_ble__SUP_TIMEOUT CYREG_BLE_BLELL_SUP_TIMEOUT
#define BLE_cy_m0s8_ble__SY CYREG_BLE_BLERD_SY
#define BLE_cy_m0s8_ble__SY_BUMP1 CYREG_BLE_BLERD_SY_BUMP1
#define BLE_cy_m0s8_ble__SY_BUMP2 CYREG_BLE_BLERD_SY_BUMP2
#define BLE_cy_m0s8_ble__TEST CYREG_BLE_BLERD_TEST
#define BLE_cy_m0s8_ble__TEST2_SY CYREG_BLE_BLERD_TEST2_SY
#define BLE_cy_m0s8_ble__THRSHD1 CYREG_BLE_BLERD_THRSHD1
#define BLE_cy_m0s8_ble__THRSHD2 CYREG_BLE_BLERD_THRSHD2
#define BLE_cy_m0s8_ble__THRSHD3 CYREG_BLE_BLERD_THRSHD3
#define BLE_cy_m0s8_ble__THRSHD4 CYREG_BLE_BLERD_THRSHD4
#define BLE_cy_m0s8_ble__THRSHD5 CYREG_BLE_BLERD_THRSHD5
#define BLE_cy_m0s8_ble__TIM_COUNTER_L CYREG_BLE_BLELL_TIM_COUNTER_L
#define BLE_cy_m0s8_ble__TRANSMIT_WINDOW_OFFSET CYREG_BLE_BLELL_TRANSMIT_WINDOW_OFFSET
#define BLE_cy_m0s8_ble__TRANSMIT_WINDOW_SIZE CYREG_BLE_BLELL_TRANSMIT_WINDOW_SIZE
#define BLE_cy_m0s8_ble__TX CYREG_BLE_BLERD_TX
#define BLE_cy_m0s8_ble__TX_BUMP1 CYREG_BLE_BLERD_TX_BUMP1
#define BLE_cy_m0s8_ble__TX_BUMP2 CYREG_BLE_BLERD_TX_BUMP2
#define BLE_cy_m0s8_ble__TX_EN_EXT_DELAY CYREG_BLE_BLELL_TX_EN_EXT_DELAY
#define BLE_cy_m0s8_ble__TX_RX_ON_DELAY CYREG_BLE_BLELL_TX_RX_ON_DELAY
#define BLE_cy_m0s8_ble__TX_RX_SYNTH_DELAY CYREG_BLE_BLELL_TX_RX_SYNTH_DELAY
#define BLE_cy_m0s8_ble__TXRX_HOP CYREG_BLE_BLELL_TXRX_HOP
#define BLE_cy_m0s8_ble__WAKEUP_CONFIG CYREG_BLE_BLELL_WAKEUP_CONFIG
#define BLE_cy_m0s8_ble__WAKEUP_CONTROL CYREG_BLE_BLELL_WAKEUP_CONTROL
#define BLE_cy_m0s8_ble__WCO_CONFIG CYREG_BLE_BLESS_WCO_CONFIG
#define BLE_cy_m0s8_ble__WCO_STATUS CYREG_BLE_BLESS_WCO_STATUS
#define BLE_cy_m0s8_ble__WCO_TRIM CYREG_BLE_BLESS_WCO_TRIM
#define BLE_cy_m0s8_ble__WHITELIST_BASE_ADDR CYREG_BLE_BLELL_WHITELIST_BASE_ADDR
#define BLE_cy_m0s8_ble__WIN_MIN_STEP_SIZE CYREG_BLE_BLELL_WIN_MIN_STEP_SIZE
#define BLE_cy_m0s8_ble__WINDOW_WIDEN_INTVL CYREG_BLE_BLELL_WINDOW_WIDEN_INTVL
#define BLE_cy_m0s8_ble__WINDOW_WIDEN_WINOFF CYREG_BLE_BLELL_WINDOW_WIDEN_WINOFF
#define BLE_cy_m0s8_ble__WL_ADDR_TYPE CYREG_BLE_BLELL_WL_ADDR_TYPE
#define BLE_cy_m0s8_ble__WL_ENABLE CYREG_BLE_BLELL_WL_ENABLE
#define BLE_cy_m0s8_ble__XTAL_CLK_DIV_CONFIG CYREG_BLE_BLESS_XTAL_CLK_DIV_CONFIG

/* PWM_cy_m0s8_tcpwm_1 */
#define PWM_cy_m0s8_tcpwm_1__CC CYREG_TCPWM_CNT0_CC
#define PWM_cy_m0s8_tcpwm_1__CC_BUFF CYREG_TCPWM_CNT0_CC_BUFF
#define PWM_cy_m0s8_tcpwm_1__COUNTER CYREG_TCPWM_CNT0_COUNTER
#define PWM_cy_m0s8_tcpwm_1__CTRL CYREG_TCPWM_CNT0_CTRL
#define PWM_cy_m0s8_tcpwm_1__INTR CYREG_TCPWM_CNT0_INTR
#define PWM_cy_m0s8_tcpwm_1__INTR_MASK CYREG_TCPWM_CNT0_INTR_MASK
#define PWM_cy_m0s8_tcpwm_1__INTR_MASKED CYREG_TCPWM_CNT0_INTR_MASKED
#define PWM_cy_m0s8_tcpwm_1__INTR_SET CYREG_TCPWM_CNT0_INTR_SET
#define PWM_cy_m0s8_tcpwm_1__PERIOD CYREG_TCPWM_CNT0_PERIOD
#define PWM_cy_m0s8_tcpwm_1__PERIOD_BUFF CYREG_TCPWM_CNT0_PERIOD_BUFF
#define PWM_cy_m0s8_tcpwm_1__STATUS CYREG_TCPWM_CNT0_STATUS
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMD CYREG_TCPWM_CMD
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMDCAPTURE_MASK 0x01u
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMDCAPTURE_SHIFT 0
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMDRELOAD_MASK 0x100u
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMDRELOAD_SHIFT 8
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMDSTART_MASK 0x1000000u
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMDSTART_SHIFT 24
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMDSTOP_MASK 0x10000u
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CMDSTOP_SHIFT 16
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CTRL CYREG_TCPWM_CTRL
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK 0x01u
#define PWM_cy_m0s8_tcpwm_1__TCPWM_CTRL_SHIFT 0
#define PWM_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE CYREG_TCPWM_INTR_CAUSE
#define PWM_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE_MASK 0x01u
#define PWM_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE_SHIFT 0
#define PWM_cy_m0s8_tcpwm_1__TCPWM_NUMBER 0u
#define PWM_cy_m0s8_tcpwm_1__TR_CTRL0 CYREG_TCPWM_CNT0_TR_CTRL0
#define PWM_cy_m0s8_tcpwm_1__TR_CTRL1 CYREG_TCPWM_CNT0_TR_CTRL1
#define PWM_cy_m0s8_tcpwm_1__TR_CTRL2 CYREG_TCPWM_CNT0_TR_CTRL2

/* Clock */
#define Clock__CTRL_REGISTER CYREG_PERI_PCLK_CTL07
#define Clock__DIV_ID 0x00000080u
#define Clock__DIV_REGISTER CYREG_PERI_DIV_16_5_CTL00
#define Clock__FRAC_MASK 0x000000F8u
#define Clock__PA_DIV_ID 0x000000FFu

/* Pin_LED */
#define Pin_LED__0__DR CYREG_GPIO_PRT3_DR
#define Pin_LED__0__DR_CLR CYREG_GPIO_PRT3_DR_CLR
#define Pin_LED__0__DR_INV CYREG_GPIO_PRT3_DR_INV
#define Pin_LED__0__DR_SET CYREG_GPIO_PRT3_DR_SET
#define Pin_LED__0__HSIOM CYREG_HSIOM_PORT_SEL3
#define Pin_LED__0__HSIOM_MASK 0xF0000000u
#define Pin_LED__0__HSIOM_SHIFT 28u
#define Pin_LED__0__INTCFG CYREG_GPIO_PRT3_INTR_CFG
#define Pin_LED__0__INTR CYREG_GPIO_PRT3_INTR
#define Pin_LED__0__INTR_CFG CYREG_GPIO_PRT3_INTR_CFG
#define Pin_LED__0__INTSTAT CYREG_GPIO_PRT3_INTR
#define Pin_LED__0__MASK 0x80u
#define Pin_LED__0__OUT_SEL CYREG_UDB_PA3_CFG10
#define Pin_LED__0__OUT_SEL_SHIFT 14u
#define Pin_LED__0__OUT_SEL_VAL 2u
#define Pin_LED__0__PA__CFG0 CYREG_UDB_PA3_CFG0
#define Pin_LED__0__PA__CFG1 CYREG_UDB_PA3_CFG1
#define Pin_LED__0__PA__CFG10 CYREG_UDB_PA3_CFG10
#define Pin_LED__0__PA__CFG11 CYREG_UDB_PA3_CFG11
#define Pin_LED__0__PA__CFG12 CYREG_UDB_PA3_CFG12
#define Pin_LED__0__PA__CFG13 CYREG_UDB_PA3_CFG13
#define Pin_LED__0__PA__CFG14 CYREG_UDB_PA3_CFG14
#define Pin_LED__0__PA__CFG2 CYREG_UDB_PA3_CFG2
#define Pin_LED__0__PA__CFG3 CYREG_UDB_PA3_CFG3
#define Pin_LED__0__PA__CFG4 CYREG_UDB_PA3_CFG4
#define Pin_LED__0__PA__CFG5 CYREG_UDB_PA3_CFG5
#define Pin_LED__0__PA__CFG6 CYREG_UDB_PA3_CFG6
#define Pin_LED__0__PA__CFG7 CYREG_UDB_PA3_CFG7
#define Pin_LED__0__PA__CFG8 CYREG_UDB_PA3_CFG8
#define Pin_LED__0__PA__CFG9 CYREG_UDB_PA3_CFG9
#define Pin_LED__0__PC CYREG_GPIO_PRT3_PC
#define Pin_LED__0__PC2 CYREG_GPIO_PRT3_PC2
#define Pin_LED__0__PORT 3u
#define Pin_LED__0__PS CYREG_GPIO_PRT3_PS
#define Pin_LED__0__SHIFT 7
#define Pin_LED__DR CYREG_GPIO_PRT3_DR
#define Pin_LED__DR_CLR CYREG_GPIO_PRT3_DR_CLR
#define Pin_LED__DR_INV CYREG_GPIO_PRT3_DR_INV
#define Pin_LED__DR_SET CYREG_GPIO_PRT3_DR_SET
#define Pin_LED__INTCFG CYREG_GPIO_PRT3_INTR_CFG
#define Pin_LED__INTR CYREG_GPIO_PRT3_INTR
#define Pin_LED__INTR_CFG CYREG_GPIO_PRT3_INTR_CFG
#define Pin_LED__INTSTAT CYREG_GPIO_PRT3_INTR
#define Pin_LED__MASK 0x80u
#define Pin_LED__PA__CFG0 CYREG_UDB_PA3_CFG0
#define Pin_LED__PA__CFG1 CYREG_UDB_PA3_CFG1
#define Pin_LED__PA__CFG10 CYREG_UDB_PA3_CFG10
#define Pin_LED__PA__CFG11 CYREG_UDB_PA3_CFG11
#define Pin_LED__PA__CFG12 CYREG_UDB_PA3_CFG12
#define Pin_LED__PA__CFG13 CYREG_UDB_PA3_CFG13
#define Pin_LED__PA__CFG14 CYREG_UDB_PA3_CFG14
#define Pin_LED__PA__CFG2 CYREG_UDB_PA3_CFG2
#define Pin_LED__PA__CFG3 CYREG_UDB_PA3_CFG3
#define Pin_LED__PA__CFG4 CYREG_UDB_PA3_CFG4
#define Pin_LED__PA__CFG5 CYREG_UDB_PA3_CFG5
#define Pin_LED__PA__CFG6 CYREG_UDB_PA3_CFG6
#define Pin_LED__PA__CFG7 CYREG_UDB_PA3_CFG7
#define Pin_LED__PA__CFG8 CYREG_UDB_PA3_CFG8
#define Pin_LED__PA__CFG9 CYREG_UDB_PA3_CFG9
#define Pin_LED__PC CYREG_GPIO_PRT3_PC
#define Pin_LED__PC2 CYREG_GPIO_PRT3_PC2
#define Pin_LED__PORT 3u
#define Pin_LED__PS CYREG_GPIO_PRT3_PS
#define Pin_LED__SHIFT 7

/* Miscellaneous */
#define CY_VERSION "PSoC Creator  3.1 Component Pack 1"
#define CYDEV_BCLK__HFCLK__HZ 48000000U
#define CYDEV_BCLK__HFCLK__KHZ 48000U
#define CYDEV_BCLK__HFCLK__MHZ 48U
#define CYDEV_BCLK__SYSCLK__HZ 48000000U
#define CYDEV_BCLK__SYSCLK__KHZ 48000U
#define CYDEV_BCLK__SYSCLK__MHZ 48U
#define CYDEV_CHIP_DIE_LEOPARD 1u
#define CYDEV_CHIP_DIE_PANTHER 6u
#define CYDEV_CHIP_DIE_PSOC4A 3u
#define CYDEV_CHIP_DIE_PSOC5LP 5u
#define CYDEV_CHIP_DIE_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_PSOC3 1u
#define CYDEV_CHIP_FAMILY_PSOC4 2u
#define CYDEV_CHIP_FAMILY_PSOC5 3u
#define CYDEV_CHIP_FAMILY_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_USED CYDEV_CHIP_FAMILY_PSOC4
#define CYDEV_CHIP_JTAG_ID 0x0E34119Eu
#define CYDEV_CHIP_MEMBER_3A 1u
#define CYDEV_CHIP_MEMBER_4A 3u
#define CYDEV_CHIP_MEMBER_4D 2u
#define CYDEV_CHIP_MEMBER_4F 4u
#define CYDEV_CHIP_MEMBER_5A 6u
#define CYDEV_CHIP_MEMBER_5B 5u
#define CYDEV_CHIP_MEMBER_UNKNOWN 0u
#define CYDEV_CHIP_MEMBER_USED CYDEV_CHIP_MEMBER_4F
#define CYDEV_CHIP_DIE_EXPECT CYDEV_CHIP_MEMBER_USED
#define CYDEV_CHIP_DIE_ACTUAL CYDEV_CHIP_DIE_EXPECT
#define CYDEV_CHIP_REV_LEOPARD_ES1 0u
#define CYDEV_CHIP_REV_LEOPARD_ES2 1u
#define CYDEV_CHIP_REV_LEOPARD_ES3 3u
#define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3u
#define CYDEV_CHIP_REV_PANTHER_ES0 0u
#define CYDEV_CHIP_REV_PANTHER_ES1 1u
#define CYDEV_CHIP_REV_PANTHER_PRODUCTION 1u
#define CYDEV_CHIP_REV_PSOC4A_ES0 17u
#define CYDEV_CHIP_REV_PSOC4A_PRODUCTION 17u
#define CYDEV_CHIP_REV_PSOC5LP_ES0 0u
#define CYDEV_CHIP_REV_PSOC5LP_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_3A_ES1 0u
#define CYDEV_CHIP_REVISION_3A_ES2 1u
#define CYDEV_CHIP_REVISION_3A_ES3 3u
#define CYDEV_CHIP_REVISION_3A_PRODUCTION 3u
#define CYDEV_CHIP_REVISION_4A_ES0 17u
#define CYDEV_CHIP_REVISION_4A_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4D_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_5A_ES0 0u
#define CYDEV_CHIP_REVISION_5A_ES1 1u
#define CYDEV_CHIP_REVISION_5A_PRODUCTION 1u
#define CYDEV_CHIP_REVISION_5B_ES0 0u
#define CYDEV_CHIP_REVISION_5B_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_USED CYDEV_CHIP_REVISION_4F_PRODUCTION
#define CYDEV_CHIP_REV_EXPECT CYDEV_CHIP_REVISION_USED
#define CYDEV_CONFIG_READ_ACCELERATOR 1
#define CYDEV_CONFIG_UNUSED_IO_AllowButWarn 0
#define CYDEV_CONFIG_UNUSED_IO CYDEV_CONFIG_UNUSED_IO_AllowButWarn
#define CYDEV_CONFIG_UNUSED_IO_AllowWithInfo 1
#define CYDEV_CONFIG_UNUSED_IO_Disallowed 2
#define CYDEV_CONFIGURATION_COMPRESSED 1
#define CYDEV_CONFIGURATION_MODE_COMPRESSED 0
#define CYDEV_CONFIGURATION_MODE CYDEV_CONFIGURATION_MODE_COMPRESSED
#define CYDEV_CONFIGURATION_MODE_DMA 2
#define CYDEV_CONFIGURATION_MODE_UNCOMPRESSED 1
#define CYDEV_DEBUG_PROTECT_KILL 4
#define CYDEV_DEBUG_PROTECT_OPEN 1
#define CYDEV_DEBUG_PROTECT CYDEV_DEBUG_PROTECT_OPEN
#define CYDEV_DEBUG_PROTECT_PROTECTED 2
#define CYDEV_DEBUGGING_DPS_Disable 3
#define CYDEV_DEBUGGING_DPS_SWD 2
#define CYDEV_DEBUGGING_DPS CYDEV_DEBUGGING_DPS_SWD
#define CYDEV_DEBUGGING_ENABLE 1
#define CYDEV_HEAP_SIZE 0x80
#define CYDEV_PROJ_TYPE 0
#define CYDEV_PROJ_TYPE_BOOTLOADER 1
#define CYDEV_PROJ_TYPE_LOADABLE 2
#define CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER 3
#define CYDEV_PROJ_TYPE_STANDARD 0
#define CYDEV_STACK_SIZE 0x0800
#define CYDEV_USE_BUNDLED_CMSIS 1
#define CYDEV_VARIABLE_VDDA 1
#define CYDEV_VDDA 3.3
#define CYDEV_VDDA_MV 3300
#define CYDEV_VDDD 3.3
#define CYDEV_VDDD_MV 3300
#define CYDEV_VDDR 3.3
#define CYDEV_VDDR_MV 3300
#define CYIPBLOCK_m0s8bless_VERSION 1
#define CYIPBLOCK_m0s8cpussv2_VERSION 1
#define CYIPBLOCK_m0s8csd_VERSION 1
#define CYIPBLOCK_m0s8ioss_VERSION 1
#define CYIPBLOCK_m0s8lcd_VERSION 2
#define CYIPBLOCK_m0s8lpcomp_VERSION 2
#define CYIPBLOCK_m0s8peri_VERSION 1
#define CYIPBLOCK_m0s8scb_VERSION 2
#define CYIPBLOCK_m0s8srssv2_VERSION 1
#define CYIPBLOCK_m0s8tcpwm_VERSION 2
#define CYIPBLOCK_m0s8udbif_VERSION 1
#define CYIPBLOCK_s8pass4al_VERSION 1
#define CYDEV_BOOTLOADER_ENABLE 0

#endif /* INCLUDED_CYFITTER_H */
