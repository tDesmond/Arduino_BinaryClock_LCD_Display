# Arduino_BinaryClock_LCD_Display

This code was used to convert a binary clock signal to a lcd display.
The binary clock three ripple counters created eith JK Flip flops.
A counter for seconds, hours and minutes. 
The ripple counter had a clock supplied by a 555 timer set at a frequnecy of one second.
The ripple countes would light up LED's show the time in Binary (E.G 00010 001111 000000 = 02h : 15m : 00s).
The arduino read which inputs were high and calculated the decimal time value from those readings.

The code includes a basic UI design with a menu to select an option to set an alarm.
