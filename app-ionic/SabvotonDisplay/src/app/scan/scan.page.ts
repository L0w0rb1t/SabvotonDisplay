import { ChangeDetectorRef, Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { IonButton, IonContent, IonHeader, IonItem, IonList, IonSpinner, IonTitle, IonToolbar } from '@ionic/angular/standalone';
import { BluetoothService } from '../services/bluetooth.service';
import { ArduinoService } from '../services/arduino.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-scan',
  templateUrl: './scan.page.html',
  styleUrls: ['./scan.page.scss'],
  standalone: true,
  imports: [IonContent, IonHeader, IonTitle, IonToolbar, IonButton, IonItem, IonList, IonSpinner, CommonModule, FormsModule]
})
export class ScanPage implements OnInit {
  public foundDevices: any[] = [];
  public scanning = false;
  public isConnected = false;

  constructor(
    private bluetooth: BluetoothService,
    private arduino: ArduinoService,
    private router: Router,
    private changeDetector: ChangeDetectorRef,
  ) {}

  ngOnInit() {
    // Monitor connection status
    this.bluetooth.getConnectionStatus().subscribe((status) => {
      this.isConnected = status;
      if(this.isConnected) {
        // Redirect to speedometer page when connected
        this.router.navigate(['/speedometer']);
      }
    });
  }
  ionViewWillEnter() {
    // Reset foundDevices array when navigating to this page
    this.foundDevices = [];
  }

  // Start scanning for BLE devices
  async scanStart() {
    this.foundDevices = [];
    this.scanning = true;

    await this.bluetooth.scan(
      (device) => {
        this.foundDevices.push(device)
        this.changeDetector.detectChanges();
      },
      () => (this.scanning = false)
    );
  }

  // Connect to a selected device
  async connectToDevice(deviceId: string) {
    await this.arduino.connect(deviceId);
  }

  // Disconnect from the current device
  async disconnect() {
    await this.arduino.disconnect();
  }

  testStatusBar() {
    
  }
}