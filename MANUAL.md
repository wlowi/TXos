# TXos Bedienungsanleitung

Diese Anleitung beschreibt die TXos Version 0.5.9 (August 2025)

---

# Inhalt

[Grundsätzliche Bedienung](#grundsätzliche-bedienung)<br/>
[Konzept](#konzept)<br/>
[Blockschaltbild](#blockschaltbild)<br/>
[Schalter](#schalter)<br/>
[Hauptbildschirm](#hauptbildschirm)<br/>

[Systemeinstellungen](#systemeinstellungen)<br/>

[Modellauswahl](#modellauswahl)<br/>
[Import/Export](#importexport)<br/>
[Servo Monitor](#servo-monitor)<br/>
[Schalter Anzeige](#schalter-anzeige)<br/>
[Empfänger Binden](#empfänger-binden)<br/>
[Reichweiten Test](#reichweiten-test)<br/>
[Mode Zuordnung](#mode-zuordnung)<br/>
[Kalibrieren](#kalibrieren)<br/>
[Trimmung Kalibrieren](#trimmung-kalibrieren)<br/>
[Batterie Warnung](#batterie-warnung)<br/>
[Statistiken](#statistiken)<br/>
[Servo Test](#servo-test)<br/>

[Modelleinstellungen](#modelleinstellungen)<br/>

[Modell](#modell)<br/>
[Trimspeicher](#trimmspeicher)<br/>
[Geber Limit](#geber-limit)<br/>
[Geber Umkehr](#geber-umkehr)<br/>
[Geberschalter](#geberschalter)<br/>
[Schaltkanal](#schaltkanal)<br/>
[Kanal Zuordnung](#kanal-zuordnung)<br/>
[Kanal Verzögerung](#kanal-verzögerung)<br/>
[Landeklappen](#landeklappen)<br/>
[Flugphasen](#flugphasen)<br/>
[Phasentrimm](#phasentrimm)<br/>
[Logikschalter](#logikschalter)<br/>
[Dual Rate & Expo](#dual-rate--expo)<br/>
[Mischer](#mischer)<br/>
[Motor Aus](#motor-aus)<br/>
[Timer](#timer)<br/>
[Servo Zuordnung](#servo-zuordnung)<br/>
[Servo Weg](#servo-weg)<br/>
[Servo Umkehr](#servo-umkehr)<br/>
[Servo Mitte](#servo-mitte)<br/>
[Servo Verzögerung](#servo-verzögerung)<br/>
[Servo Limit](#servo-limit)<br/>

---
## Grundsätzliche Bedienung
[Inhalt](#inhalt)

Die Bedienung erfolgt über einen Drück-Dreh-Steller. Beim Drücken wird zwischen einem kurzen und einem langen Druck (> 300 Millisekunden) unterschieden.

* Drehen - Auswahl der Elemente oder Einstellen eines Wertes
* Kurzer Druck - ENTER
* Langer Druck - CLEAR

Bevor ein Modell programmiert und geflogen wird müssen die Analogeingänge (Kreuzknüppel, Potentiometer und analoge Trimmhebel) kalibriert werden.

---
### Erstmalige Knüppel kalibrierung

Die Funktion zum Kalibrieren der Kreuzknüppel und Potentiometer finden Sie in den Systemeinstellungen.

siehe [Knüppel kalibrierung](#kalibrieren)

---
### Erstmalige Trimm kalibrierung

Die Funktion dazu finden Sie wieder in den Systemeinstellungen.

siehe [Trimm kalibrierung](#trimmung-kalibrieren)

---

## Konzept
[Inhalt](#inhalt)

TXos unterstützt bis zu 9 Eingabekanäle (In1-In9) und 9 Ausgabekanäle (Servoausgänge S1-S9). Intern werden 12 logische Kanäle verwendet.

Die Eingabekanäle (IN1-IN9) werden über die Hauptsteuerfunktionen (Kreuzknüppel) sowie optionalen Linearschiebern, Potentiometern und Schaltern angesprochen.

Dabei sind die Eingabekanäle wie folgt mit den Bedienelementen verbunden:

* In1-In4 sind verbunden mit den beiden Kreuzknüppeln
* In5,In6 sind verbunden mit analogen Linearschiebern oder Potentiometer
* In7-In9 sind verbunden mit Schalten.

Achtung: Werden weniger als 2 Linearschieber/Potentiometer konfiguriert, so verschiebt sich die Nummerierung.


Die 4 Haupt Steuerfunktionen können mit den Eingabekanälen In1-In4 frei belegt werden.
In den Systemeinstellungen kann unter [Mode Zuordnung](#mode-zuordnung) die Voreinstellung festgelegt werden die für jedes neu angelegte Model verwerndet wird.
Die Einstellung kann aber für jedes Model unter Modeleinstellungen [Kanal Zuordnung](#kanal-zuordnung) geändert werden.

Intern werden 12 logische Kanäle verwendet.<br/>
* Mot - Motor
* QR - Querruder
* QR2 - Querruder zweiter Kanal
* HR - Höhenruder
* SR - Seitenruder
* WK - Wölbklappen
* WK2 - Wölbklappen zweiter Kanal
* LK - Landeklappen
* LK2 - Landeklappen zweiter Kanal
* FWk - Fahrwerk
* K8 - Freier Kanal 8
* K9 - Freier Kanal 9

Mischer und andere Funktionen wie "Motor Aus" wirken auf die logischen Kanäle.

Nachdem alle Funktionen berechnet sind werden die logischen Kanäle 9 Servo Kanälen zugeordnet.

---
## Blockschaltbild
[Inhalt](#inhalt)

Die interne Verschaltung der einzelnen Module und deren Verarbeitungsreihenfolge:

![Blockschaltbild](doc/TXos-Blockschaltbild.png "Blockschaltbild")

---
## Schalter
[Inhalt](#inhalt)

TXos unterstützt folgende Schalter:

* 2-Stufen Schalter  BIn (BI1,BI2...)
* 3-Stufen Schalter  TRn (TR3,TR4...)
* Geberschalter oder Kanalschalter CSn (CS1,CS2...)
* Logische Schalter: LSn (LS1,LS2...)
* “Immer Ein” Schalter: ON1
* Phasen Schalter (3-Stufig): PHA

Eine bestimmte Schalterstellung wird mit angehängtem Doppelpunkt und der Schalterposition angegeben.

Beispiel: 2-stufen Schalter 1 in Schalterstellung 0<br/>
**BI1:0**

---
## Hauptbildschirm
[Inhalt](#inhalt)

![TXos Display](img/TXos_main_doc.png "TXos Display")

* Warnmeldungen werden durch CLEAR (Langer Druck) gelöscht.
* Wird keine Meldung angezeigt wird durch einen langen Druck der Timer zurückgesetzt.
* Ein kurzer Druck ruft die Modelleinstellungen aus.

---
---
## Systemeinstellungen
[Inhalt](#inhalt)

Die Systemeinstellungen sind über den Menüpunkt "Systemeinstellungen" im Modell Menü erreichbar.

![Systemneinstellungen](img/TXos_system_menu.png "Systemeinstellungen")

---
### Modellauswahl
[Inhalt](#inhalt)

Ein Arduino mit Mega 2560 Prozessor hat genügend Speicherkapazität für 14 Modelle.

Wählen Sie unter "Modellauswahl" das entsprechende Modell aus und drücken Sie ENTER.

![Modellauswahl](img/TXos_model_select.png "Modellauswahl")

In der folgenden Auswahl können die das Model laden, löschen oder für das kopieren vormerken.

![Modellauswahl1](img/TXos_model_select1.png "Modellauswahl1")

Achtung: Wenn sie "Entfernen" wählen wird das Model ohne weitere Abfrage gelöscht.

Wenn Sie "Kopieren" wählen wird das Modell als Quelle für die nächste "Einfügen" Operation ausgewählt.

Zum Kopieren eines Modells müssen sie:
1) das zu kopierende Modell auswählen und mit "Kopieren" als Quelle markieren.
2) den Ziel Speicherplatz auswählen und mit "Einfügen" das in (1) ausgewählte Modell in den ausgewählten Speicher kopieren.

![Modellauswahl2](img/TXos_model_select2.png "Modellauswahl2")

---
### Import/Export
[Inhalt](#inhalt)

Modelle können über die USB Schnittstelle auf einen PC gesichert und von dort wieder in den Speicher geladen werden.
Die Bedienung erfolgt über das PC Programm.

**Achtung: Die Funktion ist in TXos implementiert, das PC Programm jedoch noch nicht fertig.**

![Import/Export](img/TXos_import_export.png "Import/Export")

---
### Servo Monitor
[Inhalt](#inhalt)

Der Servo Monitor zeigt die aktuelle Position aller 9 Servos in Prozent an.

Mit dem Drück-Dreh Steller können Sie nach unten Scrollen um weitere Servos anzuzeigen.

![Servo Monitor](img/TXos_servo_monitor.png "Servo Monitor")

---
### Schalter Anzeige
[Inhalt](#inhalt)

Der Schalter Monitor zeigt den aktuellen Zustand aller Schalter an.

Dazu zählen die Hardware-Schalter, sowie logische Schalter und Phasenschalter.

#### Benennung der Schalter

* Hardware Schalter. Die Schalternamen bestehen aus einer Typbezeichnung (BI für 2-Stufen Schalter, TR für 3-Stufen Schalter) und einer fortlaufenden Nummer.
* Geberschalter. Die Schalternamen sind "CS" und eine fortlaufende Nummer.
* Logische Schalter. Logische Schalter sind Verknüpfungen von bis zu 3 anderen Schaltern. Die Schalternamen sind "LS" und eine fortlaufende Nummer.
* Immer EIN Schalter. Dieser Schalter ist fix und kann dazu verwendet werden eine Funktion fest ein zu schalten. Der Name ist "ON". Der Zustand ist immer "1".
* Phasen schalter. "PHA" ist ein 3-Stufen Schalter und wird automatisch entsprechend der aktuellen Flugphase geschalten.

Mit dem Drück-Dreh Steller können Sie nach unten Scrollen um weitere Schalter anzuzeigen.

![Schalter](img/TXos_switches.png "Schalter")

---
### Empfänger binden
[Inhalt](#inhalt)

TXos ünterstützt 2 Relais. Ein Relais dient dazu das HF Modul auszuschalten. Ein weiteres kann einen "Bind" Taster betätigen.

Je nach Sendemodul kann damit von TXos aus der Bindemodus ein- und ausgeschalten werden.

![Empf. binden](img/TXos_bind.png "Empf. binden")

Wählen Sie dazu "Bind" aus und drücken Sie ENTER.

Beim Binden wird:
* Das HF Modul ausgeschaltet
* Das "Bind" Relais eingeschaltet
* Das HF Module wieder eingeschaltet<br/>

![Empf. binden](img/TXos_bind_active.png "Empf. binden")

Beim Verlassen dieser Funktion wird aus Sicherheitsgründen ein aktivierter Bind Vorgang abgebrochen.

---
### Reichweiten Test
[Inhalt](#inhalt)

Wenn der "Bind" Taster am Sendemodul einen Reichweiten Test einschaltet kann dies von TXos aus geschehen.

![Reichw. Test](img/TXos_range_test.png "Reichw. Test")

Wählen Sie dazu "Test" aus und drücken Sie ENTER.

Für den Reichweiten Test wird:
* Das "Bind" Relais eingeschaltet

![Reichw. Test](img/TXos_range_test_active.png "Reichw. Test")

Beim Verlassen dieser Funktion wird aus Sicherheitsgründen ein aktivierter Reichweiten Test automatisch beendet.

---
### Mode Zuordnung
[Inhalt](#inhalt)

Diese Funktion erlaubt die Zuweisung eines Eingangskanals "In1" - "In4" zu die 4 Hauptsteuerfunktionen "Mot", "QR", "HR" und "SR".

![Mode Zuord.](img/TXos_mode.png "Mode Zuord.")

Zum Zuweisen eines Einganges zu einer Steuerfunktion wählen Sie die Steuerfunktion an und drücken ENTER.

Durch Drehen wird ein Eingang ausgewählt und duch ENTER bestätigt.

![Mode Zuord.](img/TXos_mode_assign.png "Mode Zuord.")

Beim Erstellen einen neuen Modells wird diese Zuordnung als Vorgabe verwendet.

---
### Kalibrieren
[Inhalt](#inhalt)

Die Kalibrierung legt die Mittelstellung sowie die Endpositionen der Kreuzknüppel und Potertiomenter fest.<br/>
Die Kalibrierung erfolgt für die Analogeingänge 1 bis 6, also die Kreuzknüppel und optionale Linearschieber oder Potentiometer.

Die Trimmung wird getrennt kalibriert.

![TXos Kalibrieren](img/TXos_callib.png "TXos Kalibrieren")

Unkalibrierte Eingänge zeigen einem Minimalwert von 0 und einen Maximalwert von 1023.<br/>
Starten sie die Kalibrierung mit ENTER nachdem Sie "Start" ausgewählt haben.

![TXos Kalibrieren 1](img/TXos_callib_1.png "TXos Kalibrieren 1")

Das Display zeigt jetzt "CENTER" an. Bewegen Sie alle Kreuzknüppel und Potertiometer in die Mittelstellung.
Danch drücken Sie nochmals ENTER.

![TXos Kalibrieren 2](img/TXos_callib_2.png "TXos Kalibrieren 2")

Das Display zeigt jetzt "MIN/MAX" an. Bewegen Sie alle Kreuzknüppel und Potertiometer in beide Endstellungen.
Danach drücken Sie ENTER.

Vergessen Sie dabei nicht die Linearschieber und Potentiometer. Durch drehen des Drück-Dreh Stellers können Sie weitere Eingabekanäle anzeigen.

![TXos Kalibrieren 3](img/TXos_callib_3.png "TXos Kalibrieren 3")

Das Display zeigt wieder "CALIB." an. Die Mittelstellung sowie die Endstellungen sind jetzt gespeichert.

![TXos Kalibrieren 4](img/TXos_callib_4.png "TXos Kalibrieren 4")

---
### Trimmung Kalibrieren
[Inhalt](#inhalt)

Zur Kalibrierung der Trimmhebel verfahren Sie wie bei der Kalibrierung der Knüppel.

![TXos Kalibrieren 5](img/TXos_callib_trim.png "TXos Kalibrieren 5")

---
### Batterie Warnung
[Inhalt](#inhalt)

Einstellen der Warn- und Alarmschwellwerte und kalibrierung der gemessenen Batteriespannung.

Wird der Warnschwellwert unterschritten wechselt die Farbe der Batteriespannungs Anzeige von Grün auf Orange.

Wird der Alarmschwellwert unterschritten wechselt die Farbe der Batteriespannungs Anzeige von Orange auf Rot und es ertönt ein Alarmsignal von 5 kurzen Tönen alle 5 Sekunden.

![Bat. Warnung](img/TXos_battery.png "Bat. Warnung")

"+/-" ist ein Korrekturwert mit dem die angezeigte Spannung der wahren Batteriespannung angeglichen wird. Dazu sollte die Batteriespannung im Betrieb gemessen und der Korrekturwert entsprechend angepasst werden.

Achtung: V0.5.4 enthält einen Fehler. Die aktuelle Spannung wird beim ändern des Korrekturwertes nicht automatisch aktualisiert.

![Bat. Warnung](img/TXos_battery_adjust.png "Bat. Warnung")

---
### Statistiken
[Inhalt](#inhalt)

Dieses Modul zeigt interne statistiken an und ist normalerweise deaktiviert.

![Statistiken](img/TXos_statistics.png "Statistiken")

---
### Servo Test
[Inhalt](#inhalt)

Dieses Modul ist normalerweise deaktiviert.

![Servo test](img/TXos_servo_test.png "Servo test")

---
---
## Modelleinstellungen
[Inhalt](#inhalt)

Modelleinstellungen werden pro Modell gespeichert und geladen.

![Modelleinstellungen](img/TXos_model_menu.png "Modelleinstellungen")

---
### Modell
[Inhalt](#inhalt)

Im "Modell" Modul werden der Modellname, der Leitwerksmischer, die Querruderdifferenzierung sowie alle vorgefertigten Flächenmischer eingestellt.

![Modell](img/TXos_model.png "Modell")

Für Funktionen die 2 getrennte Servos bedienen können, wie Querruder (QR), Wölbklappen (WK) und Landeklappen (LK) werden immer beide logische Kanäle gemischt. Also QR und QR2, WK und WK2 sowie LK und LK2. Ob der zweite Kanal dann auch wirklich verwendet wird, wird später in der Kanal - Servo zuordnung festgelegt.

---
### Trimmspeicher
[Inhalt](#inhalt)

Die Verwendung des Trimspeichers ist nicht so einfach auf den ersten Blick ersichtlich.

Das Problem beim analogen Trim ist, das für jedes Modell andere Werte gelten und diese beim Modelwechsel geändert werden müssen. Also Wert merken, Modell wechseln, neuen Wert für dieses Modell einstellen. Dies ist nicht praktikabel.

Deshalb hat jedes Modell seinen eingenen separaten Trimspeicher. Der aktuelle Wert der Trimmung besteht aus dem Wert der analogen Trimschieber plus dem Wert aus dem Trimspeicher.

Die Anzeige des Trimspeichers sieht wie folgt aus:

![Trimmspeicher](img/TXos_trim_memory.png "Trimmspeicher")

Wird während eines Fluges die Trimmung verstellt und der Wert soll in den Trimspeicher übernommen werden, passiert das einfach durch zurückstellen des Trimschiebers auf neutral, während man sich im Trimspeicher befindet.

Die linke Spalte zeigt den aktuellen Wert der Trimschieber. Die rechte Spalte den Wert des Trimspeichers für das aktuelle Modell. Im obigen Bild steht also der HR Trim auf -4 und es ist kein Wert für HR im Trimspeicher.<br/>
Um den aktuellen Wert der Trimmung in den Trimspeicher zu übernehmen braucht nur der Trimschieber auf Mittelstellung geschoben werden.

![Trimmspeicher1](img/TXos_trim_adjust.png "Trimmspeicher1")

Der Wert der Trimmung wird auf den Trimspeicher aufaddiert, bzw davon abgezogen. Man kann also nach jedem Flug, wenn die Trimmung verändert wurde, einfach den Trimspeicher aktualisieren und den Trim für den nächsten Flug oder das nächste Modell auf Mittelstellung setzen.

Der Trimspeicher kann auch manuell geändert und zurückgesetzt werden.

Dazu den Kanal auswählen und ENTER drücken. Ein langer druck auf den Drück-Dreh Steller setzt den Wert auf 0 zurück.

![Trimmspeicher2](img/TXos_trim_clear.png "Trimmspeicher2")

---
### Geber Limit
[Inhalt](#inhalt)

Analoggeber (In1-In6) Wegbegrenzung in Prozent.

Bereich: -125 % bis +125 %<br/>
Einstellung bei einem neuen Modell: +/- 100 %

Diese Funktion wird im Normalfall nicht gebraucht. Ein Limit sollte über Servolimit eingestellt werden.

![Geber Limit](img/TXos_input_limits.png "Geber Limit")

---
### Geber Umkehr
[Inhalt](#inhalt)

Analoggeber (In1-In6) Wirkrichtungsumkehr.

Einstellung bei einem neuen Modell: Aus

Diese Funktion wird im Normalfall nicht gebraucht. Eine Richtungsumkehr sollte über Servoreverse eingestellt werden.

![Geber Umkehr](img/TXos_input_reverse.png "Geber Umkehr")

---
### Geberschalter
[Inhalt](#inhalt)

Ansteuerung eines Schalters über einen analogen Eingang.

Geberschalter sind 2-Stufen Schalter deren Schaltstellung vom Wert eines analogen Eingangs abhängig ist.<br/>
Im Beispiel wird der Schalter CS1 von 0 auf 1 geschalten wenn der Wert des Eingangs In1 größer -90 % ist.

![Geberschalter](img/TXos_input_switches.png "Geberschalter")

---
### Schaltkanal
[Inhalt](#inhalt)

Ansteuerung eines analogen Kanals (In7-In9) über einen mechanischen oder logischen Schalter.

![Schaltkanal](img/TXos_switched_input.png "Schaltkanal")

---
### Kanal Zuordnung
[Inhalt](#inhalt)

Zuordnung der Eingabekanäle zu den internen logischen Kanälen.

Die Standard Zuordnung für ein neues Modell wird in den Systemeinstellungen unter [Mode Zuordnung](#mode-zuordnung) vorgegeben.

![Kanal Zuord.](img/TXos_channel_mapping.png "Kanal Zuord.")

---
### Kanal Verzögerung
[Inhalt](#inhalt)

Die Geschwindigkeit der Bewegung jedes Kanals kann auf bis zu 10 Sekunden (von 0 bis 100%) veringert werden.<br/>
Eingestellt wird die Zeit der Bewegung von 0 bis 100% in Sekunden, jeweils getrennt für beide Richtungen.

![Kanal Verz](img/TXos_channel_delay.png "Kanal Verz")

---
### Landeklappen
[Inhalt](#inhalt)

Landeklappen (Airbrakes).

Diese Einstellungen wirken auf die logischen Kanäle LK und LK2.<br/>
Der Geber für die Landeklappen wird im Menü [Kanal Zuordnung](#kanal-zuordnung) zugeordnet.<br/>
Eine Verzögerung der Bewegung wird im Menü [Kanal Verzögerung](#kanal-verzögerung) eingestellt<br/>

Landeklappen Einstellungen:

* Aktiv: Schalter zur aktivierung der Landeklappen.
* Aus Pos: Diese Position wird angefahren wenn die Landeklappen nicht aktiviert sind.
* Umkehr: Umkehrung der Wirkrichtung.

Einstellungen bei einem neuen Modell:

* Aktiv: Kein Schalter (nicht aktiv)
* Aus Pos: 0%
* Umkehr: Aus

![Kanal Verz](img/TXos_airbrakes.png "Landeklappen Verz")

---
### Flugphasen
[Inhalt](#inhalt)

TXos unterstützt bis zu drei Flugphasen.

In diesem Menüpunkt wird der Schalter sowie die Namen der Flugphasen eingestellt.

Verfügbare Flugphasen sind:
* Normal
* Thermik
* Speed
* Start
* Landung
* Akro
* Akro 2
* Strecke
* Schlepp

![Phasen](img/TXos_phases.png "Phasen")

---
### Phasentrimm
[Inhalt](#inhalt)

Die Steuerfunktionen QR, HR, WK und LK können für jede Flugphase einzeln getrimmt werden.

Die beiden QR Flächen werden dabei gleichsinning und nicht gegensinning wie bei der normalen Trimmung verstellt.

![Phasentrimm](img/TXos_phases_trim.png "Phasentrimm")

---
### Logikschalter
[Inhalt](#inhalt)

TXos unterstützt bis zu 3 Logikschalter.<br/>
Logikschalter sind boolsche Verknüpfungen von bis zu 3 anderen Schaltern.

Folgende Verknüpfungen sind möglich:<br/>
( "&" steht für "und", "|" steht für "oder" )
* A & B
* A | B
* A & B & C
* A | B | C
* (A & B) | C
* (A | B) & C

![Logikschalter](img/TXos_logic_switches.png "Logikschalter")

---
### Dual rate & Expo
[Inhalt](#inhalt)

Für die Hauptfunktionen QR, HR und SR stehen phasenabhängige Wegverkürzungen und Exponentialfunktionen zur verfügung.

**Dual Rate:**

Bereich: 0 % bis 100 %<br/>
Einstellung bei einem neuen Modell: 100 %

**Exponentialfunktion:**

Bereich: 0 % bis 100 %<br/>
Einstellung bei einem neuen Modell: 0 %

![Dual & Expo](img/TXos_dual_expo.png "Dual & Expo")

---
### Mischer
[Inhalt](#inhalt)

TXos unterstützt 3 freie Mischer.

Der Mischer wird aktiviert wenn der Mischer Schalter in der vorgegebenen Position steht.<br/>
Quell und Zielkanäle können alle einfachen logischen Kanäle sein (Also nicht QR2,WK2 und LK2).<br/>
Optional kann ein Offset auf den Mischerwert addiert werden. Der Offset kann auch negativ sein.

![Mischer](img/TXos_mixer.png "Mischer")

---
### Motor Aus
[Inhalt](#inhalt)

Beim aktivieren der "Motor Aus" Funktion wird der Motorkanal "Mot" auf einen vorher festgelegten Wert gesetzt.<br/>
Der Wert lässt sich auch durch Mischer nicht ändern und verhindert das der Motor anläuft.

![Motor Aus](img/TXos_engine_cut.png "Motor Aus")

Bei aktivierter "Motor Aus" Funktion wird auf dem Hauptbildschirm ein grünes "Mot" angezeigt.<br/>

![Motor Aus2](img/TXos_main.png "Motor Aus2")

---
### Timer
[Inhalt](#inhalt)

Auf dem Hauptbildschirm wird eine abwärts zählende Uhr angezeigt. Der Startwert und der Aktivierungsschalter kann in diesem Menu eingestellt werden.

![Timer](img/TXos_flight_timer.png "Timer")

Tipp: Programmierung eines Timers mit Motor Stopp Funktion

Schalter BI1 soll in Stellung 1 die “Motor Aus” Funktion aktivieren und der Timer soll nur laufen wenn BI1 auf 0 steht und der Gas Knüppel über -90% steht.

1) Menü: Modelleinstell. => Motor aus<br/>
Schalt auswählen, ENTER drücken und Schalter BI1 auswählen<br/>
Den Schalter BI1 auf Stellung 1 schalten, dann nochmal ENTER drücken.<br/>
Kanal Mot auf -100% stellen

![Timer1](img/TXos_timer_prog1.png "Timer1")

2) Menü: Modelleinstell. => Geberschalter<br/>
CS1 auf den Eingabekanal stellen der den Motor steuert. In diesem Fall IN1.<br/>
Schaltschwelle auf -90% stellen.<br/>
Der Schalter CS1 schaltet jetzt auf 1 wenn der Gasknüppel über -90% bewegt wird.<br/>

![Timer2](img/TXos_timer_prog2.png "Timer2")

3) Menü: Modelleinstell. => Logikschalter<br/>
Der Logikschalter LS1 soll auf 1 schalten wenn BI1 auf 0 steht und CS1 auf 1.<br/>
LS1 auf A&B stellen<br/>
A auf BI1:0 stellen   (Dazu muss der Schalter BI1 auf Stellung 0 bewegt werden)<br/>
B auf CS1:1 stellen   (Dazu muss der Gasknüppel auf > -90% bewegt werden)<br/>

![Timer3](img/TXos_timer_prog3.png "Timer3")

4) Menü: Modelleinstell. => Timer<br/>
Schalt auf LS1:1 stellen   (Dazu muss Schalter BI1 auf 0 stehen und Gasknüppel > -90%)<br/>
Zeit auf die gewünschte Zeit einstellen.

![Timer4](img/TXos_timer_prog4.png "Timer4")

---
### Servo Zuordnung
[Inhalt](#inhalt)

Zuordnung der logischen Kanäle zu Servo Kanälen S1-S9.

Verfügbare logische Kanäle:<br/>
Mot, QR, HR, SR, WK, LK, FWk, K8, K9, QR2, WK2, LK2

![Servo Zuord.](img/TXos_servo_mapping.png "Servo Zuord.")

---
### Servo Weg
[Inhalt](#inhalt)

Servo S1-S9 Weg.

Verkürzung oder Erweiterung des Servowegs.<br/>
Im Gegensatz zur Servo Wegbegrenzung wird hier der Weg eingestellt, den das Servo für die Vorgabe über den vollen Bereich (-100%,+100%) laufen soll.

Einstellung bei einem neuen Modell: -100% bis +100%

![Servo Umkehr](img/TXos_servo_range.png "Servo Weg")

---
### Servo Umkehr
[Inhalt](#inhalt)

Servo S1-S9 Laufrichtung umkehren.

Einstellung bei einem neuen Modell: Aus

![Servo Umkehr](img/TXos_servo_reverse.png "Servo Umkehr")

---
### Servo Mitte
[Inhalt](#inhalt)

Einstellung der Servo S1-S9 Mittelstellung in Prozent.

Bereich: -125 % bis +125 %<br/>
Einstellung bei einem neuen Modell: 0 %

![Servo Mitte](img/TXos_servo_mid.png "Servo Mitte")

---
### Servo Verzögerung
[Inhalt](#inhalt)

Servo S1-S9 Verzögerung.

Die Geschwindigkeit der Bewegung jedes Servos kann auf bis zu 10 Sekunden (von 0 bis 100%) veringert werden.<br/>
Eingestellt wird die Zeit der Bewegung von 0 bis 100% in Sekunden, jeweils getrennt für beide Richtungen.

Einstellung bei einem neuen Modell: 0 Sek.

![Servo Umkehr](img/TXos_servo_verz.png "Servo Verzögerung")

---
### Servo Limit
[Inhalt](#inhalt)

Servo S1-S9 Wegbegrenzung in Prozent.

Bereich: -125 % bis +125 %<br/>
Einstellung bei einem neuen Modell: +/- 125 %

![Servo Limit](img/TXos_servo_limit.png "Servo Limit")

