/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  HELP_PANEL                       1
#define  HELP_PANEL_TEXTBOX               2       /* control type: textBox, callback function: (none) */
#define  HELP_PANEL_QUIT_WINDOW           3       /* control type: command, callback function: QuitHelp */

#define  PANEL                            2
#define  PANEL_START_APLICATIE            2       /* control type: command, callback function: Start_Aplicatie */
#define  PANEL_QUITBUTTON                 3       /* control type: command, callback function: Quit */
#define  PANEL_HELP                       4       /* control type: command, callback function: Help */
#define  PANEL_TEXTMSG                    5       /* control type: textMsg, callback function: (none) */
#define  PANEL_REFERINTA                  6       /* control type: scale, callback function: (none) */
#define  PANEL_GRAPH                      7       /* control type: graph, callback function: (none) */
#define  PANEL_TEXTBOX_2                  8       /* control type: textBox, callback function: (none) */
#define  PANEL_TEXTBOX_1                  9       /* control type: textBox, callback function: (none) */
#define  PANEL_DATARX_4                   10      /* control type: numeric, callback function: (none) */
#define  PANEL_DATARX_3                   11      /* control type: numeric, callback function: (none) */
#define  PANEL_STE                        12      /* control type: numeric, callback function: (none) */
#define  PANEL_DATARX_2                   13      /* control type: numeric, callback function: (none) */
#define  PANEL_DATATX_4                   14      /* control type: numeric, callback function: (none) */
#define  PANEL_DATATX_3                   15      /* control type: numeric, callback function: (none) */
#define  PANEL_DATATX_2                   16      /* control type: numeric, callback function: (none) */
#define  PANEL_DATARX_1                   17      /* control type: numeric, callback function: (none) */
#define  PANEL_DATATX_1                   18      /* control type: numeric, callback function: (none) */
#define  PANEL_LEDTX                      19      /* control type: LED, callback function: (none) */
#define  PANEL_LEDRX                      20      /* control type: LED, callback function: (none) */
#define  PANEL_REFRESHGRAPH               21      /* control type: command, callback function: Refresh_Graph */
#define  PANEL_VAL_LED                    22      /* control type: numeric, callback function: (none) */
#define  PANEL_LED_VIRTUAL                23      /* control type: LED, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Help(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitHelp(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Refresh_Graph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Start_Aplicatie(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif