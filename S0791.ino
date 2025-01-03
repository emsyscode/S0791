/****************************************************/
/* This is only one example of code structure       */
/* OFFCOURSE this code can be optimized, but        */
/* the idea is let it so simple to be easy catch    */
/* where we can do changes and look to the results  */
/****************************************************/
//set your clock speed
//#define F_CPU 16000000UL
//these are the include files. They are outside the project folder
//#include <avr/io.h>
//#include <iom1284p.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
#define VFD_data 8// If 0 write LCD, if 1 read of LCD
#define VFD_clk 9 // if 0 is a command, if 1 is a data0
#define VFD_stb 10 // Must be pulsed to LCD fetch data of bus

#define LED1 11
#define LED2 12

#define VFD_data_4094 5// If 0 write LCD, if 1 read of LCD
#define VFD_clk_4094 6 // if 0 is a command, if 1 is a data0
#define VFD_stb_4094 7 // Must be pulsed to LCD fetch data of bus
/*Global Variables Declarations*/

/*Global Variables Declarations*/
bool flagSet = false;
bool flagReached = false;
byte myByte= 0x00;   // this variable is only related with swapLed1.
//unsigned char day = 7;  // start at 7 because the VFD start the day on the left side and move to rigth... grid is reverse way


unsigned char secs;

unsigned char digit;
unsigned char number;

unsigned char grid;

unsigned int k=0;

bool flag01 = false;
bool flag14 = false;
bool flag11 = false;
bool flag16 = false;
bool flag12 = false;

//    Numbers desing        hijbggcm     ..deklaf
unsigned char number0[]={(0b00010010),(0b00110011),(0b00000000)}; // 0      unsigned char number0[]={(0b00100011),(0b00011001),(0b00000000)}; // 0
unsigned char number1[]={(0b00010010),(0b00000000),(0b00000000)}; // 1
unsigned char number2[]={(0b00011100),(0b00110010),(0b00000000)}; // 2
unsigned char number3[]={(0b00011110),(0b00100010),(0b00000000)}; // 3
unsigned char number4[]={(0b00011110),(0b00000001),(0b00000000)}; // 4
unsigned char number5[]={(0b00001110),(0b00100011),(0b00000000)}; // 5
unsigned char number6[]={(0b00001110),(0b00110011),(0b00000000)}; // 6
unsigned char number7[]={(0b00010010),(0b00000010),(0b00000000)}; // 7
unsigned char number8[]={(0b00011110),(0b00110011),(0b00000000)}; // 8
unsigned char number9[]={(0b00011110),(0b00000011),(0b00000000)}; // 9
//    Letters design        hijbggcm     ..deklaf
unsigned char letterA[]={(0b11100011),(0b00001001),(0b00000000)}; // A
unsigned char letterB[]={(0b10110001),(0b00011010),(0b00000000)}; // B
unsigned char letterC[]={(0b00100001),(0b00011000),(0b00000000)}; // C
unsigned char letterD[]={(0b00100100),(0b00001100),(0b00000000)}; // D
unsigned char letterE[]={(0b11100001),(0b00011000),(0b00000000)}; // E
unsigned char letterF[]={(0b00001100),(0b00010011),(0b00000000)}; // F
unsigned char letterG[]={(0b01100001),(0b00011001),(0b00000000)}; // G
unsigned char letterH[]={(0b00011110),(0b00010001),(0b00000000)}; // H
unsigned char letterI[]={(0b01000000),(0b00100110),(0b00000000)}; // I
unsigned char letterJ[]={(0b00000010),(0b00011001),(0b00000000)}; // J
unsigned char letterK[]={(0b00101001),(0b00010001),(0b00000000)}; // k
unsigned char letterL[]={(0b00000000),(0b00110001),(0b00000000)}; // L
unsigned char letterM[]={(0b00110110),(0b00001001),(0b00000000)}; // M
unsigned char letterN[]={(0b00100110),(0b00001011),(0b00000000)}; // N
unsigned char letterO[]={(0b00010010),(0b00110011),(0b00000000)}; // O
unsigned char letterP[]={(0b11100011),(0b00001000),(0b00000000)}; // P
unsigned char letterQ[]={(0b00100011),(0b00011011),(0b00000000)}; // A
unsigned char letterR[]={(0b11100011),(0b00001010),(0b00000000)}; // R
unsigned char letterS[]={(0b00001110),(0b00100011),(0b00000000)}; // S
unsigned char letterT[]={(0b00001001),(0b00000000),(0b00000000)}; // T
unsigned char letterU[]={(0b00100010),(0b00011001),(0b00000000)}; // U
unsigned char letterV[]={(0b00110000),(0b00001100),(0b00000000)}; // V
unsigned char letterX[]={(0b00010100),(0b00000110),(0b00000000)}; // X
unsigned char letterZ[]={(0b00010001),(0b00010100),(0b00000000)}; // Z
unsigned char letterW[]={(0b00100010),(0b00001111),(0b00000000)}; // W
unsigned char letterSpace[]={(0b00000000),(0b00000000),(0b00000000)}; // Empty space
unsigned int segments[] ={ // Draw numbers to the display of 7 segments
  //This not respect the normal table for 7segm like "abcdefgh"  // 
      0b01110111, //0  // 
      0b00010010, //1  // 
      0b01101011, //2  // 
      0b01011011, //3  // 
      0b00011110, //4  // 
      0b01011101, //5  // 
      0b01111101, //6  // 
      0b00010011, //7  // 
      0b01111111, //8  // 
      0b00011111, //9  // 
      0b00000000, //10 // empty display
  };
