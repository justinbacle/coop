 #include <LedControl.h>

// DEFINITION VARIABLES

int REV5 = 38;
int RP2 = 39;
int REV4 = 40;
int RP4 = 41;
int REV1 = 42;
int RP3 = 43;
int REV3 = 44;
int RP1 = 45;
int REV2 = 46;
int RConv = 48; // was 52

int EV5 = 0;
int P2 = 0;
int EV4 = 0;
int P4 = 0;
int EV1 = 0;
int P3 = 0;
int EV3 = 0;
int P1 = 0;
int EV2 = 0;
int Conv = 0;

int A=0;
int B=0;
int C=0;
int D=0;
int E=0;
int F=0;
int G=0;
int S=0;
int Z=0;

int DEBUG = 1;
int N=0;
int thresh = 400;
bool Blink = 0;

double enc_Brunal = 60; //85,66
double ECSb_High = 50; //60
double ECSb_Low = 25;
double ECSb = ECSb_High;
int tempoE = 0;
int tempoE_P4 = 0;
int tempoE_EV5 = 0;
int Brunal_mini = 55;
int tConv = 0;

double Brunal, Tampon_b, Tampon_h, Ecs, Ces;

float nA = 0;
float nB = 0;
float nC = 0;
float nD = 0;
float nE = 0;
float nF = 0;
float nG = 0;
float nS = 0;
float nZ = 0;
int nTotal = 0;

unsigned long debut,fin;

// AFFICHEURS
LedControl lc1 = LedControl(50,52,34,1);
LedControl lc2 = LedControl(50,52,35,1);

// FONCTIONS
void resetLCD(){
  lc1.shutdown(0,false);lc2.shutdown(0,false);delay(1000);
  lc1.setIntensity(0,5);lc2.setIntensity(0,5);delay(1000);
  lc1.clearDisplay(0);lc2.clearDisplay(0);
}
double convertTC(double value){
  double temp = value * 5.0 / 1023.0;
  double elise = (temp - 1.18)/0.005;
  return elise;
}

void seg(double number,double number2, int device){
  int hundreds = number/100;number = number - hundreds*100;
  int tens = number / 10;int ones = number - tens*10;
  int hundreds2 = number2/100;number2 = number2 - hundreds2*100;
  int tens2 = number2 / 10;int ones2 = number2 - tens2*10; 
  if(device==1)
    {lc1.setDigit(0,7,hundreds,false);lc1.setDigit(0,6,tens,false);lc1.setDigit(0,5,ones,false);
    lc1.setDigit(0,3,hundreds2,false);lc1.setDigit(0,2,tens2,false);lc1.setDigit(0,1,ones2,false);}
  else {lc2.setDigit(0,7,hundreds,false);lc2.setDigit(0,6,tens,false);lc2.setDigit(0,5,ones,false);
  lc2.setDigit(0,3,hundreds2,false);lc2.setDigit(0,2,tens2,false);lc2.setDigit(0,1,ones2,false);}
}

void relais(){
  int t = 120;
  digitalWrite(REV5,!EV5);delay(t);
  digitalWrite(RP2,!P2);delay(t);
  digitalWrite(REV4,!EV4);delay(t);
  digitalWrite(RP4,!P4);delay(t);
  digitalWrite(REV1,!EV1);delay(t);
  digitalWrite(RP3,!P3);delay(t);
  digitalWrite(REV3,!EV3);delay(t);
  digitalWrite(RP1,!P1);delay(t);
  digitalWrite(REV2,!EV2);delay(t);
  digitalWrite(RConv,!Conv);
}

void(* resetFunc) (void) = 0;

void setup() {
  // SERIAL
    Serial.begin(9600);Serial.println("Init ...");
    Serial1.begin(9600);
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
  // DISPLAY
    resetLCD();
  // RESET SW
    pinMode(25,INPUT_PULLUP);
}

// PROGRAMME PRINCIPAL

