/********************************************************************************************/
//Aplicatie de comunicatie Seriala																	*/
/********************************************************************************************/
 
/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include <utility.h>
#include <rs232.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "commcallback.h"
#include <formatio.h> 
/********************************************************************************************/
/* Variabile Globale																					*/
/********************************************************************************************/
int stare_led;
int stareled=0;
int ledul=0;
static int 	gHelpPanelHandle;
static int 	gPanelHandle;
static char gEventChar4[2]; 
//Nume Port Serial
	int COM_PORT;
//Datele vehiculate
float dataRx_1, dataRx_2, dataRx_3, dataRx_4,dataRx_5;
float dataTx_1, dataTx_2, dataTx_3, dataTx_4,dataTx_5;
int lungime_pachet;
int led_Rx=0, led_Tx=0;
//Variabile utilizate in prelucrare
double Refk_1,Refk;
double tk_1,tk;
double yk, yk_1;
double Te;
//Variabile utilizate doar la Transmisie catre Arduino
double Referinta, Perioada_Esantionare, val_led,led;

/********************************************************************************************/
/* Functiile de comunicatie pe Porturi Seriale 																				*/
/********************************************************************************************/
void CVICALLBACK Functie_Receptie_Pe_Intrerupere (int portNo,int eventMask,void *callbackData);
	
/********************************************************************************************/
/* Configurarea aplicatiei bazata pe intreruperi																*/
/********************************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)	/* Initialize CVI libraries */
		return -1;	/* out of memory */
	if ((gPanelHandle = LoadPanel (0, "commcallback.uir", PANEL)) < 0)
		return -1;
	if ((gHelpPanelHandle = LoadPanel (0, "commcallback.uir", HELP_PANEL)) < 0)
		return -1;
	//Initializare variabile OBS vor fi duse in Start Aplicatie mai tarziu
	Refk_1=0;Refk=0;
		tk_1=0; tk=0;
		
			Te=1.000;
	yk_1=0;yk=0;
	stareled=!stareled;
	DisplayPanel (gPanelHandle);
				/* 	Configurare Port Serial */
				COM_PORT=5;
				OpenComConfig (COM_PORT, "COM5", 9600, 0, 8, 1, 512, 512);
				SetCTSMode (COM_PORT,LWRS_HWHANDSHAKE_OFF);
				//Instalare comunicatie seriala pe intreruperi
				gEventChar4[0]='\n';
						FlushInQ (COM_PORT);
							FlushOutQ (COM_PORT);
				InstallComCallback (COM_PORT, LWRS_RECEIVE, 23, 0 , Functie_Receptie_Pe_Intrerupere, 0);  
				/* 	Stergere Buffere de I/O pe porturile seriale */
						FlushInQ (COM_PORT);
							FlushOutQ (COM_PORT);
RunUserInterface ();//Pornire Runtime-Engine
	CloseCom (COM_PORT); /*	Close the open COM  */ 
	DiscardPanel (gPanelHandle);	/*	Discard the loaded panels from memory */  
		DiscardPanel (gHelpPanelHandle);
	return 0;
}
/********************************************************************************************/
/* Functia atasata butonului START APLICATIE pt. initiere Comunicatie pt COM */
/********************************************************************************************/
int CVICALLBACK Start_Aplicatie (int panel, int control, int event, void *callbackData, 
							int eventData1, int eventData2)
{	switch (event)
		{
		case EVENT_COMMIT:
			//Initiere comunicatie cu ARDUINO	
			//ComWrt (COM_PORT, "123.123 456.456 789.789 222.222", StringLength ("123.123 456.456 789.789 222.222"));
			ComWrt (COM_PORT, "Comunicatie_pornita_SUCCES", StringLength ("Comunicatie_pornita_SUCCES"));
			SetCtrlVal(gPanelHandle, PANEL_TEXTBOX_2, "Comunicatie_pornita_SUCCES");SetCtrlVal (gPanelHandle, PANEL_TEXTBOX_2, "\n");
				SetCtrlVal(gPanelHandle, PANEL_LEDTX, 1);
				
		break;
		}
	return 0;
}
/********************************************************************************************/
/* Functie_Receptie_Pe_Intrerupere (): Functie apelata pe intrerupere cand se primeste un nr. de minim de 12 octeti
		....dar se asteapta si restul pachetului prin Delay(0.020)*/
