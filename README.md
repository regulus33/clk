# CLK
### Arduino based eurorack clock source, dynamic clock divider and midi clock source. 

![module](module.jpg)

# How to Build and Run

### Dev
Run with logger:

`pio run --environment dev -t upload && pio device monitor -b 9600`

Run tests:

`pio test --environment test`

Build and upload for production:

`pio run --environment prod -t upload`

### Various Diagrams and Definitions

# Visual Overview
```mermaid
stateDiagram-v2
    [*] --> Released
    Released --> DebouncePress: pinValue == 0
    DebouncePress --> Pressed: time > debounceDelay && pinValue == 0
    Pressed --> HeldDown: time > holdDelay
    HeldDown --> DebounceRelease: pinValue == 1
    Pressed --> DebounceRelease: pinValue == 1
    DebounceRelease --> Released: time > debounceDelay && pinValue == 1
    DebouncePress --> Released: time > debounceDelay && pinValue == 1
    DebounceRelease --> Pressed: time > debounceDelay && pinValue == 0
```

## Explanation of each State 👆
1. **Released** - Default state, idle state of button before press
2. **DebouncePress**  - State in between released and pressed, we check for jitter interference here before committing to a 
full press
3. **Pressed**  - We have succesfully debounced any jitter and can confidently say that the button is pressed. Big note: this doesn't really do any single thing. this just preceded Release which is the point at which something is actually done.
4. **HeldDown** - We have stayed in pressed state for a long time...
4. **Operation Canceled** - We are cancelling the current operation because the button was not released within a resonable time...
4. **DebounceRelease** - State just before release 

# ButtonState -> ProgramState communication schema
```mermaid
classDiagram
    class ProgramState {
        -ClockMode clockMode
        -uint8_t pulseReceived
        +ClockMode getClockMode()
        +void setClockMode(ClockMode clockMode)
    }
    
    class DivisionState {
        +uint8_t division
        +DivsionMode divisionMode
        
    }
    
    class ButtonState {
        +GPIOIndex ioIndex
        -void((*onClockModeChange)(ClockMode))
        -void((*onDivisionChange)(uint8_t))
        -void((*onDivisionModeChange)(DivsionMode))
    }
    
    class GlobalCallBack {
        +static void pulseChangeCallback()
        +static void onClockModeChange(ClockMode clockMode)
        +static void onDivisionChange(uint8_t division)
        +static void onDivisionModeChange(DivsionMode divisionMode)
    }
    
    ProgramState .. ButtonState : has many
    ProgramState .. DivisionState : has many
    ButtonState .. GlobalCallBack : onClockModeChange(clockMode)
    ButtonState .. GlobalCallBack : onDivisionChange(division)
    ButtonState .. GlobalCallBack : onDivisionModeChange(divisionMode)
    GlobalCallBack --|> ProgramState : changes clock modes
    GlobalCallBack --|> DivisionState : changes division modes
    GlobalCallBack --|> DivisionState : changes divisions
```