void loop() {
  // RESET SW
    if(!digitalRead(25)){resetFunc();}
  
  // LECTURE DES CAPTEURS
  double tempo;int n = 500;double temp;

  // Brunal2.0 - Nano
  Serial1.println("TC2");
  delay(500);
  if (Serial1.available()) {
    char inData[16] = "";int index = 0;
    while (Serial1.available()) {inData[index] = Serial1.read();index++;}
    Brunal = String(inData).toFloat();
  }
  if(DEBUG){Serial.print("Brunal:");Serial.print(Brunal);Serial.print("C ");}
  
  // TAMPON_b
  tempo = 0;
  for (int e=0; e<n;e++){tempo = tempo + analogRead(2);}
  temp = tempo/n;Tampon_b = convertTC(temp);
  if(DEBUG){Serial.print("Tampon_b:");Serial.print(Tampon_b);Serial.print("C ");}

  // Tampon_h
  tempo = 0;
  for (int e=0; e<n;e++){tempo = tempo + analogRead(3);}
  temp = tempo/n;Tampon_h = convertTC(temp);
  if(DEBUG){Serial.print("Tampon_h:");Serial.print(Tampon_h);Serial.print("C ");}

  // ECS
  tempo = 0;
  for (int e=0; e<n;e++){tempo = tempo + analogRead(4);}
  temp = tempo/n;Ecs = convertTC(temp);
  if(DEBUG){Serial.print("Ecs:");Serial.print(Ecs);Serial.print("C ");}
  
  // CES
  tempo = 0;
  for (int e=0; e<n;e++){tempo = tempo + analogRead(1);}
  temp = tempo/n;Ces = convertTC(temp);
  if(DEBUG){Serial.print("Ces:");Serial.print(Ces);Serial.print("C ");}
  
  // AQUASTAT
  int aq = !digitalRead(29);
  if(DEBUG){Serial.print("eAQ=");Serial.print(aq);Serial.print(" ");}

  // ECSb
  if(Ecs<ECSb_Low){ECSb=ECSb_High;}
  if(Ecs>ECSb_High){ECSb=ECSb_Low;}
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
  if(Brunal < alerte || Ecs < alerte || Tampon_b < alerte || Tampon_h < alerte || Ces < alerte)
    {digitalWrite(30,HIGH);delay(1000);digitalWrite(30,LOW);}
  
  // AFFICHAGE
  /*
  N++;
  if(N>60){resetLCD();N=0;}
  seg(Brunal,Tampon_b,1);seg(Ecs,Ces,2);
  */

  // PORT SERIE
  if(Serial.available() > 0)
    {String serial = Serial.readString();
    if(serial=="debug")
      DEBUG=!DEBUG;
    if(serial=="resetLCD")
      resetLCD();
    if(serial=="reset")
      resetFunc();
    if(serial=="A"){Serial.print("mode A actif ~");Serial.print(nA*2/60);Serial.print(" min");}
    if(serial=="B"){Serial.print("mode B actif ~");Serial.print(nB*2/60);Serial.print(" min");}
    if(serial=="C"){Serial.print("mode C actif ~");Serial.print(nC*2/60);Serial.print(" min");}
    if(serial=="D"){Serial.print("mode D actif ~");Serial.print(nD*2/60);Serial.print(" min");}
    if(serial=="E"){Serial.print("mode E actif ~");Serial.print(nE*2/60);Serial.print(" min");}
    if(serial=="F"){Serial.print("mode F actif ~");Serial.print(nF*2/60);Serial.print(" min");}
    if(serial=="G"){Serial.print("mode G actif ~");Serial.print(nG*2/60);Serial.print(" min");}
    if(serial=="S"){Serial.print("mode S actif ~");Serial.print(nS*2/60);Serial.print(" min");}
    if(serial=="Z"){Serial.print("mode Z actif ~");Serial.print(nZ*2/60);Serial.print(" min");}
    }
  if(DEBUG){if(AUTO==1){Serial.print("AUTO \t");}else {Serial.print("MANUEL \t");}}
  
  // GESTION DES MODES
  
  // MODE A : Sécurité Aquastat
  bool CAON = Brunal>80 && Ecs>25 && Ecs<40 && Brunal>Ecs+5 && Brunal>Tampon_b+5 && Ecs<ECSb || Brunal>92 || Brunal>70 && Ecs>37 && Ecs<ECSb;
  bool CAOFF = Brunal < 70 && Ecs<37 || Brunal<Ecs+5 && Brunal<80 || Brunal<Tampon_b+5 && Brunal<80 ||Brunal<85 && Ecs>ECSb || Brunal<61 && Ecs>37;
  //Activation Mode A
  if((A==0) && (AUTO && CAON || !AUTO && FP==1)){A=1;}
  //Arret Mode A
  if((A==1) && (AUTO && CAOFF || !AUTO && FP==2)){A=0;}

  // MODE B : Tampon -> ECS
  bool CBON = Tampon_h>Ecs+25 && Ecs<ECSb && Tampon_h>45 && Brunal<25 ||Tampon_b>80 && Ecs<80&&Brunal<25;
  // ((Tampon_h>(Ecs+15)) && Ecs<ECSb && Tampon_h>25 ||(Tampon_b>80 && Ecs<80))&&Brunal<25;
  //bool CBOFF = ((Ecs+10)>Tampon_h) || (Tampon_h<50) || (Ecs>ECSb && Tampon_h<90) || (Brunal>enc_Brunal);
  bool CBOFF = (Tampon_h<(Ecs+10)) || (Tampon_h<50) || (Ecs>ECSb && Tampon_h<80) || Brunal>25;
  //Arret Mode B
  if((B==1) && (AUTO && CBOFF || !AUTO && FP==4)){B=0;}
  //Activation Mode B
  if((B==0) && (AUTO && CBON || !AUTO && FP==3)){B=1;}
  //Arret Mode B
  //if((B==1) && (AUTO && CBOFF || !AUTO && FP==4)){B=0;}

  // MODE C : Brunal -> Tampon
  bool CCON = Ecs>ECSb && Tampon_b<85 && Brunal>(enc_Brunal) && Brunal>Tampon_b+30 ;
  bool CCOFF = Brunal<Tampon_b+5 && Brunal<80|| Ecs<ECSb || Brunal<Brunal_mini ;
  //Activation Mode C
  if((C==0) && (AUTO && CCON || !AUTO && FP==5)){C=1;}
  //Arret Mode C
  if((C==1) && (AUTO && CCOFF || !AUTO && FP==6)){C=0;}

  // MODE D : Brunal -> ECS
  bool CDON = Brunal>(enc_Brunal) && Ecs<ECSb && Brunal>Ecs+30;
  bool CDOFF = Ecs>ECSb || Brunal<Ecs+5 && Brunal<80|| Brunal<Brunal_mini;
  //Activation Mode D
  if((D==0) && (AUTO && CDON || !AUTO && FP==10)){D=1;}
  //Arret Mode D
  if((D==1) && (AUTO && CDOFF || !AUTO && FP==11)){D=0;}

  // MODE E

  bool CEON = false;
    //if(Ecs<=10){CEON=Ces>(60+Ecs);}
    //if(Ecs>10&&Ecs<=15){CEON=Ces>(55+Ecs);}
    //if(Ecs>15&&Ecs<=20){CEON=Ces>(50+Ecs);}
    //if(Ecs>20&&Ecs<=30){CEON=Ces>(40+Ecs);}
    //if(Ecs>30&&Ecs<=40){CEON=Ces>(30+Ecs);}
    //if(Ecs>40&&Ecs<=50){CEON=Ces>(20+Ecs);}
    //if(Ecs>50){CEON=Ces>(5+Ecs);}
    if(Ces>55 && Ces>Ecs+30){CEON=true;}
  bool CEOFF = false;
    //if(Ecs<=10){CEOFF=Ces<(45+Ecs);}
    //if(Ecs>10&&Ecs<=15){CEOFF=Ces<(40+Ecs);}
    //if(Ecs>15&&Ecs<=20){CEOFF=Ces<(35+Ecs);}
    //if(Ecs>20&&Ecs<=30){CEOFF=Ces<(25+Ecs);}
    //if(Ecs>30&&Ecs<=40){CEOFF=Ces<(15+Ecs);}
    //if(Ecs>40&&Ecs<=50){CEOFF=Ces<(2+Ecs);}
    //if(Ecs>50){CEOFF=Ces<(1+Ecs);}
    if(Ecs){CEOFF=Ces<(10+Ecs);}
    if(Ces<55){CEOFF=true;}
    
    // Tempo mode E
    //int tempoE_ = 121;
    int tempoE_ = 1; // Changement de sonde, plus besoin de tempo
    if(tempoE < tempoE_){CEOFF = false;}

    // Tempo retard mode E P4
    int tempoE_P4_ = 50;

     // Tempo retard mode E EV5
    int tempoE_EV5_ = 50;

  //Activation Mode E
  if((E==0) && (AUTO && CEON || !AUTO && FP==14)){E=1;}
  //Arret Mode E
  if((E==1) && (AUTO && CEOFF || !AUTO && FP==15)){E=0;}

  if(E){tempoE++;}else{tempoE=0;}
  if(E){tempoE_P4++;}else{tempoE_P4=0;}
  if(E){tempoE_EV5++;}else{tempoE_EV5=0;}
  /*
  // MODE F : Desembouage circuit solaire
  bool CFON = nTotal<150;
  bool CFOFF = nTotal>=150;
  //Activation Mode F
  if((F==0) && (AUTO && CFON)){F=1;}
  //Arret Mode F
  if((F==1) && (AUTO && CFOFF)){F=0;}
  */

  // MODE F : Ouverture EV primaire
  bool CFON = 0;
  bool CFOFF = 0;
  //Activation Mode F
  if((F==0) && (AUTO && CFON)){F=1;}
  //Arret Mode F
  if((F==1) && (AUTO && CFOFF)){F=0;}

  // MODE G : Mode Hors Gel
  bool CGON = Ces<-90;
  bool CGOFF = Ces>-90;
  //Activation Mode G
  if((G==0) && (AUTO && CGON)){G=1;}
  //Arret Mode G
  if((G==1) && (AUTO && CGOFF)){G=0;}

  // MODE S : Circulation Brunal
  bool CSON = aq || (Brunal>25) ;
  bool CSOFF = !aq && (Brunal<25) ;
  //Activation Mode S
  if((S==0) && (AUTO && CSON)){S=1;}
  //Arret Mode S
  if((S==1) && (AUTO && CSOFF)){S=0;}
   
  // MODE Z : Brunal + Tampon -> ECS
  bool CZON = Tampon_b>85 && Brunal>94;
  bool CZOFF = Brunal<(enc_Brunal-10); 
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

  EV1=0;EV2=0;EV3=0;EV4=0;EV5=0;
  Conv=0;
  P1=0;P2=0;P3=0;P4=0;

  if (A || B || C || D || E || F || G || S || Z){
    // Temps de maintien du convertisseur
    tConv = 30;
  }
  else{
    if (tConv > 0){tConv = tConv -1;}
  }
  if (tConv > 0){Conv=1;}

  
  if(A){EV1=1;EV2=1;EV4=1;Conv=1;P1=1;P4=1;digitalWrite(2,HIGH);}
  if(!A){digitalWrite(2,LOW);}
  if(DEBUG && A){Serial.print("A");}

  if(B){EV3=1;EV4=1;Conv=1;P2=1;P4=1;digitalWrite(3,HIGH);}
  if(!B){digitalWrite(3,LOW);}
  if(DEBUG && B){Serial.print("B");}

  if(C){EV2=1;Conv=1;P1=1;digitalWrite(4,HIGH);}
  if(!C){digitalWrite(4,LOW);}
  if(DEBUG && C){Serial.print("C");}

  if(D){EV1=1;EV4=1;Conv=1;P1=1;P4=1;digitalWrite(7,HIGH);}
  if(!D){digitalWrite(7,LOW);}
  if(DEBUG && D){Serial.print("D");}

  //if(E){EV5=1;Conv=1;P3=1;P4=1;digitalWrite(9,HIGH);}
  if(E){Conv=1;P3=1;digitalWrite(9,HIGH);}
  if(E && tempoE_P4>tempoE_P4_){P4=1;}
  if(E && tempoE_EV5>tempoE_EV5_){EV5=1;}
  if(!E){digitalWrite(9,LOW);}
  if(DEBUG && E){Serial.print("E");}

  //if(F){Conv=1;P3=1;digitalWrite(5,HIGH);}
  if(F){EV1=1;EV2=1;EV3=1;digitalWrite(5,HIGH);}
  if(!F){digitalWrite(5,LOW);}
  if(DEBUG && F){Serial.print("F");}

  if(G){Conv=1;P3=1;digitalWrite(6,HIGH);}
  if(!G){digitalWrite(6,LOW);}
  if(DEBUG && G){Serial.print("G");}

  if(S){Conv=1;P1=1;Blink=1;}
  if(!S){Blink=0;}
  if(DEBUG && S){Serial.print("S");}

  if(Z){EV1=1;EV3=1;EV4=1;Conv=1;P2=1;P4=1;digitalWrite(8,HIGH);}
  if(!Z){digitalWrite(8,LOW);}
  if(DEBUG && Z){Serial.print("Z");}

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
   *  41  - P4
   *  42  - EV1
   *  43  - P3
   *  44  - EV3
   *  45  - P1
   *  46  - EV2
   *  52 - Conv  -> 48 
   */
