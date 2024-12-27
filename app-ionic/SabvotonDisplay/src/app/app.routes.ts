import { Routes } from '@angular/router';

export const routes: Routes = [
  {
    path: 'home',
    loadComponent: () => import('./home/home.page').then((m) => m.HomePage),
  },
  {
    path: '',
    redirectTo: 'scan',
    pathMatch: 'full',
  },
  {
    path: 'speedometer',
    loadComponent: () => import('./speedometer/speedometer.page').then( m => m.SpeedometerPage)
  },
  {
    path: 'scan',
    loadComponent: () => import('./scan/scan.page').then( m => m.ScanPage)
  },
];
