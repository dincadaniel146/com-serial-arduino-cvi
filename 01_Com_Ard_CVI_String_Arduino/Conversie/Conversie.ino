#include <TimerOne.h>
double uk;
double yk,yk_1;
double C;
double T_proces;

char Buffer_Receptie[100];
const int ledPin=8;
int stareled;
int Lungime_Pachet_Date=0;
float Val[20];
float data1=0, data2=0, data3=0, data4=0, data5=0,data6=0;
float Refk,Te,led;
long Te_arduino_vechi, Te_arduino_nou;//Te transformata in microsecunde pt Timer Arduino

//Start aplicatie
void setup() {
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
  
      Serial.flush();
    Te_arduino_vechi=1000000;
    Te_arduino_nou=Te_arduino_vechi;
  //Initializare uk si yk si C
    uk=0;
    yk=0;yk_1=0;
 
    T_proces=10; //[seunde]  
    Te=1.0; //[seunde] sau Te=Te_arduino_nou/1000000  
    C=Te/(Te+T_proces);      
  Timer1.initialize(Te_arduino_nou);//Timpul este in microsecunde
  Timer1.attachInterrupt(esantionare);
}

//Bucla infinita specifica Arduino //Echivalenta Run-Time-Engine pentru aplicatia noastra
void loop() {
  // put your main code here, to run repeatedly:
}

//Functia care implementeaza perioada de esantionare(Te) ###Functie atasata intreruperii de timer
void esantionare()
{
//Rx-Receptie de la Labwindows/CVI &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 
 if (Serial.available() > 0) 
    {   delay(20);//asteptam sa vina totii octetii
        Lungime_Pachet_Date=Serial.available();  
        Serial.readBytes(Buffer_Receptie, Lungime_Pachet_Date);//pachetul de date primit ca ARRAY
                //Serial.print(" Stringul RECEPTIE: ");    Serial.print(Buffer_Receptie);   
        Serial.flush();
        int p1=0;   int p2=0;   int j=0;
        for (int i=0; i<(Lungime_Pachet_Date); i++) 
          {
            if((Buffer_Receptie[i]==' ') || (Buffer_Receptie[i]=='\0'))
              {
                p2=i; 
                String Buff_string_Rx = String(Buffer_Receptie);//pachetul de date este transformat ca String 
                String Data_string = Buff_string_Rx.substring(p1,p2);//Extragere substring de date           
                Val[j] = Data_string.toFloat();//Transformare in Valoare numerica
                if (Val[j]>=1000)    {Val[j]=999.999;}//Limitare la 3 caractere/digiti pt. partea intreaga
                    //Serial.print(" Valoare numerica: ");    Serial.print(Val[j],3);//afisare valoare numerica extrasa din pachet cu 3 zecimale dupa virgula
                p1=i+1;//Initializare index initial pentru subsirul unei noi Date
                j=j+1;//Initializare Index pentru valorile numerice extrase                       
              }              
          }
//Atasarea variabilelor de proces a valorilor numerice
     Refk=Val[0];
      Te=Val[1];//se va folosi in realatiile iterative viitoare
      Te_arduino_nou=(long)(1000000*Te);//Treansforma Te primit din Sec in microsecunde pt. resetare Perioada de esantionare
//Te=5000000;
        data3=Val[2];       
          data4=Val[3]; 
           led=Val[4];
           stareled=Val[5];
          //Exemplu: Aici se adauga o noua data in Pachetul de date//De ex. data5=...=Temperatura1=Val[4]     
 //END Rx-Receptie de la Labwindows/CVI &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 
    }
  else
    {
      data1=0;
        data2=0;
          data3=0;
            data4=0;
             data5=0;
              data6=0;
    }
    
 //#####################################################################################################################   
       
        //....aici se va pune prelucrarea din Arduino
 //Setare din CVI a Perioadei de Esantionare Te, doar cand se va modica din CVI   
 if (stareled==1){
  digitalWrite(ledPin,LOW);
} else {
  digitalWrite(ledPin,HIGH);
  }    
if(Te_arduino_vechi != Te_arduino_nou)
  {     Timer1.initialize(Te_arduino_nou);//Timpul este in microsecunde
        Te_arduino_vechi=Te_arduino_nou;
        C=Te/(Te+T_proces); //se recalcueaza C daca se modifica Te 
  }
// Raspundul unei fdt>>>Prelucrarea unei relatii iterative
uk=Refk;
yk=yk_1+ C*(uk-yk_1);
//Actualizare Stare led in functie de Perioada de Esantionare


//Reactualizare a unui nou ciclu de esantionare
yk_1=yk;
//#####################################################################################################################

//Tx-Transmisie >>>>Formatare pachet de date ca string pentru Transmisie catre Labwindows/CVI     &&&&&&&&&&&&&&&&&&&&&&&&&              
    String Buffer_Transmisie = String (Refk, 3) + ' ' + String (Te,3) + ' ' + String (yk,3) + ' ' + String (data4,3) + String (led,3)+' ' +String (stareled,3)+ ' ';
    for (int i=0; i<Buffer_Transmisie.length(); i++) 
      {         
        
            Serial.write(Buffer_Transmisie[i]);
      } 
//END Tx-Transmisie >>>>Formatare pachet de date ca string pentru Transmisie catre Labwindows/CVI  &&&&&&&&&&&&&&&&&&&&&&&&&  
}  

