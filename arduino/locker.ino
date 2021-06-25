#define COMM LOW
#define DATA HIGH

//estados programa
// 0 - livre
// 1 - cadastrando senha
// 2 - fechado
int estado = 0;
//controla o cancelamento pelo botão * sendo apertado duas vezes
int reset = 0;
//controla a confirmação da senha
int confirm = 0;
//verifica o tamanho da senha
int tamanho = 0;
//pinos de manipulação  do  LCD lcd
int pinos[4];
//entrada que controla se é um dado ou comando
int LCDRS;
//controla o armazenamento  do  dado no  LCD
int LCDEN;
//número de colunas do display
int nColunas = 16;
//número de linhas do display
int nLinhas = 2;
//mensagen que serão escritas
char* mensagem1 = "";
//posição do cursor nas linhas e colunas
int println;
int printcl;
//controla se a mensagem já foi escrita
bool textoEscrito = false;
bool textoEscrito2 = false;
//informa se as senhas estão iguais
bool iguais = true;
//controla se o locker está em uso
bool emUso = false;
//controla se a senha já foi digitada uma vez
bool senhaEscrita;
//controla mensagem que avisa para digitar a senha
bool mensagemAbrir = false;
//impede que seja escrita a tecla pressionada antes do momento
//que a senha deve ser escrita
bool avancou = false;
bool avancou2 = false;
//pinos das linas
int linhas[] = {13,12,11,10};
//pinos das colunas
int colunas[] = {9,8,7,6};
//array 2D com as teclas do teclado 4x4
char texto[4][4] = {{'1','2','3','A'},
				  {'4','5','6','B'},
				  {'7','8','9','C'},
				  {'*','0','#','D'}};
//armazena as senhas
char senha1[6], senha2[6], senhaComp[6], senhaAbrir[6];

void setup()
{
  //incializa o LCD
  LCDBegin(16, 2, A4, A5, A0, A1, A2, A3);
  delay(1000);
  
  //configura os pinos das linhas como OUTPUT 
  //e os das colunas como INPUT
  //e coloca todas em LOW
  for(int i = 0; i <4;i++)
  {
  	pinMode(linhas[i], OUTPUT);  
    pinMode(colunas[i], INPUT);
    digitalWrite(linhas[i], LOW);
    digitalWrite(colunas[i], LOW);
  }
  Serial.begin(9600);
  
  //inicia o display no estado 0 com essa mensagem
  mensagem1 = "Livre para uso"; //salva a mensagem
  println = 0; //linha 
  printcl = 0; //coluna 0
  LCDSetCursor(printcl,println); //move o cursor para a linha/coluna indicada
  LCDPrint(mensagem1);//escreve a mensagem
}

void loop()
{  
  //função que controla a leitura do teclado
  digitar();  
  if(estado == 0)
  {	
    emUso = false;
    senhaEscrita = false;
    iguais = true;
    //zera todas os arrays que armazenam as senhas
    for (int i = 0;  i < 6; i++)
    {
      senha1[i] = '0';
      senha2[i] = '0';
      senhaAbrir[i] = '0';
      senhaComp[i] = '0';
    }
    //faz isto apenas uma vez quando o display muda para o estado 0
    if(textoEscrito == false)
    {
      LCDCommand(1); //apaga o display
  	  mensagem1 = "Livre para uso";  
  	  println = 0;
      printcl = 0;
      LCDSetCursor(printcl,println);
  	  LCDPrint(mensagem1);
      textoEscrito = true;
      iguais = true;
      tamanho = 0;
      avancou = false;
    }
  }
  else if(estado == 1)
  {
    textoEscrito = false;
  }
  else if(estado == 2)
  {
  	textoEscrito = false;
    //faz isto apenas uma vez quando o display muda para o estado 2
	if(textoEscrito2 == false)
    {
      tamanho = 0;
      mensagemAbrir = false;
      LCDCommand(1);
  	  mensagem1 = "Locker em uso";  
  	  println = 0;
      printcl = 0;
      LCDSetCursor(printcl,println);
  	  LCDPrint(mensagem1);
      textoEscrito2 = true;
      avancou2 = false;
      //zera o array da senha digitada
      for (int i = 0;  i < 6; i++)
      {      
        senhaAbrir[i] = '0';
      }
    }
  }
}

void digitar()
{
  //for que passa por cada linha
  for(int i = 0; i <4;i++)
  {
    //coloca todas as linhas em LOW
    for(int j = 0; j <4;j++)
    {
      digitalWrite(linhas[j], LOW);
    }
    //coloca a linha indicada pelo valor do for em HIGH
    digitalWrite(linhas[i], HIGH);
    
    //for que passa por cada coluna
    for (int j = 0; j <4;j++)
    {
      //se a coluna ficar em high
      if (digitalRead(colunas[j]) == HIGH)
      {
        //escreve o texto do array 2D indicado peloa posição
        //da linha e da coluna
        if(texto[i][j] != '#' && texto[i][j] != '*')
        {
          //caso tenha acabado de sair do estado 0
          //apaga tudo e move o curso para o inicio do display
          if(estado == 0)
    	  {
            LCDCommand(1);            
            println = 0;
     	    printcl = 0;
     	    LCDSetCursor(printcl,println);
            mensagem1 = " ";
    	  }
          //se o locker não estiver em uso
          if(emUso == false )
          {
            //muda para o estado 1
            estado = 1;
            //escreve uma senha de no maximo 6 digitos
            if(printcl < 6 && avancou == true)
            {	           
              reset = 0;
              //se estiver escrevendo pela primeira vez
              if(senhaEscrita == false)
              {
                senha1[printcl] = texto[i][j];//salva a senha
                tamanho++;//salva o tamanho da senha
              }
              //repetindo a senha
              else
              {
                senha2[printcl] = texto[i][j];//salva a copia da senha  
                tamanho++;
              }
              *mensagem1 = texto[i][j];//salva os digitos da senha
              LCDPrint(mensagem1);//escreve digitos no display 
              printcl++;//avança uma coluna
              LCDSetCursor(printcl,println);
            }
          }
          //se o locker  estiver em uso
          else if(emUso == true)
          {
            //Exibe apenas uma vez uma mensagem
            //indicando para digitar a senha
            if(mensagemAbrir == false)
            {
              //apaga o display e escreve
      		  //"Digite a senha:"
              LCDCommand(1);            
              println = 0;
              printcl = 0;
              LCDSetCursor(printcl,println);
              mensagem1 = "Digite a senha:";
              LCDCommand(1);            
              LCDPrint(mensagem1);
              println = 1;
              printcl = 0;
              LCDSetCursor(printcl,println);
              mensagem1 = " ";
              mensagemAbrir = true;
            }
            //se a senha for menor que 6
            if(printcl < 6 && avancou2 == true)
            {	           
              reset = 0;
              senhaAbrir[printcl] = texto[i][j];
              tamanho++;
              *mensagem1 = texto[i][j];
              LCDPrint(mensagem1);
              printcl++;
              LCDSetCursor(printcl,println);
            }
            avancou2 = true;
          }
          avancou = true;
        }
        
        //se # foi pressionado, não está no estado 0
        //e a senha tem no minimo 2 digitos
        if(texto[i][j] == '#' && estado != 0 && tamanho >= 2)
        {
          //se está no estado 1
          if(estado == 1)
          {
            //compara a senha escrita com a repetida na segunda linha
            comparaEstado1();
          }
          //se está no estado 2
          else if (estado == 2 && avancou2 == true)
          {
            //compara a senha escrita com a salva no locker em uso
            comparaEstado2();           
          }
          //Exibe apenas uma vez uma mensagem
          //indicando para digitar a senha
          else if (avancou2 == false)
          {
              //apaga o display e escreve
      		  //"Digite a senha:"
              LCDCommand(1);            
              println = 0;
              printcl = 0;
              LCDSetCursor(printcl,println);
              mensagem1 = "Digite a senha:";
              LCDCommand(1);            
              LCDPrint(mensagem1);
              println = 1;
              printcl = 0;
              LCDSetCursor(printcl,println);
              mensagem1 = " ";
              avancou2 = true;
          }
        }
        
        //se * for pressionado e não estiver no estado 0
        if(texto[i][j] == '*' && estado != 0)
        {
          //apaga tudo
          cancela();
          avancou2 = true;
        }
        //fica em delay até o botão pressionado ser solto
        while(digitalRead(colunas[j])==HIGH)
        {
          delay(100);
        }
      }     
    }
  }
  delay(10); 
}

//compara a senha escrita com a repetida na segunda linha
void comparaEstado1()
{
  tamanho = 0;
  confirm++;            
  senhaEscrita = true;
  println = 1;
  printcl = 0;
  LCDSetCursor(printcl,println);
  //caso # tenha side apertado pela segunda vez
  //para que a senha seja cadastrada
  if(confirm == 2)
  {
    confirm = 0;
    //compara ambas as senhas 
    for (int i = 0;  i < 6; i++)
    {
      if(senha1[i] != senha2[i])
      {
        iguais = false;
      }
    }
    //caso as senhas sejam iguais
    if(iguais == true)
    {
      emUso = true; //locker está em uso
      //apaga o display e escreve
      //"senha cadastrada com sucesso"
      LCDCommand(1);
      println = 0;
      printcl = 0;
      LCDSetCursor(printcl,println);
      mensagem1 = "Senha cadastrada";
      LCDPrint(mensagem1);
      println = 1;
      printcl = 0;
      LCDSetCursor(printcl,println);
      mensagem1 = "com sucesso";
      LCDPrint(mensagem1);
      estado = 2;
      //salva a senha num array
      for (int i = 0;  i < 6; i++)
      {
        senhaComp[i] = senha1[i];
      }
      textoEscrito2 = false;
      delay(2000);
    }
    //caso as senhas sejam diferentes
    else if(iguais == false)
    {
      //apaga o display e escreve
      //"senhas diferentes"
      LCDCommand(1);
      println = 0;
      printcl = 0;
      LCDSetCursor(printcl,println);
      mensagem1 = "Senhas";
      LCDPrint(mensagem1);
      println = 1;
      printcl = 0;
      LCDSetCursor(printcl,println);
      mensagem1 = "diferentes";
      LCDPrint(mensagem1);
      textoEscrito = false;
      estado = 0;//retorna para o estado 0
      delay(2000);
    }
  }
}

//compara a senha escrita com a salva no locker em uso
void comparaEstado2()
{
  iguais = true;
  //compara a senha salva com a digitada
  for (int i = 0;  i < 6; i++)
  {
  	if(senhaAbrir[i] != senhaComp[i])
  	{
        iguais = false;
 	}
  }
  //caso ambas sejam iguais
  if(iguais == true)
    {
      emUso = false;//locker não está mais em uso
      //apaga o display e escreve
      //"senha correta"
      LCDCommand(1);
      println = 0;
      printcl = 0;
      LCDSetCursor(printcl,println);
      mensagem1 = "Senha correta";
      LCDPrint(mensagem1);
      estado = 0;
      delay(2000);
    }
    //caso ambas sejam iguais
    else if(iguais == false)
    {
      //apaga o display e escreve
      //"senha incorreta
      LCDCommand(1);
      println = 0;
      printcl = 0;
      LCDSetCursor(printcl,println);
      mensagem1 = "Senha incorreta";
      LCDPrint(mensagem1);
      estado = 2;  
      textoEscrito2 = false;
      delay(2000);
    }
}

//apaga o que foi escrito no display
void cancela()
{  
  //se o locker não estiver em uso
  if(emUso == false)
  {
    reset++;
    confirm = 0;
    senhaEscrita = false;
    //apaga tudo e move o cursor para o inicio
    LCDCommand(1);
    println = 0;
    printcl = 0;
    LCDSetCursor(printcl,println);
    //se for pressionado uma segunda vez retorna para o estado 0
    if(reset == 2)
    {        
      estado = 0;      
      reset = 0;              
      textoEscrito = false;        
    }  
    for (int i = 0;  i < 6; i++)
    {
      senha1[i] = '0';
      senha2[i] = '0';     
    }
  }
  //se o locker estiver em uso
  else if(emUso == true)
  {
    tamanho = 0;
    reset++; 
    //apaga tudo 
    //escreve "digite a senha:"
    //e move o cursor para a segunda linha
    LCDCommand(1);            
    println = 0;
    printcl = 0;
    LCDSetCursor(printcl,println);
    mensagem1 = "Digite a senha:";
    LCDCommand(1);            
    LCDPrint(mensagem1);
    println = 1;
    printcl = 0;
    LCDSetCursor(printcl,println);
    mensagem1 = " "; 
    //zera o array da senha digitada
    for (int i = 0;  i < 6; i++)
    {      
      senhaAbrir[i] = '0';
    }
    //se for pressionado duas vezes
    //retorna para a mensagem "locker em uso"
    if(reset == 2)
    {  
      textoEscrito2 = false;        
      estado = 2;      
      reset = 0;              
    }  
  }
}

