# WemosD1 HomeMatic Status Display

## Zusammenfassung:
Ziel dieses Projektes ist der kostengünstige Bau einer Statusanzeige, ähnlich der von HomeMatic angebotenen.

**2 Bilder des ersten Prototypen:**<br>
<img src="Images/proto_front.jpg" width="300"> <img src="Images/proto_back.jpg" width="300">

## Teileliste:
- 1x [Wemos D1 mini](http://www.ebay.de/itm/272271662681) (ca. 3,29 EUR)
- 1x [Taster](http://www.ebay.de/itm/263057910534)
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

## Konfiguration des Wemos D1
Um den Konfigurationsmodus zu starten, muss der Wemos D1 mit gedrückt gehaltenem Taster (gestartet werden. Die blaue LED blinkt kurz und leuchtet dann dauerhaft.

Der Konfigurationsmodus ist nun aktiv.

Auf dem Handy oder Notebook sucht man nun nach neuen WLAN Netzen in der Umgebung.

Es erscheint ein neues WLAN mit dem Namen "WemosD1-xx:xx:xx:xx:xx:xx"

Nachdem man sich mit diesem verbunden hat, öffnet sich automatisch das Konfigurationsportal.

Geschieht dies nicht nach ein paar Sekunden, ist im Browser die Seite http://192.168.4.1 aufzurufen.

Es erscheint:<br>
<img src="Images/config_start.png" width="450">


"Konfigurieren" wählen. Es erscheint:<br>
<img src="Images/config_leer.png" width="300"><img src="Images/config_beispiel.png" width="300">

_(evtl. lade ich mal 2 gleichgroße Bilder hoch) ;)_

**Erklärung der Angaben:**
- SSID / WLAN-Key = Zugangsdaten zum heimischen WLAN
  - wenn eine statische IP festgelegt werden soll, kann man diese am unteren Ende der Konfigurationsseite eingeben
- IP der CCU2: IP der CCU2 :) 
- Gerätename: Name des CUxD Dimmers. Im Beispiel oben: "StatusDisplay"
- Boot: Lade Werte von CCU: wenn aktiviert, werden beim Hochfahren des Statusdisplays die letzten LED-Zustände von der CCU abgerufen
- Anzahl LEDs: Im "Normalfall" **16** - wer weniger oder mehr LEDs benutzen möchte, kann dies hier festlegen
- RGB Reihenfolge: Wenn die gewünschte Farbe nicht der tatsächlichen, von der LED wiedergegebenen Farbe entspricht, ist der Wert zu ändern. Die meisten WS2812B LEDs verwenden jedoch "GRB". 
- Farbe für Dimmer ...: hier legt man fest, welcher CUxD Dimmerwert (in 10er Schritten) mit welcher LED Farbe angezeigt werden soll. Die Farben sind in HEX-Werten ohne führende "#" anzugeben. Um die Farben zu "finden" gibt es online [Farbpaletten](http://htmlcolorcodes.com)
