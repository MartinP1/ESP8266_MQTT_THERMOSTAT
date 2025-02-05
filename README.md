ESP8366_MQTT_THERMOSTAT

# Ein Heizkörperthermostat auf Basis eines ESP8266 und MQTT

## Projektstand
Das Projekt ist seit einigen Wochen im operativen Einsatz.
- Temperaturmessung über 3 Sensoren, Weitergabe der Werte über MQTT
- Zuspielen der Solltemperatur über MQTT auf den ESP
- Ansteuerung von Lüftern, die den Heizkörper von unten zur Erhöhung der Heizleistung anblasen (PWM)
- Zeitplan für Änderung der Solltemperatur (realisiert über ein ioBroker Blockly Skript)
- Preferences library für Gerätekonfiguration über MQTT (kein hartverdrahteter Device Name im Quellcode mehr!)
- todo  
  - Fenstersensor undone
 

## Hardwareübersicht

- ESP basierte Hardware - es werden drei Module unterstützt.
  - D1 Mini (ESP8266) Probleme mit instabiler Spannungsversorgung
  - D1 Mini (ESP32 - seit 1.12.2023)
  - S2 Mini (ESP32 - seit 1.12.2023) Neue Empfehlung!

- 12 V DC Spannungsversorgung - Ein Tisch-Netzteil von der Resterampe von [www.pollin.de](https://www.pollin.de/p/tischnetzteil-gp-1223c-12-v-2-3-a-5-5-2-1-mm-gebraucht-352200) 12V / 2,3A. 
- Danfoss Stellantrieb Thermot 230 V AC, M30x1,5 NC stromlos geschlossen [088H3220](https://store.danfoss.com/de/de/Climate-Solutions-W%C3%A4rmetechnik/Warmwasser-Fu%C3%9Fbodenheizungen/Thermische-Stellantriebe/Thermischer-Stellantrieb%2C-Thermot%2C-M-30-x-1-5%2C-Versorgungsspannung-%5BV%5D-%5BAC%5D%3A-230%2C-NC-%28stromlos-geschlossen%29%2C-1-00-m/p/088H3220) ca 10 €
  - angesteuert über ein Kleinrelais
- 4 Arctic 80 mm Gehäuselüfter ([3 Pin](https://www.arctic.de/F8/ACFAN00205A) habe ich gekauft, wahrscheinlich wäre [4 Pin](https://www.arctic.de/P8-PWM/ACFAN00149A) die bessere Wahl gewesen) blasen den Heizkörper von unten an
- 3 DS 18B20 Thermosensoren von Shelly https://www.pollin.de/p/shelly-temperMQTT_PUB_DEV_PREFIXatursensor-ds18b20-591021
  - zur Messung von Zulauf- und Ablauftemperatur des Heizkörpers und der Raumtemperatur

![Schaltplan](https://github.com/MartinP1/ESP8266_MQTT_THERMOSTAT/blob/main/Board_esp8266_d1mini_backup_Schaltplan.svg "Schaltplan")

Das Prototyp-System ist stillgelegt, aktuell läuft V1 einer Kicad-Leiterplatte.
Mindestausstattung wäre ein Thermosensor für die Raumtemperatur und ein Schaltrelais für die Ansteuerung des Ventil-Stellgliedes...

## Unsortiertes

### Was fehlt noch

- OTA Library für den OTA-Update der Firmware
- Schaltplanänderung in Kicad nachpflegen, zur Stabilitätsverbesserung werden die DS18B20 jetzt mit 5 V versorgt

### Verbesserungen - was würde ich in der Serie anders machen

- 4 Pin Lüfter?
- 24 V Variante des Danfoss Stellantriebs über DCDC Wandler mit Enable-Eingang?
- Full Fan speed konfigurierbar (255 scheint zu schnell zu sein ...)
- Neues Leiterplattenlayout mit 230 V Spannungesversorgung und Schaltrelais für Danfoss-Stellglied
- Migration des Projekts von Arduino-IDE auf PlatformIO

### Worklog

- JAN-2024 Start der Nutzung der Preferences Bibliothek
