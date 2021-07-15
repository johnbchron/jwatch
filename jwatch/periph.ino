

void setup_bma() {
  BMA * sensor = ttgo->bma;

  Acfg cfg;
  cfg.odr = BMA4_OUTPUT_DATA_RATE_12_5HZ;
  cfg.range = BMA4_ACCEL_RANGE_2G;
  cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
  cfg.perf_mode = BMA4_CIC_AVG_MODE;

  sensor->accelConfig(cfg);

  sensor->enableAccel();

  pinMode(BMA423_INT1, INPUT);
  attachInterrupt(BMA423_INT1, [] {
    irq_bma = true;
  }, RISING);

  sensor->enableFeature(BMA423_STEP_CNTR, true);
  sensor->enableFeature(BMA423_TILT, true);
//  sensor->enableFeature(BMA423_WAKEUP, true);

  sensor->resetStepCounter();

  sensor->enableStepCountInterrupt();
  sensor->enableTiltInterrupt();
//  sensor->enableWakeupInterrupt();
}
