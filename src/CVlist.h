// **************************************************************** //
//                           CONFIGRATION                           //
// **************************************************************** //

// by Mattia Rossetti 9/2/2019

#define DECODER_ID 1

CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, Accessory_Address},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0},
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, 0},
  {CV_DECODER_MASTER_RESET, 0},
  {CV_To_Store_SET_CV_Address, SET_CV_Address},
  {CV_To_Store_SET_CV_Address+1, 0},
  {CONF_CV, CONF_CV_DEFAULT},
  {30, 90},   //F0  Relè centre position
  {31, 5},    //F0  Rate  Blink=Eate,PWM=Rate,Servo=Rate
  {32, 60},   //F0  Start Position F0=0
  {33, 120},  //F0  End Position   F0=1
  {34, 60},   //F0  Current Position
  {35, 90},   //  Relè centre position
  {36, 5},    //  Rate  Blink=Eate,PWM=Rate,Servo=Rate
  {37, 60},   //  Start Position Fx=0
  {38, 120},  //  End Position   Fx=1
  {39, 60},   //  Current Position
  {40, 90},   //  Relè centre position
  {41, 5},    // Rate  Blink=Eate,PWM=Rate,Servo=Rate
  {42, 60},   //  Start Position Fx=0
  {43, 120},  //  End Position   Fx=1
  {44, 60},    //  Current Position
  {45, 90},   //  Relè centre position
  {46, 5},    // Rate  Blink=Eate,PWM=Rate,Servo=Rate
  {47, 60},   //  Start Position Fx=0
  {48, 120},  //  End Position   Fx=1
  {49, 60},    //  Current Position

  {CV_SINGLE_INV, 0},
  {CV_SINGLE_INV+1, 0},
  {CV_SINGLE_INV+2, 0},
  {CV_SINGLE_INV+3, 0},
  {CV_SINGLE_INV+4, 0},
  {CV_SINGLE_INV+5, 0},
  {CV_SINGLE_INV+6, 0},
  {CV_SINGLE_INV+7, 0},

  {CV_MULTI_ADDRESS, Accessory_Address},
  {CV_MULTI_ADDRESS+1, 0},
  {CV_MULTI_ADDRESS+2, Accessory_Address+1},
  {CV_MULTI_ADDRESS+3, 0},
  {CV_MULTI_ADDRESS+4, Accessory_Address+2},
  {CV_MULTI_ADDRESS+5, 0},
  {CV_MULTI_ADDRESS+6, Accessory_Address+3},
  {CV_MULTI_ADDRESS+7, 0},
  {CV_MULTI_ADDRESS+8, Accessory_Address+4},
  {CV_MULTI_ADDRESS+9, 0},
  {CV_MULTI_ADDRESS+10, Accessory_Address+5},
  {CV_MULTI_ADDRESS+11, 0},
  {CV_MULTI_ADDRESS+12, Accessory_Address+6},
  {CV_MULTI_ADDRESS+13, 0},
  {CV_MULTI_ADDRESS+14, Accessory_Address+7},
  {CV_MULTI_ADDRESS+15, 0},
};
