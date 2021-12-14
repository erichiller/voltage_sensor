.\arduino-cli.exe compile --fqbn esp32:esp32:lolin32 --library ..\libraries\ --port COM8 --upload --verbose 


.\arduino-cli.exe compile --fqbn esp32:esp32:esp32s2 --library ..\libraries\ --port COM9 --upload --verbose 



.\arduino-cli.exe monitor -p COM8 -c baudrate=9600