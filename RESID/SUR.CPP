#include <dos.h>

/* ---------------------------------------------
   NOTA:
   Este e' o prototipo que servira' de base para
   o programa residente para SUR.(ATUALMENTE-5K)
			 by Alessander Oliveira (Aless)
   --------------------------------------------- */

static unsigned char vetor[8];//Exclusivamente pra exibicao
static unsigned long int contador=0;              //\
static unsigned long int hora,minuto,segundo=0;   // Para calcular o tempo
static unsigned long int resto,ant,ocioso,tempo=0;///

static unsigned off_velha,
		off_nova;

//Reduzindo o tamanho do HEAP e do STACK para fazer  o prog. menor em memoria
extern unsigned _heaplen = 1024;
extern unsigned _stklen  = 512;

void interrupt (*velha_int_08h)(...);
void interrupt (*velha_int_09h)(...);


//---------------------------------------------------------------
void bindec(unsigned char valor,char *vetor)
{
 int aux=valor/10;
 vetor[0]=aux + '0';
 vetor[1]=(valor%10) + '0' ;
}

//---------------------------------------------------------------

void beep(int freq,int temp)
{
 asm pusha

 _BX=freq;

 asm {
      mov   ax,34DDH
      mov   dx,0012H
      cmp   dx,bx
      jnb   cancela
      div   bx
      mov   bx,ax
      in    al,61H
      test  al,03H
      jnz   emite
      or    al,03H
      out   61H,al
      mov   al,0B6H
      out   43H,al
     };

emite:

 asm {
      mov  al,bl
      out  42H,al
      mov  al,bh
      out  42H,al
     };

cancela:

 delay(temp);

 //Cancela a emissao do som
 asm {
      in   al,61H
      and  al,0FCH
      out  61H,al
     };

 asm popa
}

//---------------------------------------------------------------
void interrupt int_08h(...)
{
 contador= *(unsigned long int far *)MK_FP(0,0x046C);
 hora    =contador/65543;
 resto   =contador%65543;
 minuto  =resto/1092;
 resto   %=1092;
 segundo =(resto*100)/1821;

 if(ant!=segundo)
  {
   if (segundo==0) ocioso++;
   if (ocioso>4) { ocioso=0; tempo++; beep(440,20); }

   ant=segundo;
   bindec(hora   ,(char *)vetor);
   bindec(minuto ,(char *)vetor+3);
   bindec(segundo,(char *)vetor+6);
   //Mostra a hora
   for(int i=0;i<8;i++) poke(0xb800,144+(i*2),vetor[i]|0x7900);
   //Mostra ocioso e tempo respectivamente
   poke(0xb800,124,ocioso + '0' |0x7900);
   poke(0xb800,132,tempo  + '0' |0x7900);
  }

 (*velha_int_08h)(...);
}

//---------------------------------------------------------------
void interrupt int_09h(...)
{
 asm {
      in   al,60h   // Tecla pressionada ?
      test al,080h
      jnz  sai      // nao, entao foi liberada, nao faca nada
     };
     ocioso=0;      // sim, reseta 'ocioso'
		    //beep(440,20);//Apenas um teste para testar esta nova int
sai:

 (*velha_int_09h)(...);
}



//---------------------------------------------------------------
//---------------------------------------------------------------
void main()
{
//Substituicao da int de relogio
 //Guarda o end. da interrupcao que estiver na posicao 8 do vetor
 velha_int_08h = getvect(0x8);
 //Guarda os offsets das int's que estao sendo trabalhadas (velhas,nova)
 off_velha =  FP_OFF(velha_int_08h);
 off_nova  =  FP_OFF(int_08h);
 //Salva a velha no vetor de int's na posicao (For User Prograns) especificada
 setvect(0x60,velha_int_08h);
 //Seta a int com a nova funcao
 setvect(0x8,int_08h);

//Substituicao da int de teclado
 velha_int_09h = getvect(0x09);
 off_velha =  FP_OFF(velha_int_09h);
 off_nova  =  FP_OFF(int_09h);
 setvect(0x63,velha_int_09h);
 setvect(0x09,int_09h);

 //Fica residente
 keep(0,_SS + (_SP/16) - _psp + 10);


 return;
}
