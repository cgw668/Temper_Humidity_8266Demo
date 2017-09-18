#pragma  once

// Advertising intervals (units of 625us, 160=100ms)
#define HID_INITIAL_ADV_INT_MIN               48          //30ms
#define HID_INITIAL_ADV_INT_MAX               80          //50ms

#define HID_HIGH_LATENCY_ADV_INT_MIN          32          //20ms
#define HID_HIGH_LATENCY_ADV_INT_MAX          48          //30ms

#define HID_PERMINANT_ADV_INT_MIN             1600        //1s
#define HID_PERMINANT_ADV_INT_MAX             1600        //1.5s

// Advertising timeouts in msec
#define HID_INITIAL_ADV_TIMEOUT               (80 * 1000)       //
#define HID_HIGH_LATENCY_ADV_TIMEOUT          (30 * 1000)
#define HID_PERMINANT_ADV_TIMEOUT             0
#define HID_DIRECTED_ADV_TIMEOUT              (1280)



#define HID_DEV_STATE_UNCONNECTED           0x00
#define HID_DEV_STATE_CONNECTED             0x01
#define HID_DEV_STATE_INIT_ADV              0x02
#define HID_DEV_STATE_DIRECT_ADV            0x03
#define HID_DEV_STATE_HIGH_ADV              0x04
#define HID_DEV_STATE_BONDED                0x05


#define HID_DEV_DATA_LEN                    8
#define HID_DEV_Q_SIZE                      10


typedef struct
{
 u8 id;
 u8 type;
 u8 len;
 u8 data[HID_DEV_DATA_LEN];
} hidDevReport_t;


typedef struct {
    u8 readPtr;
    u8 writePtr;
    hidDevReport_t cmdQ[HID_DEV_Q_SIZE];
} hidDevRptQ_t;



/*********************************************************************
 * @fn      hidDev_cmdQInit
 *
 * @brief   init the Queue
 *
 * @param   None
 *
 * @return  None
 */
void hidDev_cmdQInit(hidDevRptQ_t *q);


/*********************************************************************
 * @fn      hidDev_cmdQGetSize
 *
 * @brief   get the queue size
 *
 * @param   None
 *
 * @return  None
 */
u8 hidDev_cmdQGetSize(hidDevRptQ_t *q);


/*********************************************************************
 * @fn      hidDev_cmdQPush
 *
 * @brief   push item into Queue
 *
 * @param   Queue
 *
 * @param   item
 *
 * @return  status
 */
u8 hidDev_cmdQPush(hidDevRptQ_t *q, hidDevReport_t *item);

/*********************************************************************
 * @fn      hidDev_cmdQPop
 *
 * @brief   get the first item in Queue
 *
 * @param   Queue
 *
 * @return  item
 */
hidDevReport_t *hidDev_cmdQPop(hidDevRptQ_t *q);

/*********************************************************************
 * @fn      gap_peripheral_setListener
 *
 * @brief   Set callback to peripheral
 *
 * @param   None
 *
 * @return  None
 */
void hidDev_registerCb(ev_task_callback_t cb);

/*********************************************************************
 * @fn      hidDev_onStateChanged
 *
 * @brief   State changed callback function from GAP
 *
 * @param   state - Current new state
 *
 * @return  None
 */
void hidDev_onStateChanged(gap_stateChanged_t* pStateChangedEvt);

/*********************************************************************
 * @fn      hidDev_init
 *
 * @brief   Initialize HID profile
 *
 * @param   None
 *
 * @return  None
 */
void hidDev_init(void);


ble_sts_t hids_report(u16 connHandle, u8 id,  u8 type, u8 len, u8 *pData);