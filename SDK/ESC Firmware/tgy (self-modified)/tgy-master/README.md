https://github.com/sim-/tgy

**Binary Downloads**: http://0x.ca/tgy/downloads/

This tree contains Atmel AVR assembly code for ATmega-based 3-phase
sensor-less motor electronic speed control (ESC) boards, originally for
Turnigy and similar models. This work is based on Bernhard Konze's
"tp-18a" software, which was a port from his earlier personal work to the
TowerPro 18A and original (not current!) Turnigy Plush boards. Please see
tgy.asm for Bernhard's license.

Patches and comments are always welcome! Let me know how it goes!

Features
--------
- 16MHz operation on most boards
- 16-bit output PWM with full clock rate resolution (~18kHz PWM with
  a POWER_RANGE of 800 steps)
- 24-bit timing and PWM pulse tracking at full clock rate resolution
- ICP-based pulse time recording (on supported hardware) for zero PWM
  input control jitter
- Immediate PWM input to PWM output for best possible multicopter
  response (but NOT where soft start or really any significant current
  limiting is needed!)
- Accepts any PWM update rate (minimum ~5microseconds PWM low time)
- Optimized interrupt code (very low minimum PWM and reduced full
  throttle bump)
- Configurable board pin assignments by include file
- Smooth starting in most cases
- Forward and reverse commutation supported, including RC-car style
  reverse-neutral-forward PWM ranges, with optional braking

Hardware
--------
See http://wiki.openpilot.org/display/Doc/RapidESC+Database and/or
https://docs.google.com/spreadsheet/ccc?key=0AhR02IDNb7_MdEhfVjk3MkRHVzhKdjU1YzdBQkZZRlE
for a complete list.  
Some board pictures here: http://0x.ca/sim/esc/


Notes
-----
- If it breaks, you get to keep both pieces!
- Use at your own risk, and always test first without propellers!
- New Turnigy Plush, Basic, Sentry and Pentium boards (Hobbywing OEM)
  have all switched to SiLabs C8051F334, d'oh!
- If your ESC has 6 pads and an AVR, it's probably compatible; the pads
  are MOSI, MISO, SCLK, GND, VCC, and RESET. If it has 4 pads, it is
  probably a newer SiLabs-based one, for which this code will not work.
  (Except HK_261000001 which has 4 pads but has an AVR.)
- I build and maintain this in Linux with AVRA (1.3.0 or newer). Patches
  welcome for AVR Studio APS files, etc.
- The TowerPro/Turnigy Plush type boards typically do not come with
  external oscillators, which means their frequency drifts a bit with
  temperature and between boards. Multicopters and RC-car/boat
  controllers (with a neutral deadband) would probably be better on a
  board with an external oscillator. The Mystery/BlueSeries boards
  typically have them, as well as most higher current boards.
- This doesn't yet check temperature or battery voltage. This is not
  desired on multi-rotor platforms; however, people still want to use
  this on planes, cars, boats, etc., so I suppose I'll add it.

Building from Source
--------------------
AVRA 1.3.0 or newer or avrasm2, part of the AVR Tools, should assemble
this source. AVRA should also build on a Mac. "make all" will emit a
.hex file for every build target; "make binary_zip" will make a release
.zip file. There are some other make targets for programming.

In AVR Studio, the Makefile is not supported, and just loading tgy.asm
and attempting to build it will not define the constant indicating the
board type / build target. You must either edit tgy.asm or add an option
for the assembler command line to define the board symbol, unless
building the default "tgy" board type. For example, this option should
emit the bs_nfet target: -D bs_nfet_esc=bs_nfet_esc
Look near the top of tgy.asm for the includes and board information.

WARNING
-------
Never just randomly try build targets until one works, especially not
when directly powered from a LiPo! :P Many boards have inverted FET
drives and different pin assignments. Toggling one wrong pin can fry
multiple FETs. Some boards, like the Mystery 20A ESC, may all look the
same on the outside by have different FETs as well. Be careful and check
your board before flashing.

Installation
------------
For more information, check out these sites:

