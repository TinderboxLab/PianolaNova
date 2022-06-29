#!/bin/bash 
sleep 10
sudo python3 /home/pi/Documents/PianolaNova/http_led_server.py &
sleep 10
chromium-browser --kiosk --autoplay-policy=no-user-gesture-required --check-for-update-interval=259200 --app=https://tinderboxlab.github.io/PianolaNova/p2pmidi#c5dwqeqqb2808-e7ea-4a3f-82eb-a8a8bb905eea  &
