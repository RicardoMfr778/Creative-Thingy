
// Cathode RGB Led's 

int ledPins[2][3]={{11,9,10},{3,5,6}};
#define PIN_RED ledPins[0][0] 
#define PIN_GREEN ledPins[0][1]
#define PIN_BLUE ledPins[0][2]
//int PIN_RED = 11;
//int PIN_GREEN = 9;  USAR DEFINE p/ red blue green escolhido/
//int PIN_BLUE = 10;  adicionar as cores escolhidas tipo cores QF é [2] então em ledPins[2][i]
#define PIN_RED2 ledPins[1][0] 
#define PIN_GREEN2 ledPins[1][1]
#define PIN_BLUE2 ledPins[1][2]

typedef struct ColorChoice{
unsigned char ER1,EG1,EB1,ER2,EG2,EB2; 
 
}Struc;

int MainButtons[]={2,4};
#define button_OFF MainButtons[0]
#define button_ON MainButtons[1]
//int button_OFF= 2;
//int button_ON= 4;

int ChangeButtons[]={7,8,12,13};  // deixar de ser modo e ser add on a tds os outros modos
#define button_Rw ChangeButtons[0]
#define button_Fs ChangeButtons[1] 
#define button_QF ChangeButtons[2]
#define button_Dl ChangeButtons[3]
//int button_Rw = 7; //Rainbow Smooth
//int button_Fs=8;   //Flash
//int button_Pk=12;  //Pick your Color
//int button_Dl=13;  //DualColor

int click= 0;
int stayMain = 0;
int Change= 0;
int zero[]={0,0,0};

 
 // Para os controles de Brilho, 2 Butões secalhar ia limitar tipos de brilho, slider programavel move
int* Cptr= &click; 
int* Fptr = &zero[0];
int* Enderman = &zero[1];
int* counter = &zero[2];

//---------------------------- Como arrumar estas var?


// Number of colors used for animating, higher = smoother and slower animation)
int numColors = 255;

 // É a combinação de numColors e animationDelay que determina a
 //velocidade de animação, quanto maior o número de cores, mais lento é 
 //a animação. Higher number of colors = smoother color changing.
int animationDelay = 10; // Nº de ms entre mudança de cores

void setup() {
 
 pinMode(PIN_RED, OUTPUT);
 pinMode(PIN_BLUE, OUTPUT);
 pinMode(PIN_GREEN, OUTPUT);
  
  
 pinMode(button_OFF, INPUT);
 pinMode(button_ON, INPUT);
  
 pinMode(button_Rw, INPUT); 
 pinMode(button_Fs, INPUT);
 pinMode(button_QF, INPUT);
 pinMode(button_Dl, INPUT);
 
  
 Serial.begin(9600);
 
 
};
 
// Grad ~2.7 s de cycle kept over 1min



void loop() {
 	// This part takes care of displaying the
  // color changing in reverse by counting backwards if counter
  // is above the number of available colors  
   
	long color = ColorGenerator(counter, numColors, Change);  
 	// Get the red, blue and green parts from generated color
	int red = color >> 16 & 255;
	int green = color >> 8 & 255;
	int blue = color & 255;
  	

  	//Serial.print("MainLoop Enderman: ");
	//Serial.println(*Enderman);
	int B_off= 0;
	int B_on= 0;
	int B_rainbow= 0; 
	int B_flash= 0;
	int B_gradQF= 0;
	int B_dual = 0;
	
 
 
//------------------- como arrumo as variaveis acima?
  
 	B_on= digitalRead(button_ON); 
 	B_off= digitalRead(button_OFF);
 	B_rainbow= digitalRead(button_Rw); 
	B_flash= digitalRead(button_Fs);
 	B_gradQF= digitalRead(button_QF);
 	B_dual = digitalRead(button_Dl);

	Struc Choice;
  	Choice.EB1=70;
  	Choice.EG1=0;
  	Choice.ER1=0; //temporário -> input por app android
  	Choice.EB2=12;
  	Choice.EG2=0;
  	Choice.ER2=0;  	
  	//<- inserir valores apartir do Android 
  	//Serial.print(Choice.ER1); com map 255 para 12
  
	if (B_on ==1 || stayMain == 1) {
		stayMain=1;
    
    	if (B_rainbow==1 || Change==1 ){Change=1;}  
    	if (B_flash == 1 ){FlashMode(Fptr);}
   		if (B_gradQF== 1 || Change==2 ) {Change=2;}
    	if (B_dual== 1 || Change==3 ) {Change=3;}
   		
    	switch(Change){
   			case 1: //0.3 max brightness     		
      			
          		*counter = RainbowFunc(red, green, blue, counter, numColors, animationDelay, B_off);	       		
      			break;     
      		case 2: //max brightness 1 
      		    
          		GradFunc(Cptr,B_gradQF,B_off); //Generalizar NumColors?AnimationDelay
      			
				break;
			case 3: //max 1 brightness + tarde usar map nas vars android	
      		//Remote dar valores a estas var quando ativar butão converter 255->12
      			DualFunc(&Choice,B_off);
				break;      
        }//Changer
   
    }//MainIF    
	
  
}

