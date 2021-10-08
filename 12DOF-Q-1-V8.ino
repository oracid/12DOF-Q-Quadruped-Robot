// 12DOF-Q-1  - 03/10/2021 - 
void(* resetFunc) (void) = 0;               // soft reset function
#include <Servo.h>
int Speed=0;
const int nb=12;                            // Number of servos
Servo Srv[nb];                              // Servos table
int OnOff[nb]={1,1,1,1,1,1,1,1, 1,1,1,1};   // on/off Servos table
int FRLS=0, FRRS=1, FLLS=2, FLRS=3, BRLS=4, BRRS=5, BLLS=6, BLRS=7, FRS=8, FLS=9, BRS=10, BLS=11;
//***************** errors correction servos position  ************************************************************************************
// servos      FRLS  FRRS  FLLS  FLRS  BRLS  BRRS  BLLS  BLRS  FRS  FLS  BRS  BLS    you must modify these values according to your servos
int Err[nb]={    6,    0,    4,    0,   10,   -7,    0,   -2,  -4,   4,   -1,  7    };  
//*****************************************************************************************************************************************
         
void  setup() {
  delay(400);                             // for reset consideration
  Serial.begin(9600);
  pinMode(0,INPUT_PULLUP);                // start/stop/reset button attachment

  int a=70, b=90;                         // Servos angle initialization
  if(OnOff[FRLS]) { Srv[FRLS].attach(2); Srv[FRLS].write(    a+Err[FRLS]); }  // FR Front Right leg - LS Left   Servo
  if(OnOff[FRRS]) { Srv[FRRS].attach(3); Srv[FRRS].write(180-a+Err[FRRS]); }  // FR Front Right leg - RS Right  Servo
  if(OnOff[FLLS]) { Srv[FLLS].attach(4); Srv[FLLS].write(    a+Err[FLLS]); }  // FL Front Left  leg - LS Left   Servo
  if(OnOff[FLRS]) { Srv[FLRS].attach(5); Srv[FLRS].write(180-a+Err[FLRS]); }  // FL Front Left  leg - RS Right  Servo
  if(OnOff[BRLS]) { Srv[BRLS].attach(6); Srv[BRLS].write(    a+Err[BRLS]); }  // BR Back  Right leg - LS Left   Servo
  if(OnOff[BRRS]) { Srv[BRRS].attach(7); Srv[BRRS].write(180-a+Err[BRRS]); }  // BR Back  Right leg - RS Right  Servo
  if(OnOff[BLLS]) { Srv[BLLS].attach(8); Srv[BLLS].write(    a+Err[BLLS]); }  // BL Back  Left  leg - LS Left   Servo
  if(OnOff[BLRS]) { Srv[BLRS].attach(9); Srv[BLRS].write(180-a+Err[BLRS]); }  // BL Back  Left  leg - RS Right  Servo

  if(OnOff[FRS])  { Srv[FRS].attach(10); Srv[FRS].write(b+Err[FRS]); }        // FRS Front Right Shoulder   Servo
  if(OnOff[FLS])  { Srv[FLS].attach(11); Srv[FLS].write(b+Err[FLS]); }        // FLS Front Left  Shoulder   Servo
  if(OnOff[BRS])  { Srv[BRS].attach(12); Srv[BRS].write(b+Err[BRS]); }        // BRS Back  Right Shoulder   Servo
  if(OnOff[BLS])  { Srv[BLS].attach(13); Srv[BLS].write(b+Err[BLS]); }        // BLS Back  Left  Shoulder   Servo

  Serial.print("\n\t To start, click on the Start button");  while( digitalRead(0) );  delay(400);  Serial.print("\n\t Started");
}

void loop() {
  if (! digitalRead(0)) resetFunc();

  for( int i=0;i<=8;i++)  { Speed=7; Horse();   }
  for( int i=0;i<=14;i++) { Speed=8; Turn('R'); }
  for( int i=0;i<=8;i++)  { Speed=7; Walk('F'); }
  for( int i=0;i<=10;i++) { Speed=7; Shift('R');}
  for( int i=0;i<=14;i++) { Speed=8; Turn('L'); }
  for( int i=0;i<=8;i++)  { Speed=7; Walk('F'); }
  for( int i=0;i<=10;i++) { Speed=7; Shift('R');}
  for( int i=0;i<=14;i++) { Speed=8; Turn('L'); }
}

