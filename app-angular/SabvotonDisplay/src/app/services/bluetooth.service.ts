import { Inject, Injectable } from '@angular/core';
import { Capacitor } from '@capacitor/core';
import { BleClient } from '@capacitor-community/bluetooth-le'

@Injectable({
  providedIn: 'root'
})
export class BluetoothService {

  constructor() { }

  async scan(): Promise<void> {
    try {
      await BleClient.initialize();
  
      await BleClient.requestLEScan(
        {
          // services: [HEART_RATE_SERVICE],
        },
        result => {
          alert('received new scan result' + result);
        },
      );
  
      setTimeout(async () => {
        await BleClient.stopLEScan();
        console.log('stopped scanning');
      }, 5000);
    } catch (error) {
      console.error(error);
    }
  }
}
