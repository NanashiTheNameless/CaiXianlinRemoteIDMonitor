# CaiXianlinRemoteIDMonitor

A simple RF receiver monitor for the CaiXianlin 433MHz protocol. Captures and displays incoming shock collar commands with ID, channel, mode, and intensity information.

## Features

- Real-time RF command monitoring
- Displays command ID (hex and decimal)
- Shows channel (1-3)
- Command type detection (Shock, Vibrate, Beep, Light, Stop)
- Debouncing to prevent duplicate outputs
- Terminator signal identification

## Hardware Requirements

- Arduino compatible microcontroller
- 433MHz RF receiver module connected to pin 20

## Usage

1. Upload the sketch to your microcontroller
2. Open Serial Monitor at 115200 baud
3. RF commands will be displayed as received

Example output:

```text
ID: 0x7FFF (32767) | Channel: 1 | Mode: Vibrate
ID: 0x7FFF (32767) | Channel: 2 | Mode: Shock
ID: 0x7FFF (32767) | Channel: 1 | Mode: Stop
```

## License

BSD 2-Clause License
