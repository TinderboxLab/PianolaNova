# PianolaNova

[Leeds](https://tinderboxlab.github.io/PianolaNova/p2pmidi#c5dwqeqqb2808-e7ea-4a3f-82eb-a8a8bb905eea@Leeds)  
[Edinburgh](https://tinderboxlab.github.io/PianolaNova/p2pmidi#c5dwqeqqb2808-e7ea-4a3f-82eb-a8a8bb905eea@Edinburgh)

# Autostart scripts

The autostart script is in the pi home directory and is called ```pianola_nova_start_up.sh```
This script is executed from the system autostart located at ```/etc/xdg/lxsession/LXDE-pi/autostart```

## 2.4ghz wifi issues

when connecting to a 2.4ghz network there can be interference from HDMI. Changing resolution can solve this. 1024x768 works (1280x800 does not)

## testing MIDI

Pressing the "m" key will send a note to the remote

## LED Server on the PIs

Installation: 
```sudo pip3 install rpi_ws281x adafruit-circuitpython-neopixel```
