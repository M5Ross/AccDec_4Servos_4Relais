# AccDec_4Servos_4Relais

DCC Accessory Decoder to drive 4 servo plus 4 relais for frog polarization

Originally derived from an example of library NmraDcc.

Library required:

- [NmraDcc] (public)
- [ConfCVlib] (manual installation)
- [DccSerialCom] (manual installation)

Features:

- 4 available outputs for servos.
- 4 micro 5V relais for frog polarization with N, NO and NC contacts.
- Customizable pulse time or continuos mode in case to drive light bulbs.
- Separate power socket or autopower via DCC.
- ICSP port and SerialCom port.
- Sigle address mode (continuos address for each output) or multiple address mode (each output has it own address).

Designed to be configured with the custom PC tool [DecoderConfigurator], or in standardize way via CV.

[NmraDcc]: https://github.com/mrrwa/NmraDcc
[ConfCVlib]: https://github.com/M5Ross/ConfCVlib
[DccSerialCom]: https://github.com/M5Ross/DccSerialCom
[DecoderConfigurator]: https://github.com/M5Ross/DecoderConfigurator