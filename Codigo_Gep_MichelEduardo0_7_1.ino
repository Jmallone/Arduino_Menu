/*

 
 Versao: 0.7.1
 
 MudanÃ§a: 
 
 0.4 - IndentaÃ§ao e correÃ§ao de algumas comparaÃ§oes redundantes no codigo
 0.5 - a rotina de limpesa estava redudante entao foi feita uma rotina para toda vez que ultilizar ela
 
 0.5.1 -  Tentando fazer um SubMenu
 
 0.5.2 - Aprimoramento de como eh feito o menu
 
 0.5.3 - Testando uns SubMenu
 Bugs
 - Na hora de voltar para o menu principal nao voltava certo,
 
 0.6 - Funcionando Os subs menus
 
 0.7 - Aprimoramento do sub-menu, agora a selecao eh feita com debounce, coisa que antes poderia dar alguns bugs se apertado continuamente os botoes de selecionar.
 
 0.7.1 - Testando como o usuario ira selecionar os numeros.
 
 */


#include "LiquidCrystal.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Definindo variavels Globals e Uns defines
#define tam_lcd 20
#define SELECT 7
#define DIR 9
#define ESQ 5


int bt_down = 0;
int debounce = 0;
int num_port_analog = A0;
int debug = 0;
int last = 0;
int now = 0;
int n_menu = 0;
int lcd_limpa = 0;
int total_de_matriz;
int menu_sel = 0;
int sub, ab;
int a = 1;
int aa = 1000;
int lado;
boolean teste = true;

byte seta[8] = {
  0b00000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b00000,
  0b00000
};

byte seta_line[8] = {
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};


byte lado_dir[8] = {
  0b01000,
  0b01100,
  0b01110,
  0b01111,
  0b01110,
  0b01100,
  0b01000,
  0b00000
};

byte lado_esq[8] = {
  0b00010,
  0b00110,
  0b01110,
  0b11110,
  0b01110,
  0b00110,
  0b00010,
  0b00000
};

// Menus	
char menu[][tam_lcd +1]={
  "1 Cursor Slider", // 0
  "2 Tempo de Lapso", // 1
  "3 Numero de Parada", // 2
  "4 Delay da Camera", // 3
  "5 Numero Disparos",// 4
  "6 Movimento Linear",// 5
  "7 Op 07",// 5
  "8 Op 08", // 1
  "9 Op 09", // 2
  "10 Op 10", // 3                        
};

// Sub Menu de Cursor slider
char sub_op1[][tam_lcd +1]={
  "1 Sub Menu do Op1", // 0
  "2 Sub Menu do Op1", // 1
  "3 Sub Menu do Op1", // 2
  "4 sub menu op1", // 3
  "5 Sub Menu do Op1", // 1
  "6 Sub Menu do Op1", // 2
  "7 Sair", // 3                          
};

char sub_op2[][tam_lcd +1]={
  "1 Teste com Numero", // 0
  "2 Teste com Letras", // 1
  "7 Sair",
  " ",                          
};

int num[]={0, 0, 0, 0,};

void setup()
{
  Serial.begin(9600); 
  lcd.begin(20, 4); // tamanho do lcD
  lcd.createChar(1, seta);
  lcd.createChar(2, lado_esq);
  lcd.createChar(3, lado_dir);
  lcd.createChar(4, seta_line);
}

void imprimemenu(int valor, int matriz)
{ 
  total_de_matriz = matriz-4;
  // Imprimi Menu e como tambem eh um parametro valor recebera n_menu que esta la em baixo no loop
  for(int i =0; i<4; i++)
  {
    lcd.setCursor(2, i);
    lcd.print(menu[valor+i]);	
  }
}

void imp_sub(int valor2, int matriz2)
{ 
  total_de_matriz = matriz2-4;
  // Imprimi Menu e como tambem eh um parametro valor recebera n_menu que esta la em baixo no loop
  for(int a =0; a<4; a++)
  {
    lcd.setCursor(2, a);
    lcd.print(sub_op1[valor2+a]);	
  }
}

