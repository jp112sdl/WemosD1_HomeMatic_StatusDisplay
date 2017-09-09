# WemosD1 HomeMatic Status Display

HomeMatic Skript
```
string displayIP = "192.168.1.99";
string channel = dom.GetObject(((dom.GetObject("$src$")).Channel())).Name();
real r = dom.GetObject("$src$").Value();
r = r + 0.0000001;

integer val = ( r * 10).ToInteger();
integer ledNum = channel.StrValueByIndex(":", 1).ToInteger();

dom.GetObject("CUxD.CUX2801001:1.CMD_EXEC").State ("/usr/local/addons/cuxd/curl -s -k -d led="#ledNum#" -d dim="#val#" http://"#displayIP#"/set");
```
