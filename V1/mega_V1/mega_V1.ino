#include <LedControl.h>

// DEFINITION VARIABLES

int REV5 = 38;
int RP2 = 39;
int REV4 = 40;
int REVD = 41;
int REVB = 42;
int RP3 = 43;
int REV3 = 44;
int RP1 = 45;
int REVC = 46;
int REVSOL = 49;
int RConv = 48; // was 52

int EVB = 0;  // was EV1
int EVC = 0;  // was EV2
int EVD = 0;
int EV3 = 0;
int EV4 = 0;
int EV5 = 0;
int P1 = 0;
int P2 = 0;
int P3 = 0;
int EVSOL = 0;
int Conv = 0;

int A=0;
int B=0;
int C=0;
int D=0;
int E=0;
int H=0;
int F=0;
int G=0;
int O=0;
int S=0;
int Z=0;

int tempoS=0;

int DEBUG = 1;
int N=0;
int thresh = 400;
bool Blink = 0;

double enc_Brunal = 75; //85,66
double ECSb_High = 80; //60
double ECSb_Low = 40;
double ECSb = ECSb_High;
int tempoE = 0;
int tempoHEVD = 0;
int tempoHEV5 = 0;
int Brunal_mini = 65;
int tConv = 0;

double S_ABR, S_SECS, S_RBR, S_BTB2, S_BTB1, S_BECS, S_BTH, S_SOL_AMONT, S_SOL_AVAL;

float nA = 0;
float nB = 0;
float nC = 0;
float nD = 0;
float nE = 0;
float nF = 0;
float nG = 0;
float nH = 0;
float nS = 0;
float nZ = 0;
int nTotal = 0;

unsigned long debut,fin;

// AFFICHEURS
LedControl lc1 = LedControl(50,52,34,1);
LedControl lc2 = LedControl(50,52,35,1);

// FONCTIONS
double convertTC(double value){
  double temp = value * 5.0 / 1023.0;
  double elise = (temp - 1.18)/0.005;
  return elise;
}

float getSerial1Temp(){
  char inData[16] = "";int index = 0;
  while (Serial1.available()) {
    inData[index] = Serial1.read();index++;
  }
  return String(inData).toFloat();
}

float getSerial2Temp(){
  char inData[16] = "";int index = 0;
  while (Serial2.available()) {
    inData[index] = Serial2.read();index++;
  }
  return String(inData).toFloat();
}

float getSerial3Temp(){
  char inData[16] = "";int index = 0;
  while (Serial3.available()) {
    inData[index] = Serial3.read();index++;
  }
  return String(inData).toFloat();
}

void relais(){
  int t = 120;
  digitalWrite(REV5,!EV5);delay(t);
  digitalWrite(RP2,!P2);delay(t);
  digitalWrite(REV4,!EV4);delay(t);
  digitalWrite(REVD,!EVD);delay(t);
  digitalWrite(REVB,!EVB);delay(t);
  digitalWrite(RP3,!P3);delay(t);
  digitalWrite(REV3,!EV3);delay(t);
  digitalWrite(RP1,!P1);delay(t);
  digitalWrite(REVC,!EVC);delay(t);
  digitalWrite(REVSOL,!EVSOL);delay(t);
  digitalWrite(RConv,!Conv);
}

void(* resetFunc) (void) = 0;

void setup() {
  // SERIAL
    Serial.begin(9600);Serial.println("Init ...");
    Serial1.begin(9600);
    Serial2.begin(9600);
    Serial3.begin(9600);
    delay(500);
  // LEDS
    for (int i=2; i<10;i++){pinMode(i, OUTPUT);digitalWrite(i,HIGH);delay(100);digitalWrite(i,LOW);}
  // RELAIS
    for (int j=38;j<54;j++){pinMode(j,OUTPUT);digitalWrite(j,HIGH);}  // Buzzer
    pinMode(30,OUTPUT);digitalWrite(30,HIGH);delay(250);digitalWrite(30,LOW);
  // AQUASTAT
    pinMode(29,INPUT);
  // AUTO-MANU Switch
    pinMode(12,INPUT);
  // RESET SW
    pinMode(25,INPUT_PULLUP);
}

