const char HTTP_CONF[] PROGMEM = R"=====(
<div>LED Konfiguration:</div>
<div id='div_numleds'><input type='text' id='numleds' name='numleds' length=1 placeholder='Anzahl LEDs' value='{numleds}' required pattern='[0-9]{1,2}'></div>
<div id='div_rgborder'><label for='rgborder'>RGB Reihenfolge</label><span class='cob ckb'><select id='rgborder' name='rgborder'><option {srgb} value='10'>RGB</option><option {sgrb} value='66'>GRB</option></select></span></div>
<hr />
<div>HomeMatic Konfiguration:</div>
<div id='div_ccu'><input type='text' id='ccuip' name='ccuip' length=1 placeholder='IP der CCU2' value='{ccuip}' pattern='((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$'></div>
<div id='div_devicename'><input type='text' id='devicename' name='devicename' length=1 placeholder='Ger&auml;tename' value='{v}' pattern='\w{1,}'></div>
<div id='div_restorestate'><label for='restorestate'>Boot: Lade Werte von CCU&nbsp;&nbsp;&nbsp;</label><span class='ckb'><input id='restorestate' type='checkbox' name='restorestate' {ckrestore} value=1></span></div>
<div id='div_pirtoccu'><label for='pirtoccu'>BWM an CCU senden&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</label><span class='ckb'><input id='pirtoccu' type='checkbox' name='pirtoccu' {ckpirtoccu} value=1></span></div>
<div>Dimmer-Konfiguration:</div>
<div id='div_color1'><input type='input' id='color1' name='color1' length=1 placeholder='Farbe f&uuml;r Dimmer 10..19%' value='{color1}' onchange='setColor()' > <input type='color' id='cp_color1' name='cp_color1' onchange='setCPColor()'></div>
<div id='div_color2'><input type='input' id='color2' name='color2' length=1 placeholder='Farbe f&uuml;r Dimmer 20..29%' value='{color2}' onchange='setColor()' > <input type='color' id='cp_color2' name='cp_color2' onchange='setCPColor()'></div>
<div id='div_color3'><input type='input' id='color3' name='color3' length=1 placeholder='Farbe f&uuml;r Dimmer 30..39%' value='{color3}' onchange='setColor()' > <input type='color' id='cp_color3' name='cp_color3' onchange='setCPColor()'></div>
<div id='div_color4'><input type='input' id='color4' name='color4' length=1 placeholder='Farbe f&uuml;r Dimmer 40..49%' value='{color4}' onchange='setColor()' > <input type='color' id='cp_color4' name='cp_color4' onchange='setCPColor()'></div>
<div id='div_color5'><input type='input' id='color5' name='color5' length=1 placeholder='Farbe f&uuml;r Dimmer 50..59%' value='{color5}' onchange='setColor()' > <input type='color' id='cp_color5' name='cp_color5' onchange='setCPColor()'></div>
<div id='div_color6'><input type='input' id='color6' name='color6' length=1 placeholder='Farbe f&uuml;r Dimmer 60..69%' value='{color6}' onchange='setColor()' > <input type='color' id='cp_color6' name='cp_color6' onchange='setCPColor()'></div>
<div id='div_color7'><input type='input' id='color7' name='color7' length=1 placeholder='Farbe f&uuml;r Dimmer 70..79%' value='{color7}' onchange='setColor()' > <input type='color' id='cp_color7' name='cp_color7' onchange='setCPColor()'></div>
<div id='div_color8'><input type='input' id='color8' name='color8' length=1 placeholder='Farbe f&uuml;r Dimmer 80..89%' value='{color8}' onchange='setColor()' > <input type='color' id='cp_color8' name='cp_color8' onchange='setCPColor()'></div>
<div id='div_color9'><input type='input' id='color9' name='color9' length=1 placeholder='Farbe f&uuml;r Dimmer 90..99%' value='{color9}' onchange='setColor()' > <input type='color' id='cp_color9' name='cp_color9' onchange='setCPColor()'></div>
<div id='div_color10'><input type='input' id='color10' name='color10' length=1 placeholder='Farbe f&uuml;r Dimmer 100%' value='{color10}' onchange='setColor()' > <input type='color' id='cp_color10' name='cp_color10' onchange='setCPColor()'></div>
<div>Blinken f&uuml;r Dimmer-Werte ab...<br/>(11 = kein blinken)</div>
<div id='div_dimblink'><input type='text' id='dimblink' name='dimblink' length=1 placeholder='Blinken f&uuml;r Dimmer-Werte ab...' value='{dimblink}' required pattern='[0-9]{1,2}'></div>
)=====";

