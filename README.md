# Marlin Deltacomb

**EN** - Marlin Deltacomb is an highly customized firmware derived from Marlin 1.1.9, designed to work at best with the Deltacomb 3D Printers. In addition to the predefined configurations for every model created, I've develop a number of new unique funcionalities not existing in the official Marlin firmware.

Exlusive funcionalities of the firmware are :

- Pause and Save: in any moment is possible to stop the printing and resume in a later time
- Powerloss to EEPROM: In case of a blackout the job is saved to the EEPROM and not on the SD.
- Unified Calibration Routine UCR: An exclusive calibration routine, for delta printers, which do a fully automated printer calibration and the bed leveling with only 7 probe points
- Hotends offset Menù: In the dual hoten versions I've made a nozzle offset adjustment menù.
- GUI Optimization: The user interface is customized and optimized for a better user experience.

This fork does not regularly follow the evolution of the main branch, but 
Questo fork non segue regolarmente l'evoluzione del branch principale, rather will only be aligned the features which allow to obtain a real benefit for the Deltacomb 3D Printers.

Futher informations about Marlin Firmware are available at the main project [GitHub](https://github.com/MarlinFirmware) 

---

**IT** - Marlin Deltacomb è un firmware basato su Marlin 1.1.9, fortemente personalizzato per funzionare al meglio sulle stampanti Deltacom. Oltre ad avere preimpostate le configurazioni per tutti i modelli creati, ho implementato un buon numero funzionalità uniche e non presenti nel Marlin ufficiale.

Le funzionalità esclusive di questo Firmware sono :

- Pausa e Salva : è possibile in qualsiasi momento interrompere la stampa e riprenderla in un secondo momento
- Powerloss to EEPROM : In caso di blackout il lavoro viene salvato in EEPROM e non su SD.
- Unified Calibration Routine UCR : Una routine di calibrazione esclusiva per stampanti delta che con soli 7 punti esegue la calibrazione della stampante e il livellamento piatto
- Menù regolazione hotend : Nelle versioni con doppio ugello ho creato un menù per regolare l'offet tra i due ugelli
- Ottimizzazione GUI : L'interfaccia utente è stata personalizzata ed ottimizzata per un esperienza utente migliorata.

Questo fork non segue regolarmente l'evoluzione del branch principale, ma piuttosto viene allineato quando determinate funzioni consentono di ottenere un reale beneficio per le stampanti Deltacomb.

Per ulteriori informazioni sul firmware Marlin recarsi sul [GitHub](https://github.com/MarlinFirmware) principale del progetto 

---

**LICENSE** - Marlin is published under the [GPL license](https://github.com/COPYING.md) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