//incializa o display
void LCDBegin(int colunas, int linhas, int pRS, int pEN, int p0, int p1, int p2, int p3)
{
  delay(150);
  nColunas = colunas;
  nLinhas = linhas;
  //registro dos pinos
  LCDEN = pEN;
  LCDRS = pRS;
  
  //pinos
  pinos[0] = p0;
  pinos[1] = p1;
  pinos[2] = p2;
  pinos[3] = p3;
  
  //processos necessarios pra inicialização do LCD
  LCDPinoConfig();
  LCDStart();
  LCDConfig();
}

//inicializa os pinos
void LCDPinoConfig()
{
  pinMode(LCDRS, OUTPUT);
  pinMode(LCDEN, OUTPUT);
  for(int i = 0; i<4;i++)
  {  
    pinMode(pinos[i], OUTPUT);
  } 
}

void LCDStart()
{
  //esperar 15ms
  digitalWrite(LCDRS, LOW);
  digitalWrite(LCDEN, LOW);
  delay(50); //40ms por precaução
  
  //envia 0011
  LCDSend(3, COMM);
  delay(5);
  
  //envia 0011
  LCDSend(3, COMM);
  delayMicroseconds(150);
  
  //envia 0011
  LCDSend(3, COMM);
  
  //envia 0010
  LCDSend(2, COMM);
}

//função que envia dados para o LCD
void LCDSend(int val, int operacao)
{
  //preparação do dado
  digitalWrite(pinos[0], val&1);
  digitalWrite(pinos[1], (val>>1)&1);
  digitalWrite(pinos[2], (val>>2)&1);
  digitalWrite(pinos[3], (val>>3)&1);
  
  //tratamento do pulso
  digitalWrite(LCDEN, LOW);
  
  digitalWrite(LCDRS, operacao);
  delayMicroseconds(1);
  
  //gera pulso EN
  digitalWrite(LCDEN, HIGH);
  delayMicroseconds(1);
  
  digitalWrite(LCDEN, LOW);
  delayMicroseconds(100);
}

void LCDConfig()
{
  //configuração do LCD
  LCDSend(2, COMM);
  LCDSend(8, COMM);
  
  //configuração cursor
  LCDSend(0, COMM);
  LCDSend(15, COMM);
  
  //direção e scroll
  LCDSend(0, COMM);
  LCDSend(6, COMM);
  
  //limpeza
  LCDSend(0, COMM);
  LCDSend(1, COMM);
}

//função que escreve no LCD
void LCDPrint(char* texto)
{
  for(int i = 0; i<strlen(texto); i++)
  {
    LCDSend((texto[i]>>4)&0xF, DATA); 
    LCDSend((texto[i])&0xF, DATA); 
  }
}

//função que posiciona o cursor no LCD
void LCDSetCursor(int coluna, int linha)
{	
  int ln, cl, val;
  if(linha == 0)
  {
    ln = 8; //1000
  }			 
  else
  {
    ln = 12; //1100
  }
  
  ln = (ln << 4); //move os 4 bits para esquerda
  
  cl = coluna & B01111; //só os 4 bits menos significativos passados por coluna são mantidos
  
  val = ln | cl;
  
  LCDCommand(val); //comando que posiciona o cursor
}

//função que executa comandos no LCD
void LCDCommand(int val)
{
   LCDSend((val>>4)&0XF, COMM); 
   LCDSend(val&0xF, COMM); 
}
