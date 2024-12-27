import { ComponentFixture, TestBed } from '@angular/core/testing';
import { SpeedometerPage } from './speedometer.page';

describe('SpeedometerPage', () => {
  let component: SpeedometerPage;
  let fixture: ComponentFixture<SpeedometerPage>;

  beforeEach(() => {
    fixture = TestBed.createComponent(SpeedometerPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
