ESP32 Reklamtavla
Beskrivning

Detta projekt är en reklamtavla byggd med en ESP32 och en LCD-display. Tavlan visar olika reklammeddelanden från flera kunder. Meddelandena visas en i taget i 20 sekunder innan nästa väljs slumpmässigt.

Kunderna har olika “vikt”, vilket betyder att vissa visas oftare än andra beroende på hur mycket de har betalat.

Funktioner
Slumpmässigt val av kund baserat på vikt
Samma kund visas aldrig två gånger i rad
Varje visning är 20 sekunder
Olika kunder har olika typer av meddelanden:
Slumpmässiga texter
Tidsbaserade texter (Petter kör jämna/ojämna minuter)
LCD-display via I2C
Kunder
Kund	Vikt
Honest Harry Cars	5000
Grandma Duck Pies	3000
Blackbuilder Petter	1500
Goofy Detective Agency	4000
IOT Advertising	1000
Hur det fungerar (kort)

Programmet kör en loop där:

En kund väljs (inte samma som förra)
Ett meddelande tas fram beroende på kund
Meddelandet visas på LCD i 20 sekunder

Petter använder tid sedan start för att välja meddelande:

jämn minut → scroll-text
ojämn minut → vanlig text
Hårdvara
ESP32
LCD 16x2 med I2C (adress 0x27)
SDA = GPIO 21
SCL = GPIO 22
Bygga och köra

Projektet är gjort i PlatformIO.

Bygg:

pio run

Ladda upp:

pio run --target upload

Serial monitor:

pio device monitor
Struktur
src/
- main.c
- ads.c / ads.h
- billboard.c / billboard.h
- customers.c / customers.h
- display.c / display.h
- randomizer.c / randomizer.h
Egna tankar / förbättringar
Systemet är ganska statiskt, man skulle kunna läsa in kunder från fil eller nätverk
Display-läget (scroll/blink) används inte fullt ut i display-koden ännu
Man skulle kunna lägga till riktig klocka via internet istället för uptime
Kommentar

Projektet är gjort i ren C med ESP-IDF och utan Arduino-bibliotek. Fokus har varit att få funktionaliteten att fungera stabilt först, och sedan strukturera upp koden.

Vill du att jag gör en kort version (mer G-nivå) också eller denna räcker?

gör en version där 1. sen 2.  eller punkter inte existerera mer löpande

Här är en mer naturlig och “löpande” version, utan punktlistor och utan att det känns uppställt:

 ESP32 Reklamtavla
Beskrivning

Detta projekt är en reklamtavla som körs på en ESP32 och visar olika reklammeddelanden på en LCD-display. Tanken är att efterlikna en riktig reklamskylt där flera kunder har köpt reklamplats. Varje kund har olika mycket “vikt” beroende på hur mycket de betalat, vilket gör att vissa visas oftare än andra.

Programmet kör kontinuerligt och visar ett meddelande i taget i ungefär 20 sekunder innan nästa väljs slumpmässigt. Det är också säkerställt att samma kund aldrig visas två gånger i rad.

Hur det fungerar

När programmet startar initieras både tidtagning och display. Därefter körs en loop där en kund först väljs baserat på viktning, samtidigt som den senaste kunden undviks. Efter det väljs ett meddelande beroende på vilken kund det är.

Vissa kunder har flera möjliga meddelanden som väljs slumpmässigt, medan en kund (Petter) istället får sitt meddelande baserat på hur många minuter systemet varit igång. Om det är en jämn minut visas ett meddelande, och om det är en ojämn minut visas ett annat.

När meddelandet är valt skrivs kundens namn på första raden av displayen och själva reklamen på andra raden. Efter cirka 20 sekunder upprepas processen.

Hårdvara

Projektet använder en ESP32 tillsammans med en 16x2 LCD-display som är kopplad via I2C. Displayen körs på adress 0x27 och använder GPIO 21 för SDA och GPIO 22 för SCL.

Bygga och köra

Projektet är gjort i PlatformIO med ESP-IDF. För att bygga projektet används kommandot pio run. För att ladda upp till kortet används pio run --target upload och för att se eventuell output i terminalen används pio device monitor.

Struktur

Koden är uppdelad i flera filer där varje del har ett tydligt ansvar. Det finns en del som hanterar reklammeddelanden, en som håller reda på kunder och deras viktning, en som ansvarar för slumpning, en för displayen och en för tidshantering. Huvudprogrammet binder ihop allt och kör loopen.

Egna tankar

Projektet fungerar som tänkt men är ganska statiskt uppbyggt. En möjlig förbättring hade varit att göra systemet mer flexibelt, till exempel genom att läsa in kunder och meddelanden från en extern källa istället för att ha allt hårdkodat. Man hade också kunnat utveckla displayfunktionen så att scroll och blink faktiskt används mer tydligt. En annan förbättring hade varit att använda riktig tid via internet istället för att bara räkna minuter sedan start.

Kommentar

Projektet är skrivet i ren C med ESP-IDF utan Arduino-bibliotek. Fokus har legat på att få funktionaliteten att fungera korrekt och stabilt, och därefter strukturera upp koden så att den är lätt att förstå.
