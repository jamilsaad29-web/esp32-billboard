ESP32 Reklamtavla

Detta projekt är en reklamtavla som körs på en ESP32 och visar olika reklammeddelanden på en LCD-display. Tanken är att efterlikna en riktig reklamskylt där flera kunder har köpt reklamplats. Varje kund har olika mycket vikt beroende på hur mycket de betalat, vilket gör att vissa visas oftare än andra. Honest Harry Cars har betalat 5000 kr, Goofy Detective Agency 4000 kr, Grandma Duck Pies 3000 kr, Blackbuilder Petter 1500 kr och IOT Advertising 1000 kr.

Programmet kör kontinuerligt och visar ett meddelande i taget i ungefär 20 sekunder innan nästa väljs slumpmässigt. Det är också säkerställt att samma kund aldrig visas två gånger i rad. Det fungerar genom att den föregående kunden exkluderas från viktssumman innan ett nytt slumptal dras.

När programmet startar initieras både tidtagning och display. Därefter körs en loop där en kund först väljs baserat på viktning, samtidigt som den senaste kunden undviks. Efter det väljs ett meddelande beroende på vilken kund det är. Vissa kunder har flera möjliga meddelanden som väljs slumpmässigt, medan Petter använder tidsbaserad logik där jämna minuter sedan start ger ett meddelande och ojämna minuter ger ett annat.

Tre typer av visning används i projektet. Text visar kundnamnet på rad 1 och reklamtexten på rad 2 statiskt i 20 sekunder, men om texten är längre än 16 tecken scrollar den raden automatiskt. Scroll låter både kundnamnet och reklamtexten glida från höger till vänster tills hela texten passerat skärmen. Blink växlar mellan att visa och dölja texten var 500:e millisekund i 20 sekunder.

Projektet använder en ESP32 tillsammans med en 16x2 LCD-display som är kopplad via I2C. Displayen körs på adress 0x27 och använder GPIO 21 för SDA och GPIO 22 för SCL. Slumpen seedas med esp_timer_get_time() vid uppstart så att sekvensen blir olika varje gång enheten startar.

Koden är uppdelad i separata filer där varje fil har ett tydligt ansvar. main.c kör huvudloopen, billboard.c hanterar tidtagning, customers.c innehåller kunddata, ads.c väljer meddelande per kund, randomizer.c sköter viktad slumpning och display.c hanterar all kommunikation med LCD-displayen.

Projektet är gjort i PlatformIO med ESP-IDF. Bygg med pio run, ladda upp med pio run --target upload och öppna serial monitor med pio device monitor.

Projektet är skrivet i ren C med ESP-IDF utan Arduino-bibliotek. Fokus har legat på att få funktionaliteten att fungera korrekt och stabilt, och därefter strukturera upp koden så att den är lätt att förstå. En möjlig förbättring hade varit att göra systemet mer flexibelt genom att läsa in kunder från en extern källa, eller använda NTP för att hämta riktig tid från internet istället för att räkna minuter sedan start.
