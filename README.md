# Overview
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
    Released --> [*]
```

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
        +IOIndex ioIndex
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

Callback explanation.
There are 3 callbacks, we could have probably been better off just pulling ButtonState
about the current state of the classes. Right now, when we change the state of a DivisionState
we are doing it via a callback.
For instance, the # clk
