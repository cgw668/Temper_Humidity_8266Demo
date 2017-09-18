#include "../../proj/tl_common.h"
#include "../../proj_lib/blt_ll/blt_ll.h"
#include "../../proj_lib/ble_l2cap/ble_ll_ota.h"

#if(__PROJECT_8266_BLE_REMOTE__)

typedef struct
{
  /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  u16 intervalMin;
  /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  u16 intervalMax;
  /** Number of LL latency connection events (0x0000 - 0x03e8) */
  u16 latency;
  /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
  u16 timeout;
} gap_periConnectParams_t;

const u16 clientCharacterCfgUUID = GATT_UUID_CLIENT_CHAR_CFG;

const u16 extReportRefUUID = GATT_UUID_EXT_REPORT_REF;

const u16 reportRefUUID = GATT_UUID_REPORT_REF;

const u16 characterPresentFormatUUID = GATT_UUID_CHAR_PRESENT_FORMAT;

const u16 my_primaryServiceUUID = GATT_UUID_PRIMARY_SERVICE;

static const u16 my_characterUUID = GATT_UUID_CHARACTER;

const u16 my_devServiceUUID = SERVICE_UUID_DEVICE_INFORMATION;

const u16 my_PnPUUID = CHARACTERISTIC_UUID_PNP_ID;

const u16 my_devNameUUID = GATT_UUID_DEVICE_NAME;

//device information
const u16 my_gapServiceUUID = SERVICE_UUID_GENERIC_ACCESS;
// Device Name Characteristic Properties
static u8 my_devNameCharacter = CHAR_PROP_READ | CHAR_PROP_NOTIFY;
// Appearance Characteristic Properties
const u16 my_appearanceUIID = 0x2a01;
const u16 my_periConnParamUUID = 0x2a04;
static u8 my_appearanceCharacter = CHAR_PROP_READ;
// Peripheral Preferred Connection Parameters Characteristic Properties
static u8 my_periConnParamChar = CHAR_PROP_READ;
u16 my_appearance = GAP_APPEARE_UNKNOWN;
gap_periConnectParams_t my_periConnParameters = {20, 40, 0, 1000};

const u8	my_devName [] = {'w', 'b', 's', 'h', 'b', 'd'};

// Device Name Characteristic Properties
static u8 my_PnPCharacter = CHAR_PROP_READ;


const u8	my_PnPtrs [] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};

//////////////////////// Battery /////////////////////////////////////////////////
const u16 my_batServiceUUID       			= SERVICE_UUID_BATTERY;
static u8 my_batProp 						= CHAR_PROP_READ | CHAR_PROP_NOTIFY;
const u16 my_batCharUUID       				= CHARACTERISTIC_UUID_BATTERY_LEVEL;
u8 		  my_batVal[1] 						= {99};

//////////////////////// HID /////////////////////////////////////////////////////

const u16 my_hidServiceUUID       			= SERVICE_UUID_HUMAN_INTERFACE_DEVICE;
const u16 my_SppDataServer2ClientUUID		= TELINK_SPP_DATA_SERVER2CLIENT;
const u16 my_SppDataClient2ServiceUUID		= TELINK_SPP_DATA_CLIENT2SERVER;


const u16 my_SppNameUUID		= GATT_UUID_CHAR_USER_DESC;
const u8  my_SppName[] = {'m', 'y', 'S', 'P', 'P'};

const u16 hidServiceUUID           = SERVICE_UUID_HUMAN_INTERFACE_DEVICE;
const u16 hidProtocolModeUUID      = CHARACTERISTIC_UUID_HID_PROTOCOL_MODE;
const u16 hidReportUUID            = CHARACTERISTIC_UUID_HID_REPORT;
const u16 hidReportMapUUID         = CHARACTERISTIC_UUID_HID_REPORT_MAP;
const u16 hidbootKeyInReportUUID   = CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT;
const u16 hidbootKeyOutReportUUID  = CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT;
const u16 hidbootMouseInReportUUID = CHARACTERISTIC_UUID_HID_BOOT_MOUSE_INPUT;
const u16 hidinformationUUID       = CHARACTERISTIC_UUID_HID_INFORMATION;
const u16 hidCtrlPointUUID         = CHARACTERISTIC_UUID_HID_CONTROL_POINT;
const u16 hidIncludeUUID           = GATT_UUID_INCLUDE;
// Protocol Mode characteristic variables
static u8 protocolModeProp = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP;
u8 protocolMode = DFLT_HID_PROTOCOL_MODE;