http://wiki.openpilot.org/display/Doc/RapidESCs
http://wiki.openpilot.org/display/Doc/Flashing+Instructions
http://www.rcgroups.com/forums/showthread.php?t=1513678

See warning above! The safest arrangement is to use a current-limited
bench power supply, set to a low voltage (6V-7V), for both flashing and
initial testing. If the pinout is wrong and causes a short, the current
limiting causes the input voltage to drop below the brown-out detection
voltage of the MCU, causing all output pins to go high-impedance in
hardware, and an automatic reset when the voltage comes back.

If you do not have a current-limited supply, you can improvise by using 4
AA batteries or an old NiCd pack or even a LiPo with a 12V light bulb in
series to act as a current limiter. Be careful when touching the board,
since it can be quite easy to turn on a FET gate with just your finger.
This should be OK if you have a current-limited supply, since it should
just reset.

Even if the board appears to be one tested by others, make sure yours
has the expected FET pin assignments, inversions, and sense lines! The
assignments for each board type can be found in the .inc files, and
the actual pin mappings can be found in the first few pages of ATmega8
datasheet. This typically requires a voltmeter and, preferably, an
oscilloscope.

When not powered, use a voltmeter to check the path from the MCU pins to
either the FET resistors, transistors, or driver chips, and verify that
they match the assignments in one of the include files. Then power up
the ESC with the original firmware, and check with an oscilloscope the
inversions of the same pins. A voltmeter may be used instead when the
motor is stopped. Be careful not to short FET pins together! If all
voltages are low when the motor is off, nothing is inverted, and the
INIT_Px values in the .inc file should be 0 for all of the FET bits.

Older and smaller boards typically use P-FETs and N-FETs in the H-bridge;
the P-FETs are typically driven by three NPN transistors, while the
N-FETs are driven directly at TTL voltages with just a low resistor.
Medium-sized and newer boards have moved to all-N-FET designs, but
maintain the NPN transistors, and so have inverted P-FET pins from the
MCU. Larger current (>~30A) boards typically have separate FET driver
chips, to which the N-FET _or_ P-FET pins _may_ be inverted (but not both,
since it would blow up before the MCU initializes).

PWM is usually done on the low side of the H-bridge, where high frequency
driving is easiest. If the average voltage increases at the AVR pin as
throttle increases, and drops to 0V when stopped, the low-side FETs are
not inverted; if average voltage decreases, and rises to 5V when stopped,
the low-side FETs are likely inverted. The high side FETs are only
switched at every other motor commutation step, and so switch at a lower
frequency, and should be off 2/3rds of the time. If at 0V when stopped,
and less than 2.5V average when running, the P-FETs are not inverted. If
at 5V when stopped, and more than 2.5V when running, the P-FETs are
inverted. In the case of inverted an FET group, they should be listed in
the INIT_Px values (to turn them ON at boot), and the "on" macros should
use clear instead of set instructions. The inverse applies to the "off"
macros.

There are four sense lines. The three output phases go through resistor
dividers (to bring the voltage down to between 0-5V), and then are
connected to ADC channels which can be accessed by the comparator with
the ADC multiplexer when the ACME (Analog Comparator Multiplexer Enable)
bit is enabled. Some boards use all ADC channels while others put one pin
on AIN1 so that the ADC can sample voltages on other ADC channels while
the comparator samples that phase. A "center tap" is established with a
resistor star and connected to AIN0 on all boards, for detecting the
zero-crossing from the motor back-EMF for timing. You can check which
pins run to which output phases as they will have the lowest resistance
from output phase to AVR pin, and all three will have a common resistance
to the AIN0 pin.

Boot Loader
-----------
Since the 2012-06-01 release, the pre-built .hex files contain a boot
loader that allows for flash and EEPROM reading and writing over the PWM
input wire. The main purpose of this boot loader is to support software
updates without having to expose the MCU or ISP pads for SPI programming,
assuming this version or newer has previously been flashed.

The boot loader is available on all boards which are wired in way that
permits two-way communication over the wire. Some ESCs are isolated with
an opto-isolator or by components to invert the signal to be compatible
with ESCs that do have an opto-isolator, such as ESCs that work with the
kda.hex target. Such boards will not be able to use this protocol.

