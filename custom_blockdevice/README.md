# Add your own block device

All components of pico\_filesystem are pluggable. Adding your own block device objects to the system is easy.
This demonstration adds a noisy block device that outputs `read`, `erase` and `program` to the block device in hexadecimal to the UART.
