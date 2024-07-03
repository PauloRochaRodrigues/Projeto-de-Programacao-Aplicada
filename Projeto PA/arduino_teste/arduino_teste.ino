#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define btt_SELEC 8
#define btt_MUDAR 7
#define ino_PALAVRA 4
 
char letras[] = "AEIOU";
int letra_selec = 0;
int cont_char = 0;

char palavra[ino_PALAVRA];

bool reticencias_run = true;

byte pal;

int incomingByte = 0;

byte btt_m;
byte btt_s;

byte opcao = 0;
byte aguarde = 0;

bool M_state = false;
bool S_state = false;

void Menu_Canvas(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Escolha:");
  lcd.setCursor(0,1);
  lcd.print("( ) PC - ( ) Ino");
}

void setup(){
  pinMode(btt_MUDAR, INPUT_PULLUP);
  pinMode(btt_SELEC, INPUT_PULLUP);

  Serial.begin(9600);
  //Serial.write("Iniciando sistema...");

  lcd.init();
  lcd.backlight();    
  lcd.print(" Seja bem vindo! ");
  lcd.setCursor(0,1);   
  lcd.print(" Iniciando...   ");

  delay (1000);

  Menu_Canvas();
}

void Menu(byte value){
  
  if(value==1){
    lcd.setCursor(10,1);
    lcd.print(" ");
    lcd.setCursor(1,1);
    lcd.print("X");
  }else if(value==2){
    lcd.setCursor(10,1);
    lcd.print("X");
    lcd.setCursor(1,1);
    lcd.print(" ");
  }

}

void reticencias(){
  if (reticencias_run){
    lcd.setCursor(0,0);
    if(aguarde==0){
      lcd.setCursor(8,1);
      lcd.print("   ");
      aguarde++;
    }else if(aguarde==1){
      lcd.setCursor(8,1);
      lcd.print(".");
      aguarde++;
    }else if(aguarde==2){
      lcd.setCursor(8,1);
      lcd.print("..");
      aguarde++;
    }else if(aguarde==3){
      lcd.setCursor(8,1);
      lcd.print("...");
      aguarde = 0;
    }
  }
}

bool inoScreen(){
  if (Serial.available() > 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mensagem:");
    lcd.setCursor(0, 1);
    while(true){
      incomingByte = Serial.read();
      lcd.write(incomingByte);
      if (Serial.available() <= 0){break;}
    }
    return true;
    
  }else{
    return false;
  }
}

void opcChange(){
  if(opcao==0){
    opcao++;
  }else if(opcao==1){
    opcao++;
  }else{
    opcao--;
  }
}

bool running_menu = false;

void loop(){
  Menu_Canvas();
  Menu(opcao);
  running_menu = true;
  while(running_menu){
    btt_m = digitalRead(btt_MUDAR);
    btt_s = digitalRead(btt_SELEC);

    if(!btt_m && !M_state){
      //Serial.write("\nMudar pressionado");
      opcChange();
      Menu(opcao);
      M_state = true;
    }else if(btt_m){
      M_state = false;
    }

    if(!btt_s && !S_state){
      //Serial.write("\nSelecionar pressionado");
      S_state = true;
      running_menu = false;
      //Serial.write(running_menu);
    }else if(btt_s){
        S_state = false;
      }
  }

  if(opcao==1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Aguardando");
    lcd.setCursor(0,1);
    lcd.print("mensagem");
    lcd.setCursor(0, 0);
    bool aguardando = true;
    
    while(aguardando){
      btt_s = digitalRead(btt_SELEC);
      
      if (inoScreen()){
        reticencias_run = false;
      }

      reticencias();
      delay(500);

      if(!btt_s && !S_state){
      Serial.write("\nSelecionar pressionado");
      S_state = true;
      aguardando = false;
      }else if(btt_s){
          S_state = false;
        }
      }

  }
  if(opcao==2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Escreva:");
    lcd.setCursor(0,1);

    while(true){
      if (cont_char > ino_PALAVRA){
        break;
      }
      btt_m = digitalRead(btt_MUDAR);
      btt_s = digitalRead(btt_SELEC);
      lcd.setCursor(cont_char,1);
      lcd.print(letras[letra_selec]);
      
      if(!btt_m && !M_state){
        //Serial.write("\nMudar pressionado");
        letra_selec ++;
        if (letra_selec > 5){
          letra_selec = 0;
        }
        M_state = true;
      }else if(btt_m){
        M_state = false;
      }

      if(!btt_s && !S_state){
        //Serial.write("\nSelecionar pressionado");
        //lcd.setCursor(cont_char,1);
        palavra[cont_char] = letras[letra_selec];
        cont_char++;
        if (cont_char > 15){
          cont_char = 0;
        }
        S_state = true;
      }else if(btt_s){
        S_state = false;
      }
    }
    Serial.write(palavra);
    lcd.clear();
    lcd.setCursor(0,0);
    
    cont_char = 0;
    letra_selec = 0;
  }
  
}

 