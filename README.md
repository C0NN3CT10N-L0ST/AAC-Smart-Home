# AAC-Smart-Home

## Communication Protocol
TODO

![Communication Protocol](/assets/Smart-Home-Protocol.png)
___

## Datasheet - Communication Protocol Codes
### Request/Response Format
```
#<CODE>$<DATA>
```

### Server Codes (Requests)
| Code | Description | Data | Example Usage |
| --- | --- | --- | --- |
| P02 | Checks whether the given pin code is correct. | int - Pin Code | `#P02$3333` |
| P03 | Gets current temperature. | float - Temperature | `#P03` |
| P04 | Gets current humidity percentage. | float - Humidity | `#P04` |
| P05 | Gets current lights state. | ON - Lights ON; OFF - Lights OFF | `#P05` |
| P06 | Sets current lights state. | 0 - Lights ON; 1 - Lights OFF | `#P06$0` |
| P07 | Activates security alarm. | - | `#P07` |
| P08 | Gets current door button state. | - | `#P09` |
| P09 | Gets current environment brightness percentage. | - | `#P10` |
| P10 | Gets current flames status. | - | `#P11` |
| P11 | Gets current lights control mode. | - | `#P11` |
| P12 | Sets current lights control mode. | 0 - Program; 1 - Remote | `#P12` |
| P13 | Sets current lights color. | R - Red; G - Green; B - Blue; W - White | `#P13$B` |
| P14 | Activates fire alarm. | - | `#P14` |

### Device Codes (Responses)
| Code | Description | Data | Example Usage |
| --- | --- | --- | --- |
| D02 | Returns whether the given pin code is correct. | 0 - Incorrect; 1 - Correct | `#D02$1` |
| D03 | Returns current temperature in Celsius. | float - Temperature | `#D03$25.00` |
| D04 | Returns current humidigy percentage. | float - Humidity | `#D04$31.00` |
| D05 | Returns lights state. | ON - Lights ON; OFF - Lights OFF | `#D05$OFF` |
| D06 | Returns whether **P06** was successful. | 0 - Failure; 1 - Success | `#D06$0` |
| D07 | Returns whether **P07** was successful. | 0 - Failure; 1 - Success | `#D07$1` |
| D08 | Returns current door button state. | 0 - LOW; 1 - HIGH | `#D09$1` |
| D09 | Returns current environment brightness percentage. | int - Brightness | `#D10$23` |
| D10 | Returns current flames status. | 0 - Not detected; 1 - Flames detected | `#D11$0` |
| D11 | Returns current lights control mode. | 0 - Program; 1 - Remote | `#D11$0` |
| D12 | Returns whether **P12** was successful. | 0 - Failure; 1 - Success | `#D12$1` |
| D13 | Returns whether **P13** was successful. | 0 - Failure; 1 - Success | `#D13$0` |
| D14 | Returns whether **P14** was successful. | 0 - Failure; 1 - Success | `#D14$0` |