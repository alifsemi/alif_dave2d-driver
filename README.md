# D/AVE2D DRIVER

This repository contains D/AVE2D driver for Alif Semiconductor devices

## Layer 2 Driver (D2)

Layer 2 driver is an abstracion layer which is completey hardware independent and is provided by TES.

Complete D2 documentation is available at `d2/doc/driver_l2/index.html` and `d2/doc/sw_interface/index.html`.

Resources allocated by Layer 2 driver are described in `d2/DAVE2D_driver_allocations.xls`

## Layer 1 Driver (D1)

Layer 1 implements hardware specific functions that Layer 2 relies on.

Complete D1 documentation is available at `d1/doc/driver_l1/index.html`.

## Layer 0 (D0)

Layer 0 provides a set of memory managers.

## Requirements

This CMSIS pack requires some packs to be installed and added to the project:
* [ARM::CMSIS@5.9.0](https://github.com/ARM-software/CMSIS_5/releases/tag/5.9.0)
* [AlifSemiconductor::Ensemble@1.1.1](https://github.com/alifsemi/alif_ensemble-cmsis-dfp/releases/tag/v1.1.1)
