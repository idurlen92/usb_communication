// Vaga.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <lusb0_usb.h>
#include <cstdlib>
#include <string>
#include <Windows.h>
#include "Functions.h"
#include "Interpretacije.h"
using namespace std;





// ####################    Main()    ####################  
int main(int argc, char** argv){


	usb_init();
	usb_find_busses();
	usb_find_devices();
	struct usb_bus *busses = usb_get_busses();
	struct usb_device *uredjaj = NULL;


	//// ------------------------- Traženje ureðaja -------------------------
	if (busses->devices == NULL){
		cerr << "Uredjaj nije spojen" << endl;
		system("pause");
		exit(0);
	}
	else{// else1
		uredjaj = FindDevice(busses);
		if (uredjaj == NULL) {
			cout << "Uredjaj nije nadjen!" << endl;
			system("pause");
			exit(0);
		}
	}// end else1


	usb_device_descriptor devDesc = uredjaj->descriptor; // dio device descriptora 1
	usb_config_descriptor *configDesc = uredjaj->config; // dio device descriptora 2


	// ------------------------- Konfiguriranje ureðaja -------------------------
	int bConfig = (int)configDesc->bConfigurationValue;
	usb_dev_handle *hDevice = usb_open(uredjaj);
	if (usb_set_configuration(hDevice, bConfig) < 0){
		cerr << "Error!" << endl;
		std::system("pause");
		exit(0);
	}


	// ------------------------- Claiming interface -------------------------
	int x = usb_claim_interface(hDevice, 0); // ima samo jedan interface
	if (x != 0){
		if (x == EBUSY)
			cerr << "Ebusy" << endl;
		else if (x == ENOMEM)
			cerr << "Enomem" << endl;
		else
			cout << "Cannot claim interface!" << endl;
		usb_close(hDevice);
		system("pause");
		exit(0);
	}	


	// ------------------------- Brojanje endpointova -------------------------
	int interNum = (int)configDesc->bNumInterfaces;
	std::cout << "Broj interfejsa: " << interNum << endl;
	for (int i = 0; i < interNum; i++){
		usb_interface *interf = configDesc->interface + i;
		//  petlja ide kroz sve interfejse 
		for (int j = 0; j < interf->num_altsetting; j++){
			usb_interface_descriptor *interDesc = interf->altsetting + j;
			cout << "---------------------------------------------------------------" << endl;
			InterfaceDescription(interDesc);
			cout << "---------------------------------------------------------------" << endl;
			int epNum = interDesc->bNumEndpoints;
			// petlja koja ide kroz sve endpointove  
			for (int k = 0; k < epNum; k++){
				usb_endpoint_descriptor *ep = interDesc->endpoint+k;
				EndpointDescription(ep);
				cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
				}// end for3
		}//end for2
	}// end for 1
	


	//------------------------- Èitanje podataka s vage -------------------------
	char podaci[15];
	int ret = 0;

	//------------------------- 1) Micanje halt statusa endpointa 129 -------------------------
	if (usb_clear_halt(hDevice, 129) != 0){
		cerr << "Error" << endl;
		usb_release_interface(hDevice, 1);
		usb_close(hDevice);
		exit(1);
	}

	//------------------------- 2) Provjera statusa endpointa 129-------------------------
	ret = usb_control_msg(hDevice, 130, USB_REQ_GET_STATUS, 0, 129, podaci, 14, 1000);
	if (ret < 0)
	{
		cerr << "Error jebeni!" << endl;
		system("pause");
		exit(0);
	}// end if
	else{
		cout << "ret = " << ret << endl;
		for (int i = 0; i < 14; i++){
			if((int)podaci[i]>=0)
				cout << (int)podaci[i] << " ";
			if (i == 13)
				cout << endl;
		}// end for
	}// end else

	for (int i = 0; i < 15; i++)
		podaci[i] = '\0';
	//------------------------- 3) usb_bulk_read() -------------------------
	for (int i = 0; i < 5; i++){
		cout << "Message " << i + 1 << ":" << endl;
		for (int j = 0; j < 14; j++){
			int x = usb_bulk_read(hDevice, (1 | USB_ENDPOINT_IN), podaci + j, 1, 500);	// Moguæ problem s adresom endpointa. Pogledati u dokumentaciji bitove adrese endpointa!
			cout << x << ": " << (int)podaci[j] << endl;	// Funkcija stalno baca greške -116 (prema dokumkentaciji znaèi da je greška kod wait timeout-a) ili -22 (nepoznata interpretacija)
		}
		cout << endl << endl;
		Sleep(250);
	}

	usb_release_interface(hDevice, 1);
	usb_close(hDevice);


cout << "\n\n";
system("pause");
return 0;
}// ####################   end Main()    ####################  