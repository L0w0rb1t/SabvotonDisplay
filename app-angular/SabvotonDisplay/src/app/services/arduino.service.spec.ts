import { TestBed } from '@angular/core/testing';

import { ArduinoService } from './arduino.service';

describe('ArduinoService', () => {
  let service: ArduinoService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ArduinoService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
