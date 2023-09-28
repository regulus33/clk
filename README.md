# Overview
CLK is a master clock and clock divider with 4 configurable outputs that can either divide the master clock or an external input. The outputs are managed via Output and Divider classes.

**Hardware:** Each output is a jack with a parallel red LED, pulsing between 0V and +5V based on the clock's duty cycle.

**Software Architecture:**

Output: Manages the physical pins D8-D11 directly via PORT_B, avoiding standard Arduino libraries.
Divider: Each instance has a division and a counter that increments on every clock tick and wraps at 8.
Efficiency: Instead of writing to individual pins, an entire byte pinStates is written to PORT_B, reflecting the state of the 4 Divider instances.

**Clock Tick:** The master clock tick triggers the update of pinStates, which is then written to the outputs.

The system only requires an update to `pinStates` during each master clock tick, ensuring the outputs and connected gear stay synchronized.


# Efficient Pin State Management on ATmega328p
### Using a Single Byte to Track States
```cpp
uint8_t pulseStates = 0b00001111;  // Initialize to whatever you like

// To set a particular bit (e.g., bit 2) to 1
pulseStates |= (1 << 2);

// To set a particular bit (e.g., bit 2) to 0
pulseStates &= ~(1 << 2);

// To toggle a particular bit (e.g., bit 2)
pulseStates ^= (1 << 2);

```


| Command       | Description                                     | Example                                   |
|---------------|-------------------------------------------------|-------------------------------------------|
| `run`         | Starts your program                             | `run`                                     |
| `break`       | Sets a breakpoint at a specified function or line| `break main` or `break line_number`       |
| `continue`    | Continues running the program until a breakpoint| `continue`                                 |
| `step`        | Executes the next line of code                  | `step`                                     |
| `next`        | Similar to `step`, but doesn't enter functions  | `next`                                     |
| `print`       | Displays the value of an expression             | `print variable_name`                      |
| `quit`        | Exits GDB                                       | `quit`                                     |
| `info`        | Shows various kinds of information              | `info breakpoints`                         |
| `clear`       | Removes a breakpoint                            | `clear line_number` or `clear function_name`|
| `backtrace`   | Shows the call stack                            | `backtrace`                                |
| `list`        | Lists source code                               | `list` or `list line_number`               |
| `watch`       | Sets a watchpoint                               | `watch variable_name`                      |
| `delete`      | Deletes a breakpoint/watchpoint                 | `delete breakpoint_number`                 |
| `frame`       | Selects a frame from the call stack             | `frame frame_number`                       |
| `set`         | Changes a variable's value                      | `set variable_name=value`                  |
| `disassemble` | Shows the assembly code for a function          | `disassemble function_name`                |
| `info locals` | Lists all local variables in the current frame  | `info locals`                              |
