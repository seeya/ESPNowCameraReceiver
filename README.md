# Introduction

A simple PoC to receive images over ESPNOW protocol and write it over serial.

As the ESPNOW protocol sends a maximium of 250 bytes per transmission, there is a need to merge the data in order.

A naive approach is taken whereby

1. Receive STX and Length of Payload
2. Calculate total number of chunks
3. Receive individual chunks and store in correct order
4. End receive when all chunks done

```
[STX, LEN]
[Chunk Index 1, Data]
[Chunk Index 2, Data]
[Chunk Index 4, Data]
[Chunk Index 3, Data]
...
```

# ESPNOW Protocol Notes

1. Max size per transmission is 250
2. Delay required to have the packets arrive in order
3. Encryption is available
