// =============================
// MINI EDITOR Ver 2.0 �  Trab-2          LAPRO III - Prof. Marcio Pinho
// =====================================================================
// by Alessander Oliveira
// =====================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

#pragma inline

#define READ  0
#define WRITE 1
#define MARCA 0x3

void compactacao();
void descompactacao();

int  open_file(char str[15],char modo);
void close_file(int indice);
void create_file(char str[15]);
void compacta();
void replica_marca();
void trata_marca();

unsigned char  buffer,leitor,cont='0',auxc,marca=MARCA,flag=1;
	 char  file_in[20],file_out[20];
	 long  cont_save=0;
	 long  cont_load=0;
	  int  pointer_arq1,pointer_arq2;

//Macros para trabalho com arquivos
//Obs.: Estas macros foram utilizadas no MINI EDITOR de textos ED

 asm {
  //Gravacao de um byte em arquivo [file] de uma variavel [buf]
  load_byte macro file,buf
      pusha
      mov ah,3Fh
      mov bx,file
      mov cx,1
      lea dx,buf
      int 21h
      mov flag,al
      popa
      inc cont_load //Utilizado para o calculo da porcentagem de compactacao
  endm
  //Leitura de um byte de um arquivo [file] para uma variavel [buf]
  save_byte macro file,buf
      pusha
      mov ah,40h
      mov bx,file
      mov cx,1
      lea dx,buf
      int 21h
      popa
      inc cont_save //Utilizado para o calculo da porcentagem de compactacao
  endm

  };


void main(int argc,char *argv[])
{
 int t=0;
 char c;
 float percent;

 strcpy(file_in ,argv[2]);
 strcpy(file_out,argv[2]);
 c=argv[1][0];

 if (argc==3)
  {
   while (file_out[t]!='.') t++;
   file_out[t]='\0';
   switch(c)
   {
    case 'c': strcat(file_out,".PAK");break;
    case 'd': strcat(file_out,".DDD");break;
   }
  }
 else
  strcpy(file_out,argv[3]);

 if (argc<2)
  {
   printf("\nENXUTA (r) SLOW! Create/Update Utility version  1.4");
   printf("\nCorp. Alessander Oliveira (Aless). All Rights Reserved. \nSHAREWARE VERSION");
   printf("\n\n ENXUTA [] for basic help");
   printf("\n\n\t Usage: ENXUTA   c    [file_input]   [{file_output}]");
   printf(  "\n\t                                              ");
   printf(  "\n\t        switches � file to pack �   file packed �");
   printf("\n\n\t Usage: ENXUTA   d    [file_input]   [{file_output}]");
   printf(  "\n\t                                              ");
   printf(  "\n\t        switches �  file packed �      new file �");
   printf("\n\n   The obove usage is only a very basic example of ENXUTA's capability");
   printf("\n\n   ENXUTA is NOT FOR BUSINESS, COMMERCIALS, or GOVERNMENT USE.\n");
   exit(0);
  }
 else
 {
  switch(c)
  {
   case 'c' : compactacao();
	      asm dec cont_load;
	      printf("\n\n\tCompactacao concluida !  %s -> %s\n\n",file_in,file_out);
	      if (cont_save >= cont_load)
	       { printf("\n\n\tCompactacao nao foi possivel! \n\n");
	      remove(file_out); }
	      else
	       {
		percent = (long)( (100*cont_save)/cont_load );
		percent = 100-percent;
		printf("\n\tO arquivo foi compactado em (+/-) %d%\n\n",(int)percent);
	       }
	      exit(0);

   case 'd' : descompactacao();
	      printf("\n\n\tDescompactacao concluida ! %s -> %s\n\n",file_in,file_out);
	      exit(0);

   default  : printf("\n\n\t\tParametro Invalido !\n\n");
	      exit(0);
  }
 }
}

