# Commander LoRa Dongle

A relay dongle, to interface between incoming LoRa commands and the Serial port on the PC.

This is a dumb unit. It has no knowledge of any LoRa comms protocol, or any other protocol in general. It just forwards bytes of data!

**Uses**

+ Forward incoming LoRa signals from nodes to the serial port for further processing in web-based UI
+ Transmitting serial port commands out via LoRa

**Options**

Set `report_rssi = true` to append the RSSI for the message to the Serial string with a `|` divider.

For example, a -44dBm RSSI would be appended as: `RRr.d00.siq|-44`

**Build**

+ Environment: `PlatformIO`
+ Target hardware: `Adafruit Feather M0 with RFM95 LoRa Radio - 900MHz`

**Links**

+ https://www.adafruit.com/product/3178
+ https://shop.pimoroni.com/products/adafruit-feather-m0-with-rfm95-lora-radio-900mhz-radiofruit