void pt6311_init(void){
  delayMicroseconds(200); //power_up delay
  // Note: Allways the first byte in the input data after the STB go to LOW is interpret as command!!!
  // Configure VFD display (grids)
  cmd_with_stb(0b00000000);//  (0b01000000)    cmd1 14 Grids and 14 segments??? (IC da SGNEC ref: S0791 que é igual 6311 pinout)
  delayMicroseconds(1);
  cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
  delayMicroseconds(1);
  // turn vfd on, stop key scannig
  cmd_with_stb(0b10001000);//(BIN(01100110)); 
  delayMicroseconds(1);
  // Write to memory display, increment address, normal operation
  cmd_with_stb(0b01000000);//(BIN(01000000));
  delayMicroseconds(1);
  // Address 00H - 15H ( total of 11*2Bytes=176 Bits)
  cmd_with_stb(0b11000000);//(BIN(01100110)); 
  delayMicroseconds(1);
  // set DIMM/PWM to value
  cmd_with_stb((0b10001000) | 7);//0 min - 7 max  )(0b01010000)
  delayMicroseconds(1);
}
void send_char(unsigned char grid, unsigned char byte0, unsigned char byte1,unsigned char byte2){
 
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(2);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(2);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Auto increment
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(2);
            cmd_without_stb((0b11000000) | grid); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(byte0); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(byte1); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(byte2); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(2);
             /*
            Serial.println("******************");  // only to debug on the console print
            Serial.println(((0b11000000) | grid), BIN);
            Serial.println("------------------");
            Serial.println(grid, BIN);//
            Serial.println(byte0, BIN);//
            Serial.println(byte1, BIN);//
            Serial.println(byte2, BIN);
            Serial.println("++++++++++++++++++");
            */
            delay(40);  //Increase this time if you want see the char appears slowly
}
void cmd_4bitsWithout_stb(unsigned char a){
  // send without stb
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    delayMicroseconds(5);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(5);
   }
   //digitalWrite(VFD_clk, LOW);
}
void cmd_without_stb(unsigned char a){
  // send without stb
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    delayMicroseconds(5);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(5);
   }
   //digitalWrite(VFD_clk, LOW);
}
void cmd_4bitsWith_stb(unsigned char a){
  // send with stb
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_stb, LOW);
  delayMicroseconds(1);
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_stb, HIGH);
   delayMicroseconds(1);
}
void cmd_with_stb(unsigned char a){
  // send with stb
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_stb, LOW);
  delayMicroseconds(1);
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data, LOW);
     }
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_stb, HIGH);
   delayMicroseconds(1);
}
void cmd_with_stb_4094(unsigned char a){
  // send with stb
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  
  //This send the strobe signal
  //Note: The first byte input at in after the STB go LOW is interpreted as a command!!!
  digitalWrite(VFD_stb_4094, HIGH);
  delayMicroseconds(1);
   for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk_4094, LOW);
     delayMicroseconds(1);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_data_4094, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_data_4094, LOW);
     }
    digitalWrite(VFD_clk_4094, HIGH);
    delayMicroseconds(1);
   }
   digitalWrite(VFD_stb_4094, LOW);
   delayMicroseconds(1);
}
void test_VFD(void){
  clear_VFD();
      
      digitalWrite(VFD_stb, LOW);
      delayMicroseconds(1);
      cmd_with_stb(0b00001000); // cmd 1 // 5 Grids & 16 Segments
      cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
      cmd_with_stb(0b01000000); // cmd 2 //Write VFD, Normal operation; Set pulse as 1/16, Auto increment
      cmd_with_stb(0b10001000 | 0x07); // cmd 2 //set on, dimmer to max
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000)); //cmd 3 wich define the start address (00H to 15H)
        // Only here must change the bit to test, first 2 bytes and 1/2 byte of third.
         for (int i = 0; i < 8 ; i++){ // test base to 16 segm and 5 grids
          // Zone of test, if write 1 on any position of 3 bytes below position, will bright segment corresponding it.
         cmd_without_stb(0b00000000); // Data to fill table 5*16 = 80 bits
         cmd_without_stb(0b00000000); // Data to fill table 5*16 = 80 bits
         cmd_4bitsWithout_stb(0b00000000); // Data to fill table 5*16 = 80 bits
         }
    
      //cmd_without_stb(0b00000001); // cmd1 Here I define the 5 grids and 16 Segments
      //cmd_with_stb((0b10001000) | 7); //cmd 4
      digitalWrite(VFD_stb, HIGH);
      delay(1);
      delay(3000);  
}
void swapLed1(){
  byte myByte = 0x00;
    digitalWrite(VFD_stb, LOW);
    delayMicroseconds(20);
    cmd_without_stb(0b01000001);
    delayMicroseconds(20);
    myByte ^=(0b00000001);  // Here is only to invert bit of led 1.
    cmd_without_stb(myByte);
    delayMicroseconds(20);
    digitalWrite(VFD_stb, HIGH);
    delayMicroseconds(20);
}
void setup(){
    // put your setup code here, to run once:
    // initialize digital pin LED_BUILTIN as an output.
      pinMode(LED_BUILTIN, OUTPUT);
      Serial.begin(115200);
    //Set PORT
    DDRD = 0xFF;  // IMPORTANT: from pin 0 to 7 is port D, from pin 8 to 13 is port B
    PORTD=0x00;
    DDRB =0xFF;
    PORTB =0x00;

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    pt6311_init();
}
void clear_VFD(void){
  int sum=0;
  /*
  Here I clean all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 16; n++){  //
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(0b00000000); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b00000000); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(2);
            sum=(sum +3);
     }
}
void seg(void){
  int sum=0;
  /*
  Here I test all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 16; n++){  //
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary execute a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000)); // Start address 00;
            // 
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.// bar in middle(center)
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.// bar
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//bar
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.// left grid
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 1
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 2
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 3
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 4
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 5
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 6
            cmd_without_stb(0b00000000); cmd_without_stb(0b00100000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 7
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//DSP 8
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.//dsp small
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.// right grid
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.
            cmd_without_stb(0b00000000); cmd_without_stb(0b00000000); cmd_4bitsWithout_stb(0b00000000); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(3000);
            Serial.println(((0b11000000) | sum), BIN);
     }
}
void led_on(void){
    byte myByte = 0x00;
    digitalWrite(VFD_stb, LOW);
    delayMicroseconds(20);
    cmd_without_stb(0b01000001);
    delayMicroseconds(2);
    myByte ^=(0b00000001);  // Here is only to invert bit of led 1.
    cmd_without_stb(myByte);
    delayMicroseconds(2);
    digitalWrite(VFD_stb, HIGH);
    delayMicroseconds(2);
}
void all_ON(void){
  int sum=0;
  /*
  Here I test all registers 
  Could be done only on the number of grid
  to be more fast. The 12 * 3 bytes = 36 registers
  */
      for (int n=0; n < 16; n++){  //
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
            cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            //cmd_without_stb(0b11111111); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(0b11111111); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delayMicroseconds(3000);
            Serial.println(((0b11000000) | sum), BIN);
            sum=(sum +3);
     }
}
void segBySeg(void){
  int u=0;
  int sum=0;
  unsigned char word1, word2,word3,word4;
  unsigned int mask0=0b0000000000000001;
  unsigned int mask1=0b0000000000000001;
 
      for ( u =0; u < 32; u++){
        cmd_with_stb(0b00001101); //       cmd 1 // 14 Grids & 14 Segments
        delayMicroseconds(1);
        cmd_with_stb((0b10001000) | 7); // IMPORTANT:  After define the size GRIDs is necessary do a cmd "ON" display.
        delayMicroseconds(1);
        cmd_with_stb(0b01000000); //       cmd 2 //Normal operation; Set pulse as 1/16
        digitalWrite(VFD_stb, LOW);
        delayMicroseconds(1);
        cmd_without_stb((0b11000000) | sum); // cmd 3 //wich define the start address (00H to 15H)
            
        if (u<=15){
       word4 = (mask0 & 0xff000000) >>  24; // remaning segments 
       word3 = (mask0 & 0x00ff0000) >>  16; // remaning segments
       word2 = (mask0 & 0x0000ff00) >>  8; // grid 9 & 10 and segments
       word1 = (mask0 & 0x000000ff) >>  0; // grids
        mask0=(mask0 <<= 1);
        }
        if (u>15){
       word4 = (mask1 & 0xff000000) >>  24; // remaning segments 
       word3 = (mask1 & 0x00ff0000) >>  16; // remaning segments
       word2 = (mask1 & 0x0000ff00) >>  8; // grid 9 & 10 and segments
       word1 = (mask1 & 0x000000ff) >>  0; // grids
        mask1=(mask1 <<= 1);
        }
       //
       //Serial.println(mask0, BIN);
       //Serial.println(mask1, BIN);
            cmd_without_stb(word1); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_without_stb(word2); // Data to fill table of 5 grids * 16 segm = 80 bits on the table
            cmd_4bitsWithout_stb(word3); // only half byte of third byte.
            //
            digitalWrite(VFD_stb, HIGH);
            delay(500);
            //Serial.println(((0b11000000) | sum), BIN);
            sum=(sum +3);
    }
}
void msgNumbers(void){
  // Zone to test the numbers and letters
  send_char(0x00,number0[0], number0[1], number0[2]);
  send_char(0x03,number1[0], number1[1], number1[2]);
  send_char(0x06,number2[0], number2[1], number2[2]);
  send_char(0x09,number3[0], number3[1], number3[2]);
  send_char(0x0C,number4[0], number4[1], number4[2]);
  send_char(0x0F,number5[0], number5[1], number5[2]);
  send_char(0x12,number6[0], number6[1], number6[2]);
  send_char(0x15,number7[0], number7[1], number7[2]);
  send_char(0x18,number8[0], number8[1], number8[2]);
  send_char(0x1B,number9[0], number9[1], number9[2]);
  send_char(0x1E,number0[0], number0[1], number0[2]);
  send_char(0x21,number1[0], number1[1], number1[2]);
}
void msgHiFolks(void){
  // send_char(0x0C,letterA[0], letterA[1], letterA[2]);
  // send_char(0x0F,letterB[0], letterB[1], letterB[2]);
  // send_char(0x12,letterC[0], letterC[1], letterC[2]);
  // send_char(0x15,letterD[0], letterD[1], letterD[2]);
  // send_char(0x18,letterE[0], letterE[1], letterE[2]);
 send_char(0x09,letterF[0], letterF[1], letterF[2]);
  // send_char(0x1E,letterG[0], letterG[1], letterG[2]);
 send_char(0x00,letterH[0], letterH[1], letterH[2]);
  // clear_VFD();
 send_char(0x03,letterI[0], letterI[1], letterI[2]);
  // send_char(0x0F,letterJ[0], letterJ[1], letterJ[2]);
 send_char(0x12,letterK[0], letterK[1], letterK[2]);
 send_char(0x0F,letterL[0], letterL[1], letterL[2]);
  // send_char(0x18,letterM[0], letterM[1], letterM[2]);
  // send_char(0x1B,letterN[0], letterN[1], letterN[2]);
 send_char(0x0C,letterO[0], letterO[1], letterO[2]);
  // send_char(0x21,letterP[0], letterP[1], letterP[2]);
  // clear_VFD();
  // send_char(0x0C,letterQ[0], letterQ[1], letterQ[2]);
  // send_char(0x0F,letterR[0], letterR[1], letterR[2]);
 send_char(0x15,letterS[0], letterS[1], letterS[2]);
  // send_char(0x15,letterT[0], letterT[1], letterT[2]);
  // send_char(0x18,letterU[0], letterU[1], letterU[2]);
  // send_char(0x1B,letterV[0], letterV[1], letterV[2]);
  // send_char(0x1E,letterX[0], letterX[1], letterX[2]);
  // send_char(0x21,letterZ[0], letterZ[1], letterZ[2]);
  // clear_VFD();
  // send_char(0x0C,letterW[0], letterW[1], letterW[2]);
  send_char(0x06,letterSpace[0], letterSpace[1], letterSpace[2]);
}
void readButtonsWake(){
  //Take special attention to the initialize digital pin LED_BUILTIN as an output.
  //
  int ledPin = 13;   // LED connected to digital pin 13
  int inPin = 8;     // pushbutton connected to digital pin 7
  int val = 0;       // variable to store the read value
  int dataIn=0;

  byte array[8] = {0,0,0,0,0,0,0,0};
  byte together = 0;

  unsigned char receive = 7; //define our transmit pin
  unsigned char data = 0; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask

  array[0] = 1;

  unsigned char btn1 = 0x41;

      digitalWrite(VFD_stb, LOW);
        delayMicroseconds(2);
      cmd_without_stb(0b01000010); // cmd 2 //Read Keys;Normal operation; Set pulse as 1/16
       // cmd_without_stb((0b11000000)); //cmd 3 wich define the start address (00H to 15H)
     // send without stb
  
  pinMode(8, INPUT);  // Important this point! Here I'm changing the direction of the pin to INPUT data.
  delayMicroseconds(2);
  //PORTD != B01010100; // this will set only the pins you want and leave the rest alone at
  //their current value (0 or 1), be careful setting an input pin though as you may turn 
  //on or off the pull up resistor  
  //This don't send the strobe signal, to be used in burst data send
         for (int z = 0; z < 3; z++){
             //for (mask=00000001; mask > 0; mask <<= 1) { //iterate through bit mask
                   for (int h =8; h > 0; h--) {
                      digitalWrite(VFD_clk, HIGH);  // Remember wich the read data happen when the clk go from LOW to HIGH! Reverse from write data to out.
                      delayMicroseconds(2);
                     val = digitalRead(inPin);
                      //digitalWrite(ledPin, val);    // sets the LED to the button's value
                           if (val & mask){ // if bitwise AND resolves to true
                             //Serial.print(val);
                            //data =data | (1 << mask);
                            array[h] = 1;
                           }
                           else{ //if bitwise and resolves to false
                            //Serial.print(val);
                           // data = data | (1 << mask);
                           array[h] = 0;
                           }
                    digitalWrite(VFD_clk, LOW);
                    delayMicroseconds(2);
                    
                   } 
             
              Serial.print(z);
              Serial.print(" - " );
                        
                                  for (int bits = 7 ; bits > -1; bits--) {
                                     Serial.print(array[bits]);
                                  }
                       
                        if (z==0){
                          if(array[1] == 1){
                           cmd_with_stb_4094(0B00000000);
                           cmd_with_stb_4094(0B00010100);
                           digitalWrite(LED1, !digitalRead(LED1));
                          }
                        }
                          if (z==1){
                          if(array[4] == 1){
                           cmd_with_stb_4094(0B00000000);
                           cmd_with_stb_4094(0B00101000);
                           digitalWrite(LED2, !digitalRead(LED2));
                          }
                          }
                          if (z==1){
                          if(array[1] == 1){
                           cmd_with_stb_4094(0B10000000);
                           cmd_with_stb_4094(0B01000000);
                           flag11 = true;
                          }
                        }
                        if (z==1){
                          if(array[6] == 1){
                            cmd_with_stb_4094(0B01000000);
                            cmd_with_stb_4094(0B10000000);
                            flag16 = true;
                          }
                        }
                        if (z==1){
                          if(array[2] == 1){
                            cmd_with_stb_4094(0B00100000);
                            cmd_with_stb_4094(0B00000000); 
                            flag12 = true;
                          }
                        }
                  Serial.println();
          }  // End of "for" of "z"
      Serial.println();

 digitalWrite(VFD_stb, HIGH);
 delayMicroseconds(2);
 cmd_with_stb((0b10001000) | 7); //cmd 4
 delayMicroseconds(2);
 pinMode(8, OUTPUT);  // Important this point!  // Important this point! Here I'm changing the direction of the pin to OUTPUT data.
 delay(1); 
}
void loop() {
  cmd_with_stb_4094(0B00000000);  // Functions on first byte: //Bit 5: Buzzer; bit 6: NextUp_down; bit 7: PrevDown_down
  cmd_with_stb_4094(0B00000000);  //Functions by second byte: //Bit 0: ejectPreTun_up; Bit 1: ejectPreTun_down; bit 2: PlayAuto_down; bit 3 StopMemo_down; 
                                  //Functions by second byte: //Bit 4 PlayAuto_Up; bit 5 StopMemo_Up; bit 6 PrevDown_up; bit 7: NextUp_UP
  // delay(1000);
 
  readButtonsWake();
  delay(100);
      if (flag16 == true){
        clear_VFD();
        msgNumbers();
        flag16 = false;
      }

      if (flag11 == true){
        clear_VFD();
        msgHiFolks();
        flag11 = false;
      }

      if (flag12 == true){
        clear_VFD();
        flag12 = false;
      }
}
 