//--------------Color Setters ou geradores----------

long HSBtoRGB(float _hue, float _sat, float _brightness) {
   float red = 0.0;
   float green = 0.0;     //converte HSB 0-360 0-1 0-1
   float blue = 0.0;	  //em RGB(0-12,0-12,0-12);
   						  //Tem cm objetivo retornar um inteiro onde os vários
   				// valores de Red,G,B, para um número, que será extraido do return 
   if (_sat == 0.0) {
       red = _brightness;
       green = _brightness;
       blue = _brightness;
   } else {
       if (_hue == 360.0) {
           _hue = 0;
       }

       int slice = _hue / 60.0;   			  //um circulo dividido em frac de 60º 6 slices
       float hue_frac = (_hue / 60.0) - slice;//cada slice corresponde a uma genese de cores
       //hue_frac é o numero c/ parte decimal que é abreviada no slice. Fica sempre 0-1 (resto decimal)
       
     	
       float aa = _brightness * (1.0 - _sat);
       float bb = _brightness * (1.0 - _sat * hue_frac);  //são fatores que diminuem gradualmente e 
       float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac)); // por hierarquia cc > bb > aa
      
       //reduz efeito de _sat, cc fica maior que bb
       switch(slice) { // hue até 60:_
           case 0:
               red = _brightness; //red a branco
               green = cc;        //1a cor mais relevante  gera
               blue = aa;         //2a cor mais relevante
               break;
           case 1:
               red = bb;
               green = _brightness;
               blue = aa;
               break;
           case 2:
               red = aa;
               green = _brightness;
               blue = cc;
               break;
           case 3:
               red = aa;
               green = bb;
               blue = _brightness;
               break;
           case 4:
               red = cc;
               green = aa;
               blue = _brightness;
               break;
           case 5:
               red = _brightness;
               green = aa;
               blue = bb;
               break;
           default:
               red = 0.0;
               green = 0.0;
               blue = 0.0;
               break;
       }
   }

   long ired = red * 255.0;
   long igreen = green * 255.0;
   long iblue = blue * 255.0;
   
   return long((ired << 16) | (igreen << 8) | (iblue)); 
  

}

long ColorGenerator (int* counter,int numColors,int Change) {
  //para o modo de gradiente QF
  if (Change==2) {numColors=128;}
  
  //sempre que o counter é maior que o numColors, a contagem reinicia
  float colorNumber = *counter > numColors ? *counter - numColors: *counter;
  
  if (*Enderman==0 && Change==2); 	
  	*Enderman=colorNumber; //Permite transportar o colorNumber para um apontador
  	//Serial.println(*Enderman);
  static long aux;  			   //que serve de var global, evitando duplicar a declaração acima
  
  // Sat e Brightness controláveis por telemóvel.
  float saturation = 1; // Between 0 and 1 (0 = cinzento, 1 = cor total)
  float brightness = .05; // Between 0 and 1 (0 = escuro, 1 = brilho máx)
  float hue = (colorNumber / float(numColors)) * 360; // Numero entre 0 e 360
  
  //Serial.print("hue: ");
  //Serial.println(hue);
  
  if (Change==1){
    long color = HSBtoRGB(hue, saturation, brightness);
    aux=color;
  }
  return aux;
} 

int RainbowFunc(int red,int green,int blue,int* counter,int numColors,int animationDelay,int B_off) {

      		if (*Fptr == 1 ) {
            	delay(50);
              	setColor(0,0,0); 											
      			delay(50);
            }	
      		setColor(red, green, blue);
   			*counter = (*counter + 1) % (numColors * 2);
  			
  			//Serial.println(*counter);
  
   			delay(animationDelay);	
   			
   			switch(B_off){
      			case 0: //Caso o botão não volte a ser premido, stay on 1
   		  			Change=1; // apontador
      	  			break;
   	  	 		case 1:  //Caso o botão volte a ser premido, sai
   		   			setColor(0,0,0);         			 		
           			stayMain=0;
               		Change=0;
              		*counter=0;
              		*Enderman=0;
   		   	        break;
			}//Off
			return *counter;
}


void DualFunc(struct ColorChoice *cc,int B_off) {
	if (*Fptr == 1 ) {
 		delay(40);
        setColorDual(0,0,0,0,0,0); 											
        delay(40);
      	
    }
  	Struc Choice;
    setColorDual((*cc).ER1, (*cc).EG1 , (*cc).EB1 , (*cc).ER2 , (*cc).EG2 , (*cc).EB2); // 0 - 12	[i] e ir colocando cores + map?
   			
   	switch(B_off){
      			case 0: //Caso o botão não volte a ser premido, stay on 1
   		  			Change=3;    
      	  			break;
   	  			case 1:  //Caso o botão volte a ser premido, sai
   		  			setColor(0,0,0);         			 		
          			stayMain=0;
              		Change=0;
              		*counter=0;
              		*Enderman=0;
   		  	        break;       
    }

}  


