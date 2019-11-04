

#include <Arduino.h>
#include "USBHost_t36.h"  // Read this header first for key info
#include "usbd480.h"






void USBD480Display::Task ()
{
	//println("USBD480Display Task:  enum_state = ", device->enum_state);
	
	if (device->enum_state == USBH_ENUMSTATE_END){
		if (!start){
			start = millis();
		}else if (millis() - start > 1500){	// display startup time
			device->enum_state++;
			enableStreamDecoder();

			if (callbackFunc)
				(*callbackFunc)(USBD_MSG_DISPLAYREADY, 0, 0);	
		}
	}
}

void USBD480Display::init ()
{
	lineLength = 0;
	start = 0;
	callbackFunc = NULL;
	
	display.width = DWIDTH;
	display.height = DHEIGHT;
	display.pitch = display.width<<1;
	
	contribute_Pipes(mypipes, sizeof(mypipes)/sizeof(Pipe_t));
	contribute_Transfers(mytransfers, sizeof(mytransfers)/sizeof(Transfer_t));
	
	driver_ready_for_device(this);
}


void USBD480Display::driverReady ()
{
	println("USBD480Display driverReady  = ", (uint32_t)this, HEX);
	
	if (callbackFunc)
		(*callbackFunc)(USBD_MSG_DISPLAYREADY, NULL, 0);
}

// type 0: claim complete device
// type 1: claim per interface
bool USBD480Display::claim (Device_t *dev, int type, const uint8_t *descriptors, uint32_t len)
{
	//println();
	//println("USBD480Display claim this = ", (uint32_t)this, HEX);
	
	//println("claimType = ", type);
	//println("idVendor = ", dev->idVendor, HEX);
	//println("idProduct = ", dev->idProduct, HEX);

	if (dev->idVendor != USBD480_VID || dev->idProduct != USBD480_PID){
		println("  device is not a USBD480");
		return false;
	}else{
		println("  found a USBD480");
	}

	// only claim at interface level
	if (type != 1) return false;

	const uint8_t *p = descriptors;
	const uint8_t *end = p + len;

	// http://www.beyondlogic.org/usbnutshell/usb5.shtml
	int descriptorLength = p[0];
	int descriptorType = p[1];
	//println("descriptorType = ", descriptorType, HEX);
	//println("descriptorLength = ", descriptorLength);
	
	if (!descriptorLength) return false;
	if (descriptorType != USBH_DESCRIPTORTYPE_INTERFACE /*|| descriptorLength != 9*/)
		return false;

	descriptor_interface_t *interface = (descriptor_interface_t*)&p[0];

	//println("bInterfaceClass = ", interface->bInterfaceClass);
	//println("bInterfaceSubClass = ", interface->bInterfaceSubClass);
	
	if (interface->bInterfaceClass != USBH_DEVICECLASS_VENDOR || interface->bInterfaceSubClass != 0)
		return false;
	
	
	//println("  Interface is USBD480");
	
	p += descriptorLength;	// == sizeof(descriptor_interface_t)
	rx_ep = 0;
	tx_ep = 0;
	//int interfaceCt = 0;

	while (p < end){
		int interfaceLength = p[0];
		if (p + interfaceLength > end) return false; // reject if beyond end of data
		int interfaceType = p[1];

		//println(" ");		
		//println("interface number : ", interfaceCt++);
		
		//println("interfaceType = ", interfaceType, HEX);
		//println("interfaceLength = ", interfaceLength);

		
		if (interfaceType == USBH_DESCRIPTORTYPE_ENDPOINT){
			descriptor_endpoint_t *endpoint = (descriptor_endpoint_t*)&p[0];
			
			//println("bEndpointAddress = ", endpoint->bEndpointAddress, HEX);
			//println("bmAttributes = ", endpoint->bmAttributes, HEX);
			//println("wMaxPacketSize = ", endpoint->wMaxPacketSize);
			//println("bInterval = ", endpoint->bInterval);

			const int pipeType = endpoint->bmAttributes&0x03;
			const int pipeDir = endpoint->bEndpointAddress&0x80 >> 7;
			
			// type: 0 = Control, 1 = Isochronous, 2 = Bulk, 3 = Interrupt
			//println("  endpoint type : ", pipeType);
			//println("  endpoint dir  : ", pipeDir);
			//println("  endpoint addr : ", endpoint->bEndpointAddress&0x0F);

			if (endpoint->bEndpointAddress == 0x02){			// for bulk data writes
				tx_ep = endpoint->bEndpointAddress&0x0F;
				tx_size = endpoint->wMaxPacketSize;

				txpipe = new_Pipe(dev, pipeType, tx_ep, pipeDir, tx_size, 1/*endpoint->bInterval*/);
				if (txpipe){
					//txpipe->callback_function = tx_callback;
					
					device = dev;
					//println("txpipe device = ", (uint32_t)device, HEX);
				}
			}else if (endpoint->bEndpointAddress == 0x81){		// for touch input (reading)
				rx_ep = endpoint->bEndpointAddress&0x0F;
				rx_size = endpoint->wMaxPacketSize;
				
				rxpipe = new_Pipe(dev, pipeType, rx_ep, pipeDir, rx_size, endpoint->bInterval);
				if (rxpipe){
					//rxpipe->callback_function = rx_callback;

					device = dev;
					//println("rxpipe device = ", (uint32_t)device, HEX);
				}
			}
		}

		p += interfaceLength;
	}

	//rx_head = 0;
	//rx_tail = 0;
	
	//println("  endpoint txpipe : ", (uint32_t)txpipe, HEX);
	//println("  endpoint rxpipe  : ", (uint32_t)rxpipe, HEX);
	
	// claim if either pipe created
	return (rxpipe || txpipe);
}