void imp_sub2(int valor3, int matriz3)
{ 
  total_de_matriz = matriz3-4;
  // Imprimi Menu e como tambem eh um parametro valor recebera n_menu que esta la em baixo no loop
  for(int a =0; a<4; a++)
  {
    lcd.setCursor(2, a);
    lcd.print(sub_op2[valor3+a]);	
  }
}

void imprimeseta( int v_seta)// como isso eh um parametro v_seta recebe o valor da variavel bt_down que esta la em baixo no loop
{ 
  lcd.setCursor(0,v_seta); // ponha a seta na linha que voce passou o numero dentro do  imprimeseta( ); <-- dentro daqui, ali no loop
  lcd.write(1);
}

void imprimeseta_num( int v_seta)// como isso eh um parametro v_seta recebe o valor da variavel bt_down que esta la em baixo no loop
{ 
  lcd.setCursor(v_seta,3); // ponha a seta na linha que voce passou o numero dentro do  imprimeseta( ); <-- dentro daqui, ali no loop
  lcd.write(4);
}

void imprime_btt()
{
  lcd.setCursor(5,2); // ponha a seta na linha que voce passou o numero dentro do  imprimeseta( ); <-- dentro daqui, ali no loop
  lcd.write(2);

  lcd.setCursor(11,2); // ponha a seta na linha que voce passou o numero dentro do  imprimeseta( ); <-- dentro daqui, ali no loop
  lcd.print("OK");
}

void ler_botao_num(){
  //limpa_tela();  
  int leitura_botao = analogRead(num_port_analog)/100;
  Serial.println(leitura_botao); // Pega  aleitura e passa para o Serial Monitor
  Serial.println(" ");
  // Aqui começa a Comparaçao para descer
  if ( leitura_botao ==  DIR)
  {
    lado ++ ;
    delay(300);
    lcd.clear();
  }
  if (lado == 4)
  {
    lado = 9;
    lcd.clear();
  }
  if (lado == 11)
  {
    imprimeseta_num(12);
  }
  if (lado == 12)
  {
    lado = 6;
    lcd.clear();
  }

  // -- FUNCOES DO SELECT
  if ( leitura_botao == SELECT )
  {
    ab++;
    
    // SAIR
    if(ab >= 8 && lado == 11)
    {
      teste = false;
      a = 1;
      ab = 0;
    }
    
    if (ab >= 8 && lado == 9)
    {
      num[0] = num[0] + 1;
      if (num[0] == 10)
      {
        aa = aa - num[0];
        num[0] = 0;
      }
      aa = aa + 1;
      delay(300);
      lcd.clear();
      ab = 0;
    }      

    if (ab >= 8 && lado == 8)
    {
      num[1] = num[1] + 10;
      if (num[1] == 100)
      {
        aa = aa - num[1];
        num[1] = 0;
      }        
      aa = aa + 10;
      delay(300);
      lcd.clear();
      ab = 0;
    }

    if (ab >= 8 && lado == 7)
    {
      num[2] = num[2] + 100;
      if (num[2] == 1000)
      {
        aa = aa - num[2];
        num[2] = 0;
      }           
      aa = aa + 100;
      delay(300);
      lcd.clear();
      ab = 0;
    } 

    if (ab >= 8 && lado == 6)
    {
      num[3] = num[3] + 1000;
      if (num[3] == 10000)
      {
        aa = aa - num[3];
        num[3] = 1000;
        aa = aa+num[3];
      }       
      aa = aa + 1000;
      delay(300);
      lcd.clear();
      ab = 0;
    }     

  }    

  // Aqui começa as comparaçao para Subir
  if ( leitura_botao == ESQ ) // se estiver entre 900 a 940 faz esse comando 
  {
    lado -- ;
    delay(300);
    lcd.clear();
  }
}

