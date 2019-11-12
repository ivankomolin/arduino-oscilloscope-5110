#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <FreqCount.h>
#include <PWM.h>
#include <CyberLib.h>

#define led 9 //пин для генератора сигналов (не менять)
#define dds 10 //пин для генератора dds (не менять)

//Пользовательские настройки
#define levo 13 //кнопка ЛЕВО(можно любой пин)
#define ok 12 //кнопка ОК(можно любой пин)
#define pravo 11 //кнопка ПРАВО(можно любой пин)
#define akb A5 //любой своюодный аналоговый пин для измерения напряжения АКБ
#define overclock 16 //Частота на которой работает Ардуино

//Настройки дисплея
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 4, 3, 2); 
byte cont = 52; //контрастность дисплея
byte SinU = 30; //уровень синхронизации 0 до 255

int PWM = 128; //стартовое значение ШИМ от 0 до 255
int32_t frequency = 500; //стартовое значение частоты в Гц
float VCC = 5.0; //напряжение питания, меряем мультиметром

int d = 0;
byte hag = 0;
int mnog = 0;
boolean flag = 0;
byte mass[701];
byte x = 0;
byte menu = 0; //переменная выбора меню
bool opornoe = 1; //флаг опорного напряжения
bool paus = 0; //флаг режима паузы
byte pultoskop = 0; //флаг выбора генератора или осциллографа
byte razv = 6;
unsigned long count = 0;
byte sinX = 30;
byte meaX = 83;
int Vmax = 0; // максимальное напряжение
byte sinhMASS = 0;
long countX = 0;
long speedTTL = 9600; //скорость терминала
int prokr = 0;

void setup()
{
    pinMode(A4, INPUT);
    display.begin();
    display.setContrast(cont);
    FreqCount.begin(1000);
    display.setTextColor(BLACK);
    delay(500);
}


void Zamer()
{
    if (razv >= 6)
    {
        ADCSRA = 0b11100010;
    } // delitel 4

    if (razv == 5)
    {
        ADCSRA = 0b11100011;
    } // delitel 8
    if (razv == 4)
    {
        ADCSRA = 0b11100100;
    } // delitel 16
    if (razv == 3)
    {
        ADCSRA = 0b11100101;
    } // delitel 32
    if (razv == 2)
    {
        ADCSRA = 0b11100110;
    } // delitel 64
    if (razv < 2)
    {
        ADCSRA = 0b11100111;
    } // delitel 128
    if (razv == 0)
    {
        for (int i = 0; i < 700; i++)
        {
            while ((ADCSRA & 0x10) == 0)
                ;
            ADCSRA |= 0x10;
            delayMicroseconds(500);
            mass[i] = ADCH;
        }
    }
    if (razv > 0)
    {
        for (int i = 0; i < 700; i++)
        {
            while ((ADCSRA & 0x10) == 0)
                ;
            ADCSRA |= 0x10;
            mass[i] = ADCH;
        }
    }
}