// PROGRAMME PRINCIPAL

void loop() {
  // RESET SW
    if(!digitalRead(25)){resetFunc();}
  
  // LECTURE DES CAPTEURS
  double tempo;int n = 500;double temp;

  int SerialDelay = 300;

  // Nano on Serial1
  Serial1.println("TC1");
  delay(SerialDelay);
  S_SECS = getSerial1Temp();
  if(DEBUG){Serial.print("S_SECS:");Serial.print(S_SECS);Serial.print("C ");}

  Serial1.println("TC2");
  delay(SerialDelay);
  S_ABR = getSerial1Temp();
  if(DEBUG){Serial.print("S_ABR:");Serial.print(S_ABR);Serial.print("C ");}

  Serial1.println("TC3");
  delay(SerialDelay);
  S_RBR = getSerial1Temp();
  if(DEBUG){Serial.print("S_RBR:");Serial.print(S_RBR);Serial.print("C ");}

  // Nano on Serial2
  Serial2.println("TC1");
  delay(SerialDelay);
  S_BTB2 = getSerial2Temp();
  if(DEBUG){Serial.print("S_BTB2:");Serial.print(S_BTB2);Serial.print("C ");}

  Serial2.println("TC2");
  delay(SerialDelay);
  S_BECS = getSerial2Temp();
  if(DEBUG){Serial.print("S_BECS:");Serial.print(S_BECS);Serial.print("C ");}

  Serial2.println("TC3");
  delay(SerialDelay);
  S_BTH = getSerial2Temp();
  if(DEBUG){Serial.print("S_BTH:");Serial.print(S_BTH);Serial.print("C ");}

  Serial2.println("TC4");
  delay(SerialDelay);
  S_BTB1 = getSerial2Temp();
  if(DEBUG){Serial.print("S_BTB1:");Serial.print(S_BTB1);Serial.print("C ");}

  // Nano on Serial3
  Serial3.println("TC2");
  delay(SerialDelay);
  S_SOL_AVAL = getSerial3Temp();
  if(DEBUG){Serial.print("S_SOLav:");Serial.print(S_SOL_AVAL);Serial.print("C ");}

  Serial3.println("TC1");
  delay(SerialDelay);
  S_SOL_AMONT = getSerial3Temp();
  if(DEBUG){Serial.print("S_SOLam:");Serial.print(S_SOL_AMONT);Serial.print("C ");}

  // AQUASTAT
  int aq = !digitalRead(29);
  if(DEBUG){Serial.print("eAQ=");Serial.print(aq);Serial.print(" ");}

  // ECSb
  if(S_BECS<ECSb_Low){ECSb=ECSb_High;}
  if(S_BECS>ECSb_High){ECSb=ECSb_Low;}
  if(DEBUG){Serial.print("ECSb:");Serial.print(ECSb);Serial.print(" ");}

  // LECTURE DES BOUTONS
  int AUTO = digitalRead(12);
  int FPB[17];
  for(int j=8; j<11; j++){FPB[j]=analogRead(j);}
  for(int j=13; j<16; j++){FPB[j]=analogRead(j);}
  int FP = 0;
  if (FPB[8]>50){if(FPB[8]>thresh){FP=14;}else{FP=15;}}
  if (FPB[9]>50){if(FPB[9]>thresh){FP=12;}else{FP=13;}}
  if (FPB[10]>50){if(FPB[10]>thresh){FP=10;}else{FP=11;}}
  if (FPB[13]>50){if(FPB[13]>thresh){FP=5;}else{FP=6;}}
  if (FPB[14]>50){if(FPB[14]>thresh){FP=3;}else{FP=4;}}
  if (FPB[15]>50){if(FPB[15]>thresh){FP=1;}else{FP=2;}}      
  if(DEBUG && FP>0){Serial.print("FP = "); Serial.print(FP);Serial.print("\t");}
  if(FP>0){digitalWrite(30,HIGH);delay(250);digitalWrite(30,LOW);}

  // ALERTES
  int alerte = -100;
  if(S_ABR < alerte || S_BECS < alerte || S_BTB2 < alerte || S_BTH < alerte || S_SOL_AVAL < alerte)
    {digitalWrite(30,HIGH);delay(1000);digitalWrite(30,LOW);}

  // PORT SERIE
  if(Serial.available() > 0)
    {String serial = Serial.readString();
    if(serial=="debug")
      DEBUG=!DEBUG;
    if(serial=="reset")
      resetFunc();
    if(serial=="A"){Serial.print("mode A actif ~");Serial.print(nA*2/60);Serial.print(" min");}
    if(serial=="B"){Serial.print("mode B actif ~");Serial.print(nB*2/60);Serial.print(" min");}
    if(serial=="C"){Serial.print("mode C actif ~");Serial.print(nC*2/60);Serial.print(" min");}
    if(serial=="D"){Serial.print("mode D actif ~");Serial.print(nD*2/60);Serial.print(" min");}
    if(serial=="E"){Serial.print("mode E actif ~");Serial.print(nE*2/60);Serial.print(" min");}
    if(serial=="F"){Serial.print("mode F actif ~");Serial.print(nF*2/60);Serial.print(" min");}
    if(serial=="G"){Serial.print("mode G actif ~");Serial.print(nG*2/60);Serial.print(" min");}
    if(serial=="H"){Serial.print("mode H actif ~");Serial.print(nH*2/60);Serial.print(" min");}
    if(serial=="O"){Serial.print("mode O actif ~");Serial.print(nG*2/60);Serial.print(" min");}
    if(serial=="S"){Serial.print("mode S actif ~");Serial.print(nS*2/60);Serial.print(" min");}
    if(serial=="Z"){Serial.print("mode Z actif ~");Serial.print(nZ*2/60);Serial.print(" min");}
    }
  if(DEBUG){if(AUTO==1){Serial.print("AUTO \t");}else {Serial.print("MANUEL \t");}}
  
  // GESTION DES MODES
  
  // MODE A : Sécurité Aquastat
  bool CAON = S_ABR>95 ; //S_ABR>85 && S_BECS>60 && S_ABR>S_BECS+5 && S_ABR>S_BTB2+5 && S_BECS<ECSb || S_ABR>90;
  bool CAOFF = S_ABR<76 ; //S_ABR < 72 || S_ABR<S_BECS+5 && S_ABR<72 || S_ABR<S_BTB2+5 && S_ABR<72 ||S_ABR<85 && S_BECS>ECSb;
  //Activation Mode A
  if((A==0) && (AUTO && CAON || !AUTO && FP==1)){A=1;}
  //Arret Mode A
  if((A==1) && (AUTO && CAOFF || !AUTO && FP==2)){A=0;}

  // MODE B : Tampon -> ECS
  bool CBON = S_BTH>S_BECS+15 && S_BECS<ECSb && S_BTH>55 && S_ABR<30 && S_BTB2>40 ||S_BTB2>80 && S_BECS<80&&S_ABR<30;
  bool CBOFF = (S_BTH<(S_BECS+2)) || (S_BTH<50) || (S_BECS>ECSb && S_BTH<80) || S_ABR>25;
  //Arret Mode B
  if((B==1) && (AUTO && CBOFF || !AUTO && FP==4)){B=0;}
  //Activation Mode B
  if((B==0) && (AUTO && CBON || !AUTO && FP==3)){B=1;}

  // MODE C : Brunal -> Tampon
  bool CCON = S_BECS>ECSb && S_BTB2<85 && S_ABR>(enc_Brunal) && S_ABR>S_BTB2+60 ;
  bool CCOFF = S_ABR<S_BTB2+0.5 && S_ABR<80|| S_BECS<ECSb || S_ABR<Brunal_mini ;
  //Activation Mode C
  if((C==0) && (AUTO && CCON || !AUTO && FP==5)){C=1;}
  //Arret Mode C
  if((C==1) && (AUTO && CCOFF || !AUTO && FP==6)){C=0;}

  // MODE D : Brunal -> ECS
  bool CDON = S_ABR>(enc_Brunal) && S_BECS<ECSb && S_ABR>S_BECS+10;
  bool CDOFF = S_BECS>ECSb || S_ABR<S_BECS+2 && S_ABR<80|| S_ABR<Brunal_mini;
  //Activation Mode D
  if((D==0) && (AUTO && CDON || !AUTO && FP==10)){D=1;}
  //Arret Mode D
  if((D==1) && (AUTO && CDOFF || !AUTO && FP==11)){D=0;}

  // MODE E : Circulation SOL
  bool CEON = S_SOL_AVAL>90 && S_SOL_AVAL>S_BECS+17+10 || S_SOL_AVAL>90+10;
  bool CEOFF = S_SOL_AVAL<05+10 || S_SOL_AVAL<S_BECS+7+10;
  //Activation Mode E
  if((E==0) && (AUTO && CEON || !AUTO && FP==14)){E=1;}
  //Arret Mode E
  if((E==1) && (AUTO && CEOFF || !AUTO && FP==15)){E=0;}

  // MODE H : EVSOL
  bool CHON = E == 1 && (S_SOL_AVAL > 90) ; // (S_SOL_AVAL > 20 && S_SOL_AVAL < 45))
  bool CHOFF = E == 0 || (S_SOL_AVAL  < 65);
  //Activation Mode H
  if((H==0) && (AUTO && CHON || !AUTO && FP==14)){H=1;}
  //Arret Mode H
  if((H==1) && (AUTO && CHOFF || !AUTO && FP==15)){H=0;}

  // MODE F : Ouverture EV primaire
  bool CFON = 0;
  bool CFOFF = 0;
  //Activation Mode F
  if((F==0) && (AUTO && CFON)){F=1;}
  //Arret Mode F
  if((F==1) && (AUTO && CFOFF)){F=0;}

  // MODE G : Mode Hors Gel
  bool CGON = S_SOL_AVAL < -15;
  bool CGOFF = S_SOL_AVAL > -15;
  //Activation Mode G
  if((G==0) && (AUTO && CGON)){G=1;}
  //Arret Mode G
  if((G==1) && (AUTO && CGOFF)){G=0;}

  // MODE O : Defaut sondes
  bool COON = S_SECS==0.0 || S_ABR==0.0 || S_RBR==0.0 || S_BTB2==0.0 || S_BECS==0.0;
  bool COOFF = S_SECS!=0.0 && S_ABR!=0.0 && S_RBR!=0.0 && S_BTB2!=0.0 && S_BECS!=0.0 && S_BTB1!=0.0;
  //Activation Mode G
  if((O==0) && (AUTO && COON)){O=1;}
  //Arret Mode G
  if((O==1) && (AUTO && COOFF)){O=0;}

  // MODE S : Circulation Brunal
  bool CSON = aq || S_ABR>42;
  bool CSOFF = !aq && S_ABR<38;
  //Activation Mode S
  if((S==0) && (AUTO && CSON)){S=1;tempoS=0;}
  //Arret Mode S
  if((S==1) && (AUTO && CSOFF)){tempoS+=1;}
  // Tempo arret mode S
  if (tempoS >= 10 * 60 / 2) // 10 min
    {S=0;tempoS=0;}

  // MODE Z : Brunal + Tampon -> ECS
  bool CZON = S_BTB2>85 && S_ABR>94;
  bool CZOFF = S_ABR<(enc_Brunal-10); 
  //Activation Mode Z
  if((Z==0) && (AUTO && CZON || !AUTO && FP==12)){Z=1;}
  //Arret Mode Z
  if((Z==1) && (AUTO && CZOFF || !AUTO && FP==13)){Z=0;}

  // RESTRICTION DES MODES
  if((Z+E)>1){E=0;}
  if((A+B+C+D+Z)>1){
    if(Z){Z=1;A=0;B=0;C=0;D=0;Serial.println("Priorité Mode Z");}
    if(A){Z=0;A=1;B=0;C=0;D=0;Serial.println("Priorité Mode A");}
    if(C){Z=0;A=0;B=0;C=1;D=0;Serial.println("Priorité Mode C");}
    if(D){Z=0;A=0;B=0;C=0;D=1;Serial.println("Priorité Mode D");}
  }

  // ACTIVATION/ARRET DES MODES
  EVB=0;EVC=0;EVD=0;
  EV3=0;EV4=0;EV5=0;EVSOL=0;
  P1=0;P2=0;P3=0;
  Conv=0;

  if (A || B || C || D || E || H || F || G || S || Z){
    // Temps de maintien du convertisseur
    tConv = 30;
  }
  else{
    if (tConv > 0){tConv = tConv -1;}
  }
  if (tConv > 0){Conv=1;}

  if(A){P1=1;EVC=1;EVD=1;Conv=1;digitalWrite(2,HIGH);}
  if(!A){digitalWrite(2,LOW);}
  if(DEBUG && A){Serial.print("A");}

  if(B){P2=1;EVB=1;Conv=1;digitalWrite(3,HIGH);}
  if(!B){digitalWrite(3,LOW);}
  if(DEBUG && B){Serial.print("B");}

  if(C){P1=1;EVC=1;Conv=1;digitalWrite(4,HIGH);}
  if(!C){digitalWrite(4,LOW);}
  if(DEBUG && C){Serial.print("C");}

  if(D){P1=1;EVD=1;digitalWrite(7,HIGH);}
  if(!D){digitalWrite(7,LOW);}
  if(DEBUG && D){Serial.print("D");}

  if(E){Conv=1;P3=1;digitalWrite(9,HIGH);}
  if(!E){digitalWrite(9,LOW);}
  if(DEBUG && E){Serial.print("E");}

  // if(F){EV1=1;EV2=1;EV3=1;digitalWrite(5,HIGH);}
  // if(!F){digitalWrite(5,LOW);}
  // if(DEBUG && F){Serial.print("F");}

  if(G){Conv=1;P3=1;EVSOL=1;digitalWrite(6,HIGH);}
  if(!G){digitalWrite(6,LOW);}
  if(DEBUG && G){Serial.print("G");}

  if(H){Conv=1;EVSOL=1;}
  if(!H){}
  if(DEBUG && H){Serial.print("H");}

  // if(O){EVB=1;EVC=1;EV4=1;Conv=1;P1=1;EVD=1;digitalWrite(2,HIGH);digitalWrite(30,HIGH);}
  // if(!O){digitalWrite(2,LOW);digitalWrite(30,LOW);}
  // if(DEBUG && O){Serial.print("O");}

  if(S){Conv=1;P1=1;Blink=1;}
  if(!S){Blink=0;}
  if(DEBUG && S){Serial.print("S");}

  // if(Z){EVB=1;EV3=1;EV4=1;Conv=1;P2=1;EVD=1;digitalWrite(8,HIGH);}
  // if(!Z){digitalWrite(8,LOW);}
  // if(DEBUG && Z){Serial.print("Z");}

  //Blink mode S
  if(Blink)
  {
    if(A+C+D<1)
    {
       digitalWrite(2,nTotal%2);
       digitalWrite(4,nTotal%2);
       digitalWrite(7,nTotal%2);
    }
    else
    {
       digitalWrite(2,nTotal%2 && A);
       digitalWrite(4,nTotal%2 && C);
       digitalWrite(7,nTotal%2 && D);
    }
  }

  // Comptage nModes
  if(A){nA++;}
  if(B){nB++;}
  if(C){nC++;}
  if(D){nD++;}
  if(E){nE++;}
  if(F){nF++;}
  if(G){nG++;}
  if(H){nH++;}
  if(S){nS++;}
  if(Z){nZ++;}

  // Comptage Global 30 min -> Mode F
  int TempsCycle = 1000;
  nTotal++;
  if(nTotal>1000){nTotal=0;}

  Serial.println("");
  relais();

  // mesure temps boucle
  fin=millis();
  //Serial.print("temps boucle : ");Serial.print(fin-debut);Serial.println("ms");
  debut=millis();
}

  /* MISC STUFF
   * 
   * RELAYS
   *  38  - EV5
   *  39  - P2
   *  40  - EV4
   *  41  - EVD
   *  42  - EV1
   *  43  - P3
   *  44  - EV3
   *  45  - P1
   *  46  - EV2
   *  52 - Conv  -> 48 
   */
