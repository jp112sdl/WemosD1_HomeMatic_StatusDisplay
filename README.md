# WemosD1 HomeMatic Status Display

## Zusammenfassung:
Ziel dieses Projektes ist der kostengünstige Bau einer Statusanzeige, ähnlich der von HomeMatic angebotenen.

**2 Bilder des ersten Prototypen:**<br>
<img src="Images/proto_front.jpg" width="300"> <img src="Images/proto_back.jpg" width="300">

## Teileliste:
- 1x [Wemos D1 mini](http://www.ebay.de/itm/272271662681) (ca. 3,29 EUR)
- 1x [Taster](http://www.ebay.de/itm/263057910534), wird jedoch nur zur (Erst-)Konfiguration benötigt
(_statt des Tasters kann man auch mit einer temporären Drahtbrücke arbeiten_) 
- 16x RGB LED [WS2812B](http://www.ebay.de/itm/222580456169) (ca. 5,95 EUR für 50 Stück!)
- _optional: 1x HC-SR501 PIR Bewegungsmelder_

Aufbau:<br>
![wiring](Images/wiring.png)

## Einrichtung der CCU
### Erstellen eines neuen CUxD Exec (sofern nicht bereits vorhanden!)<br>
<img src="Images/CUxD_Exec_erstellen.png">

### Erstellen eines neuen CUxD Dimmers<br>
<img src="Images/CUxD_Dimmer_erstellen.png">

### Erstellen eines Programms<br>
<img src="Images/HomeMatic_Programm.png">

Das Skript im DANN-Block (die IP-Adresse ist entsprechend anzupassen!):
```
string displayIP = "192.168.1.99";
string channel = dom.GetObject(((dom.GetObject("$src$")).Channel())).Name();
real r = dom.GetObject("$src$").Value();
r = r + 0.0000001;

integer val = ( r * 10).ToInteger();
integer ledNum = channel.StrValueByIndex(":", 1).ToInteger();

dom.GetObject("CUxD.CUX2801001:1.CMD_EXEC").State ("/usr/local/addons/cuxd/curl -s -k -d led="#ledNum#" -d dim="#val#" http://"#displayIP#"/set");
```