// Include attribute (Battery service)
static u16 include[3] = {0x001B, 0x001F, SERVICE_UUID_BATTERY};

// Key in Report characteristic variables
u8 reportKeyInProp = CHAR_PROP_READ | CHAR_PROP_NOTIFY;
u8 reportKeyIn[8];
u8 reportKeyInCCC[2];
// HID Report Reference characteristic descriptor, key input
static u8 reportRefKeyIn[2] =
             { HID_REPORT_ID_KEYBOARD_INPUT, HID_REPORT_TYPE_INPUT };

// Key out Report characteristic variables
u8 reportKeyOutProp = CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP;
u8 reportKeyOut[1];
u8 reportKeyOutCCC[2];
static u8 reportRefKeyOut[2] =
             { HID_REPORT_ID_KEYBOARD_INPUT, HID_REPORT_TYPE_OUTPUT };

// Consumer Control input Report
static u8 reportConsumerControlInProp = CHAR_PROP_READ | CHAR_PROP_NOTIFY;
static u8 reportConsumerControlIn[2];
static u8 reportConsumerControlInCCC[2];
static u8 reportRefConsumerControlIn[2] = { HID_REPORT_ID_CONSUME_CONTROL_INPUT, HID_REPORT_TYPE_INPUT };

// HID Report characteristic, mouse input
static u8 reportMouseInProp = CHAR_PROP_READ | CHAR_PROP_NOTIFY;
static u8 reportMouseIn[4];
static u8 reportMouseInCCC[2];
static u8 reportRefMouseIn[2] = { HID_REPORT_ID_MOUSE_INPUT, HID_REPORT_TYPE_INPUT };


// Boot Keyboard Input Report
static u8 bootKeyInReportProp = CHAR_PROP_READ | CHAR_PROP_NOTIFY;
static u8 bootKeyInReport;
static u8 bootKeyInReportCCC[2];

// Boot Keyboard Output Report
static u8 bootKeyOutReportProp = CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP;
static u8 bootKeyOutReport;
static u8 bootKeyOutReportCCC[2];

// Boot Mouse Input Report
static u8 bootMouseInReportProp = CHAR_PROP_READ | CHAR_PROP_WRITE;
static u8 bootMouseInReport;
static u8 bootMouseInReportCCC[2];

// HID Information characteristic
static u8 hidInfoProps = CHAR_PROP_READ;
const u8 hidInformation[] =
{
  U16_LO(0x0111), U16_HI(0x0111),             // bcdHID (USB HID version)
  0x00,                                       // bCountryCode
  0x01                                        // Flags
};

// HID Control Point characteristic
static u8 controlPointProp = CHAR_PROP_WRITE_WITHOUT_RSP;
static u8 controlPoint;

// HID Report Map characteristic
static u8 reportMapProp = CHAR_PROP_READ;
// Keyboard report descriptor (using format for Boot interface descriptor)