void GradFunc(int *Cptr ,int B_gradQF,int B_off){
  int aux;
  if (B_gradQF == 1 ) {ClickMode(Cptr);}
  aux=*Cptr;
  if (aux==1) {QuenteGrad(*Enderman);}
  else{FrioGrad(*Enderman);}
  
  switch(B_off){
      			case 0: //Caso o botão não volte a ser premido, stay on 1
   		  			Change=2; // apontador
      	  			break;
   	  	 		case 1:  //Caso o botão volte a ser premido, sai
   		   			setColor(0,0,0);         			 		
           			stayMain=0;
               		Change=0; //apontador
   		   	        break;
  }//Off	

}

void QuenteGrad(int starter) {
	static int red,green,blue;
  	static int set=0;
  	static float _raw[3];
    int numColors = 128;
  	
 	if ( set==0 || starter > 126) {
      	_raw[0]=255;
   		_raw[1]=0;
   		_raw[2]=0;
      	set=1;    	
    } 	 
  	if (starter < 87) {     
      	_raw[1]= _raw[1] + (1.735632184);    
    }
 	else if (starter > 87) { 
 		_raw[0]= _raw[0] - (0.6097560976);
      	_raw[1]= _raw[1] + (1.926829268);
    } 
  
 	float _red  =   (_raw[0]*70)/256;
 	float _green=   (_raw[1]*70)/256;
 	float _blue =   (_raw[2]*70)/256; 
    
   
 	red=  abs(_red);   
 	blue=  abs(_blue); 
 	green=  abs(_green);
  
  	Serial.println(red);
    Serial.println(green);
  	Serial.println(blue);
    Serial.println("------NEW----");  
      
  
 	setColor(red, green, blue);	
  	*counter = (*counter + 1) % (numColors * 2);
	
  	delay(20);
}
	  




void FrioGrad(int starter) { 
  	static int red,green,blue;
  	static int set=0;
  	static float _raw[3];
    int numColors = 128;
   	//if com set para dar setcolor(000);
  	
 	if ( set==0 || starter > 126) {
      	_raw[0]=0;
   		_raw[1]=230;
   		_raw[2]=230;
      	set=1;
      	Serial.println("AAAAAAAAAAAdsajjsd");
    }
  	if (starter < 64) {
      	_raw[1]= _raw[1] - (3.59375);
    }
 	else if (starter > 64) { 
 		_raw[0]= _raw[0] + (2.984375); 
    } 
  
 	float _red  =   (_raw[0]*80)/256;
 	float _green=   (_raw[1]*80)/256;
 	float _blue =   (_raw[2]*80)/256; 

 	red=  abs(_red);   
 	blue=  abs(_blue); 
 	green=  abs(_green);
    Serial.println(red);
    Serial.println(green);
  	Serial.println(blue);
    Serial.println("------NEW----"); 
 	setColor(red, green, blue);
    //Serial.println(starter);
  	*counter = (*counter + 1) % (numColors * 2);
  	
  	delay(20);
}






// Alternadores --------------------------------


void FlashMode(int* Fptr ) {
  static int click = 0;
  click= click + 1;
  delay(50);   // este delay é o lag que evita overlap de clicks qnd dou um click
  if (click == 2) {   
    *Fptr = 0; 
    *Cptr = 0;	//Caso o click seja 2, reinicia, dando o valor de 0 a um apontador
    click=0;		//que desligará a função de flash nos restantes modos
  }
  else {*Fptr=click;}
   //*Fptr =1 ou =0
  
}  
  
void ClickMode(int* Cptr ) {
  
  static int click = 0;
  click= click + 1;
  delay(50);   
  if (click == 2) {    // semelhante ao método acima
    click=0;
    *Fptr = 0;
    *Cptr=0;
  }
  else {*Cptr=click;}  
}




void setColor (unsigned char red, unsigned char green, unsigned char blue)
{   
 /*	Serial.println("----");
  	Serial.println(red);
  	Serial.println(green);
  	Serial.println(blue);
  	Serial.println("----"); */
  	analogWrite(PIN_RED, red);
   	analogWrite(PIN_GREEN, green);
   	analogWrite(PIN_BLUE, blue);
   	analogWrite(PIN_RED2, red);
   	analogWrite(PIN_GREEN2, green);
   	analogWrite(PIN_BLUE2, blue);  	
  
}


void setColorDual (unsigned char red, unsigned char green, unsigned char blue, unsigned char red2, unsigned char green2, unsigned char blue2)
{	
 /* Serial.println("----");
  	Serial.println(red);
  	Serial.println(green);
  	Serial.println(blue);
  	Serial.println("----"); */
 	analogWrite(PIN_RED, red);
   	analogWrite(PIN_GREEN, green);
   	analogWrite(PIN_BLUE, blue);
   	analogWrite(PIN_RED2, red2);
   	analogWrite(PIN_GREEN2, green2);
   	analogWrite(PIN_BLUE2, blue2);   
}
      	   		