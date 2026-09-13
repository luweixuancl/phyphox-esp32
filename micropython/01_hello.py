# Copy phyphoxBLE.py from https://github.com/phyphox/phyphox-micropython

import math
import time

from machine import Pin
from phyphoxBLE import PhyphoxBLE

ble = PhyphoxBLE()
boot = Pin(9, Pin.IN, Pin.PULL_UP)
led = Pin(8, Pin.OUT)

ble.start("Maker-C3-Hello")
print("Open phyphox -> + -> Bluetooth -> Maker-C3-Hello")

t0 = time.ticks_ms()
while True:
    t = time.ticks_diff(time.ticks_ms(), t0) / 1000.0
    sine = math.sin(t * 2.0)
    pressed = 0.0 if boot.value() else 1.0
    led.value(0 if pressed else 1)
    ble.write(sine)
    time.sleep_ms(50)
