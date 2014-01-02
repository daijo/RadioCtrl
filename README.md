Radio Controller
================

Code for having an Arduino control a homebrew CW tranceiver.

![FrontPanel](https://github.com/daijo/RadioCtrl/raw/master/images/front.jpg)

Menu
----

- Menu mode is toggled by a long press (2 s) on the rotary knob. Times out in 5 s. 
- Short press to select.
- Short press when not in menu switches between frequency and RIT selection on rotary encoder.

    Step <-> Sidetone <-> ( Band ) <-> Keyer <-> ( CW Decode )
    
    Step: 1MHz <-> 1kHz <-> 100Hz <-> 10Hz
    
    Sidetone: On <-> Off
    
    ( Band: 160m <-> 80m <-> 40m <-> 30m <-> 20m <-> 15m <-> 10m <-> 6m )
    
    Keyer: Mode A <-> Mode B
    
    ( CW Decode: On <-> Off )

Pin assignment
--------------

    D0 - Serial RX (unused)
    D1 - Serial TX (unused)
    D2 - Mute in (RX) (interrupt)
    D3 - Encoder (interupt)
    D4 - Encoder
    D5 - Encoder push
    D6 - Paddle Dih (TX)
    D7 - Paddle Dah (TX)
    D8 - DDS VFO Load
    D9 - DDS VFO Clock
    D10 - DDS VFO Data
    D11 - Sidetone (PWM)
    D12 - Unused
    D13 - Unused
    
    A0 - Voltage sense
    A1 - Audio detect (CW decode)
    A2/D16 - I2C (LCD etc.)
    A3/D17 - I2C (LCD etc.)
    A4/D18 - Mute/T/R out (TX)
    A5/D19 - Key out (TX)

USB Serial Control
------------------

The controller read commands and writes status over the hardware UART.

TBD define protocol.

DDS VFO Control
---------------

I2C devices
-----------

The controller sends commands to I2C slave controllers for additional functionallity.

### 16x2 Display

### Bandpass Board

TBD define protocol.

Build
-----

    export VFO_IF_OFFSET=0
    export ARDUINODIR=/Users/daijo/Code/Arduino
    export SERIALDEV=/dev/tty.usbserial-A400fXmd
    export BOARD=atmega328
    make & make upload

Libraries
---------

Rotary - Quadrature encoder lib. - http://www.pjrc.com/teensy/td_libs_Encoder.html
SerialLCD - Seedstudio 16x2 Serial LCD display lib.

DDS AD9851 code borrowed from Peter Marks & George Smart. - http://blog.marxy.org/2008/05/controlling-ad9851-dds-with-arduino.html
