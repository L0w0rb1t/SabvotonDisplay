import { Component } from '@angular/core';
import { IonHeader, IonToolbar, IonTitle, IonContent, IonSpinner } from '@ionic/angular/standalone';
import { BluetoothService } from '../services/bluetooth.service';
import { ArduinoService } from '../services/arduino.service';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
  imports: [IonHeader, IonToolbar, IonTitle, IonContent, IonSpinner, CommonModule],
})
export class HomePage {
  public foundDevices:any = [];
  public scanning = false;
  public connected = false;


  constructor(public bluetooth: BluetoothService, public arduino: ArduinoService) {}

  //Starts a fresh scan for nearby BLE devices
  scanStart() {
    this.foundDevices = [];
    this.scanning = true;
    this.bluetooth.scan(this.scanCallback.bind(this), this.scanCompleteCallback.bind(this));
  }

  //Callback function to be called every time a nearby BLE device is found while scanning
  scanCallback(result:any) {
    this.foundDevices.push(result);
    console.log(result);
  }

  //Callback function to be called when scanning has concluded
  scanCompleteCallback() {
    this.scanning = false;
  }

  //Callback function to be called when a connection has been established to a device
  connectCallback(deviceId:string) {
    this.connected = true;
  }

  //Callback function to be called when a connection has been closed
  disconnectCallback(deviceId:string) {
    this.connected = false;
  }
}
