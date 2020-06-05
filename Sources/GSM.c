#include <hidef.h> /* for EnableInterrupts macro */
#include <stdio.h>
#include <stdlib.h>
#include "derivative.h" /* include peripheral declarations */
#include "GSM.h"
#include "hardware.h"
#include "SCI.h"



unsigned char Hex2Dec (unsigned char hexDigit){
  if(hexDigit>='A' && hexDigit<='F'){
    hexDigit-='A';
    return hexDigit+10;
  }else if(hexDigit>='a' && hexDigit<='f'){
    hexDigit-='a';
    return hexDigit+10;
  }else{
    return hexDigit-'0';
  }
}

t_buf_serial RxBuff;
void ClrRxBuff(void){
  RxBuff.pIn=0;
  do{
    RxBuff.Buffer[RxBuff.pIn]=0;
    RxBuff.pIn++;
  }while (RxBuff.pIn);
  
}

enum{
  GSMOFF,
  GSMON,
  GSMCMDAT,
  GSMCMDATE,
  GSMCMDCPIN,
  GSMCMDCGREG,
  GSMCMDCGDCONT,
  GSMCMDSGACT,
  GSMCMDUSERID,
  GSMCMDPASSW,
  GSMCMDGPRS0,
  GSMCMDGPRS1,
  GSMCMDSD,
  GSMCMDENV
};

unsigned char gsm_state    	=GSMOFF;        //Control de la maquina de estados
unsigned char gsm_substate 	=__FALSE;       //Control subestado
unsigned char wait          =__FALSE;
unsigned char rx_tout       =__FALSE;
unsigned char retrys        =__FALSE;
unsigned char *reply;                       //Puntero a la respuesta buscada
unsigned char reply_wait    =__FALSE;       //TRUE-> Estamos esperando una respuesta
unsigned char reply_error   =__FALSE;       //TRUE-> El modulo devolvio error
unsigned char reply_fail    =__FALSE;       //TRUE-> El modulo devolvio fail
unsigned char reply_conn    =__FALSE;       //TRUE-> Estamos esperando que venga CONNECT
unsigned char *pch;                         //Apunta al principio de lo encontrado
unsigned char proc_buff     =__FALSE;       //Flag que nos avisa que tenemos Buffer para analizar

unsigned char temp_decena=2,temp_unidad=2,temp_decimal=2;
unsigned char dht11_humedad_decena=5,dht11_humedad_unidad=0;
unsigned char ster_decena=2,ster_unidad=4,ster_decimal=3;
unsigned char vv_decimal=0,vv_unidad=8;
unsigned char direccion_viento=9;
unsigned char indice_uv=2;
unsigned char lluvia_decena=2,lluvia_unidad=2;
unsigned char presion_milesima=1,presion_centena=1,presion_decimal=4, presion_unidad=0;

void GSM_init(void){ 
  gsm_state    	=GSMOFF;
  gsm_substate 	=__FALSE;
  wait          =__FALSE;
  rx_tout       =__FALSE;
  retrys        =__FALSE;
  reply         =__FALSE;
  reply_wait    =__FALSE;
  reply_error   =__FALSE;
  reply_fail    =__FALSE;
  reply_conn    =__FALSE;
  proc_buff     =__FALSE;
}

void GSM_proc (void) {
    
  if(reply_wait){    
    pch=strstr(RxBuff.Buffer,(const char*)reply);
    if(pch){
		reply_wait = __FALSE;
    }else if(strstr(RxBuff.Buffer,"ERROR")){
      reply_error= __TRUE;
    }else if(reply_conn && strstr(RxBuff.Buffer,"CONNECT\r\n")){
      reply_wait = __FALSE;
      reply_conn=__FALSE;
      return;
	  }
	}	
}

void InDataRxBuffer(unsigned char dato){ 
  RxBuff.Buffer[RxBuff.pIn++]=dato;
  if(RxBuff.Buffer[RxBuff.pIn-2]=='\r' && RxBuff.Buffer[RxBuff.pIn-1]=='\n' ){
    proc_buff=__TRUE;
  }
}

//Cada un segundo
unsigned short gsm_tic_1Seg=0;

void GSM_tic (void){
  gsm_tic_1Seg++;
    
  if(gsm_tic_1Seg>1000){
    gsm_tic_1Seg=0;
    
    if(wait)      wait--;  
    if(rx_tout)   rx_tout--;
    
  }
}


