## UID: 006284295

## Pipe Up

This program takes in programs as arguments and returns the output as if there were pipes between them.

## Building

Simply run make to build the program
``` shell
make
```

## Running

You may pass in arguments as programs (without options) and expect the same output as pipes.
For example, the following command
``` shell
./pipe ls wc
```
Should return the same as
``` shell
ls | wc
```
In my case, this returns:
      7       7      63

## Cleaning up
Simply run "make clean" to clean up the binary files.
``` shell
make clean
```
