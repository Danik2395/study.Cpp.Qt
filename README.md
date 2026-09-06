# Qt labs

## General architecture

```
       [MainWindow]       *-------->[QVboxLayout]
           |              |              |
        (central)         |         (adjust size)
           |              |              |
           V              |              V
       [TabWidget ]-->[HubWidget]-->{lab button}
           |                             |
           |                             V
           *---->[LabNWidget]<------(opened on tap)
```