static const u8 reportMap[] =
{
    0x05, 0x01,  // Usage Page (Generic Desktop)
    0x09, 0x02,  // Usage (Mouse)
    0xA1, 0x01,  // Collection (Application)
    0x85, 0x01,  // Report Id (1)
    0x09, 0x01,  //   Usage (Pointer)
    0xA1, 0x00,  //   Collection (Physical)
    0x05, 0x09,  //     Usage Page (Buttons)
    0x19, 0x01,  //     Usage Minimum (01) - Button 1
    0x29, 0x03,  //     Usage Maximum (03) - Button 3
    0x15, 0x00,  //     Logical Minimum (0)
    0x25, 0x01,  //     Logical Maximum (1)
    0x75, 0x01,  //     Report Size (1)
    0x95, 0x03,  //     Report Count (3)
    0x81, 0x02,  //     Input (Data, Variable, Absolute) - Button states
    0x75, 0x05,  //     Report Size (5)
    0x95, 0x01,  //     Report Count (1)
    0x81, 0x01,  //     Input (Constant) - Padding or Reserved bits
    0x05, 0x01,  //     Usage Page (Generic Desktop)
    0x09, 0x30,  //     Usage (X)
    0x09, 0x31,  //     Usage (Y)
    0x09, 0x38,  //     Usage (Wheel)
    0x15, 0x81,  //     Logical Minimum (-127)
    0x25, 0x7F,  //     Logical Maximum (127)
    0x75, 0x08,  //     Report Size (8)
    0x95, 0x03,  //     Report Count (3)
    0x81, 0x06,  //     Input (Data, Variable, Relative) - X & Y coordinate
    0xC0,        //   End Collection
    0xC0,        // End Collection

    #if 1
    // Report ID 3
    0x05, 0x0C,   // Usage Page (Consumer)
    0x09, 0x01,   // Usage (Consumer Control)
    0xA1, 0x01,   // Collection (Application)
    0x85, 0x02,   //     Report Id (3)

    0x09, 0xE9,   //   Usage (Volume Up)
    0x09, 0xEA,   //   Usage (Volume Down)
    0x15, 0x00,   //   Logical Min (0)
    0x75, 0x01,   //   Report Size (1)
    0x95, 0x02,   //   Report Count (2)
    0x81, 0x02,   //   Input (Data, Var, Abs)
    0x09, 0xE2,   //   Usage (Mute)
    0x09, 0x30,   //   Usage (Power)
    0x09, 0x41,   //   Usage (Menu Pick)
    0x09, 0x42,   //   Usage (Menu Up)
    0x09, 0x43,   //   Usage (Menu Down)
    0x09, 0x44,   //   Usage (Menu Left)
    0x09, 0x45,   //   Usage (Menu Right)
    0x0a, 0x23, 0x02,   //   Usage (HOME)
    0x09, 0xB4,   //   Usage (Rewind)
    0x09, 0xB5,   //   Usage (Scan Next)
    0x09, 0xB6,   //   Usage (Scan Prev)
    0x09, 0xB7,   //   Usage (Stop)
    0x15, 0x01,   //   Logical Min (1)
    0x25, 0x0C,   //   Logical Max (12)
    0x75, 0x04,   //   Report Size (4)
    0x95, 0x01,   //   Report Count (1)
    0x81, 0x00,   //   Input (Data, Ary, Abs)
    0x09, 0x80,   //   Usage (Selection)
    0xA1, 0x02,   //   Collection (Logical)
    0x05, 0x09,   //     Usage Pg (Button)
    0x19, 0x01,   //     Usage Min (Button 1)
    0x29, 0x03,   //     Usage Max (Button 3)
    0x15, 0x01,   //     Logical Min (1)
    0x25, 0x03,   //     Logical Max (3)
    0x75, 0x02,   //     Report Size (2)
    0x81, 0x00,   //     Input (Data, Ary, Abs)
    0xC0,         //   End Collection
    0x81, 0x03,   //   Input (Const, Var, Abs)
    0xC0,         // End Collection
#endif


    0x05, 0x01,     // Usage Pg (Generic Desktop)
    0x09, 0x06,     // Usage (Keyboard)
    0xA1, 0x01,     // Collection: (Application)
    0x85, 0x03,     // Report Id (3 for keyboard)
                  //
    0x05, 0x07,     // Usage Pg (Key Codes)
    0x19, 0xE0,     // Usage Min (224)  VK_CTRL:0xe0
    0x29, 0xE7,     // Usage Max (231)  VK_RWIN:0xe7
    0x15, 0x00,     // Log Min (0)
    0x25, 0x01,     // Log Max (1)
                  //
                  // Modifier byte
    0x75, 0x01,     // Report Size (1)   1 bit * 8
    0x95, 0x08,     // Report Count (8)
    0x81, 0x02,     // Input: (Data, Variable, Absolute)
                  //
                  // Reserved byte
    0x95, 0x01,     // Report Count (1)
    0x75, 0x08,     // Report Size (8)
    0x81, 0x01,     // Input: (Constant)

    //keyboard output
    //5 bit led ctrl: NumLock CapsLock ScrollLock Compose kana
    0x95, 0x05,    //Report Count (5)
    0x75, 0x01,    //Report Size (1)
    0x05, 0x08,    //Usage Pg (LEDs )
    0x19, 0x01,    //Usage Min
    0x29, 0x05,    //Usage Max
    0x91, 0x02,    //Output (Data, Variable, Absolute)
    //3 bit reserved
    0x95, 0x01,    //Report Count (1)
    0x75, 0x03,    //Report Size (3)
    0x91, 0x01,    //Output (Constant)

	// Key arrays (6 bytes)
    0x95, 0x06,     // Report Count (6)
    0x75, 0x08,     // Report Size (8)
    0x15, 0x00,     // Log Min (0)
    0x25, 0xF1,     // Log Max (241)
    0x05, 0x07,     // Usage Pg (Key Codes)
    0x19, 0x00,     // Usage Min (0)
    0x29, 0xf1,     // Usage Max (241)
    0x81, 0x00,     // Input: (Data, Array)

    0xC0,            // End Collection
};