#if 0
static int tx_state = 0;


void USBD480Display::tx_callback (const Transfer_t *transfer)
{
	//println("USBD480Display tx_callback");
	
	if (transfer->driver){
		if (tx_state == -1){
			//println("USBD480Display tx_callback tx_state ", tx_state);
			
			((USBD480Display*)(transfer->driver))->drawScreenSetup();

		}else if (tx_state == 1){
			tx_descript_t *context = &((USBD480Display*)(transfer->driver))->tx_writeCtx;
				
			int ret = (*((USBD480Display*)(transfer->driver))->callbackFunc)(USBD_MSG_WRITEREADY, (intptr_t*)context, 0);
			if (ret){
				
				//printf("tx_callback %i %i\r\n", (int)context->frame, (int)context->row);
				
				((USBD480Display*)(transfer->driver))->writeData(context->buffer, context->len);
				context->row += context->rows;
			}else{
				tx_state = 0;
				(*((USBD480Display*)(transfer->driver))->callbackFunc)(USBD_MSG_WRITEEND, (intptr_t*)context, 0);
			}
		}
	}
}

int USBD480Display::drawScreenArea2 (uint8_t *buffer, const int32_t bwidth, const int32_t bheight)
{
	setLineLength(bwidth);
	
	return 1;
}

#endif

/*
void USBD480Display::rx_callback (const Transfer_t *transfer)
{
	//println("USBD480Display rx_callback");
	if (transfer->driver){
		//((USBD480Display *)(transfer->driver))->new_data(transfer);
	}
}
*/

void USBD480Display::control (const Transfer_t *transfer)
{
	//println("USBD480Display control()");
	
}

void USBD480Display::disconnect ()
{
	//println("USBD480Display disconnect()");
	
	// TODO: free resources
	device = NULL;
	start = 0;
	lineLength = 0;
}



/*

static inline USBD480Display::uint16_t paletteGet (const uint8_t idx)
{
	return colourTable[idx];
}*/

void USBD480Display::paletteSet (const uint8_t idx, const uint16_t colour)
{
	colourTable[idx] = colour;
}


int USBD480Display::writeData (const void *data, const size_t size)
{
	__disable_irq();
	int ret = usb_bulk_write(this, txpipe, data, size);
	__enable_irq();
	return ret;
}	

int USBD480Display::setBaseAddress (const uint32_t addr)
{
	const int dsize = 6;
	uint8_t data[dsize];

	data[0] = 0x42; // frame base address
	data[1] = 0x5B;
	data[2] = addr&0xFF;
	data[3] = (addr>>8)&0xFF;
	data[4] = (addr>>16)&0xFF;
	data[5] = (addr>>24)&0xFF;

	int ret = writeData(data, dsize);
	delayMicroseconds(25);
	//println("USBD480Display::setBaseAddress ret = ", ret);
	return ret;
}

int USBD480Display::setFrameAddress (const uint32_t addr)
{
	int ret = usb_control_msg(device, 0x40, USBD480_SET_FRAME_START_ADDRESS, addr, (addr>>16)&0xFFFF, NULL, 0);
	//println("USBD480Display::setFrameAddress ret = ", ret);
	delay(1);
	return ret;
}

