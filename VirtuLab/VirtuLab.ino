// ===========================================================================
// IFSC - Câmpus Araranguá
// LABORATÓRIO DE CONTROLE VIRTUAL
// Prof. Dr. Werther Serralheiro
//
// Configuração da Planta (G) 
// Configuração do Controlador (C)
// Mostra os parâmetros configurados (H)
// Mostra os parâmetros do Controlador (P)
// Execução em Malha Aberta (A)
// Execução em Malha Aberta (F)
// Zera variáveis (Z)
//
// ===========================================================================



#include <EEPROM.h>
#include <math.h>


unsigned long tempo_anterior;

int addr_Ke = 0;      // endereço EEPROM para Ke
int addr_tau = 10;    // endereço EEPROM para tau
int addr_Ts = 20;     // endereço EEPROM para Ts

int Ts;               // Ts
float u = 0.0;        // u entrada
float y = 0.0;        // y saída
float yp = 0.0;       // y processo
float ya = 0.0;       // y anterior
float yr = 0.0;       // y referência
float e = 0.0;        // erro
float e_a;            // erro anterior
float alfa;           // constante do filtro
float Ke;             // Ganho Estático
float tau;            // Tempo de Resposta
float Kp = 0.0;       // ganho proporcional
float Ki = 0.0;       // ganho integral
float Kd = 0.0;       // ganho derivativo
float up;             // sinal de controle proporcional
float ui;             // sinal de controle integral
float ud;             // sinal de controle derivativo
float ie;             // integral do erro
float de;             // derivada do erro


char comando;
float par1, par2, par3;
bool malha_fechada;

// ===========================================================

void setup() {
  Serial.begin(115200);  
  tempo_anterior = millis(); 
  EEPROM.get(addr_Ke, Ke);
  EEPROM.get(addr_tau, tau);
  EEPROM.get(addr_Ts, Ts);
  alfa = pow(M_E, -1.0*((Ts/1000.0)/tau));
  Serial.println("=============================");
  Serial.println("Carregando o processo");
  Serial.println("=============================");
  malha_fechada = false;
  delay(1000);  
}

// ===========================================================

void loop() { 
  if ((millis() - tempo_anterior) >= Ts) {
    tempo_anterior = millis(); 
  // coisas que queremos executar a cada Ts  
    if (malha_fechada) executa_controle();
    executa_processo();
    imprime_na_serial();   
  } 
  if (Serial.available() >= 2){
    comando = Serial.read();
    par1 = Serial.parseFloat();
    par2 = Serial.parseFloat();
    par3 = Serial.parseFloat();
    processa_comando();
  }
}
// ===========================================================
// Procedimentos extras
// ===========================================================

void processa_comando(){
  switch (comando) {
    // ========================= Configuração da Simulação (T)
      case 'T':
        Ts = par1;
        EEPROM.put(addr_Ts, Ts);
        Serial.println("=============================");
        Serial.println("Configurando a simulação:");
        Serial.println("=============================");
        Serial.print("Ts = ");
        Serial.println(Ts);
        delay(5000);
        break;   

    // ========================= Configuração da Planta (G)
      case 'G':
        Ke = par1;
        tau = par2;
        alfa = pow(M_E, -((Ts/1000.0)/tau));
        EEPROM.put(addr_Ke, Ke);
        EEPROM.put(addr_tau, tau);
        Serial.println("=============================");
        Serial.println("Configurando a planta:");
        Serial.println("=============================");
        Serial.print("Ke = ");
        Serial.println(Ke);
        Serial.print("Tau = ");
        Serial.println(tau);
        Serial.print("Alfa = ");
        Serial.println(alfa); 
        Serial.println("=============================");
        delay(5000);
        break;

    // ========================= Configuração do Controlador (C)
     case 'C':    
        Kp = par1;
        Ki = par2;
        Kd = par3;
        Serial.println("=============================");
        Serial.println("Configurando o controlador:");
        Serial.println("=============================");
        Serial.print("Kp = ");
        Serial.println(Kp);
        Serial.print("Ki = ");
        Serial.println(Ki);
        Serial.print("Kd = ");
        Serial.println(Kd);
        Serial.println("=============================");
        delay(1000);
        break;

    // ========================= Mostra os parâmetros configurados (H)
      case 'H':
        Serial.println("=============================");
        Serial.println("Parâmetros do VirtuaLab");
        Serial.println("=============================");
        Serial.print("Ke = ");
        Serial.println(Ke);
        Serial.print("Tau = ");
        Serial.println(tau); 
        Serial.print("Alfa = ");
        Serial.println(alfa);
        Serial.print("Ts = ");
        Serial.println(Ts);
        Serial.println("=============================");
        delay(5000);
        break;

    // ========================= Mostra os parâmetros do Controlador (P)
      case 'P':
        Serial.println("=============================");
        Serial.println("Parâmetros do Controlador");
        Serial.println("=============================");
        Serial.print("Kp = ");
        Serial.println(Kp);
        Serial.print("Ki = ");
        Serial.println(Ki);
        Serial.print("Kd = ");
        Serial.println(Kd);
        Serial.println("=============================");
        delay(5000);    
        break;    

    // ========================= Execução em Malha Aberta (A)
      case 'A':     
        u = par1; 
        malha_fechada = false;     
        break;
   
    // ========================= Execução em Malha Fechada (F)
      case 'F':     
        yr = par1;  
        ie = 0.0;    
        malha_fechada = true;    
        break;

    // ========================= Zera variáveis (Z)
      case 'Z':     
        yr = 0.0;  
        ie = 0.0;    
        y = 0.0;  
        ya = 0.0;  
        u = 0.0;    
        e = 0.0;    
        malha_fechada = false;    
        break;    
    
    // =========================
      default:
        // statements
        break;
    }
}

// ===========================================================

void imprime_na_serial(){
  Serial.print(yr);  
  Serial.print(" ");
  Serial.print(u);  
  Serial.print(" ");
  Serial.println(y);  
}

// ===========================================================

void executa_processo(){
  yp = (((1-alfa)*u) + (alfa*ya));
  ya = yp;
  y = Ke * yp;
}

// ===========================================================

void executa_controle(){
  e = yr - y;
  ie = ((e+e_a)/2)*(Ts/1000.0) + ie;
  de = (e-e_a)/(Ts/1000.0);
  up = Kp * e;
  ui = Ki * ie;
  ud = Kd * de;
  e_a = e;
  u = up + ui + ud;
}
// ==========================================================