import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { BleClient } from '@capacitor-community/bluetooth-le';

@Injectable({
  providedIn: 'root',
})
export class BluetoothService {
  private connectedDeviceId: string | null = null;
  private connectionStatus = new BehaviorSubject<boolean>(false);
  private notifications: Array<any> = [];

  constructor() {}

  // Observable to track connection status
  getConnectionStatus() {
    return this.connectionStatus.asObservable();
  }

  // Scan for BLE devices
  async scan(scanCallback: (device: any) => void, completeCallback: () => void) {
    try {
      console.log('Started scanning');
      await BleClient.initialize();
      await BleClient.requestLEScan({}, scanCallback);

      setTimeout(async () => {
        await BleClient.stopLEScan();
        console.log('Finished scanning');
        completeCallback();
      }, 5000);
    } catch (error) {
      console.error('Scan failed:', error);
    }
  }

  // Connect to a device
  async connect(deviceId: string, servicesToListen: Array<any>) {
    try {
      await BleClient.initialize();
      await BleClient.connect(deviceId, async () => {
        //called on disconnect
        this.connectedDeviceId = null;
        this.connectionStatus.next(false); // Notify disconnection
        console.log('Device disconnected');
      });

      this.connectedDeviceId = deviceId;
      this.connectionStatus.next(true); // Notify connection
      console.log(`Connected to device: ${deviceId}`);

      //I don't know if there's a better way to do this - I noticed that if the device suddenly loses connection it doesn't remove the notification listeners
      //I also couldn't call stopNotifications on disconnect because I need to be connected. Seems to work fine calling it here and correctly removes listeners (?)
      if(this.notifications.length > 0) {
        for(const notification of this.notifications) {
          console.log('Stopping notifications for: '+notification.characteristic);
          await BleClient.stopNotifications(notification.deviceId, notification.service, notification.characteristic);
        }
        this.notifications = [];
      }

      // Start listening to specified services and characteristics
      for (const service of servicesToListen) {
        await BleClient.startNotifications(
          deviceId,
          service.service,
          service.characteristic,
          service.callback
        );
        this.notifications.push({
          deviceId: deviceId,
          service: service.service,
          characteristic: service.characteristic
        });
      }
    } catch (error) {
      console.error('Connection failed:', error);
    }
  }

  // Disconnect from the device
  async disconnect() {
    if (this.connectedDeviceId) {
      try {
        await BleClient.disconnect(this.connectedDeviceId);
        this.connectedDeviceId = null;
        this.connectionStatus.next(false);
        console.log('Disconnected from device');
      } catch (error) {
        console.error('Disconnection failed:', error);
      }
    }
  }
}