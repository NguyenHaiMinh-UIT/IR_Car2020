#include "IR_Car.h"
#define Right true
#define Left  false
IRCar c;
uint16_t* val;
uint16_t threshold[7] = { 500 , 620, 500, 500, 500, 500, 500};

int speed_car = 255;
int max_handle = 60;
//int speedy = 100;
int pattern = 0;
//uint8_t sensor = 0;

void Show_Leds(void) {
  c.IRLed_GetAllAnalog();
  uint8_t filted = c.IRLed_GetAllFilted();
  c.Led_Display(filted);
}
void setup() {
  // put your setup code here, to run once:
  c.Init();
  Serial.begin(9600);
  c.IRLed_SetAllThreshold(threshold);
  c.Stop();


  TCCR2B = 0x07; // prescaler /1024
  TCNT2 = 250;
  TIMSK2 = 0x01; //interrupt enable
  interrupts();
}
void loop()
{
  speed_car = 160;
  uint8_t sensor = c.IRLed_GetAllFilted() ;
  chay_binh_thuong ();
  if (sensor == 0b1111111 || sensor == 0b0111111 || sensor == 0b1111110 || sensor == 0b1111100 || sensor == 0011111 )
  {
    delay (80);
    sensor = c.IRLed_GetAllFilted() ;
    cua_vuong();
  }
  if ( sensor == 0b0001111 || sensor == 0b0011111 || sensor == 0b1110000)
  {
    chuyen_lan (Right);
  }
  if ( sensor == 0b1111000 || sensor == 0b1111100 || sensor ==  0b1110000 || sensor == 0b1111100)
  {
    {
      chuyen_lan (Left);
    }
  }
  if (c.Buttons_Check() == BT_4)
  {
    c.Led_Display(0b11111111);
    Show_Leds();
    Read_sensor ();
  }
}
void chay_binh_thuong()
{
  uint8_t sensor = c.IRLed_GetAllFilted();
  // thẳng
  if (sensor == 0b0001000 || sensor == 0b0011000 || sensor == 0b0001100)
  {
    pattern = 0;
  }
  // lệch trái
  if ((pattern == 0 || pattern == 1) && (sensor == 0b0010000 || sensor == 0b0110000 || sensor == 0b0100000 || sensor == 0b1100000 || sensor == 0b1000000))
  {
    pattern = 1;
  }
  // lệch phải
  if ((pattern == 0 || pattern == 2) && (sensor == 0b0000100 || sensor == 0b0000110 || sensor == 0b0000010 || sensor == 0b0000011 || sensor == 0b0000001))
  {
    pattern = 2;
  }
  if (pattern == 0 )
  {
    switch (sensor)
    {
      case 0b0001000:
        c.Turn (0);
        c.Run (0.99 * speed_car, speed_car);
        break;
      case 0b0011000:
        c.Turn (-3);
        c.Run (0.99 * speed_car, speed_car);
        break;
      case 0b0001100:
        c.Turn (3);
        c.Run (0.99 * speed_car, speed_car);
        break;
    }
  }
  // lệch trái
  if (pattern == 1)
  {
    switch (sensor)
    {
      case 0b0010000:
        c.Turn (-0.15 * max_handle);
        c.Run (0.9 * speed_car, 0.95 * speed_car);
        break;
      case 0b0110000:
        c.Turn (-0.25 * max_handle);
        c.Run (0.8 * speed_car, 0.85 * speed_car);
        break;
      case 0b0100000:
        c.Turn (-0.4 * max_handle);
        c.Run (0.7 * speed_car, 0.8 * speed_car);
        break;
      case 0b1100000:
        c.Turn (-0.45 * max_handle);
        c.Run (0.6 * speed_car, 0.7 * speed_car);
        break;
      case 0b1000000:
        c.Turn (-0.55 * max_handle);
        c.Run (0.6 * speed_car, 0.8 * speed_car);
        break;
      case 0b1000001:
        c.Turn (-0.7 * max_handle);
        c.Run (0.3 * speed_car, 0.8 * speed_car);
        break;
    }
  }
  // lệch phải
  if (pattern == 2)
  {
    switch (sensor)
    {
      case 0b0000100:
        c.Turn (0.15 * max_handle);
        c.Run (0.85 * speed_car, 0.8 * speed_car);
        break;
      case 0b0000110:
        c.Turn (0.25 * max_handle);
        c.Run (0.8 * speed_car, 0.75 * speed_car);
        break;
      case 0b0000010:
        c.Turn (0.4 * max_handle);
        c.Run (0.8 * speed_car, 0.7 * speed_car);
        break;
      case 0b0000011:
        c.Turn (0.45 * max_handle);
        c.Run (0.7 * speed_car, 0.6 * speed_car);
        break;
      case 0b0000001:
        c.Turn (0.55 * max_handle);
        c.Run (0.8 * speed_car, 0.6 * speed_car);
        break;
      case 0b1000001:
        c.Turn (0.7 * max_handle);
        c.Run (0.8 * speed_car, 0.3 * speed_car);
        break;
    }
  }
}

