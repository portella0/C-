enum botoes {res_bot=10, start_stop_bot, mux_u, mux_d};

void desenha_display(unsigned char, unsigned int), iniciar_parar_reset(), contar();
int contagem(unsigned char);
void mux(unsigned int);

char numeros[10] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6};
unsigned int display, iniciar_parar, reset, botao, botao_anterior, contador;

unsigned long millisAtual, millisAnterior = 0;   

void setup()
{
  for(int k = 12; k<14; k++)pinMode(k, OUTPUT);
  for(int k = 12; k<14; k++)digitalWrite(k, HIGH);
  pinMode(res_bot, INPUT);
  pinMode(start_stop_bot, INPUT);
  for(int k = 2; k<10; k++)pinMode(k, OUTPUT);
  for(int k = 2; k<10; k++)digitalWrite(k, HIGH);
  display, reset, contador = 0;
  iniciar_parar = 1;
  botao_anterior = digitalRead(start_stop_bot);
}

void loop()
{
  millisAtual = millis();
  if (millisAtual - millisAnterior >= 1000 && iniciar_parar == 1) 
  {
    millisAnterior = millisAtual;
    (contador < 99) ? contador++ : contador = 0;
    Serial.println(contador);
  }
  reset = digitalRead(res_bot);
  botao = digitalRead(start_stop_bot);
  iniciar_parar_reset();
  contar();
}

void contar()
{
  for (int i = 0; i<2; i++)
  {
   desenha_display(numeros[contagem(i)], i);
   delay(20);
  }  
}

void desenha_display(unsigned char numero, unsigned int display)
{
  mux(display);
  for(int k = 0; k<8; k++)
  {
    (numero > 0x7F) ? digitalWrite(k+2,LOW) : digitalWrite(k+2,HIGH);
    numero <<= 1;   
  }
}

int contagem(unsigned char tipo)
{
  return (tipo == 0) ? (contador % 10): ((contador/10) % 10);
}

void iniciar_parar_reset()
{
  (botao == LOW && botao_anterior == HIGH) ? iniciar_parar = !iniciar_parar : iniciar_parar=iniciar_parar;  
  botao_anterior = digitalRead(start_stop_bot);

  (reset == LOW)?contador = 0:contador=contador;
}

void mux(unsigned int display)
{
  (display == 0) ? (digitalWrite(mux_u, LOW),digitalWrite(mux_d, HIGH)) : (digitalWrite(mux_u, HIGH),digitalWrite(mux_d, LOW));
}