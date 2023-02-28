ESP8366_MQTT_THERMOSTAT

# Ein Heizkörperthermostat auf Basis eines ESP8266 und MQTT

## Projektstand
Das Projekt ist in der Alphaphase. Bisher sind nur kleine Teile realisiert.
   - Testbar
     - Temperaturmessung über 3 Sensoren, Weitergabe der Werte über MQTT
     - Zuspielen der Solltemperatur über MQTT auf den ESP
   - In der Planung
     - Ansteuerung von Lüftern, die den Heizkörper von unten zur Erhöhung der Heizleistung anblasen (PWM)
     - Fenstersensor
     - Zeitplan für Änderung der Solltemperatur
     - Es steht die Entscheidung aus, ob der Thermostat Standalone-Fähig ist, oder ob die Verknüpfung der Eingangsgrößen in iobroker erfolgt.


## Hardwareübersicht

- ESP8266 (Wemos D1Mini)
- 12 V DC Spannungsversorgung - Ein Tisch-Netzteil von der Resterampe von [www.pollin.de](https://www.pollin.de/p/tischnetzteil-gp-1223c-12-v-2-3-a-5-5-2-1-mm-gebraucht-352200) 12V / 2,3A. 
- Danfoss Stellantrieb Thermot 230 V AC, M30x1,5 NC stromlos geschlossen [088H3220](https://store.danfoss.com/de/de/Climate-Solutions-W%C3%A4rmetechnik/Warmwasser-Fu%C3%9Fbodenheizungen/Thermische-Stellantriebe/Thermischer-Stellantrieb%2C-Thermot%2C-M-30-x-1-5%2C-Versorgungsspannung-%5BV%5D-%5BAC%5D%3A-230%2C-NC-%28stromlos-geschlossen%29%2C-1-00-m/p/088H3220) ca 10 €
  - angesteuert über ein Kleinrelais
- 4 Arctic 80 mm Gehäuselüfter ([3 Pin](https://www.arctic.de/F8/ACFAN00205A) habe ich gekauft, wahrscheinlich wäre [4 Pin](https://www.arctic.de/P8-PWM/ACFAN00149A) die bessere Wahl gewesen) blasen den Heizkörper von unten an
- 3 DS 18B20 Thermosensoren von Shelly https://www.pollin.de/p/shelly-temperatursensor-ds18b20-591021
  - zur Messung von Zulauf- und Ablauftemperatur des Heizkörpers und der Raumtemperatur


Der Prototyp wird einen "Vollausbau" bekommen. Ggfs. lasse ich später eine Leiterplatte machen, die dann optional auch nur teilweise bestückt funktioniert (mit angepasster Firmware?)
Mindestausstattung wäre ein Thermosensor für die Raumtemperatur und ein Schaltrelais für die Ansteuerung des Ventil-Stellgliedes...

## Unsortiertes

### Was fehlt noch

- Schaltplanentwurf ablegen usw...
- Die DS18B20 Sensoren haben eine eindeutige "eingebrannte" 64Bit-ID. Ich spreche sie derzeit über den "Index" an (0...2). Ist sicher, dass die Sensoren nach jedem Neustart des ESP8266 in selber Reihenfolge gefunden werden? Ggfs zusätzlich die 64-Bit-ID publishen?

### Verbesserungen - was würde ich in der Serie anders machen

- 4 Pin Lüfter?
- 24 V Variante des Danfoss Stellantriebs über DCDC Wandler mit Enable-Eingang?


