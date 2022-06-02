#include <LiquidCrystal.h>

LiquidCrystal lcd(12, NULL, 11, 10,9,8,7);

byte semnOhm[8] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000
};

int analogPinO = 0;
int analogPinV = 1;
int analogPinA = 2;
int in = 0;
int ino_copy = 0;
int inv_copy = 0;
int curent_copy = 0;
int Vin = 5;
float Vout = 0;
float v = 0;
float curent = 0;
float R1_ohm = 5000;
float R2_ohm = 0;
float R1_volt = 10000;
float R2_volt = 1000;
float R1_a = 50;
float R2_a = 5;
int metru = 0;
bool switched = false;

void setup()
{
	lcd.begin(16, 2);
  	lcd.createChar(0, semnOhm);
    pinMode(analogPinO, INPUT);
    pinMode(analogPinV, INPUT);
    pinMode(analogPinA, INPUT);
    Serial.begin(9600);
  
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
  
    attachInterrupt(digitalPinToInterrupt(2),ISR_b1,FALLING);
    attachInterrupt(digitalPinToInterrupt(3),ISR_b2,FALLING);
  //activare PCI2 (buton 3 - PCINT20 - PCI2)
    PCICR  |= (1 << PCIE2);
    PCMSK2 |= (1<<PCINT20);
  
}

void ohmetru(){
	in = analogRead(analogPinO);
	if((in != ino_copy || switched)) 
	{
        lcd.clear();
        ino_copy = in;
		Vout= (in*Vin)/1023.0;
		R2_ohm = R1_ohm * (Vin/Vout - 1);
      
        lcd.setCursor(0, 0);
        lcd.print("Ohmmetru");

		lcd.setCursor(0, 1);
		lcd.print("R = ");
        if(R2_ohm > 1000 && R2_ohm < 1000000){
        	lcd.print(R2_ohm/1000);
            lcd.print("k");
        	lcd.write((byte)0);
        }
        else if(R2_ohm > 1000000){
            lcd.print(R2_ohm/1000000);
            lcd.print("M");
        	lcd.write((byte)0);
        }   
        else if(R2_ohm < 1){
        	lcd.print(R2_ohm*1000);
            lcd.print("m");
            lcd.write((byte)0);
      	}
        else{
        	lcd.print(R2_ohm);
            lcd.write((byte)0);
        }
		
		delay(1000);
	}
}

void voltmetru(){
	in = analogRead(analogPinV);
    if((in != inv_copy || switched)) 
	{
        lcd.clear();
        inv_copy = in;
		Vout= (in*Vin)/1023.0;
		v = Vout / (R2_volt/(R1_volt+R2_volt));
      
        lcd.setCursor(0, 0);
        lcd.print("Voltmetru");

		lcd.setCursor(0, 1);
		lcd.print("V = ");
        if(v > 1000){
        	lcd.print(v/1000);
            lcd.print("kV");
        }
        else if(v < 1){
        	lcd.print(v*1000);
            lcd.print("mV");
      	}
        else{
        	lcd.print(v);
            lcd.print("V");
        }
		
		delay(1000);
	}
      
}

void ampermetru(){
	in = analogRead(analogPinA);
    Vout= (in*Vin)/1023.0;
    curent = (Vout / (R2_a/(R1_a+R2_a))/(R1_a+R2_a));
    if((curent != curent_copy || switched)) 
	{
        lcd.clear();
        curent_copy = curent;
      
        lcd.setCursor(0, 0);
        lcd.print("Ampermetru");

		lcd.setCursor(0, 1);
		lcd.print("I = ");
        if(curent > 1000){
        	lcd.print(curent/1000);
            lcd.print("kA");
        }
        else if(curent < 1 && curent > 0.001){
        	lcd.print(curent*1000);
            lcd.print("mA");
      	}
        else if(curent < 0.001){
          lcd.print(curent * 1000000);
          lcd.print("uA");   
        }
        else{
        	lcd.print(curent);
            lcd.print("A");
        }
		
		delay(1000);
	}
    	  
}

void loop()
{
    if(metru == 0){
       ohmetru();
       switched = false;
    }
    else if(metru == 1){
    	ampermetru();
        switched = false;
    }
  	else{
    	voltmetru();
        switched = false;
    }   
}

void ISR_b1() 
{
  metru = 1;
  switched = true;
  _delay_ms(200);
}

void ISR_b2() 
{
  metru = 2;
  switched = true;
  _delay_ms(200);
}

ISR(PCINT2_vect) 
{
 metru = 0;
 switched = true;
  _delay_ms(200);
}