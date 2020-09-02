# OuvertureServo-EnFonc-HumTemp-API
Ces fichiers de code permettent la le pilotage par Arduino d'un servomoteur s'ajustant en fonction de la température et de l'humidité, ainsi que l'affichage de ceux-ci sur un écran LCD. La récupération des information passe par une connexion série RX/TX entre l'arduino et un ESP32, lui même connécté à une Gateway qui lui transmet les information par réseau LoRa, directement depuis l'API OpenWeatherMap.


Remarque : 
-La Gateway dragino_L01_P est configurée en 915Mhz et branchée au réseau par module CPL. 
-Les pins GPIO du ESP32 sont configuré pour un ESP32 LoRa32 V2.1 1.6 mais il faudra peut-être modifier la valeur de chacun suivant le module utilisé.
