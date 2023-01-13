# AAC-Smart-Home

## Datasheet - Program Codes
### Program Code Format
```
#<OP_CODE>$<DATA>
```

### Device Codes (Requests)
| OpCode | Description | Data | Example Usage |
| --- | --- | --- | --- |
| D02 | Returns whether the given pin code is correct. | 0 - Incorrect; 1 - Correct | `#D02$1` |
| D03 | Returns current temperature in Celsius. | float - Temperature | `#D03$25.00` |
| D04 | Returns current humidigy percentage. | float - Humidity | `#D04$31.00` |
| D05 | Returns lights state. | ON - Lights ON; OFF - Lights OFF | `#D05$OFF` |
| D06 | Returns whether **P06** was successful. | 0 - Failure; 1 - Success | `#D06$0` |
| D07 | Returns whether **P07** was successful. | 0 - Failure; 1 - Success | `#D07$1` |
| D08 | Returns whether **P08** was successful. | 0 - Failure; 1 - Success | `#D08$1` |
| D09 | Returns current door button state. | 0 - LOW; 1 - HIGH | `#D09$1` |

### Program Codes (Responses)
| OpCode | Description | Data | Example Usage |
| --- | --- | --- | --- |
| P02 | Checks whether the given pin code is correct. | int - Pin Code | `#P02$3333` |
| P03 | Gets current temperature. | float - Temperature | `#P03` |
| P04 | Gets current humidity percentage. | float - Humidity | `#P04` |
| P05 | Gets current lights state. | ON - Lights ON; OFF - Lights OFF | `#P05` |
| P06 | Sets current lights state. | ON - Lights ON; OFF - Lights OFF | `#P06$0` |
| P07 | Activates alarm. | - | `#P07` |
| P08 | Sets ideal temperature value. | int - Temperature | `#P08$16` |
| P09 | Gets current door button state. | - | `#P09` |