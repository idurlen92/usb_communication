#include <iostream>
#include <lusb0_usb.h>
#include <string>
using namespace std;



// --------------- Class Type Info --------------- 
string ClassInfo(int clss){
	switch (clss){
	case(USB_CLASS_HID) : return "Human Interface Device";
	case(USB_CLASS_AUDIO) : return "Audio";
	case(USB_CLASS_COMM) : return "Comm";
	case(USB_CLASS_DATA) : return "Data";
	case(USB_CLASS_HUB) : return "Hub";
	case(USB_CLASS_MASS_STORAGE) : return "Mass storage device";
	case(USB_CLASS_PER_INTERFACE) : return "Class per interface";
	case(USB_CLASS_PRINTER) : return "Printer";
	case(USB_CLASS_VENDOR_SPEC) : return "Vendor spec";
	default: return "Neam pojma!";
	}// end switch
}// end DvClass




// --------------- Descriptor Type Info --------------- 
string DescriptorType(int d){
	switch (d){
	case USB_DT_DEVICE: return "Device descriptor";
	case USB_DT_CONFIG: return "Configuration descriptor";
	case USB_DT_STRING: return "String descriptor";
	case USB_DT_INTERFACE: return "Interface descriptor";
	case USB_DT_ENDPOINT: return "Endpoint descriptor";
	case USB_DT_ENDPOINT_SIZE: return "Device size descriptor";
	case USB_DT_HID: return "HID descriptor";
	case USB_DT_REPORT: return "HID report descriptor";
	case USB_DT_PHYSICAL: return "Physical descriptor";
	case USB_DT_HUB: return "Hub descriptor";
	case USB_DT_CONFIG_SIZE: "Config size descriptor";
	}// end switch
}




// --------------- bmAttributes info --------------- 
void bmAttrInfo(int bmAttr){ // bitovi 0 - 255
	int result = 0;
	cout << "Transfer type:  ";
	result = bmAttr & 3;
	// 1) Transfer Type
	switch (result){
	case 0: {
				cout << "Control Transfer" << endl;
				break;
			}
	case 1: {
				cout << "Isinchronus" << endl;
				break;
			}
	case 2: {
				cout << "Bulk transfer" << endl;
				break;
			}
	case 3: {
				cout << "Interrupt transfer" << endl;
				break;
			}
	}//end switch
// 2) Synchronization type
	result = bmAttr & 12;
	cout << "Synchronization type:  ";
	switch (result){
	case 0: {
				cout << "No Synchro" << endl;
				break;
			}
	case 4: {
				cout << "Async" << endl;
				break;
			}
	case 8: {
				cout << "Adaptive" << endl;
				break;
			}
	case 12: {
				 cout << "Sync" << endl;
				 break;
			 }
	}//end switch
// 3) Usage type
	result = bmAttr & 48;
	cout << "Usage type:  ";
	switch (result){
	case 0: {
				cout << "Data endpoint" << endl;
				break;
			}
	case 16: {
				 cout << "Feedback endpoint" << endl;
				 break;
			 }
	case 32: {
				 cout << "Implicit feedback Data EP" << endl;
				 break;
			 }
	case 48: {
				 cout << "Reserved" << endl;
				 break;
			 }
	}//end switch
}// end bmAttrInfo()




// --------------- MaxPcktSizeInfo ---------------
void MaxPcktSizeInfo(int m){
	int size = m & 2047;
	cout << "\tMax velicina paketa: " << size << endl;
	int r = m & 6144;
	cout << "\tBroj transakcija po microframeu: ";
	switch (r){
		case 0: cout << " 0 (1 per microframe)" << endl;
	     		break;
		case 2048: cout << "1 (2 per microframe)" << endl;
			       break;
		case 4096: cout << "2 (3 per microframe)" << endl;
			break;
		case 6144: cout << "3 (Reserved)" << endl;
				   break;
	}// end switch
}




// --------------- Endpoint Address Info --------------- 
void EndpointAdressInfo(int adresa){
	cout << "\tAdresa: " << adresa << endl;
	int epNum = adresa & 15;
	cout << "\tEndpoint number: " << epNum << endl;
	int direction = adresa & 128;
	cout << "\tDirection: ";
	if (direction == 0)
		cout << "Out " << endl;
	else
		cout << "In" << endl;
}




// --------------- Endpoint Description --------------- 
void EndpointDescription(usb_endpoint_descriptor *e){
	EndpointAdressInfo((int)e->bEndpointAddress);
	cout << "\tTip deskriptora: " << DescriptorType((int)e->bDescriptorType) << endl;
	cout << "\tbInterval: " << (int)e->bInterval << endl;
	cout << "\tbLength: " << (int)e->bLength << endl;
	cout << "\tbRefresh: " << (int)e->bRefresh << endl;
	cout << "\tbSynchAdress: " << (int)e->bSynchAddress << endl;
	MaxPcktSizeInfo((int)e->wMaxPacketSize);
	bmAttrInfo((int)e->bmAttributes);
}




// --------------- Interface Description --------------- 
void InterfaceDescription(usb_interface_descriptor *i){
	cout << "\tbAlternateSettings: " << (int)i->bAlternateSetting << endl;
	cout << "\tTip deskriptora: " << DescriptorType((int)i->bDescriptorType) << endl;
	cout << "\tKlasa inteface-a: " << ClassInfo((int)i->bInterfaceClass) << endl;
	cout << "\tPodklasa interface-a: " << ClassInfo((int)i->bInterfaceSubClass) << endl;
	cout << "\tBroj ovog interface-a: " << (int)i->bInterfaceNumber << endl;
	cout << "\tInterface Protocol: " << (int)i->bInterfaceProtocol << endl;
	cout << "\tVelicina deskriptora: " << (int)i->bLength << " bajtova" << endl;
	cout << "\tBroj endpointova: " << (int)i->bNumEndpoints << endl;
}