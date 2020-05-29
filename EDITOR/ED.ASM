;ษอออออออออออออออออออออป
;บ MINI EDITOR Ver 2.0 บ  Trab    LAPRO III - Prof. Marcio Pinho
;ฬอออออออออออออออออออออสออออออออออออออออออออออออออออออออออออออออออออออออป
;บ by Alessander Oliveira,
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

INCLUDE edmacro.h

DOSSEG
.MODEL  SMALL
.386
.STACK  300h  ;reserva 764k para pilha
.DATA
   
INCLUDE eddata.h

.CODE
 
; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Procedimentos ...                                           บ
; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

SALVA_ARQ  PROC near
  creatfile
  openfile
  fileindex
  sendfile 20160
  closefile
  ret
SALVA_ARQ  ENDP

BUSCA_ARQ  PROC near
  m_zera_vetor
  m_ctrlpageup

  openfile
  readfile 20160
  closefile
  mov PRIM_LINHA,0
  refresh
  printit cr
  ret
BUSCA_ARQ  ENDP
   
outimpressora proc far
  pusha
   mov ah,1
   mov dx,0
   int 17h
   mov cx,10000
   lea bx,vetor
   denovo:
     mov dx,DGROUP
     mov es,dx
     mov al,es:[bx]
     inc bx
     inc bx
     mov ah,0
     mov dx,0
     int 17h
   loop denovo
  popa
outimpressora endp

; ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ Bloco Principal ...                                         บ
; ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

main proc far
   begin     
   tela
   mov ah,02       ;Inicializa o cursor na tela de edicao
   mov dh,02       ;na coordenada (2,0)
   mov dl,0        ;dh(linha)     dl(coluna)
   mov bh,0        ;
   int 10h         ;
   mouse SHOW_
   mouse_setpos 0,0
   mov PRIM_LINHA,0
   mov ULT_LINHA,160
   mov FLAG_INSERT,1
   mov AUXCONT_LINHA,0
   mov ax,DGROUP
   mov ds,ax
   lea bx,VETOR
   
   m_poe_str 0,0,st0,12,14
   
   m_about

   jmp     digita

help:
 m_help
 jmp digita
	
out_impressora:
 call outimpressora
 jmp digita

page_up:
  m_pageup
 jmp digita

page_down:
  m_pagedown
 jmp digita

about:
  m_about
 jmp digita

zera_vetor:
  m_zera_vetor
  m_ctrlpageup
 jmp page_up

cr_cl:
 m_desce
 m_home
 jmp digita

imprime_tela:   
  mouse HIDE_
  mov cx,FLAG_INSERT
  cmp cx,1
  jne segue
    m_insert
  segue :
   poe_na_tela     ;
   inc dl
   mouse SHOW_
   cmp dl,79       ;Troca de linha se estivar na coluna 79 
   je cr_cl;
   push bx
   mov ah,02
   mov bh,00
   int 10h         ;atualiza cursor  
   pop bx
  jmp digita

nova_linha:
  salva_linha
  apaga_resto
  m_home
  abre_espaco
  m_desce
  restaura_linha
  add ULT_LINHA,160
  jmp digita       

direita :
  cmp dl,78       ;Troca de linha
  jne normal      ;
   m_desce        ;  
   m_home         ;
 normal:          ;
  m_direita       ;
  jmp digita      ;

esquerda:
  cmp dl,0        ;Se estiver na coluna 1 e na linha2 nao pode voltar
  je digita       ;
  m_esquerda      ;
  jmp digita      ;
 back:
  cmp dl,0
  je digita
    m_esquerda
    m_del
  jmp digita

desce:
  m_desce
  jmp digita

sobe:
  m_sobe
  jmp digita

home:
  m_home
  jmp digita

label_end:
  m_end
  jmp digita

del:
  m_del
  jmp digita

insert:
  mov cx,FLAG_INSERT
  cmp cx,0
  je seta_f 
    tam_cursor 0,6
    mov FLAG_INSERT,0
  jmp digita
  seta_f:
    tam_cursor 5,6
    mov FLAG_INSERT,1
  jmp digita

control_fim :
  m_busca_espaco
  jmp digita

control_inicio :
  m_busca_espaco_volta
  jmp digita

control_end:
  apaga_resto
  jmp digita

control_home:
  copia_resto
  jmp digita

control_y :
  m_ctrl_y
  jmp digita

salva_arquivo:
  call SALVA_ARQ
  jmp digita

busca_arquivo:
  call BUSCA_ARQ
  jmp digita

digita:
   m_poe_num  0,57,AUXCONT_LINHA,4
   m_poe_num  0,65,dl,2
   ;m_poe_num  0,77,edpage,14

   mov ah,07h      ;Le uma tecla
   int 21h         ;e nao exibe , coloca caracter em al
   cmp al,0        ;Se for duplo pula para o tratamento
   je  duplo_ret   ;especial

   cmp al,ENTER_
    je nova_linha
   cmp al,CTRL_Y
    je control_y
   cmp al,BACKSPACE
    je back
   cmp al,ESC_      ;Termina
    je  fim
   jmp imprime_tela
   jmp digita

duplo_ret :        ;Aqui e feito o tratameto das teclas de duplo retorno
   mov ah,07h      ;Le uma tecla
   int 21h         ;e nao exibe , coloca caracter em al
   cmp al,BAIXO_
    je  desce
   cmp al,DIREI_
    je  direita
   cmp al,ESQU_
    je  esquerda
   cmp al,CIMA_
    je  sobe
   cmp al,HOME_
    je  home
   cmp al,END_
    je  label_end
   cmp al,DEL_
    je  del
   cmp al,INS_
    je  insert
   cmp al,F1_
    je  help
   cmp al,CTRL_DIR
    je  control_fim
   cmp al,CTRL_ESQ
    je  control_inicio
   cmp al,CTRL_END
    je  control_end
   cmp al,CTRL_HOME
    je  control_home
   cmp al,F2_
    je  salva_arquivo
   cmp al,F3_
    je  busca_arquivo
   cmp al,F4_
    je  zera_vetor
   cmp al,F6_
    je  about
   cmp al,F7_
    je  out_impressora
   cmp al,PAGEUP_
    je  page_up
   cmp al,PAGEDOWN_
    je  page_down
   jmp digita      ;Incondicionalmente volta para a digita

fim:
   m_finaliza
   setmode TEXTMODE_80x25
   retdos
desloca :
  mouse HIDE_
   rola_tela
  mouse SHOW_
  jmp digita

main endp
     end main