void ler_botao(){
  //limpa_tela();  
  int leitura_botao = analogRead(num_port_analog)/100;
  Serial.println(leitura_botao); // Pega  aleitura e passa para o Serial Monitor
  Serial.println(" ");
  // Aqui comeÃ§a a ComparaÃ§ao para descer
  if ( leitura_botao ==  ESQ)
  {
    bt_down ++ ;
    delay(300);
    lcd.clear();
  }

  if ( leitura_botao == SELECT ) //Botao selecionar
  {
    // Debounce para o botao selecionar
    now = debounce;
    int b_last = last + 49;
    if (b_last <= now)
    {
      a ++;

    }
    last = now;

    //--Menu principal
    if (a >= 2 && bt_down == 0 && n_menu == 0 && menu_sel == 0) // Op 1
    {
      a = 0;
      menu_sel = 1;
      sub = 1;
      limpa_tela();
    }

    if (a >= 2 && bt_down == 1 && n_menu == 0 && menu_sel == 0) // Op2
    {
      a = 0;
      bt_down = 0; // Nao precissa, ponhei so pra ponhar
      menu_sel = 1;
      sub = 2;
      limpa_tela();
    }
    //-- Fim Menu principal

    // sub menus
    if (a >= 2 && bt_down == 0 && n_menu == 0 && sub == 2)
    {
      limpa_tela();
      //aa = 0;
      debug ++;
      lado = 5;
      while(teste){
        imprimeseta_num( lado ); // 0 primeira linha se mudar o 0 para 1 vai pra segunda linha
        ler_botao_num();
        lcd.setCursor(6, 2);
        lcd.print(aa);
        imprime_btt();
      }
      limpa_tela();
      //aa = 1;
      //ab = 0;
      a = 0;
      lado = 6;
      teste = true;
      delay(100);

    }

  }    

  // Aqui comeÃ§a as comparaÃ§ao para Subir
  if ( leitura_botao == DIR ) // se estiver entre 900 a 940 faz esse comando 
  {
    bt_down -- ;
    delay(300);
    lcd.clear();
  }
}

void limpa_tela()
{
  delay(50);
  lcd.clear();
  lcd_limpa = 1;    
}

void sair()
{
  if( a == 2 && bt_down == 3 && n_menu == 3 && menu_sel == 1) // Sair da OP1
  { 
    if (sub == 1 )
    {
      bt_down = sub-1;
      sub = 0;
      n_menu = 0;
      menu_sel = 0;
      limpa_tela();
      a = 0;
    }
  }

  if( a == 2 && bt_down == 2 && n_menu == 0 && menu_sel == 1) // aqui sair da OP2
  { 
    if (sub == 2 )
    {
      bt_down = sub-1;
      sub = 0;
      n_menu = 0;
      menu_sel = 0;
      limpa_tela();
      a = 0;
    }
  }

}

void compara_menu()
{
  // se a seta estiver na Primeira opÃ§ao e apertar para cima vai para ultima OpÃ§ao
  if(n_menu == 0 &&  bt_down == -1)
  {
    bt_down = 3;
    n_menu = total_de_matriz;
    limpa_tela();
  }

  // se a seta estiver na ultima opÃ§ao e apertar para baixo vai para a primeira opÃ§ao 
  if(n_menu == total_de_matriz && bt_down == 4)
  {
    bt_down = 0;
    n_menu = 0;
    limpa_tela();
  }

  if(bt_down == -1){
    bt_down = 0;
    n_menu = n_menu - 1;
    limpa_tela();
  }

  if(bt_down == 4){
    bt_down = 3;
    n_menu = n_menu + 1;
    limpa_tela();
  }
  
  
  if(n_menu == -1) 
  {
    n_menu = 0;
  }
  // para o sub menu da OP 2
  
  if(bt_down == 3 && sub == 2 && menu_sel == 1)
  {
    n_menu = 0;
    bt_down = 0;
  }
  
  
}

void loop()
{
  debounce = millis();
  imprimeseta( bt_down ); // 0 primeira linha se mudar o 0 para 1 vai pra segunda linha
  ler_botao(); 
  compara_menu(); // Compara o menu kk
  if(menu_sel == 0)
  {
    imprimemenu( n_menu, 10 ); // Imprimi a index da array em 0,0, no caso esse 0 eh a index
  } 
  else
  {
    switch(sub){
    case 1:
      imp_sub(n_menu, 7 );
      sair();
      break;
    case 2:
      imp_sub2(n_menu, 3 );
      sair();
      break;        
    }
  }
}

