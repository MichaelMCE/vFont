

#ifndef _USBD480_H_
#define _USBD480_H_


#define USBD480_VID						0x16C0
#define USBD480_PID						0x08A6

#define USBD480_SET_ADDRESS				0xC0 // set SDRAM address
#define USBD480_WRITE_DATA				0xC1 // write one 16-bit word
#define USBD480_SET_FRAME_START_ADDRESS	0xC4 // set frame start address
#define USBD480_SET_STREAM_DECODER		0xC6
#define USBD480_TOUCH_READ				0xE1
#define USBD480_TOUCH_MODE				0xE2
#define USBD480_GET_DEVICE_DETAILS		0x80
#define USBD480_SET_CONFIG_VALUE		0x82
#define USBD480_GET_CONFIG_VALUE		0x83
#define USBD480_SAVE_CONFIG				0x84 // write config to flash
#define USBD480_GET_SAVED_CONFIG_VALUE  0x86
#define USBD480_EEPROM_PREPARE_WRITE	0xBC
#define USBD480_READ_PARAM_STORE		0xBF

#define CFG_TOUCH_MODE					2
#define CFG_TOUCH_DEBOUNCE_VALUE		3	// default 50 - 0 to 254
#define CFG_TOUCH_SKIP_SAMPLES			4	// default 40
#define CFG_TOUCH_PRESSURE_LIMIT_LO		5	// default 30
#define CFG_TOUCH_PRESSURE_LIMIT_HI		6	// default 120
#define CFG_BACKLIGHT_BRIGHTNESS		20	// 0 - 255, default 255
#define CFG_USB_ENUMERATION_MODE		22	// 0 - 1, default is 0. 0:HID device, 1:vender specific device

#define DEVICE_NAMELENGTH				20
#define DEVICE_SERIALLENGTH				10

#define DEBOUNCETIME					80		/* millisecond period*/
#define DRAGDEBOUNCETIME				50





// LFRM_BPP_16 - RGB 565
#define RGB_16_RED		0xF800
#define RGB_16_GREEN	0x07E0
#define RGB_16_BLUE		0x001F	
#define RGB_16_WHITE	(RGB_16_RED|RGB_16_GREEN|RGB_16_BLUE)
#define RGB_16_BLACK	0x0000
#define RGB_16_MAGENTA	(RGB_16_RED|RGB_16_BLUE)
#define RGB_16_YELLOW	(RGB_16_RED|RGB_16_GREEN)
#define RGB_16_CYAN		(RGB_16_GREEN|RGB_16_BLUE)






class USBD480Display : public USBDriver {
public:
	enum { SYSEX_MAX_LEN = 60 };
	
	USBD480Display (USBHost &host)
	{
		init();
	}
	USBD480Display (USBHost *host)
	{
		init();
	}

	
	void setCallbackFunc (int (*func)(uint32_t msg, intptr_t *value1, uint32_t value2))
	{
		callbackFunc = func;
	}

	struct {
		int width;
		int height;
		int pitch;
	}display;
		
	int setBaseAddress (const uint32_t addr);
	int setFrameAddress (const uint32_t addr);
	int setWriteAddress (const uint32_t addr);
	int setConfigValue (const uint32_t cfg, const uint8_t value);
	int getConfigValue (const uint32_t cfg, uint8_t *value);
	int setBrightness (const uint8_t level);
	int setLineLength (uint32_t length);
	int disableStreamDecoder ();
	int enableStreamDecoder ();
	int setTouchMode (const uint32_t mode);
	int setConfigDefaults ();
	int getDeviceDetails (char *name, uint32_t *width, uint32_t *height, uint32_t *version, char *serial);
	int writeData (const void *data, const size_t size);
	int drawScreenArea (uint8_t *__restrict__ buffer, const int32_t __restrict__ bwidth, const int32_t __restrict__ bheight, const int32_t __restrict__ dx, const int32_t __restrict__ dy, uint32_t frameAddrOffset = 0, int swap = 0);
	int fillScreen (const uint16_t colour);
	
	//int drawScreenArea2 (uint8_t *buffer, const int32_t bwidth, const int32_t bheight);
	//int drawScreenSetup ();
	//int ustate = 0;
	
	int drawScreenSetup (const int32_t bwidth, const int32_t bheight, const int32_t dx, const int32_t dy, uint32_t frameAddrOffset);
	void paletteSet (const uint8_t idx, const uint16_t colour);
	
	tx_descript_t tx_writeCtx;
	
	// 8bit palette to 16bit lookup table
	uint16_t colourTable[256];
	
protected:

	virtual void driverReady ();

	virtual void Task ();
	virtual bool claim (Device_t *device, int type, const uint8_t *descriptors, uint32_t len);
	virtual void control (const Transfer_t *transfer);
	virtual void disconnect ();
	
	//static void tx_callback (const Transfer_t *transfer);	
	//static void rx_callback (const Transfer_t *transfer);
	
	//void rx_data(const Transfer_t *transfer);
	//void tx_data(const Transfer_t *transfer);
	void init();
	
		


private:



	Device_t *device;
	Pipe_t *rxpipe;
	Pipe_t *txpipe;
	
	uint32_t start = 0;
	uint32_t lineLength = 0;
	
	uint8_t pixbuffer[(480*2)+8];

	
	inline uint16_t paletteGet (const uint8_t idx)
	{
		return colourTable[idx];
	}
	
/*	
	enum {
		MAX_TX_PACKET_SIZE = 512, 
		MAX_RX_PACKET_SIZE = 16
	};
	
	enum { RX_QUEUE_SIZE = 40 }; // must be more than MAX_PACKET_SIZE/4
*/
	//uint32_t tx_buffer[MAX_TX_PACKET_SIZE/sizeof(uint32_t)];
	uint16_t tx_size;
	uint16_t rx_size;
	uint8_t tx_ep;
	uint8_t rx_ep;			
	//uint32_t rx_buffer[MAX_RX_PACKET_SIZE/sizeof(uint32_t)];
	//uint32_t rx_queue[RX_QUEUE_SIZE];

	//uint16_t rx_head;
	//uint16_t rx_tail;
	//bool rx_packet_queued;

	Pipe_t mypipes[4] __attribute__ ((aligned(32)));
	Transfer_t mytransfers[136] __attribute__ ((aligned(32)));
	int (*callbackFunc) (uint32_t msg, intptr_t *value1, uint32_t value2);
};




#endif

