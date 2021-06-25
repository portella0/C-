//Matheus Portella e Thiago Rodrigues

enum outputs {entrada=5, saida=7, deslocamento};

void shiftRegister(int entrada, int deslocamento, byte numero);

byte numero = 0;
byte lista_numeros[10] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 255};

void setup()
{
  for(int i = 7; i<9; i++)pinMode(i, OUTPUT);
  for(int i = 7; i<9; i++)digitalWrite(i, 0);
  pinMode(entrada, OUTPUT);
  digitalWrite(entrada, 0);
}

void loop()
{
	for (int i = 0; i < 10; i++) 
    {
      //pega um numero da lista  
      numero = lista_numeros[i];
      //desliga a saída
      digitalWrite(saida, 0);
      //funcao do shift register
      shiftRegister(entrada, deslocamento, numero);
      //transmite os dados no registrador para a saída
      digitalWrite(saida, 1);
      delay(1000);
    }  
}

void shiftRegister(int entrada, int deslocamento, byte numero) 
{
  //variavel que controla se é 0 ou 1 
  //o bit que vai para o registrador
  int estado;

  //zera tudo
  digitalWrite(entrada, 0);
  digitalWrite(deslocamento, 0);

  //função q passa por cada bit do número
  for (int i=7; i>=0; i--)  
  {  
    digitalWrite(deslocamento, 0);
	
    //se o bit na posição indicada pelo valor de i
    //for 1 então estado = 1, se não estado = 0
    //Ex: 4 = 00000100,
    //i = 1 -> 00000100 & 00000010 -> estado = 0
    //i = 2 -> 00000100 & 00000100 -> estado = 1
    (numero & (1<<i)) ? estado= 1: estado = 0;

    //coloca em 1 ou 0 o bit q será passado para o registrador
    digitalWrite(entrada, estado);
    delay(10);
    //faz shift no bit para o registrador
    digitalWrite(deslocamento, 1);
    //coloca o bit em zero
    digitalWrite(entrada, 0);  
  }
  //para de dar shift nos bits
  digitalWrite(deslocamento, 0);
}