// HID External Report Reference Descriptor for report map
static u16 extServiceUUID;


/////////////////////////////////////////////////////////
const u8 my_AudioUUID[16]   = TELINK_AUDIO_UUID_SERVICE;
const u8 my_MicUUID[16]		= TELINK_MIC_DATA;
const u8 my_SpeakerUUID[16]	= TELINK_SPEAKER_DATA;
const u8 my_OtaUUID[16]		= TELINK_SPP_DATA_OTA;
const u16 userdesc_UUID		= GATT_UUID_CHAR_USER_DESC;


static u8 my_MicProp 		= CHAR_PROP_READ | CHAR_PROP_NOTIFY;
static u8 my_SpeakerProp 	= CHAR_PROP_WRITE_WITHOUT_RSP;
static u8 my_OtaProp		= CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP;


u8		my_MicData 		= 0x80;
u8		my_SpeakerData 	= 0x81;
u8	 	my_OtaData 		= 0x00;

const u8  my_MicName[] = {'M', 'i', 'c'};
const u8  my_SpeakerName[] = {'S', 'p', 'e', 'a', 'k', 'e', 'r'};
const u8  my_OtaName[] = {'O', 'T', 'A'};


int		speakerWrite(void *p)
{
	return 1;
}

// TM : to modify
const attribute_t my_Attributes[] = {
	{50,0,0,0,0,0},	//

	// gatt
	{7,2,2,2,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_gapServiceUUID), 0},
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&my_devNameCharacter), 0},
	{0,2,sizeof (my_devName), sizeof (my_devName),(u8*)(&my_devNameUUID), 			(u8*)(my_devName), 0},
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&my_appearanceCharacter), 0},
	{0,2,sizeof (my_appearance), sizeof (my_appearance),(u8*)(&my_appearanceUIID), 	(u8*)(&my_appearance), 0},
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&my_periConnParamChar), 0},
	{0,2,sizeof (my_periConnParameters), sizeof (my_periConnParameters),(u8*)(&my_periConnParamUUID), 	(u8*)(&my_periConnParameters), 0},

	//////////////// all handles below need +7 /////////////////////////////////////////////////////////////////////
	// 8. Device Information Service
	{3,2,2,2,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_devServiceUUID), 0},
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&my_PnPCharacter), 0},
	{0,2,sizeof (my_PnPtrs), sizeof (my_PnPtrs),(u8*)(&my_PnPUUID), 			(u8*)(my_PnPtrs), 0},

