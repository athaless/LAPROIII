#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

#define ATRIB 0x7900

unsigned _heaplen=1;
unsigned _stklen=256;
unsigned long int contador;
unsigned char vetor[8];
unsigned long int hora,minuto,segundo;
unsigned long int resto,ant;

unsigned off_velha,off_nova;
unsigned seg_tela;

void interrupt (*velha_int_08h)(...);
int count;

//---------------------------------------------------------------
void mostra_hora()
{
 for(int i=0;i<8;i++) poke(seg_tela,144+(i*2),vetor[i]|ATRIB);
}
//---------------------------------------------------------------
void bindec(unsigned char valor,char *vetor)
{
int aux;
aux=valor/10;
vetor[0]=aux + '0';
vetor[1]=(valor%10) + '0' ;
}

//---------------------------------------------------------------
void troca_cor_fundo()
{
unsigned seg_tela;
unsigned char *ptr,aux;

  if( (peek(0,0x0410) & 0x30) == 0x30) seg_tela=0xB000;
   else seg_tela=0xB800;
  ptr = (unsigned char *) MK_FP (seg_tela,0);
  ptr++;

  for (int cont=0; cont < 2000; cont ++)
    {
     aux = *ptr & 0x80;
     *ptr = *ptr + 0x10;
     if ((*ptr & 0x80) != aux)
	*ptr = *ptr ^ 0x81;
     ptr = ptr+2;

    }
}

//---------------------------------------------------------------
void interrupt int_08h(...)
{
 int num=0;

 (*velha_int_08h)(...);
 contador= *(unsigned long int far *)MK_FP(0,0x046C);
 hora=contador/65543;
 resto=contador%65543;
 minuto=resto/1092;
 resto%=1092;
 segundo=(resto*100)/1821;
 if(ant!=segundo)
  {
   ant=segundo;
   bindec(hora,(char *)vetor);
   bindec(minuto,(char *)vetor+3);
   bindec(segundo,(char *)vetor+6);
   mostra_hora();
   num++;
  }
}
//---------------------------------------------------------------
void main(int argc,char *argv[])
{
 int aux_num=0;

 if ((argc < 2) )
  { printf ("\n\n\t\t Formato: resid <tempo> \n\n");
    printf ("\n\n\t\t onde <op‡Æo> ‚:");
    printf ("\n\n\t\t         i --> para instalar");
    printf ("\n\n\t\t         d --> para desinstalar");
    exit(0);
  }

 //Verifica o tipo do monitor usado
 if( (peek(0,0x0410) & 0x30) == 0x30) seg_tela=0xB000;
 else seg_tela=0xB800;

 if ( !strcmp(argv[1],"i") )
 {
  //Guarda o end. da interrupcao que estiver na posicao 8 do vetor
  velha_int_08h = getvect(0x8);
  //Guarda os offsets das int's que estao sendo trabalhadas (velhas,nova)
  off_velha =  FP_OFF(velha_int_08h);
  off_nova  =  FP_OFF(int_08h);
  //Compara int's
  if(off_velha==off_nova)
   { printf("\n Programa ja instalado...\n"); printf("\a"); exit(1); }
  //Salva a velha no vetor de int's na posicao (For User Prograns) especificada
  setvect(0x60,velha_int_08h);
  //Pega o tempo que o programa deve ficar residente
  setvect(0x8,int_08h);               //Seta a int com a nova funcao
  keep(0,_SS + (_SP/16) - _psp + 10); //Fica residente
 }
 else
 {
  velha_int_08h =  getvect(0x60);
  off_velha     =  FP_OFF(velha_int_08h);

  if (off_velha==NULL)
   { exit(0); }
  else
   {
    setvect(0x60,(void*)MK_FP(0,0));
    setvect(0x8,velha_int_08h);
    //_dos_set_block(640,0,NULL);
    //_dos_freemem(0);
    free((void *)0x0158);
   }
 }

 exit(0);
}