void Horse(){
  // 03/10/2021 - 018 - Horse - 
  int FRx[]= {-10,-30,-50,-70,    -50,-30,-10, 10, 30, 50, 30, 10};
  int FLx[]= {-10, 10, 30, 50,     30, 10,-10,-30,-50,-70,-50,-30};
  int BRx[]= {-10, 10, 30, 50,     30, 10,-10,-30,-50,-70,-50,-30};
  int BLx[]= {-10,-30,-50,-70,    -50,-30,-10, 10, 30, 50, 30, 10};
  
  int FRy[]= {-15,-15,-15,-15,     15, 15, 15, 15, 15, 15,-15,-15};
  int FLy[]= { 15, 15, 15, 15,    -15,-15,-15,-15,-15,-15, 15, 15};
  int BRy[]= { 15, 15, 15, 15,    -15,-15,-15,-15,-15,-15, 15, 15};
  int BLy[]= {-15,-15,-15,-15,     15, 15, 15, 15, 15, 15,-15,-15};
  
  int FRz[]= {  0,-20,-40,-50,    -40,-20,  0, 20, 40, 50, 40, 20};
  int FLz[]= {  0,-20,-40,-50,    -40,-20,  0, 20, 40, 50, 40, 20};
  int BRz[]= {  0, 20, 40, 50,     40, 20,  0,-20,-40,-50,-40,-20};
  int BLz[]= {  0, 20, 40, 50,     40, 20,  0,-20,-40,-50,-40,-20};
  
  int lgTab = sizeof(FRx)/sizeof(int);
  int i=0, j=lgTab-1;
  for( i;i<=j;i++){
    IK(+FRx[abs(i)],FRy[abs(i)],+FRz[abs(i)],FRLS,FRRS,FRS);   // Front Right Leg
    IK(-BLx[abs(i)],BLy[abs(i)],+BLz[abs(i)],BLLS,BLRS,BLS);   // Back Left Leg
    IK(-FLx[abs(i)],FLy[abs(i)],-FLz[abs(i)],FLLS,FLRS,FLS);   // Front Left Leg
    IK(+BRx[abs(i)],BRy[abs(i)],-BRz[abs(i)],BRLS,BRRS,BRS);   // Back Right Leg
  }  
}
void Walk(char c){    // Forward c = 'F'    Backward c = 'B'
  // 05/10/2021 - 020 - Forward -
  int FRx[]= {-10,-30,-50,-70,    -50,-30,-10, 10, 30, 50, 30, 10};
  int FLx[]= {-10, 10, 30, 50,     30, 10,-10,-30,-50,-70,-50,-30};
  int BRx[]= {-10, 10, 30, 50,     30, 10,-10,-30,-50,-70,-50,-30};
  int BLx[]= {-10,-30,-50,-70,    -50,-30,-10, 10, 30, 50, 30, 10};
  
  int FRy[]= {-15,-15,-15,-15,     15, 15, 15, 15, 15, 15,-15,-15};
  int FLy[]= { 15, 15, 15, 15,    -15,-15,-15,-15,-15,-15, 15, 15};
  int BRy[]= { 15, 15, 15, 15,    -15,-15,-15,-15,-15,-15, 15, 15};
  int BLy[]= {-15,-15,-15,-15,     15, 15, 15, 15, 15, 15,-15,-15};
  
  int FRz[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int FLz[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int BRz[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int BLz[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};

  int lgTab = sizeof(FRx)/sizeof(int);
  int i=0, j=lgTab-1;  if(c=='B'){  i=-(lgTab-1); j=0; }
  for( i;i<=j;i++){
    IK(+FRx[abs(i)],FRy[abs(i)],+FRz[abs(i)],FRLS,FRRS,FRS);   // Front Right Leg
    IK(-BLx[abs(i)],BLy[abs(i)],+BLz[abs(i)],BLLS,BLRS,BLS);   // Back Left Leg
    IK(-FLx[abs(i)],FLy[abs(i)],-FLz[abs(i)],FLLS,FLRS,FLS);   // Front Left Leg
    IK(+BRx[abs(i)],BRy[abs(i)],-BRz[abs(i)],BRLS,BRRS,BRS);   // Back Right Leg
  }
}

void Turn(char c){    // Turn Right c = 'R'   Turn Left c = 'L'
  // 03/10/2021 - 017 - Turning Riht and Left -
  int FRx[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int FLx[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int BRx[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int BLx[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  
  int FRy[]= {-20,-20,-20,-20,     20, 20, 20, 20, 20, 20,-20,-20};
  int FLy[]= { 20, 20, 20, 20,    -20,-20,-20,-20,-20,-20, 20, 20};
  int BRy[]= { 20, 20, 20, 20,    -20,-20,-20,-20,-20,-20, 20, 20};
  int BLy[]= {-20,-20,-20,-20,     20, 20, 20, 20, 20, 20,-20,-20};
  
  int FRz[]= {  0,-20,-40,-60,    -40,-20,  0, 20, 40, 60, 40, 20};
  int FLz[]= {  0,-20,-40,-60,    -40,-20,  0, 20, 40, 60, 40, 20};
  int BRz[]= {  0,-20,-40,-60,    -40,-20,  0, 20, 40, 60, 40, 20};
  int BLz[]= {  0,-20,-40,-60,    -40,-20,  0, 20, 40, 60, 40, 20};
  
  int lgTab = sizeof(FRx)/sizeof(int);
  int i=0, j=lgTab-1;  if(c=='L'){  i=-(lgTab-1); j=0; }
  for( i;i<=j;i++){
    IK(+FRx[abs(i)],FRy[abs(i)],+FRz[abs(i)],FRLS,FRRS,FRS);   // Front Right Leg
    IK(-BLx[abs(i)],BLy[abs(i)],+BLz[abs(i)],BLLS,BLRS,BLS);   // Back Left Leg
    IK(-FLx[abs(i)],FLy[abs(i)],-FLz[abs(i)],FLLS,FLRS,FLS);   // Front Left Leg
    IK(+BRx[abs(i)],BRy[abs(i)],-BRz[abs(i)],BRLS,BRRS,BRS);   // Back Right Leg
  }
}

void Shift(char c){    // Shift Right c = 'R'   Shift Left c = 'L'
  // 03/10/2021 - 018 - Lateral Walk -
  int FRx[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int FLx[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int BRx[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  int BLx[]= {  0,  0,  0,  0,      0,  0,  0,  0,  0,  0,  0,  0};
  
  int FRy[]= {-15,-15,-15,-15,     15, 15, 15, 15, 15, 15,-15,-15};
  int FLy[]= { 15, 15, 15, 15,    -15,-15,-15,-15,-15,-15, 15, 15};
  int BRy[]= { 15, 15, 15, 15,    -15,-15,-15,-15,-15,-15, 15, 15};
  int BLy[]= {-15,-15,-15,-15,     15, 15, 15, 15, 15, 15,-15,-15};
  
  int FRz[]= {  0,-20,-40,-50,    -40,-20,  0, 20, 40, 50, 40, 20};
  int FLz[]= {  0,-20,-40,-50,    -40,-20,  0, 20, 40, 50, 40, 20};
  int BRz[]= {  0, 20, 40, 50,     40, 20,  0,-20,-40,-50,-40,-20};
  int BLz[]= {  0, 20, 40, 50,     40, 20,  0,-20,-40,-50,-40,-20};
  
  int lgTab = sizeof(FRx)/sizeof(int);
  int i=0, j=lgTab-1;  if(c=='L'){  i=-(lgTab-1); j=0; }
  for( i;i<=j;i++){
    IK(+FRx[abs(i)],FRy[abs(i)],+FRz[abs(i)],FRLS,FRRS,FRS);   // Front Right Leg
    IK(-BLx[abs(i)],BLy[abs(i)],+BLz[abs(i)],BLLS,BLRS,BLS);   // Back Left Leg
    IK(-FLx[abs(i)],FLy[abs(i)],-FLz[abs(i)],FLLS,FLRS,FLS);   // Front Left Leg
    IK(+BRx[abs(i)],BRy[abs(i)],-BRz[abs(i)],BRLS,BRRS,BRS);   // Back Right Leg
  }
}

void IK(int Px, int Py, int Pz, int LS, int RS, int SS){  // Inverse Kinematics function
  if (! digitalRead(0)) resetFunc();

  float Ax=0, Ay=180, Az=0, Bx=Ax, By=Ay+24, Bz=Az, c=112; // Values of paw coordonates

  float dz=By-Py, ez=Bz-Pz;                   // Z diagonal calculations
  float hz=sqrt((dz*dz)+(ez*ez));             //
  float D=acos(dz/hz);   if(ez<0)D=(-D);      //

        Ay=By-((By-Ay)*cos(D));               // X Y diagonal calculations
        Az=Bz-(Az*sin(D));                    //
  float d=Ay-Py, e=Ax-Px;                     //

  float h=sqrt((d*d)+(e*e));                  //
  float E=acos(d/h);    if(e<0)E=(-E);        //

  float G=acos(h/(2*c));                      // Diamond half top A angles
  float GL=(PI/2)-G-E;                        // left servo angle
  float GR=(PI/2)-G+E;                        // right servo angle

  int S1=round(GL*57.296)+Err[LS];            // left paw  servo angle in degrees +Err
  int S2=round(180-(GR*57.296))+Err[RS];      // right paw servo angle in degrees +Err
  int S3=round(90+(D*57.296))+Err[SS];        // shoulder servo angle in degrees +Err

//DEBUG
//  Serial.print("\n\n\tTarget coordinates : Px=");Serial.print(Px);Serial.print("  Py=");Serial.print(Py);Serial.print("  Pz=");  Serial.print(Pz);Serial.print("\t\tLeg servos :   LS=");Serial.print(LS);Serial.print("  RS=");Serial.print(RS);Serial.print("  SS=");Serial.print(SS);
//  Serial.print("\n\tInverse Kinematics values : D°= ");Serial.print(round(D*57.296));Serial.print("     E°= ");Serial.print(round(E*57.296));  Serial.print("     G°= ");Serial.print(round(G*57.296));Serial.print("     GL°= ");Serial.print(round(GL*57.296));Serial.print("     GR°= ");Serial.print(round(GR*57.296));
//  Serial.print("\n\tServo angles results : S1= ");Serial.print(S1);Serial.print("°\t\tS2= ");Serial.print(S2);Serial.print("°\tS3= ");Serial.print(S3);

// - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL -
  if (h>(c+c))        {Serial.print("\n\t Target : Px=");Serial.print(Px);Serial.print(" Py=");Serial.print(Py);Serial.print(" Pz=");Serial.print(Pz);Serial.print("\t h=");Serial.print(h);Serial.print(" > ");Serial.print(c+c);Serial.print(" is too long !!!!!");return;}
  if (S1<0)   {Serial.print("\n\t Target : Px=");Serial.print(Px);Serial.print(" Py=");Serial.print(Py);Serial.print(" Pz=");Serial.print(Pz);Serial.print("\t angle S1=");Serial.print(S1);Serial.print(" <0° is not reachable !!!!!");return;}
  if (S1>95)  {Serial.print("\n\t Target : Px=");Serial.print(Px);Serial.print(" Py=");Serial.print(Py);Serial.print(" Pz=");Serial.print(Pz);Serial.print("\t angle S1=");Serial.print(S1);Serial.print(" >95° is not reachable !!!!!");return;}
  if (S2>180) {Serial.print("\n\t Target : Px=");Serial.print(Px);Serial.print(" Py=");Serial.print(Py);Serial.print(" Pz=");Serial.print(Pz);Serial.print("\t angle S2=");Serial.print(S2);Serial.print(" >180° is not reachable !!!!!");return;}
  if (S2<85)  {Serial.print("\n\t Target : Px=");Serial.print(Px);Serial.print(" Py=");Serial.print(Py);Serial.print(" Pz=");Serial.print(Pz);Serial.print("\t angle S2=");Serial.print(S2);Serial.print(" <85° is not reachable !!!!!");return;}
  if (S3<45)  {Serial.print("\n\t Target : Px=");Serial.print(Px);Serial.print(" Py=");Serial.print(Py);Serial.print(" Pz=");Serial.print(Pz);Serial.print("\t angle S3=");Serial.print(S3);Serial.print(" <45° is not reachable !!!!!");return;}
  if (S3>135) {Serial.print("\n\t Target : Px=");Serial.print(Px);Serial.print(" Py=");Serial.print(Py);Serial.print(" Pz=");Serial.print(Pz);Serial.print("\t angle S3=");Serial.print(S3);Serial.print(" >135° is not reachable !!!!!");return;}
// - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL - RESTRICTED AREA CONTROL -

// Servos activation
  if (OnOff[LS]) Srv[LS].write(S1);   // set target Left servo if servo switch is On
  if (OnOff[RS]) Srv[RS].write(S2);   // set target Right servo if servo switch is On
  if (OnOff[SS]) Srv[SS].write(S3);   // set target Shoulder servo if servo switch is On
  delay(Speed);                       // speed of action
}
