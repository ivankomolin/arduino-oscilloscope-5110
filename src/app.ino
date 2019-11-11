//Страница проэкта  http://srukami.inf.ua/pultoscop_v25110.html
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <FreqCount.h> 
#include <PWM.h>
#include <CyberLib.h>
#define led  9   //пин для генератора сигналов (не менять)
#define dds  10   //пин для генератора dds (не менять)
//#################№№№№№№####пользовательские настройки
#define power 8 //пин который опрашивает кнопку включения
#define OFF 14//пин который управляет ключем питания
#define  timepowerON 50 //время удержания кнопки выключения
#define levo 13  //кнопка ЛЕВО(можно любой пин)
#define ok 12    //кнопка ОК(можно любой пин)
#define pravo 11 //кнопка ПРАВО(можно любой пин)
#define akb A5 //любой своюодный аналоговый пин для измерения напряжения АКБ 
#define overclock 16  //Частота на которой работает Ардуино 
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 4, 3, 2);//пины к которым у вас подключен дисплей         
byte cont=52;//контрастность дисплея
byte SinU=30;   //уровень синхронизации 0 до 255 
int PWM = 128;//стартовое значение ШИМ от 0 до 255        
int32_t frequency = 500; //стартовое значение частоты в Гц
float VCC=5.0;  //напряжение питания, меряем мультиметром
//###########################################################
int d=0;
byte menuDDS=0; 
byte sinM[32]={1,6,15,29,48,69,92,117,143,168,191,212,229,243,251,255,254,248,237,222,203,181,156,131,106,81,59,39,22,10,3,1};
byte pilaM[32]={1,9,17,25,33,41,49,57,65,73,81,89,97,105,113,121,129,137,145,153,161,169,177,185,193,201,209,217,225,235,245,255};
byte RpilaM[32]={250,246,238,230,222,214,206,198,190,182,174,166,158,150,142,134,126,118,110,102,94,86,78,70,62,54,41,33,25,17,9,1};
byte trianglM[32]={1,18,35,52,69,86,103,120,137,154,171,188,205,222,239,255,239,223,207,191,175,159,143,127,111,95,79,63,47,31,15,1};
int powerON=0;//состояние кнопки питания
byte hag=0;
int mnog=0;
boolean flag=0;
byte mass[701];
byte x=0; 
byte menu=0;//переменная выбора меню 
bool opornoe=1; //флаг опорного напряжения
bool paus=0; //флаг режима паузы
byte pultoskop=0; //флаг выбора генератора или осциллографа
byte razv=6;  
unsigned long count =0;
byte sinX=30; 
byte meaX=83;
int Vmax=0;// максимальное напряжение 
byte sinhMASS=0;
long countX=0;
long speedTTL=9600; //скорость терминала 
int prokr=0;
void setup(){
pinMode(A4,INPUT);
digitalWrite(OFF,HIGH);//включем питание 
//Serial.begin(9600);
display.begin();
display.setContrast(cont);  
while(digitalRead(ok)==LOW){  
/////////////////////////////////////////удержание кнопки отключения
if(digitalRead(power)==HIGH){powerON++;delay(10);}  
if(powerON>=timepowerON){digitalWrite(OFF,LOW);}///отключаем питание
/////////////////////////////////////////удержание кнопки отключения 
  if(pultoskop==0){
    display.clearDisplay();
    display.setCursor(10,0);
    display.setTextColor(WHITE, BLACK); // 'inverted' text
    display.println("Пультоскоп");
    display.setCursor(10,10);
    display.setTextColor(BLACK); 
    display.println("Генератор");
    display.setCursor(10,20);
    display.println("DDSгенератор");
    display.setCursor(10,30);
    display.println("Терминал");
    display.setCursor(0,40);
    display.print("Батарея=");
    display.print(analogRead(akb)*5.0/1024);
    display.print("В");    
  }
    if(pultoskop==1){
    display.clearDisplay();
    display.setCursor(10,0);
    display.setTextColor(BLACK); // 'inverted' text
    display.println("Пультоскоп");
    display.setCursor(10,10);
    display.setTextColor(WHITE, BLACK); // 'inverted' text
    display.println("Генератор");
     display.setTextColor(BLACK); // 'inverted' text;
    display.setCursor(10,20);
    display.println("DDSгенератор");
    display.setCursor(10,30);
    display.println("Терминал");
    display.setCursor(0,40);
    display.setTextColor(BLACK); 
    display.print("Батарея=");
    display.print(analogRead(akb)*5.0/1024);
    display.print("В");   
  } 
    if(pultoskop==2){
    display.clearDisplay();
    display.setCursor(10,00);
    display.setTextColor(BLACK); // 'inverted' text
    display.println("Пультоскоп");
    display.setCursor(10,10);
    display.println("Генератор");
    display.setTextColor(WHITE, BLACK); // 'inverted' text;
    display.setCursor(10,20);
    display.println("DDSгенератор");
    display.setTextColor(BLACK);
    display.setCursor(10,30);
    display.println("Терминал");
    display.setCursor(0,40);
    display.setTextColor(BLACK); 
    display.print("Батарея=");
    display.print(analogRead(akb)*5.0/1024);
    display.print("В");   
    }
        if(pultoskop==3){
    display.clearDisplay();
    display.setCursor(10,00);
    display.setTextColor(BLACK); // 'inverted' text
    display.println("Пультоскоп");
    display.setCursor(10,10);
    display.println("Генератор");
    display.setTextColor(BLACK);
    display.setCursor(10,20);
    display.println("DDSгенератор");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(10,30);
    display.println("Терминал");
    display.setCursor(0,40);
    display.setTextColor(BLACK); 
    display.print("Батарея=");
    display.print(analogRead(akb)*5.0/1024);
    display.print("В");   
    }
if(digitalRead(levo)==HIGH){delay(300);pultoskop=pultoskop+1;}
if(digitalRead(pravo)==HIGH){delay(300);pultoskop=pultoskop+1;}
if(pultoskop>3){pultoskop=0;}
delay(50);  
display.display(); }
if(pultoskop==2){InitTimersSafe(); bool success = SetPinFrequencySafe(led,200000);}
if(pultoskop==0){FreqCount.begin(1000);}
if(pultoskop==1){InitTimersSafe(); bool success = SetPinFrequencySafe(led, frequency);}
display.setTextColor(BLACK);
delay(500); }   
void Zamer(){
  if (razv>=6){ADCSRA = 0b11100010;}//delitel 4
  if (razv==5){ADCSRA = 0b11100011;}//delitel 8
  if (razv==4){ADCSRA = 0b11100100;}//delitel 16
  if (razv==3){ADCSRA = 0b11100101;}//delitel 32
  if (razv==2){ADCSRA = 0b11100110;}//delitel 64
  if (razv<2){ADCSRA = 0b11100111;}//delitel 128
  if (razv==0){
      for(int i=0;i<700;i++){ 
          while ((ADCSRA & 0x10)==0);
          ADCSRA|=0x10;
          delayMicroseconds(500);
          mass[i]=ADCH;
      }
  }
  if (razv>0){
      for(int i=0;i<700;i++){ 
          while ((ADCSRA & 0x10)==0);
          ADCSRA|=0x10;
          mass[i]=ADCH;
      }
  }
  
}
void loop() {
/////////////////////////////////////////удержание кнопки отключения
if(digitalRead(power)==HIGH){powerON++;delay(10);}  
if(powerON>=timepowerON){digitalWrite(OFF,LOW);}///отключаем питание
/////////////////////////////////////////удержание кнопки отключения    
if(pultoskop==0){  
if(opornoe==0){ADMUX = 0b11100011;}//выбор внутреннего опорного 1,1В
if(opornoe==1){ADMUX = 0b01100011;}//Выбор внешнего опорного
delay(5);
if(paus==0){Zamer();}
//#######################################определение точки синхронизации
bool flagSINHRO=0;
bool flagSINHRnull=0;
for(int y=1;y<255;y++){
  if(flagSINHRO==0){if(mass[y]<SinU){flagSINHRnull=1;}}
  if(flagSINHRO==0){if(flagSINHRnull==1){if(mass[y]>SinU){flagSINHRO=1;sinhMASS=y;}}}}
//#######################################определение точки синхронизации
//максимальное значение сигнала##########################
Vmax=0; 
for(int y=1;y<255;y++){if(Vmax<mass[y]){Vmax=mass[y];} }
//максимальное значение сигнала##########################
//#######################################определение точки синхронизации
//#######################################отрисовка графика 
if(paus==0){
display.clearDisplay();
display.fillCircle(80,47-SinU/7, 2, BLACK);//рисуем уровень синхронизации    
x=3;
for(int y=sinhMASS;y<sinhMASS+80;y++){
      if(razv<7){x++;}
      if(razv==7){x=x+2;}
      if(razv==8){x=x+3;} 
      display.drawLine(x, 47-mass[y]/7, x+1, 47-mass[y+1]/7, BLACK);
      display.drawLine(x+1, 47-mass[y]/7+1, x+2, 47-mass[y+1]/7+1, BLACK);        
}
sinhMASS=0;}
if(paus==1){
display.clearDisplay();
display.drawLine(prokr/8,8,prokr/8+6,8, BLACK);//шкала прокрутки
display.drawLine(prokr/8,9,prokr/8+6,9, BLACK);//шкала прокрутки
          x=3;
          for(int y=prokr;y<prokr+80;y++){
                if(razv<7){x++;}
                if(razv==7){x=x+2;}
                if(razv==8){x=x+3;} 
                display.drawLine(x, 47-mass[y]/7, x+1, 47-mass[y+1]/7, BLACK);
                display.drawLine(x+1, 47-mass[y]/7+1, x+2, 47-mass[y+1]/7+1, BLACK); 
          }}
//#######################################отрисовка графика
for(byte i=47;i>5;i=i-7){display.drawPixel(0,i, BLACK);display.drawPixel(1,i, BLACK);display.drawPixel(2,i, BLACK);}//разметка экрана  вертикальная
//////////////////////////////////////////////////сетка
for(byte i=47;i>5;i=i-3){display.drawPixel(21,i, BLACK);display.drawPixel(42,i, BLACK);display.drawPixel(63,i, BLACK);}
for(byte i=3;i<84;i=i+3){display.drawPixel(i,33, BLACK);display.drawPixel(i,19, BLACK);}
//////////////////////////////////////////////////сетка
//#######################################отрисовка menu
if(menu==0){
    display.setCursor(0,0);
    display.setTextColor(WHITE,BLACK);
    if(opornoe==0){display.print("1.1");}
    if(opornoe==1){display.print(VCC,1);}
    display.setTextColor(BLACK); 
    display.print(" ");
    display.print(razv);
    display.print(" P");
    if(digitalRead(levo)==HIGH){opornoe=!opornoe;}
    if(digitalRead(pravo)==HIGH){opornoe=!opornoe;}    
}
if(menu==1){
    display.setCursor(0,0);
    display.setTextColor( BLACK);
    if(opornoe==0){display.print("1.1");}
    if(opornoe==1){display.print(VCC,1);}
    display.setTextColor(WHITE, BLACK); // 'inverted' text 
    display.print(" ");
    display.print(razv);
    display.setTextColor( BLACK); // 'inverted' text
    display.print(" P");
    if(digitalRead(levo)==HIGH){razv=razv-1;if(razv==255){razv=0;}}
    if(digitalRead(pravo)==HIGH){razv=razv+1;if(razv==9){razv=8;}}
}
if(menu==2){
    display.setCursor(0,0);
    display.setTextColor( BLACK);
    if(opornoe==0){display.print("1.1");}
    if(opornoe==1){display.print(VCC,1);}
    display.print(" ");
    display.print(razv);
    display.setTextColor(WHITE, BLACK); // 'inverted' text 
    display.print(" P");
    paus=1;
    if(digitalRead(levo)==HIGH){prokr=prokr-10;if(prokr<0){prokr=0;}}
    if(digitalRead(pravo)==HIGH){prokr=prokr+10;if(prokr>620){prokr=620;}}    
}
if(menu==3){
    prokr=0;
    paus=0;
    display.setCursor(0,0);
    display.setTextColor( BLACK);
    if(opornoe==0){display.print("1.1");}
    if(opornoe==1){display.print(VCC,1);}
    display.print(" ");
    display.print(razv);
    display.setTextColor(BLACK);
    display.print(" P");
    if(digitalRead(levo)==HIGH){SinU=SinU-20;if(SinU<20){SinU=20;}}
    if(digitalRead(pravo)==HIGH){SinU=SinU+20;if(SinU>230){SinU=230;}}   
    display.fillCircle(80,47-SinU/7, 5, BLACK);
    display.fillCircle(80,47-SinU/7, 2, WHITE); 
}
if(digitalRead(ok)==HIGH){menu++;if(menu==4){menu=0;paus=0;}}//перебор меню
if (FreqCount.available()) { count = FreqCount.read();}//вывод частоты по готовности счетчика
//#######################################частоты сигнала
byte Frec1=0;
long Frec=0;
bool flagFrec1=0;
bool flagFrec2=0;
bool flagFrec3=0;
for(int y=1;y<255;y++){
  if(flagFrec1==0){if(mass[y]<SinU){flagFrec2=1;}}
  if(flagFrec1==0){if(flagFrec2==1){if(mass[y]>SinU){flagFrec1=1;Frec1=y;}}}
  if(flagFrec1==1){if(mass[y]<SinU){flagFrec3=1;}}
  if(flagFrec3==1){if(mass[y]>SinU){
  if (razv>=6){Frec=1000000/((y-Frec1-1)*3.27);}//delitel 4
  if (razv==5){Frec=1000000/((y-Frec1)*3.27)/2;}//delitel 8
  if (razv==4){Frec=1000000/((y-Frec1)*3.27)/4;}//delitel 16
  if (razv==3){Frec=1000000/((y-Frec1)*3.27)/8;}//delitel 32
  if (razv==2){Frec=1000000/((y-Frec1)*3.27)/16;}//delitel 64
  if (razv==2){Frec=1000000/((y-Frec1)*3.27)/32;}//delitel 128
  if (razv==1){Frec=1000000/((y-Frec1)*3.27)/32;}//delitel 128
  if (razv==0){Frec=1000000/((y-Frec1)*500);}//delitel 128
  flagFrec1=0;flagFrec3=0;}}}
//#######################################частоты сигнала
display.setTextColor( BLACK);
if(opornoe==1){
if((Vmax*VCC/255)>2.5){countX=count*(overclock/16.0);}
if((Vmax*VCC/255)<2.5){countX=Frec*(overclock/16.0);}}
if(opornoe==0){countX=Frec*(overclock/16.0);}
if(countX<1000){display.print(" ");display.print(countX);display.print("Hz");}
if(countX>1000){float countXK=countX/1000.0;display.print(countXK,1);display.print("KHz");}
if(opornoe==1){display.setCursor(0,40);display.setTextColor(BLACK);
display.print(Vmax*VCC/255,1);}
if(opornoe==0){display.setCursor(0,40);display.setTextColor(BLACK);
display.print(Vmax*1.1/255,1);}
display.print("V");
//#######################################отрисовка menu
delay(200);  
display.display();
}
if(pultoskop==1){Generator();}
if(pultoskop==2){DDSGenerator();}
if(pultoskop==3){TTL();}
}
//#######################################режим ренератора
void Generator(){
display.clearDisplay();  
if (flag==0){//флаг выборов режима настройки ШИМ или Частоты
            if(digitalRead(levo)==HIGH){
              frequency=frequency-mnog;
              if(frequency<0){frequency=0;}
              bool success = SetPinFrequencySafe(led, frequency);
              delay(3);//защита от дребезга 
            }
            if(digitalRead(pravo)==HIGH){
              frequency=frequency+mnog;
              bool success = SetPinFrequencySafe(led, frequency);
              delay(3);//защита от дребезга 
            }  
}
if (flag==1){//флаг выборов режима настройки ШИМ или Частоты
            if(digitalRead(levo)==HIGH){
              PWM=PWM-1;
              if(PWM<0){PWM=255;} 
              delay(3);//защита от дребезга
            
            }
            if(digitalRead(pravo)==HIGH){
              PWM=PWM+1;
              if(PWM>255){PWM=0;} 
              delay(3);//защита от дребезга 
            }  
}      
if(digitalRead(ok)==HIGH){//переключение разряда выбора частоты 
  delay(3);//защита от дребезга
  hag++;
  if(hag>=5){hag=0;}
} 
////////////
display.setTextSize(1);
display.setCursor(0,5);
display.print("PWM=");
display.print(PWM*100.0/255);
display.print(" %");
display.drawLine(0,0,83*PWM/255.0,0, BLACK);
display.drawLine(0,1,83*PWM/255.0,1, BLACK);
display.drawLine(0,2,83*PWM/255.0,2, BLACK);
display.drawLine(0,15,83*PWM/255.0,15, BLACK);
display.drawLine(0,16,83*PWM/255.0,16, BLACK);
display.drawLine(0,17,83*PWM/255.0,17, BLACK);
///////////    
display.setCursor(5,20);
 display.setTextSize(2);
long frequencyX=frequency*(overclock/16.0);
if(frequencyX<1000){display.print(frequencyX);display.setTextSize(1);display.println("Hz");}
if(frequencyX>1000){if(frequencyX<10000){display.print((frequencyX/1000.0),2);display.setTextSize(1);display.println("KHz");}}
if(frequencyX>=10000){if(frequencyX<100000){display.print((frequencyX/1000.0),1);display.setTextSize(1);display.println("KHz");}}
if(frequencyX>=100000){display.print((frequencyX/1000.0),0);display.setTextSize(1);display.println("KHz");}
display.setCursor(0,40);
display.setTextSize(1);
display.print(">>X ");    
      if(hag==0){//выбор множителя частоты
        
          display.print(1*(overclock/16.0),1); 
          mnog=1;
          flag=0;
      }
      if(hag==1){//выбор множителя частоты
          display.print(10*(overclock/16.0),0); 
          mnog=10;
      }
      if(hag==2){//выбор множителя частоты
          display.print(100*(overclock/16.0),0); 
          mnog=100;
      }
      if(hag==3){//выбор множителя частоты
          display.print(1000*(overclock/16.0),0); 
          mnog=1000;
      } 
      if(hag==4){//выбор  PWM
          display.print("PWM ");
          display.print(PWM*100.0/255);
          display.print("%"); 
          flag=1;
      } 
display.print("<<");        
pwmWrite(led, PWM);
delay(300);  
display.display();
}
/////////////////////DDS
void DDSGenerator(){
int fr=10;
if(menuDDS==0){
    display.clearDisplay(); 
    display.setTextColor(WHITE, BLACK); // 'inverted' text
    display.setCursor(10,0);
    display.println("Синус");
    display.setTextColor(BLACK);
    display.setCursor(10,10); 
    display.println("Треугольник");
    display.setCursor(10,20);
    display.println("Пила");
    display.setCursor(10,30);
    display.println("Пила Обр");
    display.setTextColor(BLACK);
    display.setCursor(0,40);
    //display.print("Частота=");
    //display.print(57);
    //display.print("Гц"); 
    delay(100);  
    display.display();
    while(D11_Read==LOW){
      PWM=sinM[d];
      pwmWrite(dds,PWM);
      //delayMicroseconds(fr);
      d++;
      if(d==32){d=0;}}
    menuDDS++;
    delay(200);}
if(menuDDS==1){
    display.clearDisplay(); 
    display.setTextColor(BLACK); // 'inverted' text
    display.setCursor(10,0);
    display.println("Синус");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(10,10); 
    display.println("Треугольник");
    display.setTextColor(BLACK);
    display.setCursor(10,20);
    display.println("Пила");
    display.setCursor(10,30);
    display.println("Пила Обр");
    display.setTextColor(BLACK);
    //display.setCursor(0,40);
    //display.print("Частота=");
   // display.print(57);
    //display.print("Гц");
    delay(100);  
    display.display();
    while(D11_Read==LOW){
      PWM=trianglM[d];
      pwmWrite(dds,PWM);
      //delayMicroseconds(fr);
      d++;
      if(d==32){d=0;}}
    menuDDS++;
    delay(200);}
if(menuDDS==2){
    display.clearDisplay(); 
    display.setTextColor(BLACK); // 'inverted' text
    display.setCursor(10,0);
    display.println("Синус");
    display.setTextColor(BLACK);
    display.setCursor(10,10); 
    display.println("Треугольник");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(10,20);
    display.println("Пила");
    display.setTextColor(BLACK);
    display.setCursor(10,30);
    display.println("Пила Обр");
    display.setTextColor(BLACK);
    //display.setCursor(0,40);
    //display.print("Частота=");
   // display.print(57);
    //display.print("Гц");
    delay(100);  
    display.display();
    while(D11_Read==LOW){
      PWM=pilaM[d];
      pwmWrite(dds,PWM);
     // delayMicroseconds(fr);
      d++;
      if(d==32){d=0;}}
      menuDDS++;
    delay(200);}
if(menuDDS==3){
    display.clearDisplay(); 
    display.setTextColor(BLACK); // 'inverted' text
    display.setCursor(10,0);
    display.println("Синус");
    display.setTextColor(BLACK);
    display.setCursor(10,10); 
    display.println("Треугольник");
    display.setTextColor(BLACK);
    display.setCursor(10,20);
    display.println("Пила");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(10,30);
    display.println("Пила Обр");
    display.setTextColor(BLACK);
    //display.setCursor(0,40);
    //display.print("Частота=");
   // display.print(57);
   // display.print("Гц");
    delay(100);  
    display.display();
    while(D11_Read==LOW){
      PWM=RpilaM[d];
      pwmWrite(dds,PWM);
      //delayMicroseconds(fr);
      d++;
      if(d==32){d=0;}}
      menuDDS++;
    delay(200);}
if(menuDDS==4){menuDDS=0;}

}
/////////////////////DDS
/////////////////////TTL
void TTL(){
display.clearDisplay(); 
display.setTextColor(BLACK); 
display.setCursor(10,0);
display.println("Терминал");
display.setCursor(10,10);
display.println("Скорость");
display.setCursor(10,20);
display.print("-");
display.print(speedTTL);
display.println("+");
display.setCursor(0,30);
display.println("Нажми ОК-старт");
if(digitalRead(pravo)==HIGH){speedTTL=speedTTL+100;}
if(digitalRead(levo)==HIGH){speedTTL=speedTTL-100;}
if(speedTTL<0){speedTTL=250000;}
if(speedTTL>250000){speedTTL=0;}
if(digitalRead(ok)==HIGH){Serial.begin(speedTTL*(16/overclock));
      display.clearDisplay(); 
      delay(100);  
      display.display();
      int x=0;
      int y=0; 
      while(1){ 
            char incomingBytes;  
            if (Serial.available() > 0) { // Если в буфере есть данные
                incomingBytes=Serial.read(); // Считывание байта в переменную incomeByte
                display.setCursor(x,y);
                display.print(incomingBytes); // Печать строки в буффер дисплея
                display.display(); x=x+6;
                if(x==84){x=0;y=y+8;}
                if(y==48){x=0;y=0;
                  display.clearDisplay(); 
                  delay(100);  
                  display.display();}}         
      }} 
delay(100);  
display.display();  
}
/////////////////////TTL