void GSM_step(void){

  if(proc_buff){
    proc_buff=__FALSE;
    GSM_proc();
  }
  
  if(wait)            return;
  if(!gsm_substate)  ClrRxBuff();
  
  switch(gsm_state) 
  {
    case GSMOFF:
      //EN_GSM=1;
      LED_GSM=0;

      EnviarDatos();//PRUEBA
      gsm_state=GSMON;
      gsm_substate=__FALSE;
      wait=5;
    break;
    
    case GSMON:
		//EN_GSM=1;
    	LED_GSM=1;
		gsm_state=GSMCMDAT;
		gsm_substate=__FALSE;
		wait=5;
    break;
    
    case GSMCMDAT:
      if(gsm_substate==__FALSE){
        rx_tout=3;
        SCI2_Print("AT\r\n");
        reply="OK\r\n";
        gsm_substate=__TRUE;
        reply_wait=__TRUE;
        reply_error=__FALSE;
      }else{
        if(rx_tout && !reply_error){
          if(reply_wait==__FALSE){
            gsm_state=GSMCMDATE;
            gsm_substate=__FALSE;
            wait=0;
          }     
        }else{
          gsm_state=GSMOFF;
        }
      }
    break;
    
    case GSMCMDATE:
      if(!gsm_substate){
        rx_tout=3;
        SCI2_Print("ATE0\r\n");
        reply="OK\r\n";
        gsm_substate=__TRUE;
        reply_wait=__TRUE;
        reply_error=__FALSE;
      }else{
        if(rx_tout && !reply_error){
          if(!reply_wait){
        	  //Si entramos por aca esta ok a respuesta esperada
            gsm_state=GSMCMDCPIN;
            gsm_substate=__FALSE;
            wait=0;
          }     
        }else{
        	//Si estamos por aca no vino la respuesta esperada
          gsm_state=GSMOFF;
        }
      }
    break;
    
    case GSMCMDCPIN:
        if(!gsm_substate){
          rx_tout=3;
          SCI2_Print("AT+CPIN?\r\n");
          reply="READY\r\n";
          gsm_substate=__TRUE;
          reply_wait=__TRUE;
          reply_error=__FALSE;
        }else{
          if(rx_tout && !reply_error){
            if(!reply_wait){
          	  //Si entramos por aca esta ok a respuesta esperada
              gsm_state=GSMCMDCGREG;
              retrys=0;
              gsm_substate=__FALSE;
              wait=0;
            }     
          }else{
          	//Si estamos por aca no vino la respuesta esperada
            gsm_state=GSMOFF;
          }
        }
    break;

    case GSMCMDCGREG:
        if(!gsm_substate){
          rx_tout=3;
          if(!retrys)
        	  retrys=20;
          SCI2_Print("AT+CGREG?\r\n");
          reply="0,1";
          gsm_substate=__TRUE;
          reply_wait=__TRUE;
          reply_error=__FALSE;
        }else{
          if(rx_tout && !reply_error){
            if(!reply_wait){
          	  //Si entramos por aca esta ok a respuesta esperada
              gsm_state=GSMCMDCGDCONT;
              gsm_substate=__FALSE;
              wait=0;
            }     
          }else{
        	  //Si estamos por aca no vino la respuesta esperada
        	  if(retrys){
        		  retrys--;
        	  }
        	  if(!retrys) 	gsm_state=GSMOFF;
        	  else{
        		  gsm_state=GSMCMDCGREG;				//HASTA ACA OK
                  gsm_substate=__FALSE;
                  wait=0;
        	  }
          }
        }
    break;
    
    case GSMCMDCGDCONT:					//Entra bien
        if(gsm_substate==__FALSE){
          rx_tout=3;
          SCI2_Print("AT+CGDCONT=1,\"IP\",\"internet.gprs.unifon.com.ar\" \r\n");
          reply="OK";
          gsm_substate=__TRUE;
          reply_wait=__TRUE;
          reply_error=__FALSE;
        }else{
          if(rx_tout && !reply_error){
            if(reply_wait==__FALSE){
              gsm_state=GSMCMDSGACT;
              gsm_substate=__FALSE;
              wait=0;
            }     
          }else{
            gsm_state=GSMOFF;
          }
        }
    break;
    
    case GSMCMDSGACT:
          if(!gsm_substate){
            rx_tout=3;
            if(!retrys)
          	  retrys=20;
            SCI2_Print("AT#SGACT=1,1,ctigprs,ctigprs999\r\n");
            reply="#SGACT:";
            gsm_substate=__TRUE;
            reply_wait=__TRUE;
            reply_error=__FALSE;
          }else{
            if(rx_tout && !reply_error){
              if(!reply_wait){
            	  //Si entramos por aca esta ok a respuesta esperada
                gsm_state=GSMCMDUSERID;
                gsm_substate=__FALSE;
                wait=0;
              }     
            }else{
          	  //Si estamos por aca no vino la respuesta esperada
          	  if(retrys){
          		  retrys--;
          	  }
          	  if(!retrys) 	gsm_state=GSMOFF;
          	  else{
          		  gsm_state=GSMCMDSGACT;
                    gsm_substate=__FALSE;
                    wait=0;
          	  }
            }
          }
    break;
  
    case GSMCMDUSERID:
        if(!gsm_substate){
          rx_tout=3;
          if(!retrys)
        	  retrys=20;
          SCI2_Print("AT#USERID=\"WAP\"\r\n");
          reply="OK";
          gsm_substate=__TRUE;
          reply_wait=__TRUE;
          reply_error=__FALSE;
        }else{
          if(rx_tout && !reply_error){
            if(!reply_wait){
          	  //Si entramos por aca esta ok a respuesta esperada
              gsm_state=GSMCMDPASSW;
              gsm_substate=__FALSE;
              wait=0;
            }     
          }else{
        	  //Si estamos por aca no vino la respuesta esperada
        	  if(retrys){
        		  retrys--;
        	  }
        	  if(!retrys) 	gsm_state=GSMOFF;
        	  else{
        		  gsm_state=GSMCMDUSERID;
                  gsm_substate=__FALSE;
                  wait=0;
        	  }
          }
        }
    break;
    
    case GSMCMDPASSW:
        if(!gsm_substate){
          rx_tout=3;
          if(!retrys)
        	  retrys=20;
          SCI2_Print("AT#PASSW=\"WAP\"\r\n");
          reply="OK";
          gsm_substate=__TRUE;
          reply_wait=__TRUE;
          reply_error=__FALSE;
        }else{
          if(rx_tout && !reply_error){
            if(!reply_wait){
          	  //Si entramos por aca esta ok a respuesta esperada
              gsm_state=GSMCMDGPRS0;
              gsm_substate=__FALSE;
              wait=0;
            }     
          }else{
        	  //Si estamos por aca no vino la respuesta esperada
        	  if(retrys){
        		  retrys--;
        	  }
        	  if(!retrys) 	gsm_state=GSMOFF;
        	  else{
        		  gsm_state=GSMCMDPASSW;
                  gsm_substate=__FALSE;
                  wait=0;
        	  }
          }
        }
    break;
    
    case GSMCMDGPRS0:
        if(!gsm_substate){
          rx_tout=3;
          if(!retrys)
        	  retrys=20;
          SCI2_Print("AT#GPRS=0\r\n");
          reply="OK";
          gsm_substate=__TRUE;
          reply_wait=__TRUE;
          reply_error=__FALSE;
        }else{
          if(rx_tout && !reply_error){
            if(!reply_wait){
          	  //Si entramos por aca esta ok a respuesta esperada
              gsm_state=GSMCMDGPRS1;
              gsm_substate=__FALSE;
              wait=0;
            }     
          }else{
        	  //Si estamos por aca no vino la respuesta esperada
        	  if(retrys){
        		  retrys--;
        	  }
        	  if(!retrys) 	gsm_state=GSMOFF;
        	  else{
        		  gsm_state=GSMCMDGPRS0;
                  gsm_substate=__FALSE;
                  wait=0;
        	  }
          }
        }
    break;
    
    case GSMCMDGPRS1:
        if(!gsm_substate){
          rx_tout=3;
          if(!retrys)
        	  retrys=20;
          SCI2_Print("AT#GPRS=1\r\n");
          reply="+IP";
          gsm_substate=__TRUE;
          reply_wait=__TRUE;
          reply_error=__FALSE;
        }else{
          if(rx_tout && !reply_error){
            if(!reply_wait){
          	  //Si entramos por aca esta ok a respuesta esperada
              gsm_state=GSMCMDSD;
              gsm_substate=__FALSE;
              wait=0;
            }     
          }else{
        	  //Si estamos por aca no vino la respuesta esperada
        	  if(retrys){
        		  retrys--;
        	  }
        	  if(!retrys) 	gsm_state=GSMOFF;
        	  else{
        		  gsm_state=GSMCMDGPRS1;
                  gsm_substate=__FALSE;
                  wait=0;
        	  }
          }
        }
    break;
    
    case GSMCMDSD:
        if(!gsm_substate){
          rx_tout=3;
          if(!retrys)
        	  retrys=20;
          SCI2_Print("AT#SD=1,0,9138,\"186.19.232.173\"\r\n");				//PROBAR IP DEL PIO
          reply="CONNECT";
          gsm_substate=__TRUE;
          reply_wait=__TRUE;
          reply_error=__FALSE;
        }else{
          if(rx_tout && !reply_error){
            if(!reply_wait){
          	  //Si entramos por aca esta ok a respuesta esperada
              gsm_state=GSMCMDSD;
              gsm_substate=__FALSE;
              wait=0;
            }     
          }else{
        	  //Si estamos por aca no vino la respuesta esperada
        	  if(retrys){
        		  retrys--;
        	  }
        	  if(!retrys) 	gsm_state=GSMOFF;
        	  else{
        		  gsm_state=GSMCMDENV;
                  gsm_substate=__FALSE;
                  wait=15;
        	  }
          }
        }
    break;
    
    case GSMCMDENV:
        if(!gsm_substate){
          rx_tout=3;
          if(!retrys)
        	  retrys=20;
          EnviarDatos();
          //SCI2_Print("T266;H60;ST23;VV10;DVNE;UV1;LL10;PR1050");		//enviar datos sensados en este estado
          //reply="1";						//esperar devolucion de caracteres
          gsm_state=GSMOFF;
          gsm_substate=__FALSE;
          wait=10;
        }
    	break;
    	
    default:
    	gsm_state=GSMOFF;
    break;
  }
}

