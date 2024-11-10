// ESP32 Touch Test
// Just test touch pin - Touch0 is T0 which is on GPIO 4.
 int TT7 =0 ;
 int TT8 =0 ;
 int TT9 =0 ;
 int TT10 =0 ;
 int TT11 =0 ;
 int TT12 =0 ;
 int TT13 =0 ;
 int TT14 =0 ;
void setup()
{
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
  // TT7 = touchRead(T7);
  // TT8 = touchRead(T8);
  // TT9 = touchRead(T9);
  // TT10 = touchRead(T10);
  // TT11 = touchRead(T11);
  // TT12 = touchRead(T12);
  // TT13 = touchRead(T13);
  // TT14 = touchRead(T14) + 10;
    delay(1000);
}

void loop()
{
  
  Serial.print(" T7   ");
  Serial.println(touchRead(T7) - TT7);
  Serial.print(" T8   ");
  Serial.println(touchRead(T8) - TT8);
  Serial.print(" T9   ");
  Serial.println(touchRead(T9) - TT9);
  Serial.print(" T10  ");
  Serial.println(touchRead(T10) - TT10);
  Serial.print(" T11  ");
  Serial.println(touchRead(T11) - TT11);
  Serial.print(" T12  ");
  Serial.println(touchRead(T12) - TT12);
  Serial.print(" T13  ");
  Serial.println(touchRead(T13) - TT13);
  Serial.print(" T14  ");
  Serial.println(touchRead(T14) - TT14);
  Serial.println(" ");
  delay(1000);
}
