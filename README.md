# Comunicatie in timp real dintre Arduino Uno si LabWindows/CVI

Acest proiect realizat in timpul stagiului de practica demonstreaza comunicatia in timp real dintre o placa Arduino Uno si aplicatia LabWindows CVI.

Astfel, LabWindows serveste ca interfata grafica si ofera utilizatorului o modalitate de a simula si vizualiza comportamentul unui sistem controlat in timp real.

## Mod de functionare:

- LabWindows CVI trimite comenzi precum semnalul de referinta, perioada de esantionare si starea LED-ului catre Arduino prin portul serial.

- Arduino verifica daca datele sunt disponibile in memoria tampon serial, le citeste si le converteste in valori numerice urmand a fi atribuite variabilelor de control `Refk` , `Te` , `led`. Acesta efectueaza calcule pentru a actualiza iesirea de control `yk` utilizand o ecuatie de sistem in timp discret si porneste/stinge LED-ul in functie de valoarea `stareled` primita de la LabWindows CVI.

- Pentru executia periodica a functiei de esantionare, este initializataa si setata o intrerupere a temporizatorului utilizand `TimerOne`.

- LabWindows/CVI actualizeaza interfata grafica cu datele primite de la Arduino, permitand monitorizarea si controlul in timp real al sistemului.

