#include <cstring>
#include <lusb0_usb.h>
using namespace std;


#define VAGA_PID 60000
#define VAGA_VID 4292

#define PRINTER_VID 1155
#define	PRINTER_PID 22336

#define PRINTER2_VID 1171
#define PRINTER2_PID 34656



// --------------- Found specific device --------------- 
bool Found(struct usb_device *_dev, int vendor, int product){
	if (_dev->descriptor.idProduct != product)
		return false;
	else if (_dev->descriptor.idVendor != vendor)
		return false;
	else
		return true;
}




// --------------- Finding specific device in the list --------------- 
struct usb_device *FindDevice(struct usb_bus *_bus){
	struct usb_device *_dev;
	_dev = _bus->devices;
	while (1){ // while			 
		if (_dev == NULL)
			break;
		else{// else				
			if (Found(_dev, VAGA_VID, VAGA_PID))
				return _dev;
		}// end else
		_dev = _dev->next;
	} // end while 
	return NULL;
}// end void FindDevice()