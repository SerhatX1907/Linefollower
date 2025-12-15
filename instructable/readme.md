# Instructable

Een instructable is een stappenplan - zonder verdere uitleg - hoe je vertrekkend van de bill of materials en gebruik makend van de technische tekeningen de robot kan nabouwen. Ook de nodige stappen om de microcontroller te compileren en te uploaden staan beschreven.  

stap 1
Verzamel alle onderdelen (bill of materials).

stap 2
Voorzie het gereedschap (soldeerbout, schroevendraaier, laptop met Arduino IDE).

stap 3
Maak het chassis.

stap 4
Monteer de motoren, sensoren en wielen op het chassis.

stap 5
Plaats de H-brug en de Arduino stevig op het chassis

Sluit alle componenten correct aan volgens het aansluitschema

stap 6
Upload eerst de proof-of-concept codes:

motor-test (vooruit/achteruit/variabele snelheid)

sensor-test (raw waarden uitlezen in Serial Monitor)


stap 7
Upload nu volledige code Kalibreer de QTR-sensoren (kal via de Serial Monitor).

Test de commando’s (go, stop, p=, d=, base=, help?)

stap 8
Zet de wagen op het parcours (zwarte lijn op witte achtergrond).

Laat de robot rijden en observeer hoe hij de lijn volgt.

Pas indien nodig de parameters (kp, kd, Speed) aan voor betere prestaties.

Gebruik de stop/start commando als noodstop.
