import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { BluetoothService } from './bluetooth.service';

@Injectable({
  providedIn: 'root',
})
export class ArduinoService {
  private batteryVoltageSubject = new BehaviorSubject<number | null>(null);
  private speedSubject = new BehaviorSubject<number | null>(null);

  private sabvotonDisplayService = '623304fc-a720-4d25-b50b-db235188ef6e';
  private batteryVoltageCharacteristic = 'dc31e889-6d41-4a09-9b6a-2a027fb3ed4c';
  private speedCharacteristic = 'fc56d22a-1e95-43c9-8844-826285950c7c';

  constructor(private bluetooth: BluetoothService) {}

  // Stream observables for battery voltage and speed
  getBatteryVoltageStream() {
    return this.batteryVoltageSubject.asObservable();
  }

  getSpeedStream() {
    return this.speedSubject.asObservable();
  }

  // Connect and listen to characteristics
  async connect(deviceId: string) {
    const servicesToListen = [
      {
        service: this.sabvotonDisplayService,
        characteristic: this.batteryVoltageCharacteristic,
        callback: (value: DataView) =>
          this.batteryVoltageSubject.next(parseFloat(value.getFloat32(0, true).toFixed(2))),
      },
      {
        service: this.sabvotonDisplayService,
        characteristic: this.speedCharacteristic,
        callback: (value: DataView) =>
          this.speedSubject.next(parseFloat(value.getFloat32(0, true).toFixed(1))),
      },
    ];

    await this.bluetooth.connect(deviceId, servicesToListen);
  }

  // Disconnect from the device
  async disconnect() {
    await this.bluetooth.disconnect();
  }
}