The boot loader uses an implementation of the same wire encoding as the
Turnigy USB Linker. This is a simple 9600bps serial to half duplex wire
converter that encodes signals in a way that should make it difficult to
start a motor, even on an ESC not supporting the protocol, regardless of
data sent. However, it is still possible that this could occur, so follow
the same precautions as with normal ISP flashing, below.

If the Turnigy USB Linker is not available, it is possible to use an
Arduino or MultiWii board as a gateway between (USB and) serial and the
wire protocol used by the boot loader. See the ArduinoUSBLinker project
by Chris Osgood: https://github.com/c---/ArduinoUSBLinker

Boot Loader Fuses
-----------------
Since the 2012-09-30 release, long periods of high PWM input while
disarmed will cause a jump to the boot loader, if present. Since the
2013-04-24 release, this became detangled from the included boot loader
(BOOT_JUMP vs BOOT_LOADER), so other boot loaders may be used instead.

As a result of the automatic jumping, it is not necessary to change the
fuses to enable the boot loader. It may or may not be desirable to set
the BOOTRST flag depending on intended operation and the hardware in use.

If the hardware does not have any low pass filter or pull-down on the PWM
input, it may easily float high by the time the boot loader checks it,
which can prevent normal startup if another input (eg: I2C) is expected
to be used. There should be pull-down or load present on the PWM input
to prevent this if BOOTRST is enabled.

Confusion may also result if the pin floats high with nothing connected
and power is connected to check for starting beeps. Likewise, touching
the connector or connecting it later may drain the input and cause normal
startup to occur, which may seem like the ESC is resetting while powered.
This is not a problem if the PWM input is normally intended to be used.

On the other hand, with BOOTRST not set, it is not possible to recover
from an interrupted flash, flash of the wrong board type, or flash of
other software or versions older than 2012-09-30 without connecting to
the ISP pins to do the flashing.

To enable the the boot loader on ATmega8 boards, the low nibble of the
hfuse should be set to 'a'. See "make bootload_usbasp" to do this
automatically. BOOTRST and BOOTSZ1 should be enabled to set 512 words,
start at $0E00.

Flashing and Testing
--------------------
Sort out how you want to connect an ISP programming device to the chip.
Some boards have 6 pads in a row for this purpose. You can either solder
wires, or make up some kind of springy-pin connector that touches the
pads or chip pins without needing to solder. This is helpful when
flashing more than one board. See here for some ideas and discussion:
http://www.rcgroups.com/forums/showthread.php?t=1513678

Sort out which software you will use for flashing. The KKMulticopter
Flashtool now supports ESC flashing and is commonly used. You may also
download AVR Studio and the AVR Toolchain from www.atmel.com, or try
"avrdude" on most OSes. There are plenty of resources on the web for AVR
ISP programming, and many ways to do it.

Power the ESC from a current-limited supply and try to read the stock
firmware (flash) _and_ EEPROM, to use as a backup. Most are locked and
will still appear to read, but the files will contain just a series of
repeating/increasing digits. If you do manage to get something, consider
yourself lucky!

Write down the stock fuse values, and check that they are sane. Most AVR
programmers have a menu for this, but with avrdude or uisp, Google "AVR
fuse calculator", select the ATmega8 target, and type in the hex values.
If the "watchdog" fuse is enabled, you will want to disable it for now.
The brown-out voltage should be set to 4.0V and enabled (BODEN). Leave
the rest of the fuse values as shipped, and write down the new values.

Flash the desired target .hex file to the AVR, then set the fuses, if
anything needs changing. If you have any errors, check the connections
to and voltage at the chip. Sometimes, a weak power or signal connection
can temporarily work and then fail part-way through programming, giving
verification errors. This can happen particularly if the target chip is
powered weakly by the programmer itself, which then back-feeds to the
rest of the circuit and tries to charge the capacitor, etc. Also check
that the ground pin (black wire) actually connects to a pin marked "GND",
not "NC" as on some USBasp devices!

