// Production 17 Function DCC Decoder 
// Version 5.1  Geoff Bunza 2014,2015,2016
// NO LONGER REQUIRES modified software servo Lib
// Software restructuring mods added from Alex Shepherd and Franz-Peter
//   With sincere thanks

      /*********************************************
       *                                           *
       *  modified by M.Ross => AccDec4Servo4Relè  *
       *                                           *
       *********************************************/


#define HW_VERSION 20
#define SW_VERSION 41

#include <NmraDcc.h>
#include <Servo.h>

Servo servo;
#define servo_start_delay 50
#define servo_init_delay 11
#define servo_slowdown  3   //servo loop counter limit
int servo_slow_counter = 0; //servo loop counter to slowdown servo transit
int8_t b = -1;

byte numfpins = 4;

#if defined(ARDUINO_ARCH_RP2040)
byte PWMpin = 3;
byte ACKpin = 4;
byte Spin[] = {5,6,7,8};
byte Rpin[] = {9,10,11,12};
byte LED = LED_BUILTIN;
#else
byte PWMpin = 11;
byte ACKpin = 4;
byte Spin[] = {12,13,A0,A1};
byte Rpin[] = {A2,A3,A4,A5};
byte LED = 10;
#endif

NmraDcc  Dcc ;

#define SET_CV_Address  99                // THIS ADDRESS IS FOR SETTING CV'S Like a Loco

                                          // WHICH WILL EXTEND FOR 16 MORE SWITCH ADDRESSES
uint8_t CV_DECODER_MASTER_RESET =   120;  // THIS IS THE CV ADDRESS OF THE FULL RESET
#define CV_To_Store_SET_CV_Address	121
#define CV_Accessory_Address CV_ACCESSORY_DECODER_ADDRESS_LSB

#define CV_SINGLE_INV     70
#define CV_MULTI_ADDRESS  80

struct QUEUE
{
  int16_t centre_value;
  int16_t inuse;
  int16_t current_position;
  int16_t increment;
  int16_t stop_value;
  int16_t start_value;
  int16_t multi_address;
  int16_t single_invert;
};
QUEUE *ftn_queue = new QUEUE[numfpins];

struct CVPair
{
  uint8_t  CV;
  uint8_t   Value;
};

/*********************************************************************************************/
#include "config.h"
/*********************************************************************************************/
confCV   CV28(Dcc) ;

uint8_t FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);

#if defined(SERIALCOM)

  #include <DccSerialCom.h>
  
  DccSerialCom SerialCom(FactoryDefaultCVIndex, CV_MULTI_ADDRESS, CV_SINGLE_INV);

  void notifyExecuteFunction(uint8_t function, uint8_t state) {
    exec_function(function, state);
  }
  
  uint16_t notifyGetCVnum(uint16_t index) {
    return FactoryDefaultCVs[index].CV;
  }

  uint16_t notifyGetCVval(uint16_t CV) {
    return Dcc.getCV(CV);
  }

  void notifySetCV(uint16_t CV, uint16_t value) {
    Dcc.setCV(CV, value);
  }
  
#endif

void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
};

void setup()   //******************************************************
{
#if defined(DEBUG) | defined(SERIALCOM)
  Serial.begin(9600);
  SerialCom.init(Serial);
  //Serial.println("AccDec4servo4rele setup...");
#endif
  //uint8_t cv_value;
  // initialize the digital pins as outputs
  for (byte i=0; i < numfpins; i++) {
      pinMode(Spin[i], OUTPUT);
      digitalWrite(Spin[i], 0);
      pinMode(Rpin[i], OUTPUT);
      digitalWrite(Rpin[i], 0);
     }
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  pinMode(ACKpin, OUTPUT);
  digitalWrite(ACKpin, 0);
  
  // Setup which External Interrupt, the Pin it's associated with that we're using 
  Dcc.pin(digitalPinToInterrupt(2), 2, 0);
  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, 100, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, CV_To_Store_SET_CV_Address);
  delay(1000);

#ifdef DECODER_LOADED
  if ( Dcc.getCV(CV_DECODER_MASTER_RESET)== CV_DECODER_MASTER_RESET || Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) == 255) 
#endif
     {
       for (int j=0; j < sizeof(FactoryDefaultCVs)/sizeof(CVPair); j++ )
         Dcc.setCV( FactoryDefaultCVs[j].CV, FactoryDefaultCVs[j].Value);
     }

  CV28.init();

  servo.attach(PWMpin);
  
  for (byte i=0; i < numfpins; i++) {
      CVrefresh(i);
      
      servo.write(ftn_queue[i].current_position);
      digitalWrite(Spin[i],1);
      
      if (ftn_queue[i].current_position > ftn_queue[i].centre_value) {
        digitalWrite(Rpin[i], 1);
      }
  
      for (byte t=0; t<servo_start_delay; t++) 
        {delay(servo_init_delay);}
  
      digitalWrite(Spin[i], 0);
        
      ftn_queue[i].inuse = 0;
      delay(1);
  }

  servo.detach();

  digitalWrite(LED, 0);
}