void compactacao()
{
 pointer_arq1=open_file(file_in,READ);
  if (pointer_arq1==2)
   { printf("\n\n\t\t Cannot open input file.\n"); exit(0); }
 create_file(file_out);
 pointer_arq2=open_file(file_out,WRITE);

 printf("\nARQUIVO DE ENTRADA [%s]-byte em analise > ",file_in);

 asm load_byte pointer_arq1,buffer
 asm jmp principal

 deve_compactar:              //Quando encontra uma sequencia de bytes
    compacta();               //de numero maior que 3, deve compactar
    asm  jmp atualiza_buffer  //esta sequencia

 duplicar_marca:              //Quando encontra um caracter que e' igual
    replica_marca();          //a nossa marca de compactacao com um Nro
    asm  jmp atualiza_buffer  //menor que 4 repeticoes, deve duplica-lo

 repassa_byte:
    asm  inc cx                          //Enquanto nao ha repeticao
    asm  xor bx,bx                       //apenas repassa o byte lido
    laco1:                               //
      asm  save_byte pointer_arq2,buffer //
      asm  inc bx                        //
      asm  cmp bx,cx                     //
      asm  jne laco1                     //
      asm  jmp atualiza_buffer           //


 //Rotina de leitura principal

 principal:
  asm xor cx,cx
  repete1:

   asm pusha           // Apenas para exibir
   asm mov ah,10       // o byte que estiver
   asm mov al,buffer   // sendo analizado
   asm mov cx,1        //
   asm mov bh,0        //
   asm int 10h         //
   asm popa            //

    asm {
	 load_byte pointer_arq1,leitor;
	 cmp flag,0
	 je  passo
	 mov al,buffer
	 cmp leitor,al
	 jne passo
	 cmp cx,254      //Isto e' que estava errado na versao ISADORA
	 je  passo,254   //porque estava com o valor 255
	 inc cx
	 jmp repete1
	};
  passo:
    asm {
	 cmp cx,2          //Verifica se o Nro de repeticoes e' inferior a 4
	 jg deve_compactar //
	 push ax
	 mov al,marca
	 cmp buffer,al
	 pop ax
	 je duplicar_marca
	 jmp repassa_byte
	};
  atualiza_buffer:
    asm {
	 mov al,leitor
	 mov buffer,al
	 cmp flag,0
	 je  fim1
	 jmp principal
	};
  fim1:

  close_file(pointer_arq1);
  close_file(pointer_arq2);
}



void descompactacao()
{
  pointer_arq1=open_file(file_in,READ);
   if (pointer_arq1==2)
    { printf("\n\n\t\t Cannot open input file.\n"); exit(0); }
  create_file(file_out);
  pointer_arq2=open_file(file_out,WRITE);
  printf("\nARQUIVO DE ENTRADA [%s]-byte em analise > ",file_in);

  asm jmp repete2

  trata_marca:
    trata_marca();
    asm jmp repete2

  repete2 :

    asm load_byte pointer_arq1,buffer

    asm pusha           // Apenas para exibir
    asm mov ah,10       // o byte que estiver
    asm mov al,buffer   // sendo analizado
    asm mov cx,1        //
    asm mov bh,0        //
    asm int 10h         //
    asm popa            //

    asm cmp flag,0
    asm je fim2
    asm mov al,marca
    asm cmp buffer,al
    asm je trata_marca
    asm save_byte pointer_arq2,buffer
    asm jmp repete2

  fim2:
    close_file(pointer_arq1);
    close_file(pointer_arq2);
}


void create_file(char str[15])
{
 asm mov ah,03Ch
 asm mov cx,00h
 asm mov dx,offset str
 asm int 21h
}


int open_file(char str[15],char modo)
{
 int aux=0;

 asm mov al,modo
 asm mov ah,03Dh
 asm mov dx,offset str
 asm int 21h;
 asm mov aux,ax

 return aux;
}

void close_file(int indice)
{
 asm mov ah,03Eh
 asm mov bx,indice
 asm int 21h
}


void compacta()
{
 asm inc cx
 asm save_byte pointer_arq2,marca   // Grava a marca de compactacao
 asm mov auxc,cl
 asm save_byte pointer_arq2,auxc    // Grava o numero de vezes a rapetir um caracter
 asm save_byte pointer_arq2,buffer  // Grava o caracter que devera ser repetido na descompactacao
}

void replica_marca()
{
 asm  inc cx
 asm  xor bx,bx
 laco2:
   asm save_byte pointer_arq2,buffer //Grava a marca duas vezs
   asm save_byte pointer_arq2,buffer //
   asm inc bx
   asm cmp bx,cx
   asm jne laco2
}

void trata_marca()
{
 asm load_byte pointer_arq1,leitor
 asm mov al,leitor
 asm cmp buffer,al
 asm jne descompacta
 asm save_byte pointer_arq2,buffer
 return;
 descompacta:
   asm mov cl,leitor;
   asm load_byte pointer_arq1,leitor
   asm xor bx,bx
   laco3:
     asm save_byte pointer_arq2,leitor
     asm inc bx
     asm cmp bx,cx
     asm jne laco3
   return;
}