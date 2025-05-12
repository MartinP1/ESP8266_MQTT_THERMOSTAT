# Probleme mit DS18B20 Sensoren

Die Sensoren fallen regelmäßig aus. 
Die letzte Firmware-Änderung soll die Überwachung noch einmal verbessern.
Ich werde hier mal etwas Protokoll führen, ob ein bestimmter Sensor betroffen ist, oder reihum alle Sensoren Ausfallneigung haben.
Bisher konnte ich den Fehler jeweils durch komplettes Spannungslos machen des ESP-Moduls beheben.

Die Versorgungsspannung der Sensoren habe ich schon von 3,3 auf 5 Volt angehoben.

## Protokoll Vorfälle

Seriennummern /IDs der Sensoren.
| Datum | 3C13E381F66D |  3C1E0457CBD0 |  3C610457DB22 |
|---|---|---|---|
| 10-MAY-2025 | ok | fail | ok |