void loop()
{
    if (pultoskop == 0)
    {
        if (opornoe == 0)
        {
            ADMUX = 0b11100011;
        }

        //выбор внутреннего опорного 1,1В
        if (opornoe == 1)
        {
            ADMUX = 0b01100011;
        } 

        //Выбор внешнего опорного
        delay(5);
        if (paus == 0)
        {
            Zamer();
        }

        //Определение точки синхронизации
        bool flagSINHRO = 0;
        bool flagSINHRnull = 0;
        for (int y = 1; y < 255; y++)
        {
            if (flagSINHRO == 0)
            {
                if (mass[y] < SinU)
                {
                    flagSINHRnull = 1;
                }
            }
            if (flagSINHRO == 0)
            {
                if (flagSINHRnull == 1)
                {
                    if (mass[y] > SinU)
                    {
                        flagSINHRO = 1;
                        sinhMASS = y;
                    }
                }
            }
        }

        //Определение точки синхронизации
        //Максимальное значение сигнала##########################
        Vmax = 0;
        for (int y = 1; y < 255; y++)
        {
            if (Vmax < mass[y])
            {
                Vmax = mass[y];
            }
        }
        //Максимальное значение сигнала
        //Определение точки синхронизации
        //Отрисовка графика
        if (paus == 0)
        {
            display.clearDisplay();
            display.fillCircle(80, 47 - SinU / 7, 2, BLACK); //рисуем уровень синхронизации
            x = 3;
            for (int y = sinhMASS; y < sinhMASS + 80; y++)
            {
                if (razv < 7)
                {
                    x++;
                }
                if (razv == 7)
                {
                    x = x + 2;
                }
                if (razv == 8)
                {
                    x = x + 3;
                }
                display.drawLine(x, 47 - mass[y] / 7, x + 1, 47 - mass[y + 1] / 7, BLACK);
                display.drawLine(
                    x + 1, 47 - mass[y] / 7 + 1, x + 2, 47 - mass[y + 1] / 7 + 1, BLACK);
            }
            sinhMASS = 0;
        }
        if (paus == 1)
        {
            display.clearDisplay();
            display.drawLine(prokr / 8, 8, prokr / 8 + 6, 8, BLACK); //шкала прокрутки
            display.drawLine(prokr / 8, 9, prokr / 8 + 6, 9, BLACK); //шкала прокрутки
            x = 3;
            for (int y = prokr; y < prokr + 80; y++)
            {
                if (razv < 7)
                {
                    x++;
                }
                if (razv == 7)
                {
                    x = x + 2;
                }
                if (razv == 8)
                {
                    x = x + 3;
                }
                display.drawLine(x, 47 - mass[y] / 7, x + 1, 47 - mass[y + 1] / 7, BLACK);
                display.drawLine(
                    x + 1, 47 - mass[y] / 7 + 1, x + 2, 47 - mass[y + 1] / 7 + 1, BLACK);
            }
        }
        //Отрисовка графика
        for (byte i = 47; i > 5; i = i - 7)
        {
            display.drawPixel(0, i, BLACK);
            display.drawPixel(1, i, BLACK);
            display.drawPixel(2, i, BLACK);
        }
        
        //Сетка
        for (byte i = 47; i > 5; i = i - 3)
        {
            display.drawPixel(21, i, BLACK);
            display.drawPixel(42, i, BLACK);
            display.drawPixel(63, i, BLACK);
        }
        for (byte i = 3; i < 84; i = i + 3)
        {
            display.drawPixel(i, 33, BLACK);
            display.drawPixel(i, 19, BLACK);
        }

        //Отрисовка menu
        if (menu == 0)
        {
            display.setCursor(0, 0);
            display.setTextColor(WHITE, BLACK);
            if (opornoe == 0)
            {
                display.print("1.1");
            }
            if (opornoe == 1)
            {
                display.print(VCC, 1);
            }
            display.setTextColor(BLACK);
            display.print(" ");
            display.print(razv);
            display.print(" P");
            if (digitalRead(levo) == HIGH)
            {
                opornoe = !opornoe;
            }
            if (digitalRead(pravo) == HIGH)
            {
                opornoe = !opornoe;
            }
        }
        if (menu == 1)
        {
            display.setCursor(0, 0);
            display.setTextColor(BLACK);
            if (opornoe == 0)
            {
                display.print("1.1");
            }
            if (opornoe == 1)
            {
                display.print(VCC, 1);
            }
            display.setTextColor(WHITE, BLACK); // 'inverted' text
            display.print(" ");
            display.print(razv);
            display.setTextColor(BLACK); // 'inverted' text
            display.print(" P");
            if (digitalRead(levo) == HIGH)
            {
                razv = razv - 1;
                if (razv == 255)
                {
                    razv = 0;
                }
            }
            if (digitalRead(pravo) == HIGH)
            {
                razv = razv + 1;
                if (razv == 9)
                {
                    razv = 8;
                }
            }
        }

        if (menu == 2)
        {
            display.setCursor(0, 0);
            display.setTextColor(BLACK);
            if (opornoe == 0)
            {
                display.print("1.1");
            }
            if (opornoe == 1)
            {
                display.print(VCC, 1);
            }
            display.print(" ");
            display.print(razv);
            display.setTextColor(WHITE, BLACK); // 'inverted' text
            display.print(" P");
            paus = 1;
            if (digitalRead(levo) == HIGH)
            {
                prokr = prokr - 10;
                if (prokr < 0)
                {
                    prokr = 0;
                }
            }
            if (digitalRead(pravo) == HIGH)
            {
                prokr = prokr + 10;
                if (prokr > 620)
                {
                    prokr = 620;
                }
            }
        }

        if (menu == 3)
        {
            prokr = 0;
            paus = 0;
            display.setCursor(0, 0);
            display.setTextColor(BLACK);
            if (opornoe == 0)
            {
                display.print("1.1");
            }
            if (opornoe == 1)
            {
                display.print(VCC, 1);
            }
            display.print(" ");
            display.print(razv);
            display.setTextColor(BLACK);
            display.print(" P");
            if (digitalRead(levo) == HIGH)
            {
                SinU = SinU - 20;
                if (SinU < 20)
                {
                    SinU = 20;
                }
            }
            if (digitalRead(pravo) == HIGH)
            {
                SinU = SinU + 20;
                if (SinU > 230)
                {
                    SinU = 230;
                }
            }
            display.fillCircle(80, 47 - SinU / 7, 5, BLACK);
            display.fillCircle(80, 47 - SinU / 7, 2, WHITE);
        }

        if (digitalRead(ok) == HIGH)
        {
            menu++;
            if (menu == 4)
            {
                menu = 0;
                paus = 0;
            }
        }

        //Перебор меню
        if (FreqCount.available())
        {
            count = FreqCount.read();
        }

        //вывод частоты по готовности счетчика
        //Частоты сигнала
        byte Frec1 = 0;
        long Frec = 0;
        bool flagFrec1 = 0;
        bool flagFrec2 = 0;
        bool flagFrec3 = 0;
        for (int y = 1; y < 255; y++)
        {
            if (flagFrec1 == 0)
            {
                if (mass[y] < SinU)
                {
                    flagFrec2 = 1;
                }
            }
            if (flagFrec1 == 0)
            {
                if (flagFrec2 == 1)
                {
                    if (mass[y] > SinU)
                    {
                        flagFrec1 = 1;
                        Frec1 = y;
                    }
                }
            }
            if (flagFrec1 == 1)
            {
                if (mass[y] < SinU)
                {
                    flagFrec3 = 1;
                }
            }
            if (flagFrec3 == 1)
            {
                if (mass[y] > SinU)
                {
                    if (razv >= 6)
                    {
                        Frec = 1000000 / ((y - Frec1 - 1) * 3.27);
                    } // delitel 4
                    if (razv == 5)
                    {
                        Frec = 1000000 / ((y - Frec1) * 3.27) / 2;
                    } // delitel 8
                    if (razv == 4)
                    {
                        Frec = 1000000 / ((y - Frec1) * 3.27) / 4;
                    } // delitel 16
                    if (razv == 3)
                    {
                        Frec = 1000000 / ((y - Frec1) * 3.27) / 8;
                    } // delitel 32
                    if (razv == 2)
                    {
                        Frec = 1000000 / ((y - Frec1) * 3.27) / 16;
                    } // delitel 64
                    if (razv == 2)
                    {
                        Frec = 1000000 / ((y - Frec1) * 3.27) / 32;
                    } // delitel 128
                    if (razv == 1)
                    {
                        Frec = 1000000 / ((y - Frec1) * 3.27) / 32;
                    } // delitel 128
                    if (razv == 0)
                    {
                        Frec = 1000000 / ((y - Frec1) * 500);
                    } // delitel 128
                    flagFrec1 = 0;
                    flagFrec3 = 0;
                }
            }
        }
        //Частоты сигнала
        display.setTextColor(BLACK);
        if (opornoe == 1)
        {
            if ((Vmax * VCC / 255) > 2.5)
            {
                countX = count * (overclock / 16.0);
            }
            if ((Vmax * VCC / 255) < 2.5)
            {
                countX = Frec * (overclock / 16.0);
            }
        }
        if (opornoe == 0)
        {
            countX = Frec * (overclock / 16.0);
        }
        if (countX < 1000)
        {
            display.print(" ");
            display.print(countX);
            display.print("Hz");
        }
        if (countX > 1000)
        {
            float countXK = countX / 1000.0;
            display.print(countXK, 1);
            display.print("KHz");
        }
        if (opornoe == 1)
        {
            display.setCursor(0, 40);
            display.setTextColor(BLACK);
            display.print(Vmax * VCC / 255, 1);
        }
        if (opornoe == 0)
        {
            display.setCursor(0, 40);
            display.setTextColor(BLACK);
            display.print(Vmax * 1.1 / 255, 1);
        }
        display.print("V");
        //Отрисовка menu
        delay(200);
        display.display();
    }
}
