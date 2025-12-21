S využitím libovolného vývojového kitu na bázi SoC ESP32 od společnosti Espressif a vhodného senzoru s analogovým výstupem (napěťová amplituda) realizujte systém pro měření srdečního tepu.

Upřesnění požadavků na implementaci:

S využitím mikrokontroleru Espressif ESP32, modulu se snímačem srdečního tepu PulseSensor, grafického OLED displeje a aplikačních rámců ESP-IDF vytvořte systém pro měření srdečního tepu. Použitý typ senzoru srdečního tepu poskytuje analogový výstup, který převedete do číslicové podoby pomocí modulu analogově-číslicového převodníku uvnitř mikrokontroleru ESP32. Měřené údaje budou následně zobrazeny na grafickém OLED displeji, který připojíte k zapůjčené vývojové platformě na bázi SoC ESP32.

https://www.fit.vut.cz/person/simekv/public/IMP_projekt_board_ESP32_Wemos_D1_R32.pdf


1. [Schéma zapojení desky Wemos D1 R32](https://www.fit.vut.cz/person/simekv/public/IMP_projekt_board_ESP32_Wemos_D1_R32.pdf)
2. [Podrobný manuál k desce Wemos D1 R32](https://www.fit.vut.cz/person/simekv/public/IMP_projekt_board_ESP32_Wemos_D1_R32_manual.pdf)
3. [Modul s detektorem srdečního tepu](https://www.hwkitchen.cz/snimac-pro-tep-srdce-opticky-senzor-arduino-microbit/)
4. [Doplňující informace k detektoru srdečního tepu](https://lastminuteengineers.com/pulse-sensor-arduino-tutorial/)
5. [Modul s OLED displejem 0,96", IIC rozhraní](https://www.hadex.cz/m508b-displej-oled-096-128x64-znaku-iici2c-4piny-bily/)