Once programming is successful, hook up a small motor without propeller
and reset the power. You should hear three increasing beeps. If not, or
if you hear only some beeps, the FET pinout may be incorrect or one or
more FETs may be broken. Repetitive clicking can also indicate that the
pinout is incorrect and causing continuous brown-out resets.

Now, if you attach a valid PWM servo pulse with low-enough pulse length,
you should hear a forth beep indicating that the ESC is armed. If not,
try lowering the trim as far as possible. If it still doesn't work, you
may need to raise the STOP_RC_PULS value in the code.

Once armed, the ESC will try to start the motor if the pulse length
(throttle) is increased. Try running up the motor slowly, and make sure
everything runs smoothly. If on a variable supply, increase the voltage
to the expected running voltage, and try again, with slow and rapid
throttle changes.

Finally, test the ESC in the intended application, with the usual power
source, without anything attached to the motor shaft/bell first. Then,
attach the propeller or gear, etc., LOCK IT DOWN so it doesn't hit you in
the face, and try a slow sweep from idle to full throttle. Finally, try
rapid throttle changes from slow to fast. The ESC and motor should run
smoothly, never lose timing, and the ESC should not reset. If using a
flight control board, you can sometimes tap the board to make it output a
sudden throttle increase.

For debugging reset causes, recent code has different beep sequences for
each AVR reset case. See the Troubleshooting section below.

Throttle Calibration and Programming
------------------------------------

Since the 2012-01-04 release, the input throttle range may be calibrated.
This should be used whenever PWM input mode is used, including where
external resonators are present, to set the usable throttle range.

The default range is set at the top of tgy.asm (stop at 1060us, full
throttle at 1860us) and is not changed if no new value is saved to the
EEPROM. However, boards without external oscillators (typically those
which use tgy.hex) must use the internal RC oscillator on the Atmega8,
which may be off by 5% - 10% between each board, and will also drift by
10% or more over a 40 degree Celsius temperature range. This can cause
throttle differences between boards if not calibrated, and issues arming
the ESC, particularly in cold environments.

Some flight boards may have automatic calibration procedures, or it may
just be possible to set the min/max motor output to exactly match the ESC
defaults listed above. ESCs with external resonators can use this method
to avoid calibration. With a flight board such as the Naze32 running
baseflight, the min/max can be set with CLI commands as follows:

set minthrottle=1064

set maxthrottle=1864

Otherwise, to calibrate the ESC, REMOVE ALL PROPELLERS and follow the
steps that may be documented for your flight controller board. KK2 boards
have a two-button procedure that emits calibration pulses automatically.
With KK boards, the Yaw pot must be set to the minimum position in order
to enable "pass through" mode from the RX input to the ESC output. The
ESCs are then calibrated to the radio's throttle stick.

Calibration may also be done with a servo tester or with the ESC directly
connected to an RX channel. The only requirement is that the input pulse
has to be at or above PROGRAM_RC_PULS (default 1460us) to enter
programming mode. This will differ slightly on boards with no external
oscillator. If calibrating ESCs individually, try to maintain a close
temperature and at least 6-7V input voltage (the ATmega8 oscillator
speeds up as temperature and Vcc decrease).

With the propellers removed and the source (radio, servo tester, or flight
control board) set to full throttle, power up the ESC and wait for a
single beep after the typical rising initialization beeps. This indicates
the high pulse length has been saved to RAM. Move the stick or knob to
the lowest setting, and wait for two beeps. This indicates that the low
pulse length has been saved to RAM.

If RC_PULS_NEUTRAL has been enabled (RC Car-style reverse mode), move the
stick/knob to the center, and wait for three beeps. This indicates that
the neutral pulse length has been saved to RAM.

If the stick is not moved after the final setting, the ESC should now
write the RAM settings to EEPROM, and continue startup. The ESC should
recognize the same pulse length input as a valid arming pulse, and arm
and work as usual.

