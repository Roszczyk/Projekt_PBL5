# PBL5-PAM Server


## Use virtual environments (venv)
Install necessary dependencies:
```
git clone https://github.com/Roszczyk/Projekt_PBL5.git
cd Projekt_PBL5\Server
python -m venv venv
venv\Scripts\activate
pip install -r requirements.txt
```

##
#### Server usage:
```
py .\src\app.py
```
Receives messeges via MQTT, stores in SQL-Lite, gives API for mobile app.
Swagger available at ```http://127.0.0.1:5000/swagger/#/```.

##
#### RIOT-simulator:
```
py .\test\RIOT-simulator.py
```
Simulates work of one LORA sensor node.

##
#### db-addFakeReadings:
```
py .\test\db-addFakeReadings.py
```
Generates fake entries in database for testing purposes.


##
##
Using ```python3``` instead of ```py``` may solve some problems.
