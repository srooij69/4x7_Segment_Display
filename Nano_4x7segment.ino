/*

references:
 https://www.youtube.com/watch?v=gesrM2J8VnY

 https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/
 https://www.norwegiancreations.com/2018/10/arduino-tutorial-avoiding-the-overflow-issue-when-using-millis-and-micros/

*/

const byte dataPin  = 2;
const byte latchPin = 3;
const byte clockPin = 4;

const byte digit0Pin = 5; //only the first pin specified
const byte nrDigits  = 4;

const unsigned long multiplex_delay = 5000; //microseconds to refresh each digit; 6000->stable 7000->hint of flicker 8000->flicker

const int counter_max   = 0x1FF;   //count up to
const int counter_delay =    50;   //milliseconds to show each number
const int text_delay    =  2000;   //milliseconds to show the text

char display_buffer[4] = "....";
byte char_to_byte[255];


/* =========================== display plumbing ===================== */
byte last_digit = digit0Pin;  
void plumbing_writeSingleDigit(byte digit, char c){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, char_to_byte[c] );

    digitalWrite(last_digit, LOW); 
    digitalWrite(latchPin, HIGH);    
    digitalWrite(digit0Pin+digit, HIGH); 
    last_digit = digit0Pin+digit;
}

void setup_char_to_byte_array(){
/*  Segments      A
                F   B
                  G
                E   C
                  D  
*/
    //by default initialize all values to show nothing
    for(int i=0; i<255; i++){
      char_to_byte[i] = 0b00000000; 
    }
  
    //values to show, the segments are controled by 0b0GFEDCBA
    char_to_byte[' '] = 0b00000000; 
    char_to_byte['0'] = 0b00111111; 
    char_to_byte['1'] = 0b00000110; 
    char_to_byte['2'] = 0b01011011; 
    char_to_byte['3'] = 0b01001111; 
    char_to_byte['4'] = 0b01100110; 
    char_to_byte['5'] = 0b01101101; 
    char_to_byte['6'] = 0b01111101; 
    char_to_byte['7'] = 0b00000111; 
    char_to_byte['8'] = 0b01111111; 
    char_to_byte['9'] = 0b01101111; 
    char_to_byte['a'] = 0b01110111; 
    char_to_byte['b'] = 0b01111100; 
    char_to_byte['c'] = 0b00111001; 
    char_to_byte['d'] = 0b01011110; 
    char_to_byte['e'] = 0b01111001; 
    char_to_byte['f'] = 0b01110001; 
    char_to_byte['h'] = 0b01110110; 
    char_to_byte['l'] = 0b00111000; 
    char_to_byte['o'] = 0b01011100;       
    
    char_to_byte['x'] = 0b01110100;       
}

unsigned long previous_multiplex_action = - multiplex_delay;
byte current_digit = 0;
void plumbing_tick(){
    if(micros() - previous_multiplex_action >= multiplex_delay){
        previous_multiplex_action = micros();
        
        current_digit++;
        if(current_digit>=nrDigits) current_digit=0;
        
        plumbing_writeSingleDigit(current_digit, display_buffer[current_digit]);
    }
}


/* ========================== logic ======================== */
void write_txt_to_display(char txt[]){
    for(int i=0; i< nrDigits; i++){
      display_buffer[i] = txt[i];
    }
}

void write_hex_to_display(int nr){
    char txt[10];
    sprintf(txt, "x%3x", nr);
    write_txt_to_display(txt);
}

unsigned long previous_logic_action = - text_delay;
unsigned long logic_delay = 0; 

int logic_counter=0;
void logic_tick(){
   if(millis() - previous_logic_action >= logic_delay){
        previous_logic_action = millis();
        
        if(logic_counter<counter_max){       
            logic_delay=counter_delay;
            
            logic_counter++;     
            write_hex_to_display(logic_counter);
        } else {
            logic_delay=text_delay;
            logic_counter =0;
          
            write_txt_to_display("halo");
        }
        
    }
}

/* ========================== generic ======================== */
void setup(){
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
   
    for(int dgt=0; dgt<nrDigits; dgt++){
        pinMode(digit0Pin+dgt, OUTPUT);
    }

    setup_char_to_byte_array(); 
}

void loop(){
    plumbing_tick();
    logic_tick();
}