#if 1
	/////////////////////////////////// 4. HID Service /////////////////////////////////////////////////////////
	{27,2,2,2,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_hidServiceUUID), 0},

	//include battery service
	{0,2,sizeof(include),sizeof(include),(u8*)(&hidIncludeUUID), 	(u8*)(include), 0},

	//protocol mode
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&protocolModeProp), 0},				//prop
	{0,2, sizeof(protocolMode), sizeof(protocolMode),(u8*)(&hidProtocolModeUUID), 	(u8*)(&protocolMode), 0},	//value

	//15. boot keyboard input report (char-val-client)
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&bootKeyInReportProp), 0},				//prop
	{0,2, sizeof(bootKeyInReport), sizeof(bootKeyInReport),(u8*)(&hidbootKeyInReportUUID), 	(u8*)(&bootKeyInReport), 0},	//value
	{0,2, sizeof(bootKeyInReportCCC), sizeof(bootKeyInReportCCC),(u8*)(&clientCharacterCfgUUID), 	(u8*)(bootKeyInReportCCC), 0},	//value

	//18. boot keyboard output report (char-val)
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&bootKeyOutReportProp), 0},				//prop
	{0,2, sizeof(bootKeyOutReport), sizeof(bootKeyOutReport),(u8*)(&hidbootKeyOutReportUUID), 	(u8*)(&bootKeyOutReport), 0},	//value

	//20. consume report in: 4 (char-val-client-ref)
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&reportConsumerControlInProp), 0},				//prop
	{0,2, sizeof(reportConsumerControlIn), sizeof(reportConsumerControlIn),(u8*)(&hidReportUUID), 	(u8*)(reportConsumerControlIn), 0},	//value
	{0,2, sizeof(reportConsumerControlInCCC), sizeof(reportConsumerControlInCCC),(u8*)(&clientCharacterCfgUUID), 	(u8*)(reportConsumerControlInCCC), 0},	//value
	{0,2, sizeof(reportRefConsumerControlIn), sizeof(reportRefConsumerControlIn),(u8*)(&reportRefUUID), 	(u8*)(reportRefConsumerControlIn), 0},	//value

	//24. report in : 4 (char-val-client-ref)
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&reportKeyInProp), 0},				//prop
	{0,2, sizeof(reportKeyIn), sizeof(reportKeyIn),(u8*)(&hidReportUUID), 	(u8*)(reportKeyIn), 0},	//value
	{0,2, sizeof(reportKeyInCCC), sizeof(reportKeyInCCC),(u8*)(&clientCharacterCfgUUID), 	(u8*)(reportKeyInCCC), 0},	//value
	{0,2, sizeof(reportRefKeyIn), sizeof(reportRefKeyIn),(u8*)(&reportRefUUID), 	(u8*)(reportRefKeyIn), 0},	//value

	//28. report out: 3 (char-val-ref)
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&reportKeyOutProp), 0},				//prop
	{0,2, sizeof(reportKeyOut), sizeof(reportKeyOut),(u8*)(&hidReportUUID), 	(u8*)(reportKeyOut), 0},	//value
	{0,2, sizeof(reportRefKeyOut), sizeof(reportRefKeyOut),(u8*)(&reportRefUUID), 	(u8*)(reportRefKeyOut), 0},	//value

	//31. report map: 3
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&reportMapProp), 0},				//prop
	{0,2, sizeof(reportMap), sizeof(reportMap),(u8*)(&hidReportMapUUID), 	(u8*)(reportMap), 0},	//value
	{0,2, sizeof(extServiceUUID), sizeof(extServiceUUID),(u8*)(&extReportRefUUID), 	(u8*)(&extServiceUUID), 0},	//value

	//34. hid information: 2
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&hidInfoProps), 0},				//prop
	{0,2, sizeof(hidInformation), sizeof(hidInformation),(u8*)(&hidinformationUUID), 	(u8*)(hidInformation), 0},	//value

	//36. control point: 2
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&controlPointProp), 0},				//prop
	{0,2, sizeof(controlPoint), sizeof(controlPoint),(u8*)(&hidCtrlPointUUID), 	(u8*)(&controlPoint), 0},	//value

#endif
	////////////////////////////////////// 31. Battery Service /////////////////////////////////////////////////////
	{3,2,2,2,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_batServiceUUID), 0},
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&my_batProp), 0},				//prop
	{0,2,1,1,(u8*)(&my_batCharUUID), 	(u8*)(my_batVal), 0},	//value

	// Audio 41
	{10,2,16,16,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_AudioUUID), 0},

	// 42
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&my_MicProp), 0},				//prop
	{0,16,1,1,(u8*)(&my_MicUUID), 	(u8*)(&my_MicData), 0},	//value
	{0,2,sizeof (my_MicName), sizeof (my_MicName),(u8*)(&userdesc_UUID), (u8*)(my_MicName), 0},

	// 45
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&my_SpeakerProp), 0},				//prop
	{0,16,1,1,(u8*)(&my_SpeakerUUID), 	(u8*)(&my_SpeakerData), &speakerWrite},//value
	{0,2,sizeof (my_SpeakerName), sizeof (my_SpeakerName),(u8*)(&userdesc_UUID), (u8*)(my_SpeakerName), 0},

	// 48
	{0,2,1,1,(u8*)(&my_characterUUID), 		(u8*)(&my_OtaProp), 0},				//prop
	{0,16,1,1,(u8*)(&my_OtaUUID),	(&my_OtaData), &otaWrite, &otaRead},			//value
	{0,2,sizeof (my_OtaName), sizeof (my_OtaName),(u8*)(&userdesc_UUID), (u8*)(my_OtaName), 0},
};

void	my_att_init ()
{
	blt_set_att_table ((u8 *)my_Attributes);
#if(BLE_REMOTE_SECURITY_ENABLE)
	blt_smp_func_init ();
#endif
}

#endif