The input PWM pulse is measured in 24-bit space and scaled in 16.16 space
to fit the number of PWM steps defined by POWER_RANGE - MIN_DUTY. There
should be no measurable aliasing or quantization. Alternatively, the
values may be adjusted in EEPROM directly. While calibrating, margins of
1/16th on the low end and 1/32nd on the high end are used to try to
avoid problems with arming and reaching full throttle during temperature
extremes. If desired, margins may be adjusted in tgy.asm between rc_prog2
and rc_prog5.

There is currently no way to reset (remove) the calibration other than by
clearing the EEPROM (or reflashing without EESAVE set). This may be
implemented in the future by some basic stick programming feature.

NOTE: As of 2012-03-15, throttle calibration is disabled when a brown-out
reset is detected. I accidentally calibrated an ESC when testing with a
NiCd pack. The pack could not supply enough current, resulting in a
brown-out reset of all ESCs, excluding the flight controller. I did not
lower the throttle in time, resulting in one ESC getting a stable enough
signal to store a new calibration. When intentionally calibrating, be
sure that you cleanly connect the power. If you don't hear the rising
beeps, remove the power for a few seconds to allow the capacitors to
discharge, then try again.

Troubleshooting
---------------

There are 4 main beep frequencies used at different intervals and lengths
to signal the various operation and fault states. These beep frequencies
are labelled here as f1 through f4, where a higher number indicates a
higher frequency. Repetition indicates longer beeps.

If CHECK_HARDWARE is enabled (default on the Afro and TBS boards), the
drive and sense circuitry will be tested for correct operation at boot.
This check will prevent beeping if it would be unsafe to do so (for
example, if beeping would end up completing a short circuit), and so may
prevent further damage in some cases.

Error conditions from CHECK_HARDWARE are flashed through the LED(s), if
present, or beeped (if possible) before anything else, in a looping
pattern, for as long as the error condition(s) persist. All conditions
cannot be checked on some hardware due to pin routing, but will be
indicated by beeping or flashing a particular count followed by a pause.
If more than one error condition is present, each error will be reported
in sequence. The error conditions are:

	1: Phase A stuck high
	2: Phase B stuck high
	3: Phase C stuck high
	4: AIN1 stuck high
	5: AIN0 stuck high
	6: Phase A low-side drive broken
	7: Phase B low-side drive broken
	8: Phase C low-side drive broken
	9: Phase A high-side drive broken
	10: Phase B high-side drive broken
	11: Phase C high-side drive broken

Note that if the ESC resets while the motor is still spinning (such as
after a brief power cut), it is possible that the induced current will
cause some transient errors that will clear once the motor stops, and
then operation will continue.

During boot, the MCUCSR register is checked to see the reason for reset.
For exact behaviour, see near "Check reset cause" in tgy.asm. Here are the
expected beep sequences:

f1 f2 f3: Regular startup with nothing special detected

f3 f1: Voltage brown-out bit was set (MCU voltage dropped below 2.7V/4.0V)

f4: External reset (via the reset pin, as in after programming)

looping f1 f1 f3 f3: Watchdog reset (previous execution locked up)

looping beeps (8) of f2 or f4: Unknown (beeps out all MCUCSR bits, LSF)

fast repeating beeps of f4 while idle: Noise on PWM input detected

Once a valid input source is found and receiving idle throttle, f4 f4 f4
(a long f4 beep) indicates that the ESC is armed and will start the motor
when throttle goes non-zero. If you are unable to start the motor and are
not hearing the forth, long beep, try lowering the throttle trim, or
raise it all the way to start throttle calibration (above).

If no input command is received for about 1 second, f3 f2 is beeped and
the ESC returns to disarmed state, waiting for a valid arming signal.

If no valid input is received while disarmed and BEACON is enabled, an f3
beep will be emitted after about 8 seconds and then every 4 seconds.

The various beep frequencies use different FET combinations (rather than
all FETs at the same time) to try to help diagnose boards with failed
FETs or possible incorrect firmware pin configuration (build target). If
you hear only one or two of the usual three power-up beeps, and the board
worked previously, it is likely that one of the FETs has burned out. The
ESC may still start and run the motor like this, but the motor will sound
bad, and power and efficiency will be reduced.