void EnviarDatos(void){
	//SCI2_Print("T266;H60;ST23;VV10;DVNE;UV1;LL10;PR1050");

	SCI2_Print("T");
	EnvVar(temp_decena);
	EnvVar(temp_unidad);
	EnvVar(temp_decimal);
	SCI2_Print(";H");
	while(SCI2S1_TDRE == 0);
	EnvVar(dht11_humedad_decena);
	EnvVar(dht11_humedad_unidad);
	SCI2_Print(";ST");
	EnvVar(ster_decena);
	EnvVar(ster_unidad);
	EnvVar(ster_decimal);
	SCI2_Print(";VV");
	EnvVar(vv_decimal);
	EnvVar(vv_unidad);
	SCI2_Print(";DV");
	DirecViento();
	SCI2_Print(";UV");
	EnvVar(indice_uv);
	SCI2_Print(";LL");
	EnvVar(lluvia_decena);
	EnvVar(lluvia_unidad);
	SCI2_Print(";PR");
	EnvVar(presion_milesima);
	EnvVar(presion_centena);
	EnvVar(presion_decimal);
	EnvVar(presion_unidad);	
}

void EnvVar(unsigned char Var){
	SCI2D='0'+Var;
	while(SCI2S1_TDRE == 0);
}

void DirecViento(void){
	switch (direccion_viento){
	case 0:	//N
		SCI2_Print("N");
		break;
	case 1:	//S
		SCI2_Print("S");
		break;
	case 2:	//E
		SCI2_Print("E");
		break;
	case 3:	//O
		SCI2_Print("O");
		break;
	case 4:	//NE
		SCI2_Print("NE");
		break;
	case 5:	//NO
		SCI2_Print("NO");
		break;
	case 6:	//SE
		SCI2_Print("SE");
		break;
	case 7:	//SO
		SCI2_Print("SO");
		break;
	case 8:	//NNE
		SCI2_Print("NNE");
		break;
	case 9:	//NNO
		SCI2_Print("NNO");
		break;
	case 10://SSO
		SCI2_Print("SSO");
		break;
	case 11://SSE
		SCI2_Print("SSE");
		break;
	case 12:	//ONO
		SCI2_Print("ONO");
		break;
	case 13:	//OSO
		SCI2_Print("OSO");
		break;
	case 14:	//ESE
		SCI2_Print("ESE");
		break;
	case 15:	//ENE
		SCI2_Print("ENE");
		break;
	}
	
}
