   
   st0  db '  MINI EDIT Ver 2.1:                                Line     Col       PAGE     $'
   st1  db '様様様様様様様様様様様様様様様様�TEXTO.EDI様様様様様様様様様様様様様様様様様様様$'
   st2  db '様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様$'
   st3  db ' F1-Help  F2-Save  F3-Load  F4-New  F5-Reserv  F6-About  F7-Imprime    ESC-Exit$'

   hlpst1  db ' � Mini Edit HELP 様様様様様様様様様様様様様様様� $'
   hlpst2  db ' � CTRL_>    Busca proximo espaco a direita      $'
   hlpst3  db ' � CTRL_<    Busca proximo espaco a esquerda    � $'
   hlpst4  db ' � CTRL Y    Apaga linha                        � $'
   hlpst5  db ' � CTRL+END  Apaga do ponto ao fim da linha     � $'
   hlpst6  db ' � CTRL+HOME Apaga do ponto ao inicio da linha  � $'
   hlpst7  db ' � PAGEUP    Sobe uma pagina                    � $'
   hlpst8  db ' � PAGEDOWN  Desce uma pagina                    $'
   hlpst9  db ' 藩様様様様様様様様様様様様様様様様様様様様様様夕 $'
   
   about1  db ' 浜様様様様様様様様様様様様様様様様様様様様様様融 $'
   about2  db ' � LAPRO III - Trab 1            ASSEMBLY 80386 � $'
   about3  db ' �                                              � $'
   about4  db ' � 浜様様様様様様様様様様�    栩栩栩 臼臼臼     � $'
   about5  db ' � �                     �    �      臼    �    � $'
   about6  db ' � � Alessander Oliveira �    栩�    臼    �    � $'
   about7  db ' � �             (Aless) �    �      臼    �    � $'
   about8  db ' � �                     �    栩栩栩 臼臼臼     � $'
   about9  db ' � 藩様様様様様様様様様様�                      � $'   
   about10 db ' 藩様様様様様様様様様様様様様様様様様様様様様様夕 $'
  
   HIDE_ equ 2  ;Flags de exibicao de mause
   SHOW_ equ 1  ;
   
   ESC_      equ 27
   ENTER_    equ 13
   CIMA_     equ 72
   BAIXO_    equ 80
   ESQU_     equ 75
   DIREI_    equ 77
   PAGEUP_   equ 49h 
   PAGEDOWN_ equ 51h
   CTRLPAGEDOWN_ equ 76h 
   HOME_   equ 47h
   INS_    equ 52h
   END_    equ 4fh
   BACKSPACE equ 8h
   CTRL_DIR equ 74h
   CTRL_ESQ equ 73h
   CTRL_Y equ 19h
   CTRL_END equ 75h
   CTRL_HOME equ 77h
   DEL_ equ 53h    
   F1_ equ 3bh
   F2_ equ 3ch
   F3_ equ 3dh
   F4_ equ 3eh
   F5_ equ 3fh
   F6_ equ 40h
   F7_ equ 41h
   ESPACO_ equ 20h
   edpage db 0   ;Contador de pagina
   TEXTMODE_80x25    equ 3   ;Modo para passar para funcao set_modo
   GRAFMODE_320x200  equ 19  ;
   INICIO_MEMO equ 0b800h    ;Inicio na memoria de video (VGA)
   LINHA    equ 160
   L_AUX    equ 80
   PRIM_POS equ 320d
   PRIM_LINHA dw 0
   ULT_LINHA  dw 0 
   FLAG_INSERT dw 0   ; Flag para o modo de escrita 1-insert 0-Overwrite
   AUXCONT_LINHA db 0 ; Auxiliar que indica a linha atual 
   BUFFER_AUX   dw 2000 dup (?) ;buf para salvar toda tela (quando exibe um help, ...)
   BUFFER_LINHA dw 80 dup (0720h)
   tapa_buraco db '   $'  ;Para limpar(na tela) os contadores linha e coluna 
   
   filestruct label byte    ;Estrutura para efetivar a manipulacao de arquivo
   maxlen   db 40           ;
   actlen   db ?            ;
   filename db 'texto.edi'  ;
   infostruct label byte    ;
   mlenght  db 120          ;
   infoline db ?            ;
   filehand dw ?            ;
   cr       db 0Dh,'$'      ;
   
   VETOR     dw 10000 dup (0720h) ; memoria do vetor (5 paginas)
   BUG       db 'O u t   o f   R a n g e !'
