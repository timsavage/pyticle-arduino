###############
Pyticle Arduino
###############

A Pyticle client library for Arduino. 

This client library includes an implementation of MQTT-SN that is used as the underlying transport for Pyticle messages.

The client library currently work with any network that extends from the Arduino UDP interface, in future 
it will also support other stream and packet based physical layers including TCP, Serial, SPI and I2C. 
`COBS <https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing>`_ will be utilised to break the 
streams into message frames.
