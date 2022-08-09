# RC Input PWM
Captures RC PWM signals from connected receiver channels

Due to the board design, the following pin setup is mandatory:

```C
// PWM Pins (Output PDW => servo/esc control/etc...)
// 11   PCINT3    PB3
// 10   PCINT2    PB2
// 9    PCINT1    PB1
// 6    PCINT22   PD6
// 5    PCINT21   PD5
// 3    PCINT19   PD3

// Non PWM Pins (Can use to read PWM)
// 13   PCINT5    PB5   // LED
// 12   PCINT4    PB4
// 8    PCINT0    PB0
// 7    PCINT23   PD7
// 4    PCINT20   PD4
// 2    PCINT18   PD2   // Interrupt pin (for IMU)

// Analog Pins
// A0   PCINT8    PC5
// A1   PCINT9    PC4
// A2   PCINT10   PC3
// A3   PCINT11   PC2
// A4   PCINT12   PC1   // SDA
// A5   PCINT13   PC0   // SCL
```

# API Reference:

* `setupRCPWMInterruptPins()` - Use in setup to activate pins for input
* `readPWMIn(rx_pwm_signal)` - Writes `microsecond values for PWM signals from the RC receiver` to the passed in `int rx_pwm_signal[RX_CHANNELS];` variable
* `printRCPWMValues()` - Use for debugging
