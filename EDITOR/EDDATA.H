   
   st0  db '  MINI EDIT Ver 2.1:                                Line     Col       PAGE     $'
   st1  db 'อออออออออออออออออออออออออออออออออTEXTO.EDIออออออออออออออออออออออออออออออออออออออ$'
   st2  db 'ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ$'
   st3  db ' F1-Help  F2-Save  F3-Load  F4-New  F5-Reserv  F6-About  F7-Imprime    ESC-Exit$'

   hlpst1  db ' ษ Mini Edit HELP ออออออออออออออออออออออออออออออป $'
   hlpst2  db ' บ CTRL_>    Busca proximo espaco a direita      $'
   hlpst3  db ' บ CTRL_<    Busca proximo espaco a esquerda    ฑ $'
   hlpst4  db ' บ CTRL Y    Apaga linha                        ฑ $'
   hlpst5  db ' บ CTRL+END  Apaga do ponto ao fim da linha     ฑ $'
   hlpst6  db ' บ CTRL+HOME Apaga do ponto ao inicio da linha  ฑ $'
   hlpst7  db ' บ PAGEUP    Sobe uma pagina                    ฑ $'
   hlpst8  db ' บ PAGEDOWN  Desce uma pagina                    $'
   hlpst9  db ' ศออออออออออออออออออออออออออออออออออออออออออออออผ $'
   
   about1  db ' ษออออออออออออออออออออออออออออออออออออออออออออออป $'
   about2  db ' บ LAPRO III - Trab 1            ASSEMBLY 80386 บ $'
   about3  db ' บ                                              บ $'
   about4  db ' บ ษอออออออออออออออออออออป    ÛÛÛÛÛÛ ฑฑฑฑฑฑ     บ $'
   about5  db ' บ บ                     บ    Û      ฑฑ    ฑ    บ $'
   about6  db ' บ บ Alessander Oliveira บ    ÛÛÛ    ฑฑ    ฑ    บ $'
   about7  db ' บ บ             (Aless) บ    Û      ฑฑ    ฑ    บ $'
   about8  db ' บ บ                     บ    ÛÛÛÛÛÛ ฑฑฑฑฑฑ     บ $'
   about9  db ' บ ศอออออออออออออออออออออผ                      บ $'   
   about10 db ' ศออออออออออออออออออออออออออออออออออออออออออออออผ $'
  
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
