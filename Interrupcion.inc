;-------------------------------------------------------------------------
;		   Rutina de Servicio de Interrupci�n			    
;-------------------------------------------------------------------------

Interrupcion
    
    ;--- Salvaguarda del contexto ---
    movwf   w_temp	    ;W_TEMP = W
    swapf   status,w	    ;W = STATUS con nibbles intercambiados
    movwf   status_temp	    ;STATUS_TEMP = STATUS con nibbles intercambiados
      
    ;-----------------------------------------------------------------------
    ;-- Comprobaci�n interrupci�n TMR1--
    banco0
    btfsc   pir1,tmr1if	    ;Comprueba la bandera del TMR1
    call    tmr1int	    ;Se ha producido interrupci�n TMR1
    
    ;-----------------------------------------------------------------------
    ;-- Comprobaci�n interrupci�n CCP2--
    banco0
    btfsc   pir2,ccp2if	    ;Comprueba la bandera del CCP2
    call    ccpint	    ;Se ha producido interrupci�n CCP2
    
    ;-----------------------------------------------------------------------
    ;-- Comprobaci�n interrupci�n palmada-- 
    
    btfss   intcon,intf	    ;Comprueba la bandera de int. exterior
    goto    sigue	    ;No se ha producido int. exterior
    call    aplauso	    ;Se ha producido int.exterior
    bcf	    intcon,intf	    ;Borra bandera int. exterior
    ;------------------------------    
sigue	
    
    ;-- Comprobaci�n interrupci�n fin de conversi�n A/D--
     banco0
     btfss  pir1, adif	    ;Comprueba la bandera de fin conversi�n A/D
     goto   sigue1	    ;No se ha producido ADC
     call   LDR		    ;Se ha producido int. por fin ADC
     bcf    pir1,adif	    ;Borra bandera de fin conversi�n A/D
     call   delay	    ;Tiempo de adquisici�n
     bsf    adcon0,go	    ;Vuelve a inicia conversi�n A/D
     
    ;-------------------------------
sigue1			    
    
    ;-- Comprobaci�n interrupci�n TMR2--
    banco0
    btfsc   pir1,tmr2if	    ;Comprueba la bandera del TMR2
    call    tmr2int	    ;Se ha producido interrupci�n TMR2
    bcf	    pir1,tmr2if	    ;Borra bandera TMR2
    
    
    ;--- Restauraci�n del contexto ---
    swapf   status_temp,w   ;W = STATUS original con nibbles correctos
    movwf   status	    ;STATUS = STATUS original
    swapf   w_temp,f	    ;W_TEMP = W original con nibbles intercambiados
    swapf   w_temp,w	    ;W = W original con nibbles correctos
    
    retfie