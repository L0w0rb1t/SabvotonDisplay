import { Component } from '@angular/core';
import { BluetoothService } from '../../../services/bluetooth.service';

@Component({
  selector: 'app-home',
  imports: [],
  templateUrl: './home.component.html',
  styleUrl: './home.component.css'
})
export class HomeComponent {

  constructor(public bluetooth: BluetoothService) {}

}
