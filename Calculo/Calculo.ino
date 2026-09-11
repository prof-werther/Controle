#define pinPot A0     // constantes
#define Ts 500.00        // tempo de processamento

int entrada;          // sinal lido     
float sinal;          // sinal de entrada
float is = 0.0;       // inicializa a integral;
float ds;             // derivada do sinal
float sa;             // sina anterior
   
unsigned long tempoAtual;
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
    sinal = lerSinal();         // ler a sinal de entrada (-10 a +10)
    ds = calculaDerivada();
    is = calculaIntegral();
    escreveSerial();            // escreve valor na serial
    sa = sinal;
  }
}
// =============================================
float lerSinal(void){
  entrada = analogRead(pinPot);
  return ((20.0*(entrada/1023.0))-10.0);  
}
// =============================================
float calculaDerivada(){
  return (sinal-sa)/(Ts/1000.0);
}
// =============================================
float calculaIntegral(){
  return is + (((sinal+sa)/2.0)*(Ts/1000.0));
}
// =============================================
void escreveSerial(void){
  Serial.print(sinal);
  Serial.print(" "); 
  Serial.print(ds);   
  Serial.print(" "); 
  Serial.println(is);   
}
// =============================================





