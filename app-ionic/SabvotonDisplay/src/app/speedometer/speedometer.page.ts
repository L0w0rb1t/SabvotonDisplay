import { Component, OnInit, OnDestroy, ChangeDetectorRef } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { IonCard, IonCardContent, IonCardHeader, IonCardTitle, IonContent, IonHeader, IonIcon, IonTitle, IonToolbar } from '@ionic/angular/standalone';
import { Subscription } from 'rxjs';
import { ArduinoService } from '../services/arduino.service';
import { BluetoothService } from '../services/bluetooth.service';
import { Router } from '@angular/router';
import { addIcons } from 'ionicons';
import { batteryDead, batteryFull, batteryHalf, cog, hammer } from 'ionicons/icons'

@Component({
  selector: 'app-speedometer',
  templateUrl: './speedometer.page.html',
  styleUrls: ['./speedometer.page.scss'],
  standalone: true,
  imports: [IonContent, IonHeader, IonTitle, IonToolbar, IonCardContent, IonCardTitle, IonCardHeader, IonCard, IonIcon, CommonModule, FormsModule]
})
export class SpeedometerPage implements OnInit, OnDestroy {
  batteryVoltage: number | null = null;
  speed: number | null = null;
  speedWhole:string | null = null;
  speedDecimal:string | null = null;
  isConnected = false;

  private subscriptions: Subscription[] = [];

  constructor(
    private arduino: ArduinoService,
    private bluetooth: BluetoothService,
    private changeDetector: ChangeDetectorRef,
    public router: Router,
  ) {
    addIcons({batteryDead, batteryHalf, batteryFull, cog, hammer});
  }

  ngOnInit() {
    this.subscriptions.push(
      this.arduino.getBatteryVoltageStream().subscribe(
        (voltage) => {
          this.batteryVoltage = voltage ?? 0
          this.changeDetector.detectChanges();
        }
      ),
      this.arduino.getSpeedStream().subscribe(
        (speed) => {
          this.speed = speed ?? 0
          try {
            this.speedWhole = this.speed.toString().split('.')[0];
            this.speedDecimal = this.speed.toString().split('.')[1];
          } catch(error) {
            console.log(error);
          }
          this.changeDetector.detectChanges();
        }
      ),
      this.bluetooth.getConnectionStatus().subscribe(
        (status) => {
          this.isConnected = status;
          this.changeDetector.detectChanges();
          if(!this.isConnected) {
            this.router.navigate(['scan']);
          }
        }
      )
    );
  }

  ngOnDestroy() {
    this.subscriptions.forEach((sub) => sub.unsubscribe());
  }
}