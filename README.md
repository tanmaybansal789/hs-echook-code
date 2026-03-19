# eChook Telemetry Board

## Overview
The eChook Board is a data-collection hub for electric vehicles. It uses an Arduino Nano Every to monitor motor/battery temperature, RPM, and throttle position. Data is sent via Bluetooth to the OMNI Telemetry App and can be uploaded to the eChook Live Data portal.

## Technical specifications
* **Microcontroller:** Arduino Nano Every (ATmega4809)
* **Bluetooth Name:** eChook (Pairing PIN: xyz123)
* **Live Data Portal:** data.echook.uk (Car: 'The Predator' | Pass: 'aNATeRin')

---

## Software setup
To modify or upload code, you must configure the Arduino IDE:

1.  **Install Drivers:** Go to `Tools` -> `Board` -> `Boards Manager`. Search for **megaAVR** and install "Arduino megaAVR Boards".
2.  **IDE Settings:**
    * **Board:** Arduino Nano Every
    * **Processor:** ATmega4809
    * **Port:** [Select the active COM port]

---

## How to update code
1.  **Backup:** Always save a copy of the current code before making changes.
2.  **Verify:** Click the **Checkmark** icon to ensure there are no syntax errors.
3.  **Upload:** Connect the board via USB and click the **Arrow** icon.
4.  **Monitor:** Open `Tools` -> `Serial Monitor` to verify the board is outputting data correctly.

---

## Troubleshooting
* **Board not found:** Try a different USB cable or port. Restart the IDE.
* **Upload error:** Ensure the correct "Port" and "Processor" are selected. Press the physical **Reset** button on the board just before uploading.
* **Bluetooth won't pair:** Hold the button under the Bluetooth module until the light flashes to enter pairing mode.