void loop()   //**********************************************************************
{
  // Serial comunication
  #if defined(SERIALCOM)
    SerialCom.process();
  #endif
  
  //MUST call the NmraDcc.process() method frequently 
  // from the Arduino loop() function for correct library operation
  Dcc.process();
  delay(8);
  
  for (int8_t i=0; i < numfpins; i++) {
    if (ftn_queue[i].inuse==1 && (b==i || b<0))  {
      
      if(b<0)
        servo.attach(PWMpin);
      b=i;
      digitalWrite(LED, 1);
      digitalWrite(Spin[i],1);

	      if (servo_slow_counter++ > servo_slowdown)
	      {
          
          if (ftn_queue[i].current_position > ftn_queue[i].centre_value) {
            digitalWrite(Rpin[i], 1);
          }
          else {
            digitalWrite(Rpin[i], 0);
          }
          ftn_queue[i].current_position += ftn_queue[b].increment;

	        if (ftn_queue[i].increment > 0) {
	          if (ftn_queue[i].current_position > ftn_queue[i].stop_value) {
		          ftn_queue[i].current_position = ftn_queue[i].stop_value;
              ftn_queue[i].inuse = 0;
              b = -1;
              
              if(CV28.GetSave()) Dcc.setCV( 34+(i*5), ftn_queue[i].current_position);
              
              digitalWrite(LED, 0);
              digitalWrite(Spin[i], 0);
              servo.detach();
            }
          }
	        if (ftn_queue[i].increment < 0) { 
	          if (ftn_queue[i].current_position < ftn_queue[i].start_value) { 
	            ftn_queue[i].current_position = ftn_queue[i].start_value;
              ftn_queue[i].inuse = 0;
              b = -1;

              if(CV28.GetSave()) Dcc.setCV( 34+(i*5), ftn_queue[i].current_position);
              
              digitalWrite(LED, 0);
              digitalWrite(Spin[i], 0);
              servo.detach();
            }
	        }
          servo.write(ftn_queue[i].current_position);
          servo_slow_counter = 0;
	      }

       break;
    }
  }
  
}


void CVrefresh(uint8_t i) {
  if (CV28.GetMultiAdr()) {
    ftn_queue[i].multi_address = int (word(char (Dcc.getCV( CV_MULTI_ADDRESS+1+(i*2))), char (Dcc.getCV( CV_MULTI_ADDRESS+(i*2)))));
  }
  else {
    ftn_queue[i].multi_address = 0;
  }
  ftn_queue[i].single_invert = int (Dcc.getCV( CV_SINGLE_INV+i));
  ftn_queue[i].current_position = int (Dcc.getCV( 34+(i*5)));
  ftn_queue[i].stop_value = int (Dcc.getCV( 33+(i*5)));
  ftn_queue[i].start_value = int (Dcc.getCV( 32+(i*5)));
  ftn_queue[i].centre_value = int (char (Dcc.getCV( 30+(i*5))));
  ftn_queue[i].increment = -int (char (Dcc.getCV( 31+(i*5))));
}

// This function is called by the NmraDcc library
// when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain
// on the power supply for 6ms to ACK a CV Read
void notifyCVAck(void) {
  digitalWrite( ACKpin, HIGH );
  digitalWrite( LED, HIGH );
  delay(6);
  digitalWrite( ACKpin, LOW );
  digitalWrite( LED, LOW );
}

void notifyCVChange( uint16_t CV, uint8_t Value) {
  digitalWrite( LED, HIGH );
  delay(20);
  digitalWrite( LED, LOW );
  CV28.init();
  for (uint8_t i=0; i < numfpins; i++) {
    CVrefresh(i);
  }
}

extern void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower ) {
  if (CV28.GetMultiAdr()) {
    for (uint8_t i=0; i < numfpins; i++) {
      if (ftn_queue[i].multi_address == Addr) {
        if (ftn_queue[i].single_invert) {
          exec_function(i, Direction ? 0 : 1);
        }
        else {
          exec_function(i, Direction ? 1 : 0);
        }
      }
    }
  }
  else {
    uint16_t Current_Decoder_Addr = Dcc.getAddr();
    if ( Addr >= Current_Decoder_Addr && Addr < Current_Decoder_Addr+numfpins) { //Controls Accessory_Address+8
      if (ftn_queue[Addr-Current_Decoder_Addr].single_invert) {
        exec_function(Addr-Current_Decoder_Addr, Direction ? 0 : 1);
      }
      else {
        exec_function(Addr-Current_Decoder_Addr, Direction ? 1 : 0);
      }
    }
  }
}

/*
extern void notifyDccAccState( uint16_t Addr, uint16_t BoardAddr, uint8_t OutputAddr, uint8_t State) {
  uint16_t Current_Decoder_Addr;
  uint8_t Bit_State;
  Current_Decoder_Addr = Dcc.getAddr();
  Bit_State = OutputAddr & 0x01;
  
  if ( Addr >= Current_Decoder_Addr && Addr < Current_Decoder_Addr+numfpins) { //Controls Accessory_Address+3
	  exec_function(Addr-Current_Decoder_Addr, Bit_State );
  } 
}
*/
void exec_function (int function, int FuncState)  {
	    ftn_queue[function].inuse = 1;

      if (FuncState ^ CV28.GetInv()) {
        ftn_queue[function].increment = char ( Dcc.getCV( 31+(function*5)));
        ftn_queue[function].stop_value = Dcc.getCV( 33+(function*5));
      }
      else {
        ftn_queue[function].increment = - char(Dcc.getCV( 31+(function*5)));
        ftn_queue[function].stop_value = Dcc.getCV( 32+(function*5));
      }
}
