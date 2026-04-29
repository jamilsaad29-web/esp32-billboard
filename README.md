ESP32 Reklamtavla

Beskrivning

Detta projekt är en reklamtavla som körs på en ESP32 och visar olika reklammeddelanden på en LCD-display. Tanken är att efterlikna en riktig reklamskylt där flera kunder har köpt reklamplats. Varje kund har olika mycket vikt beroende på hur mycket de betalat, vilket gör att vissa visas oftare än andra.

Programmet kör kontinuerligt och visar ett meddelande i taget i ungefär 20 sekunder innan nästa väljs slumpmässigt. Det är också säkerställt att samma kund aldrig visas två gånger i rad.

Hur det fungerar

När programmet startar initieras både tidtagning och display. Därefter körs en loop där en kund först väljs baserat på viktning, samtidigt som den senaste kunden undviks. Efter det väljs ett meddelande beroende på vilken kund det är.

Vissa kunder har flera möjliga meddelanden som väljs slumpmässigt, medan en kund använder tidsbaserad logik. Beroende på hur många minuter systemet varit igång visas olika meddelanden.

När meddelandet är valt skrivs kundens namn på första raden av displayen och själva reklamen på andra raden. Efter cirka 20 sekunder upprepas processen.

Hårdvara

Projektet använder en ESP32 tillsammans med en 16x2 LCD-display som är kopplad via I2C. Displayen körs på adress 0x27 och använder GPIO 21 för SDA och GPIO 22 för SCL.

Bygga och köra

Projektet är gjort i PlatformIO med ESP-IDF.

Bygg:
pio run

Ladda upp:
pio run --target upload

Serial monitor:
pio device monitor

Kommentar

Projektet är skrivet i ren C med ESP-IDF utan Arduino-bibliotek. Fokus har legat på att få funktionaliteten att fungera korrekt och stabilt, och därefter strukturera upp koden så att den är lätt att förstå.