/********************************************************************************************/
void CVICALLBACK Functie_Receptie_Pe_Intrerupere (int portNo,int eventMask,void *callbackData)
{
	char Buf_Receptie[1000], transmitBuf[1000], buffsend1[1000];
	double Val[1000];
	int lungime_octeti_Rx;
		led_Rx=!led_Rx;
		SetCtrlVal(gPanelHandle, PANEL_LEDRX, led_Rx);//Pornire Led pe Rx
		
	/*GetCtrlVal(gPanelHandle, PANEL_VAL_LED ,&led);
	/*if (led>0){
		SetCtrlVal(gPanelHandle, PANEL_LED_VIRTUAL,1);
	}else {
	SetCtrlVal(gPanelHandle, PANEL_LED_VIRTUAL,0);
	}
	*/
	
		SetCtrlVal(gPanelHandle, PANEL_LED_VIRTUAL, ledul);
	ledul=!ledul;
	
//Receptie de la Arduino ##################
	Delay(0.020);//asteapta din momentul intreruperii pe comunicatie sa vina in Buffer toti octetii/caracterele
	lungime_octeti_Rx=GetInQLen (COM_PORT);//citire lungime pachet de date
	//ComRdTerm (COM_PORT4, Buf_Receptie, lungime_octeti_Rx, '\0');//citire pachet de date
	ComRd (COM_PORT, Buf_Receptie, lungime_octeti_Rx);
	//Afisare pachet de date Receptionat ca string
		SetCtrlVal(gPanelHandle, PANEL_TEXTBOX_1, Buf_Receptie);		SetCtrlVal (gPanelHandle, PANEL_TEXTBOX_1, "\n");
	Scan (Buf_Receptie, "%s>%4f[x]", Val);//Extragere Valori numerice din string >>> Datele Receptionate
	Refk=Val[0];		SetCtrlVal (gPanelHandle, PANEL_DATARX_1, Refk);
		Te=Val[1];			SetCtrlVal (gPanelHandle, PANEL_DATARX_2, Te);
			yk=Val[2];				SetCtrlVal (gPanelHandle, PANEL_DATARX_3, dataRx_3);
				dataRx_4=Val[3];					SetCtrlVal (gPanelHandle, PANEL_DATARX_4, dataRx_4);
				 stare_led=Val[4];
				 
				 
//Exemplu extragere a unei noi date
//data_5=Val[4];					SetCtrlVal (gPanelHandle, PANEL_DATA_5, data_5);
//OBS: Trebuie creata caseta numerica in UIR cu numele DATA_5
	FlushInQ (COM_PORT);//Se strege Bufferul de intrare pt. o noua comunicatie
	FlushOutQ (COM_PORT);//Se strege Bufferul de iesire pt. o noua comunicatie
//END Receptie de la Arduino ##################
	
	
//Procesare algoritm     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		///aici se va pune Procesarea la nivel de Labwindows/CVI
		PlotLine (gPanelHandle, PANEL_GRAPH, tk_1, Refk_1, tk, Refk, VAL_GREEN);
		PlotLine (gPanelHandle, PANEL_GRAPH, tk_1, yk_1, tk, yk, VAL_RED);
		
		//Actualizare un nou ciclu de intrerupere sau perioada urmatoare de esantionare
		Refk_1=Refk;
		tk_1=tk;
			tk=tk+Te;
		yk_1=yk;
		if(tk>100)//Stergere grafice cand tk creste peste 100 secunde
			{	DeleteGraphPlot (gPanelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW); 
				tk_1=0; tk=Te;
			}

//END Procesare algoritm %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
		
//Transmisie to Arduino##############################################
	GetCtrlVal (gPanelHandle, PANEL_REFERINTA, &Referinta);
		GetCtrlVal (gPanelHandle, PANEL_STE, &Perioada_Esantionare);
			GetCtrlVal (gPanelHandle, PANEL_DATATX_3, &dataTx_3);
				GetCtrlVal (gPanelHandle, PANEL_DATATX_4, &dataTx_4);
				  GetCtrlVal(gPanelHandle, PANEL_VAL_LED ,&led);
				   GetCtrlVal(gPanelHandle, PANEL_LED_VIRTUAL ,&stareled);
		//FormatarePachetul de date catre Client/Arduino
			Fmt (buffsend1, "%s<%f[p3]", Referinta);
				strcat (buffsend1," ");
					strcpy(transmitBuf,buffsend1);//aici se copiaza prima data
			Fmt (buffsend1, "%s<%f[p3]", Perioada_Esantionare);
				strcat (buffsend1," ");
					strcat(transmitBuf,buffsend1);//aici se concateneaza a 2-a data
			Fmt (buffsend1, "%s<%f[p3]", dataTx_3);
				strcat (buffsend1," ");
					strcat(transmitBuf,buffsend1);//aici se concateneaza a 3-a data
			Fmt (buffsend1, "%s<%f[p3]", dataTx_4);
				strcat (buffsend1," ");
					strcat(transmitBuf,buffsend1);//aici se concateneaza a 4-a data
			Fmt (buffsend1, "%s<%f[p3]", led);
				strcat (buffsend1," ");
					strcat(transmitBuf,buffsend1);//aici se concateneaza a 5-a data
			Fmt (buffsend1, "%s<%d[p3]", stareled);
				strcat (buffsend1," ");
					strcat(transmitBuf,buffsend1);//aici se concateneaza a 5-a data		
		
//Exemplu de adaugare in pachetul de date a unei noi date
	//GetCtrlVal (gPanelHandle, PANEL_DATATX_5, &dataTx_5);
			//Fmt (buffsend1, "%s<%f[p3]", dataTx_5);
				//strcat (buffsend1," ");
					//strcat(transmitBuf,buffsend1);//aici se concateneaza a 5-a data
//OBS: Trebuie creata caseta numerica in UIR cu numele DATATX_5
	ComWrt (COM_PORT, transmitBuf, StringLength (transmitBuf));
	//Afisarea Pachetului de date ca String ce va fi transmis
	SetCtrlVal(gPanelHandle, PANEL_TEXTBOX_2, transmitBuf);		SetCtrlVal (gPanelHandle, PANEL_TEXTBOX_2, "\n");
		led_Tx=!led_Tx;
		SetCtrlVal(gPanelHandle, PANEL_LEDTX, led_Tx);
//END Transmisie to Arduino##############################################
	
	return;
} 
//Stergere grafice cand se apasa butonul
int CVICALLBACK Refresh_Graph (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{	switch (event)
	{	case EVENT_COMMIT:
			DeleteGraphPlot (gPanelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW); 
			tk_1=0;		tk=Te;
		break;
	}
	return 0;
}

/********************************************************************************************/
/* PanelCB ():  Quits this application.														*/
/********************************************************************************************/
int CVICALLBACK Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{	case EVENT_COMMIT:
					QuitUserInterface(0);
				break;
		}
	return 0;
}
/********************************************************************************************/
/* Help ():  Display help text.																*/
/********************************************************************************************/
int CVICALLBACK Help (int panel, int control, int event, void *callbackData, int eventData1,
						int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			InstallPopup (gHelpPanelHandle);
			break;
		}
	return 0;
}
/********************************************************************************************/
/* QuitHelpCB ():  Remove help text.														*/
/********************************************************************************************/
int CVICALLBACK QuitHelp (int panel, int control, int event, void *callbackData, 
							int eventData1, int eventData2)
{
	switch (event) 
		{
		case EVENT_COMMIT:
			RemovePopup (0);
			break;
		}
	return 0;
}


