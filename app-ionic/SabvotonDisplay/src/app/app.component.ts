import { Component } from '@angular/core';
import { IonApp, IonRouterOutlet, Platform } from '@ionic/angular/standalone';
import { StatusBar, Style } from '@capacitor/status-bar';
import { ScreenOrientation } from '@capacitor/screen-orientation';

@Component({
  selector: 'app-root',
  templateUrl: 'app.component.html',
  imports: [IonApp, IonRouterOutlet],
})
export class AppComponent {
  currentOrientation: string = '';
  
  constructor(private platform: Platform) {
    this.initializeOrientation();
  }

  initializeApp() {
    console.log('Initializing');
    this.platform.ready().then(async () => {
      // Set status bar to black
      StatusBar.setOverlaysWebView({overlay: true});
      StatusBar.hide();
      // await StatusBar.setStyle({ style: Style.Dark }); // Match text/icons for dark background
    });
  }

  async initializeOrientation() {
    try {
      // Get the current orientation
      const orientation = await ScreenOrientation.orientation();
      this.currentOrientation = orientation.type.split('-')[0];
      console.log('Initial Screen Orientation:', this.currentOrientation);

      this.updateStatusbarByOrientation(this.currentOrientation);

      // Listen for orientation changes
      ScreenOrientation.addListener('screenOrientationChange', (event) => {
        this.currentOrientation = event.type.split('-')[0];
        console.log('Screen Orientation changed to:', this.currentOrientation);
        this.updateStatusbarByOrientation(this.currentOrientation);
      });
    } catch (error) {
      console.error('Error detecting screen orientation:', error);
    }
  }

  updateStatusbarByOrientation(orientation:string) {
    if(orientation == 'portrait') {
      StatusBar.setOverlaysWebView({overlay: false});
      StatusBar.show();
    } else {
      StatusBar.setOverlaysWebView({overlay: true});
      StatusBar.hide();
    }
  }
}
