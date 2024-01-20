### Various Diagrams and Definitions

![image](state_machine.jpeg)

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

TODO:
Callback divisionDisplayCallback
is right now being called in the HELDDOWN 
state machine location. It should not, it should
immediately show when you enter pressed state, then
we need a global flag to indicate that the button
press expired for that particular button when the
HELDDOWN state is entered for that IOIndex


