#include <dos.h>
#include <stdlib.h>

static unsigned off_velha,off_nova;

//Reduzindo o tamanho do HEAP e do STACK para fazer  o prog. menor em memoria
extern unsigned _heaplen = 1024;
extern unsigned _stklen  = 512;

void interrupt (*velha_int_08h)(...);

//---------------------------------------------------------------
void interrupt int_08h(...)
{
 poke(0xb800,rand()%32000,'A');

 (*velha_int_08h)(...);
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

 //Fica residente
 keep(0,_SS + (_SP/16) - _psp + 10);


 return;
}