//========================================================
void cua_vuong ()
{
  uint8_t sensor = c.IRLed_GetAllFilted();
  bool Done = false;
  bool Thang = false;
  bool Signal = false;
  bool a = false ;
  bool b = false ;
  while (!Done)
  {
    sensor = c.IRLed_GetAllFilted();
    while (!Thang)
    {
      speed_car = 50;
      sensor = c.IRLed_GetAllFilted();
      chay_binh_thuong ();

      if (sensor == 0b0001111 || sensor == 0b0011111 || sensor == 0b0000111)
      {
        while ( sensor != 0b0001100 && sensor != 0b0011000)
        {
          sensor = c.IRLed_GetAllFilted();

          c.Run (80, 0);
          c.Turn ( 60 );
          //          delay (10);
          //          if ( sensor == 0b0001100 || sensor == 0b0011000)
          //          {
          //            Signal = true;
          //          }
        }
        Thang = true ;
        Done = true ;
      }
      if (sensor == 0b1111000 || sensor == 0b1111100 || sensor == 0b1110000)
      {
        while (sensor != 0b001100 && sensor != 0b0011000)
        {
          //        while ( !Signal)
          //        {
          sensor = c.IRLed_GetAllFilted();
          c.Run (0, 80);
          c.Turn ( -60  );
          //
          //          if ( sensor == 0b0001100 || sensor == 0b0011000)
          //          {
          //            Signal = true;
          //          }
        }
        Thang = true ;
        Done = true ;
      }
      //===================================================
      // mat line
      if (sensor == 0b0000000)
      {
        uint8_t sensor = c.IRLed_GetAllFilted();
        while (!a)
        {
          sensor = c.IRLed_GetAllFilted();
          c.Run (70, 70);
          c.Turn (13);
          if (sensor == 0b0000001)
          {
            a = true;
          }
        }
        while (!b)
        {
          sensor = c.IRLed_GetAllFilted();
          c.Run (65, 65);
          c.Turn (-13);
          if (sensor == 0b0001000 || sensor == 0b0001100 || sensor == 0b0011000 || sensor == 0b0011100)
          {
            b = true;
          }
        }
        Thang = true ;
        Done = true ;
      }
    }
  }
}

void chuyen_lan (bool isLeft) {
  uint8_t sensor = c.IRLed_GetAllFilted();
  bool isDone = false;
  bool isThang = false;
  bool isSignal = false;
  unsigned long _time = millis();
  while (!isDone) {
    sensor = c.IRLed_GetAllFilted();
    while (!isThang) {
      sensor = c.IRLed_GetAllFilted();
      speed_car = 70;
      chay_binh_thuong ();
      if (sensor == 0b1111111 || sensor == 0b1111110 || sensor == 0b0111111)
      {
        while (!isSignal )
        {
          sensor = c.IRLed_GetAllFilted();
          if (isLeft == true )
          {
            c.Run (80, 0);
            c.Turn ( 60);
            if (sensor == 0b0001100 || sensor == 0b0000110 )
            {
              isSignal = true ;
            }
          }
          if ( isLeft == false )
          {
            c.Run (0, 80);
            c.Turn ( -60 );
            if (sensor == 0b0011000 || sensor == 0b0000110) {
              isSignal = true ;
            }
          }
          isThang = true ;
        }
        isDone = true ;
      }
      //====================================================================
      if (sensor == 0b0000000)
      {
        while (!isSignal) {
          sensor = c.IRLed_GetAllFilted();
          if (isLeft == false)
          {
            c.Turn (-20);
            c.Run ( 80, 80);
            sensor = c.IRLed_GetAllFilted();
            if ( sensor == 0b1000000  )
            {
              isSignal = true;
            }
          }
          else {
            c.Turn (20);
            c.Run (80, 80);
            if (  sensor == 0b0000001)
            {
              isSignal = true;
            }
          }

        }
        _time = millis();
        if (isLeft == true) {
          // be goc phai
          c.Turn (-30);
          c.Run (50, 60);
        }
        //re phai
        else {
          //be goc trai
          c.Turn (30);
          c.Run (60, 50);
        }
        while (millis() - _time < 600 ) {
          //re lai dung huong
          //          c.Run (100, 100);
        }
        isThang = true ;
        isDone = true;
      }
    }
  }
}
void Read_sensor ()
{
  bool Done = false ;
  uint16_t temp1[7] ;
  uint16_t temp2[7]   ;
  int max_value = 0;

  while (!Done)
  {
    //      Serial.println("Stuck in while");
    if (c.Buttons_Check() == BT_1)
    {

      for ( int i = 0 ; i < 7; i++)
      {
        temp1[i] =  c.IRLed_GetEachAnalog ( i , true )  ;
      }
      //      Serial.println ( temp1[1]);
      //      Serial.println("Press 1");
    }
    c.Led_Display(0b11111111);
    //        Show_Leds();

    if (c.Buttons_Check() == BT_2)
    {
      for ( int i = 0 ; i < 7; i++)
      {
        temp2 [i] = c.IRLed_GetEachAnalog ( i , true ) ;
      }
      //      Serial.println ( temp2[2] );
      //      Serial.println("Press 2");
      //        Serial.println("Press 2");
      c.Led_Display(0b11111111);
      //      Show_Leds();
    }
    if (c.Buttons_Check() == BT_3)
    {
      c.Led_Display(0b11111111);
      //      Serial.println("Press 3");
      //      Serial.println("Press 3");
      for (int k = 0 ; k < 7; k++)
      {
        threshold[k] = ((temp1[k] + temp2[k]) / 2) ;
      }
      c.IRLed_SetAllThreshold(threshold);
      //            Serial.println(threshold[1]);
      //      Show_Leds();

      Done = true;
    }
  }
}

void PrintSensorVal(unsigned int *val)
{
  for (int i = 0; i < 7; i++)
  {
    Serial.print((String)val[i] + "\t");
  }
  Serial.println("\n");
}

ISR(TIMER2_OVF_vect) //ISR for Timer1 Overflow
{
  Show_Leds();
}
