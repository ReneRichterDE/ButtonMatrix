# Changelog

**Note:** Unreleased changes are checked in but not part of an official release (available through the Arduino IDE or PlatfomIO) yet. This allows you to test WiP features and give feedback to them.

## [unreleased]

- Added support for large keypads connected to multiple MCPs

## [1.0.3] - 2024-09-13

- Added support for input inversion (for keypads where the circuit is opend when key is pressed)
- Updated examples to show how to activate input inversion

## [1.0.2] - 2024-09-09

- Fixed declaration of Button array in Example04_i2c_ioexpansion.ino
- Added preprocessor statements to ButtonBaseItf.h to avoid duplicate definition of data types

## [1.0.1] - 2024-01-23

- Changed pin numbers in examples to match the Arduino Nano
- Name of the library is now the same for the Arduino IDE as well as for PlatformIO
- Removed nonexisting main.cpp reference from .gitignore


## [1.0.0] - 2024-01-21

- initial release

## Note

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).
