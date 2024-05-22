# D/AVE2D DRIVER

This repository contains D/AVE2D driver for Alif Semiconductor devices

## Layer 2 Driver (D2)

Layer 2 driver is an abstracion layer which is completey hardware independent and is provided by TES.

Complete driver overview is available [here](https://lpccs-docs.renesas.com/DA1470x/UM-B-157_DA1470x-GPU-API-Manual/files/doc/overview-txt.html).

Resources allocated by Layer 2 driver are described in `d2/DAVE2D_driver_allocations.xls`

## Layer 1 Driver (D1)

Layer 1 implements hardware specific functions that Layer 2 relies on

## Layer 0 (D0)

Layer 0 provides a set of memory managers.