int USBD480Display::setWriteAddress (const uint32_t addr)
{
	int ret = usb_control_msg(device, 0x40, USBD480_SET_ADDRESS, addr, (addr>>16)&0xFFFF, NULL, 0);
	//println("USBD480Display::setWriteAddress ret = ", ret);
	return ret;
}

int USBD480Display::setConfigValue (const uint32_t cfg, const uint8_t value)
{
	char val = value&0xFF;
	int ret = usb_control_msg(device, 0x40, USBD480_SET_CONFIG_VALUE, cfg, 0, &val, 1);
	delay(1);
	//println("USBD480Display::setConfigValue ret = ", ret);
	return ret;
}

int USBD480Display::getConfigValue (const uint32_t cfg, uint8_t *value)
{
	*value = 0;
	int ret = usb_control_msg(device, 0x40, USBD480_GET_CONFIG_VALUE, cfg, 0, value, 1);
	//println("USBD480Display::getConfigValue val = ", *value);
	//println("USBD480Display::getConfigValue ret = ", ret);
	return ret;
}

int USBD480Display::setBrightness (const uint8_t level)
{	
	int ret = setConfigValue(CFG_BACKLIGHT_BRIGHTNESS, level);
	//println("USBD480Display::setBrightness ret = ", ret);
	return ret;
}

#if 0
int USBD480Display::setConfigDefaults ()
{
	int ret = setConfigValue(CFG_TOUCH_MODE, 0);
	setConfigValue(CFG_TOUCH_DEBOUNCE_VALUE, 20);
	setConfigValue(CFG_TOUCH_SKIP_SAMPLES, 20);
	setConfigValue(CFG_TOUCH_PRESSURE_LIMIT_LO, 50);
	setConfigValue(CFG_TOUCH_PRESSURE_LIMIT_HI, 120);

	//setConfigValue(CFG_USB_ENUMERATION_MODE, 0);

	setBrightness(255);

#if 0
	// read back values

	int value = 0;
	for (int i = 0; i < 23; i++){
		getConfigValue(i, &value);
		//printf("config: %d value: %i\n", i, (unsigned char)value);
	}
#endif
	return ret;
}
#endif



int USBD480Display::setTouchMode (const uint32_t mode)
{
	int ret = usb_control_msg(device, 0x40, USBD480_TOUCH_MODE, mode, 0, NULL, 0);
	//println("USBD480Display::setTouchMode ret = ", ret);
	return ret;
}

int USBD480Display::enableStreamDecoder ()
{
	int ret = usb_control_msg(device, 0x40, USBD480_SET_STREAM_DECODER, 0x06, 0, NULL, 0);
	delay(1);
	//println("USBD480Display::enableStreamDecoder ret = ", ret);
	return ret;
}

int USBD480Display::disableStreamDecoder ()
{
	int ret = usb_control_msg(device, 0x40, USBD480_SET_STREAM_DECODER, 0x00, 0, NULL, 0);
	delay(1);
	//println("USBD480Display::disableStreamDecoder ret = ", ret);
	return ret;
}

int USBD480Display::setLineLength (uint32_t length)
{
	if (lineLength == length)
		return -1;

	lineLength = length--;		// a linewrap of 0 is 1 pixel, a wrap of 13 is 14 pixels, etc..
	//tx_state = 1;
	const uint8_t data[] = {0x43, 0x5B, (uint8_t)((length)&0xFF), (uint8_t)((length>>8)&0xFF)};
	int ret = writeData(data, sizeof(data));
	delayMicroseconds(25);
	//println("USBD480Display::setLineLength ret = ", ret);
	return ret;
}

int USBD480Display::fillScreen (const uint16_t colour)
{
	setLineLength(display.width);


	uint8_t data[display.pitch];
	const uint32_t writelength = (display.height * display.width) - 1;
	
	// buffer 1 = address 0;
	// buffer 2 = address 130560;
	// buffer 3 = address 130560*2;
	uint32_t writeaddress = 0;
		
	data[0] = 0x41; // pixel write
	data[1] = 0x5B;
	data[2] =  writeaddress&0xFF;
	data[3] = (writeaddress>>8)&0xFF;
	data[4] = (writeaddress>>16)&0xFF;
	data[5] = (writeaddress>>24)&0xFF;
	data[6] =  writelength&0xFF;
	data[7] = (writelength>>8)&0xFF;
	data[8] = (writelength>>16)&0xFF;
	data[9] = (writelength>>24)&0xFF;
			
	int ret = writeData(data, 10);
	delayMicroseconds(15);
	
	uint16_t *pixels = (uint16_t*)data;
	for (int x = 0; x < display.width; x++)
		pixels[x] = colour;
	
	// fill screen line by line
	for (int y = 0; y < display.height; y++){
		ret += writeData(pixels, display.pitch);
		delayMicroseconds(41);
	}

	//println("USBD480Display::fillScreen ret = ", ret);
	return ret;
}

