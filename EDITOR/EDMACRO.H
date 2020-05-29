;    ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;    บ Macros ... Este arquivo faz parte do mini edit   -   ed.asm   บ
;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ by Alessander Oliveira  บ
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

 ; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออป
 ; บ Macros para manipulacoes com arquivos em modo texto บ
 ; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออผ
    printit macro buffer ;Esta macro imprime a string na tela
      push ax            ;na posicao corrente do cursor
      push dx
      lea  dx,buffer
      mov  ah,09
      int  21h
      pop  dx
      pop  ax
    endm

    creatfile macro      ;Esta macro cria um arquivo
      push ax
      push cx
      push dx
      mov  ah,3Ch    ;com nome e extensao fornecidos pela
      mov  cx,00h    ;variavel "filename"
      lea  dx,filename
      int  21h
      pop dx
      pop cx
      pop ax
    endm

    openfile macro       ;Esta macro abre o arquivo
      push ax
      push dx
      mov al,02h     ;com nome e extensao fornecidos pela
      mov ah,3Dh     ;variavel "filename"
      lea dx,filename
      int 21h
      mov filehand,ax
      pop dx
      pop ax
    endm

    fileindex macro   ;Esta macro coloca o indice do arquivo
      push ax
      push bx
      push cx
      push dx
      mov al,02h      ;"filename" na primeira posicao
      mov ah,42h
      mov bx,filehand
      mov cx,0h
      mov dx,0h
      int 21h
      pop dx
      pop cx
      pop bx
      pop ax
    endm

    sendfile macro sizefile ;Esta macro envia dados para o arquivo
      push ax
      push bx
      push cx
      push dx
      mov ah,40h            ;"filename" atraves da variavel "infodata"
      mov bx,filehand
      mov cx,sizefile
      lea dx,VETOR
      int 21h
      pop dx
      pop cx
      pop bx
      pop ax
    endm

    readfile macro sizefile
      push ax
      push bx
      push cx
      push dx
      mov ah,3Fh
      mov bx,filehand
      mov cx,sizefile
      lea dx,VETOR
      int 21h
      pop dx
      pop cx
      pop bx
      pop ax
    endm

    closefile macro
      push ax
      push bx
      mov ah,3Eh
      mov bx,filehand
      int 21h
      pop bx
      pop ax
    endm

 ; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออป
 ; บ Macros primitivas usuais                            บ
 ; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออผ
    m_delay macro tempo
     local laco
     push cx 
     mov cx,WORD PTR tempo
     laco:
      ;Gasta tempo
     loop laco
     pop cx
    endm
    
    keypress macro
     mov ah,07  ;Espera intervencao do teclado
     int 21h    ;
    endm 

    retdos macro  
     mov ah,4Ch  ;Retorna ao controle  do DOS
     int 21h     ;
    endm
  
   setmode macro modo
    mov ah,00    ;Restaura o modo texto selecionado
    mov al,modo  
    int 10h
   endm

   begin macro
    mov ax,DGROUP   ;Iniciaizacao padrao
    mov ds,ax       ;
    clrscr
   endm
   
  tam_cursor macro inic,fim
    pusha 
    mov ch,inic ;Reticulo inicial
    mov cl,fim  ;Reticulo final
    mov ah,1   
    int 10h    
    popa
   endm

   clrscr macro
    setmode TEXTMODE_80x25
    mov ah,02  ;coloca o cursor na posicao de tela (0,0)
    mov dh,00  ;
    mov dl,00  ;
    push bx
    mov bh,00  ;pagina de video 0
    int 10h    ;
    pop bx
   endm

 ; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออป
 ; บ Macros para manipulacao de mouse                    บ
 ; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออผ
   mouse macro status
    pusha ;Salva contexto
    mov ax,status  ;Mostra ou Esconde o mouse
    int 33h        ;
    popa ;Restaura contexto
   endm;

   mouse_setpos macro x,y
    pusha ;Salva contexto
    mov ax,4    ;Coloca o mouse na posicao indicada
    mov cx,x    ;    
    mov dx,y    ;
    int 33h     ;
    popa ;Restaura contexto
   endm

   mouse_getpos macro 
    mov ax,3    ;Pega a posicao atual do mause
    int 33h     ;Coloca o resultado em cx(coluna) e em dx(linha)
   endm

   mouse_status macro botao
    mov ax,6      ;Esta rotina retorna o status do botao do mouse
    mov bx,botao  ;Recebe o numero do botao : 0 = botao esquerdo
		  ;                           1 = botao direito
    int 33h       ;                           2 = botao do meio 
		  ;Obs:
		  ;Coloca o resultado em ax   1->pressionado 
		  ;                           0->solto       
   endm
 
 ; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออป
 ; บ Macros especificas para o editor                    บ
 ; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออผ
   m_ctrlpageup macro
    mov ah,02       ;Inicializa o cursor na tela de edicao
    mov dh,02       ;na coordenada (2,0)
    mov dl,0        ;dh(linha)     dl(coluna)
    mov bh,0        ;
    int 10h         ;
    mov PRIM_LINHA,0
    mov AUXCONT_LINHA,0
    mov ax,DGROUP
    mov ds,ax
    lea bx,VETOR
    refresh
    mov edpage,0
   endm 

  m_pageup macro
    local sobe
    mov cx,21
    sobe:
     push cx
     m_sobe
     pop cx
    loop sobe
    ;sub edpage,1
  endm
   
  m_pagedown macro
    local desce,sobe
    mov cx,41
    desce:
     push cx
     m_desce
     pop cx
    loop desce
    mov cx,20
    sobe:
     push cx
     m_sobe
     pop cx
    loop sobe
    ;add edpage,1
  endm
   
   m_zera_vetor macro
    local denovo
    pusha 
      mov al,' '
      mov cx,10000
      lea bx,vetor
    denovo:
      mov dx,DGROUP
      mov es,dx
      mov es:[bx],word ptr ax
      inc bx
      inc bx
    loop denovo
    popa
   endm  

     linear macro    ; macro que converte x e y  ( dh e dl)
       push ax
       push dx
       xor ax,ax
       mov al,dh
       mov cx,LINHA
       mul cx
       xor cx,cx
       pop  dx
       mov cl,dl
       shl cl,1
       add cx,ax
       pop ax
     endm

     poe_na_tela macro
       mov cx,INICIO_MEMO
       mov es,cx
       linear
       mov ah,7
       mov di,cx
       mov es:[di],ax
       mov cx,DGROUP
       mov es,cx
       mov es:[bx],word ptr ax
       inc bx
       inc bx
     endm

     m_esquerda macro
       dec dl        ;Movimenta o cursor a esquerda
       dec bx
       dec bx
       mov ah,02     ; decrementa dl(coluna
       push bx
       mov bh,00     ;
       int 10h       ;
       pop bx
     endm

     m_direita macro
      inc dl          ;Movimente o cursor a direita
      inc bx
      inc bx
      mov ah,02       ; incremente dl(coluna)
      push bx
      mov bh,00       ;
      int 10h         ;
      pop bx
     endm

     m_desce macro
      local fim
      lea cx,VETOR 
      add cx,20000
      cmp bx,cx
      jge digita
      cmp dh,22
      jne fim
	add PRIM_LINHA,LINHA
	refresh
	dec dh
      fim :
       inc dh          ;Movimenta o cursor para baixo
       add bx,LINHA
       add ULT_LINHA,LINHA
       mov ah,02       ; incrementa dh(linha)
       push bx
       mov bh,00       ;
       int 10h         ;
       pop bx
     add AUXCONT_LINHA,1
    endm

    m_sobe macro
     local fim
     cmp dh,02
     jne fim
       cmp PRIM_LINHA,0
       je digita
       sub PRIM_LINHA,LINHA
       refresh
       inc dh
     fim :
       dec dh          ;Movimenta cursor para cima
       sub bx,LINHA
       push bx
       mov ah,02       ; decrementa dh(linha)
       mov bh,00       ;
       int 10h         ;
       pop bx
     sub AUXCONT_LINHA,1
    endm

     m_busca_espaco macro
      local busca
      busca:
       cmp dl,78        ;Se estiver na coluna 1 e na linha2 nao pode voltar
       je digita       ;
       m_direita       ;
       
       mov ax,DGROUP
       mov es,ax
       mov cx,bx
       
       mov al,es:[bx]
       cmp al,' '
       jne busca
     
     endm

     m_busca_espaco_volta macro
      local busca
      busca:
       cmp dl,0        ;Se estiver na coluna 1 e na linha2 nao pode voltar
       je digita       ;
       m_esquerda      ;
       
       mov ax,DGROUP
       mov es,ax
       mov cx,bx
       
       mov al,es:[bx]
       cmp al,' '
       jne busca
     endm
     
     m_home macro
       xor cx,cx
       mov cl,dl
       shl cx,1
       sub bx,cx
       mov dl,0
       mov ah,02     
       push bx
       mov bh,00     
       int 10h       
       pop bx
     endm

     m_end macro
       local repete,fim,busca
       mov cx,78
      repete:
       m_busca_espaco
       cmp dl,78
	je fim
       loop repete
      fim:
      busca:
       cmp dl,0        ;Se estiver na coluna 1 e na linha2 nao pode voltar
       je digita       ;
       m_esquerda      ;
       mov ax,DGROUP
       mov es,ax
       mov cx,bx
       mov al,es:[bx]
       cmp al,' '
       je busca
     endm

     m_del macro
       push ds
       mov ax,DGROUP
       mov es,ax
       mov ds,ax
       pusha
       mov cx,79
       sub cl,dl

       mov di,bx
       inc bx
       inc bx
       mov si,bx
       cld
       rep movsw

       popa
       pop ds
       refresh
     endm

     m_insert macro
       push ds
       pusha
       mov ax,DGROUP
       mov es,ax
       mov ds,ax

       xor cx,cx
       mov cx,78
       sub cl,dl
       shl cx,1
       add bx,cx
       shr cx,1
       mov di,bx
       dec bx
       dec bx
       mov si,bx
       std
       rep movsw

       popa
       pop ds
       refresh
     endm

     
     m_ctrl_y macro
       m_home
       push ds
       push bx
       mov ax,DGROUP
       mov es,ax
       mov ds,ax

       mov di,bx
       mov si,bx
       add si,LINHA
       mov cx,10000  ;com 20000 ele sobe o que nao deve    
       lea ax,VETOR
       sub bx,ax
       sub cx,bx
       cld
       rep movsw

       pop bx
       pop ds
       refresh
     endm

     m_imprime macro
       mov ah,02h      ;Imprime o caracter que esta em al
       push dx         ;salva dx
       mov dl,al       ;coloca o caracter em dl
       int 21h         ;
       pop dx          ;recupera o valor anterior de dl(coluna)
       inc dl          ;Acerta a coluna dl = coluna
     endm

     impr_str macro x,y,buffer  ;Imprime uma str na posicao indicada por (x,y)
       pusha
       mov ah,02      ;Posiciona o cursor
       mov dh,x       ;nas coordenadas (x,y)
       mov dl,y       ;
       mov bh,00      ;
       int 10h        ;
       lea dx,buffer
       mov ah,09      ;Imprime a string na tela
       int 21h        ;na posicao do cursor
       popa
       push bx
       mov ah,02      ;Posiciona o cursor
       mov bh,00      ;
       int 10h        ;
       pop bx
     endm

     seta_pagina macro pag ; seta a pagina de video
       mov ah,05
       mov al,pag
       int 10h
     endm

    rola_tela macro
      mov ah,06  ;Funcao 6 ->Rola uma janela
      mov ch,02  ;Coordenadas iniciais, canto superior esquerdo
      mov cl,0   ; ch(posy) cl(posx)      dh(posy) dl(posx)
      mov dh,21  ;Coordenadas finais, canto inferior direito
      mov dl,79  ;
      mov al,10  ;Numero de linhas a 'rolar'  (0-para rolar todas)
      mov bh,7   ;
      int 10h

      inc edpage

      mov dh,12  ;Coloca o cursor na posicao de tela (1,12)
      mov dl,0   ;        dh(linha)   dl(coluna)
      mov bh,00  ;Pagina ativa
      mov ah,02  ;
      int 10h    ;
    endm

    tela macro
      clrscr
      impr_str  0,0,st0
      impr_str 1,0,st1
      impr_str 23,0,st2
      impr_str 24,0,st3
      mov ah,02
      mov dh,03
      mov dl,00
      mov bh,00
      int 10h
    endm

    m_help macro
      pusha
      mouse HIDE_
      save_screen
      impr_str  6,15,hlpst1 ;
      impr_str  7,15,hlpst2 ;
      impr_str  8,15,hlpst3 ;
      impr_str  9,15,hlpst4 ;
      impr_str 10,15,hlpst5 ;
      impr_str 11,15,hlpst6 ;
      impr_str 12,15,hlpst7 ;
      impr_str 13,15,hlpst8 ;
      impr_str 14,15,hlpst9 ;
      mouse SHOW_
      push dx
      mov dl,0
      mov dh,0
      mov ah,02   
      mov bh,00
      int 10h    
      
      keypress
      mouse HIDE_
      restore_screen
      pop dx
      mov ah,02  ;Coloca o cursor na posicao
      mov bh,00
      int 10h    ;que estava anteriormente
      mouse SHOW_
      popa
    endm
    
    m_about macro
      pusha
      mouse HIDE_
      save_screen
      impr_str  6,15,about1 ;
      impr_str  7,15,about2 ;
      impr_str  8,15,about3 ;
      impr_str  9,15,about4 ;
      impr_str 10,15,about5 ;
      impr_str 11,15,about6 ;
      impr_str 12,15,about7 ;
      impr_str 13,15,about8 ;
      impr_str 14,15,about9 ;
      impr_str 15,15,about10 ;
      mouse SHOW_
      push dx
      mov dl,0
      mov dh,0
      mov ah,02   
      mov bh,00
      int 10h    
      
      keypress
      mouse HIDE_
      restore_screen
      pop dx
      mov ah,02  ;Coloca o cursor na posicao
      mov bh,00
      int 10h    ;que estava anteriormente
      mouse SHOW_
      popa
    endm
    
    m_finaliza macro
     ;Preparar algum tipo de finalizacao
    endm

    abre_espaco macro
      push ds
      mov ax,DGROUP
      mov es,ax
      mov ds,ax
      lea si,VETOR
      add si,20000
      mov cx,si
      sub si,2
      mov di,si
      add di,LINHA
      sub cx,bx
      shr cx,1
      std
      rep  movsw
      pop ds
    endm

    apaga_resto macro
     local repete
     mov cx,79
     sub cl,dl
     repete :
       dec cx
       m_del
       cmp cx,0
       jne repete
    endm

    copia_resto macro  ;CTRL+HOME
     local repete
     xor cx,cx   ;
     mov cl,dl   ;
     cmp dl,0    ;Nao pode fazer nada se a coluna for 0
     je digita   ;
     push cx
     m_home
     pop cx
     repete:
       dec cx
       m_del
       cmp cx,0
       jne repete
    endm

    salva_linha macro
     push ds
     pusha
     mov ax,DGROUP
     mov ds,ax
     mov es,ax
     mov si,bx
     lea di,BUFFER_LINHA
     mov cx,79
     sub cl,dl
     cld
     rep movsw
     popa
     pop ds
    endm
    
    restaura_linha macro
     push ds
     pusha
     mov ax,DGROUP
     mov ds,ax
     mov es,ax
     mov di,bx
     lea si,BUFFER_LINHA
     mov cx,79
     cld
     rep movsw
     popa
     pop ds
     refresh
    endm
    
    save_screen macro
     push ds
     pusha
     mov ax,INICIO_MEMO
     mov ds,ax
     xor si,si
     mov ax,DGROUP
     mov es,ax
     lea di,BUFFER_AUX
     mov cx,2000
     cld
     rep movsw
     popa
     pop ds
    endm

    restore_screen macro
    pusha
    push ds
     mov ax,INICIO_MEMO
     mov es,ax
     xor di,di
     mov ax,DGROUP
     mov ds,ax
     lea si,BUFFER_AUX
     mov cx,2000
     cld
     rep movsw
    pop ds
    popa
    endm

    refresh macro
     mouse HIDE_
     pusha
     push ds
     mov ax,INICIO_MEMO
     mov es,ax
     mov di,PRIM_POS
     mov ax,DGROUP
     mov ds,ax
     lea si,VETOR
     add si,PRIM_LINHA
     mov cx,1680
     cld
     rep movsw
     pop ds
     popa
     mouse SHOW_
    endm

   m_poe_num macro lin,col,num,cor
    local laco_div,imp_num
    impr_str lin,col,tapa_buraco 
   pusha
    mov bx,10 ;Valor para divisao
    xor ax,ax
    mov al,num ;Valor a ser convertido para tela
    mov cx,0  ;Numero de vezes que o valor vai para pilha
    laco_div: ;
     mov dx,0 ;
     div bx   ;
     push dx  ;Coloca o resto na pilha(ultimo digito do numero)
     inc cx
     cmp al,0
     jne laco_div
    
     xor bx,bx
     mov bl,col
    imp_num:
      pop ax
      add al,'0'       ;tranforma em numero
      mov ah,cor       ;seta a cor
      
      mov dx,0b800h
      mov es,dx
      
      mov dx,bx
      add dx,lin*LINHA
      shl dx,1
      mov di,dx
      mov es:[di],ax
      inc bx
     loop imp_num
     popa
  endm
  
   m_poe_str macro lin,col,str,tam,cor
    local imp_str
    pusha
    push ds
    
    mov si,0
    mov cx,tam ;Numero de caracteres na string
    xor bx,bx
    mov bl,col ;coluna em que comeca a imprimir
    imp_str:
      mov al,str[si]   ;caracter
      mov ah,cor       ;cor do caracter
      
      mov dx,0b800h ;apenas seta o es do inicio da memoria de video
      mov es,dx     ;
      
      mov dx,bx        ;passa o numero da coluna para o dx
      add dx,lin*LINHA ;calculo para o local da memoria de video que 
      shl dx,1         ;corresponde a coluna desejada
      mov di,dx        ;
      mov es:[di],ax   ;coloca o caracter na posicao de memoria calculada
      inc bx     ;soma coluna para imprimir proximo caracter
      inc si     ;soma si para pegar o proximo caracter da string
     loop imp_str
    
    pop ds
    popa
   endm

