#define pinPot A0     // constantes
#define Ts 50
#define alfa 0.005        // parâmetro do filtro recursivo
#define tam_buffer 300    // parâmetro do filtro média móvel
#define amp_Ruido 500     // em millivolt

float tensao;         // variáveis
float sinal;
float sinal_fr = 0.0;
float sinal_mm = 0.0;
unsigned long tempoAtual;
int i = 0;
float buffer[tam_buffer];
float soma;


// =============================================
void setup() {
  Serial.begin(115200);   // abre a serial
  tempoAtual = millis();  // inicia o tempo atual  
}
// =============================================
void loop() {  
  if ((millis() - tempoAtual) >= Ts){
    tempoAtual = millis();          // atualiza o tempo atual
    // a partir daqui, colocar os procedimentos e funções
    tensao = lerTensao();         // ler a tensão de entrada
    sinal = geraRuido(tensao);    // gera um sinal ruidoso
    sinal_fr = filtroFr(sinal);   // filtra o sinal ruidoso por recursivo
    sinal_mm = filtroMm(sinal);   // filtra o sinal ruidoso por média móvel
    escreveSerial();              // escreve valor na serial
  }
}
// =============================================
float lerTensao(void){
  int entrada = analogRead(pinPot);
  return (entrada/1023.0)*5.0;  
}
// =============================================
float geraRuido(float tensao){
  float ruido = (random(-amp_Ruido,amp_Ruido))/1000.0;
  return tensao + ruido;
}
// =============================================
float filtroFr(float sinal){
    sinal_fr = (alfa*sinal)+((1.0-alfa)*sinal_fr);
    return sinal_fr;
}
// =============================================
float filtroMm(float sinal){
    soma = soma - buffer[i];  // passo 1
    soma = soma + sinal;      // passo 2
    buffer[i] = sinal;        // passo 3
    i++;                 // passo 4
    if (i >= tam_buffer){
      i = 0;
    }
    return soma/tam_buffer;   // retorno
}
// =============================================
void escreveSerial(void){
  Serial.print(tensao);
  Serial.print(" ");
  Serial.print(sinal);
  Serial.print(" ");
  Serial.print(sinal_fr);
  Serial.print(" ");
  Serial.println(sinal_mm);   
}
// =============================================





