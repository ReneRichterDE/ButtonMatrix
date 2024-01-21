# ButtonMatrix

Arduino/ESP library for keypad support

## Description

The library allows easy interfacing with a keypad.
Not only keypads connected directly to the IO pins of the microcontroller are supported, but also via I2C.
There is a connector provided for the Adafruit MCP23017 library, but you can also easily create your custom one if required.

Development of the library has been inspired by the Keypad library of Mark Stanley and Alexander Brevig
I wanted it to be more flexible and implement a more object oriented approach.
The interface has been slightly aligned to the Button2 library created by Lennart Hennigs.


The library supports event based handling of button state changes and button actions like click and long press detection.
NOTE: Double click actions are currently not supported. This will be implemented in the future though.


## License

Copyright (c) 2023-2024 Rene Richter

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; version
2.1 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.





