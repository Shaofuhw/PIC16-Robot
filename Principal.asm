
;============================================================================-
;		   PROYECTO ROBOT CONTROL POR PALMADAS
;-----------------------------------------------------------------------------
;    
;-- Microcontrolador: PIC16F883			
;-- Curso: 2015/2016
;-- Autores: Manuel Carrasco, Shao Fu Hu Wu, David Chamizo, Eva Masero
;    
;---------------------------- Descripción -------------------------------------
; En este programa se realiza el control del robot en función del número de
; palmadas recibidas. El robot reacciona ante obstáculos en su trayectoria   
;==============================================================================
  
    
#include <p16f883.inc>   ;archivo de descripción  

;----Bits de configuracion-----
    
 __config _config1, _INTOSC & _LVP_OFF & _WDT_OFF & _DEBUG_ON 
 ;Reloj interno, Prog.bajo voltaje OFF, watchdog OFF, pines RB6,RB7 depuración
 
 __config _config2, _BOR21V
 ;Brown-out Reset set to 2.1V
        
;------Variables---------------
    cblock  0x20 
  dlay
  palmadas
  npalmadas
  timer
  
  status_temp
  w_temp
  porta_temp
  
  siled
  contador
  contador2 
  contador3    
    endc
   
;--- Otras Definiciones ------ 
;--- Movimiento motores ---    
#define	der_ret	     porta,ra1
#define der_av	     porta,ra2
#define izq_ret      porta,ra3
#define izq_av	     porta,ra4

#define LED	     portc,rc4    
#define	LED_DEL_DER  portc,rc6
#define	LED_DEL_IZQ  portb,rb3
#define	LED_TRAS_IZQ portb,rb4
#define	LED_TRAS_DER portc,rc5
   
#define TRIG	     porta,ra5
  
;----Vector de reset ---------
	org	0x00
	goto	inicio	
	
;----Vector de interrupción---	
	org	0x04
	goto	Interrupcion
	
;----Definición de macros-----
#include <macros.inc>	


;=============================================================================
;			     Programa Principal
;=============================================================================
	org	0x06
inicio	
	call	ini_osc	    ;Inicializa el oscilador interno a 8 MHz
	call    ini_es      ;Inicializa los puertos E/S	  
	call    ini_ADC	    ;Inicializa módulo conversión ADC
	call	ini_tmr2    ;Inicializa el tmr2
	call	ini_ccp2    ;Inicializa el tmr1 y ccp2
	banco0
	bcf	TRIG	    ;Inicializa a 0 el trigger
	call    ini_int	    ;Inicializa las interrupciones
	clrf	palmadas    ;Limpia la variable palmadas
	
bucle	btfsc	palmadas,2  ;Comprobación del número de palmadas
	goto	es45
	btfsc	palmadas,1  
	goto	es23
	goto	es01
es45	btfsc	palmadas,0
	goto	cinco	    ;Cuenta 5 palmadas
	goto	cuatro	    ;Cuenta 4 palmadas
es23	btfsc	palmadas,0
	goto	tres	    ;Cuenta 3 palmadas
	goto	dos	    ;Cuenta 2 palmadas
es01	btfsc	palmadas,0
	goto	uno	    ;Cuenta 1 palmada
	goto	cero	    ;Cuenta 0 palmadas
	
bucle2	call	retardo2    ;Retardo de 0,66 s
	bsf	TRIG	    ;Manda un trigger al sensor de ultrasonido
	call	retardo	    ;Durante 10us
	bcf	TRIG	    ;Deja de mandar el trigger 
	call	retardo3    ;Retardo de 90 ms
	goto	bucle
	
;==============================================================================
;				Subrutinas
;==============================================================================

#include <Inicializaciones.inc>	
#include <Interrupcion.inc>
#include <LDR.inc>	
#include <obstaculos.inc>
#include <retardos.inc>
#include <palmadas.inc>	
	
	
    end