int USBD480Display::drawScreenSetup (const int32_t bwidth, const int32_t bheight, const int32_t dx, const int32_t dy, uint32_t frameAddrOffset)
{
	setLineLength(bwidth);
		
	uint32_t writelength = (bheight * bwidth) - 1;
	uint32_t writeaddress = ((dy * display.width) + dx) + frameAddrOffset;
	
	uint8_t data[10];
	data[0] = 0x41; // pixel write
	data[1] = 0x5B;
	data[2] = writeaddress&0xFF;
	data[3] = (writeaddress>>8)&0xFF;
	data[4] = (writeaddress>>16)&0xFF;
	data[5] = (writeaddress>>24)&0xFF;
	data[6] = writelength&0xFF;
	data[7] = (writelength>>8)&0xFF;
	data[8] = (writelength>>16)&0xFF;
	data[9] = (writelength>>24)&0xFF;

	int ret = writeData(data, 10);
	delayMicroseconds(5);
	return ret;
}


int USBD480Display::drawScreenArea (uint8_t *__restrict__ buffer, const int32_t __restrict__ bwidth, const int32_t __restrict__ bheight, const int32_t __restrict__ dx, const int32_t __restrict__ dy, uint32_t frameAddrOffset, int swap)
{

	//println("USBD480Display::drawScreenArea delayTime = ", delayTime);

	int delayTime = 1+((41.0f / (float)display.width) * bwidth);
	if (delayTime < 30) delayTime = 30;

	int ret = drawScreenSetup(bwidth, bheight, dx, dy, frameAddrOffset);
	uint16_t *tmpbuffer = (uint16_t*)pixbuffer;

	for (int32_t y = 0; y < bheight && y < display.height; y++){
		//memcpy(tmpbuffer, buffer, bwidth * sizeof(uint16_t));
		for (int i = 0; i < bwidth; i++)
			tmpbuffer[i] = paletteGet(buffer[i]);

		if (swap && y == (bheight-1)){
			
			uint8_t *data = (uint8_t*)&tmpbuffer[bwidth/* * sizeof(uint16_t)*/];
			data[0] = 0x42; // frame base address
			data[1] = 0x5B;
			data[2] = frameAddrOffset&0xFF;
			data[3] = (frameAddrOffset>>8)&0xFF;
			data[4] = (frameAddrOffset>>16)&0xFF;
			data[5] = (frameAddrOffset>>24)&0xFF;
	
			ret += writeData(tmpbuffer, 6+(bwidth * sizeof(uint16_t)));
		}else{
			ret += writeData(tmpbuffer, bwidth * sizeof(uint16_t));
		}

		buffer += (bwidth * sizeof(uint8_t));
		delayMicroseconds(delayTime);
	}

	//println("USBD480Display::drawScreenArea ret = ", ret);
	return ret;
}

int USBD480Display::getDeviceDetails (char *name, uint32_t *width, uint32_t *height, uint32_t *version, char *serial)
{
	//println("USBD480Display::getDeviceDetails device = ", (uint32_t)device, HEX);
	
	if (device == NULL)
		return 0;
	
	const unsigned char rsize = 64;
	char data[rsize];
	memset(data, 0, sizeof(data));
	
	int result = usb_control_msg(device, 0xC0, USBD480_GET_DEVICE_DETAILS, 0, 0, data, rsize);
	delay(1);

	
	if (result > 0){
		if (name){
			strncpy(name, data, DEVICE_NAMELENGTH);
			name[DEVICE_NAMELENGTH] = 0;
		}
		if (serial){
			strncpy(serial, &data[26], DEVICE_SERIALLENGTH);
			serial[DEVICE_SERIALLENGTH] = 0;
		}
		if (width) *width = (unsigned char)data[20] | ((unsigned char)data[21] << 8);
		if (height) *height = (unsigned char)data[22] | ((unsigned char)data[23] << 8);
		if (version) *version = (unsigned char)data[24] | ((unsigned char)data[25] << 8);
	}
	
	//println("USBD480Display::getDeviceDetails ret = ", result);
	return result;
}
