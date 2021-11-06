// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <RTClib.h>


RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int waterStopHour = 8;
bool wateringInProgress = false;
const int relay1 = 10;

DateTime lastWateredTime = DateTime(2012, 1, 21, 6, 59, 40);
void setup () {
  Serial.begin(9600);
  //delay(3000); // wait for console opening

  //Relay 1 setup
  pinMode(relay1, OUTPUT);
  //Define initial state as OFF
  digitalWrite(relay1, HIGH);
  
  //Real Time clock DS1307 setup
 if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
   while (1);
 }else{
  Serial.println("RTC Found");
 }
 if (! rtc.isrunning()) {
   Serial.println("RTC is NOT running!");
   // following line sets the RTC to the date & time this sketch was compiled
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // This line sets the RTC with an explicit date & time, for example to set
   // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
 }else{
  Serial.println("RTC is running !");
  // following line sets the RTC to the date & time this sketch was compiled
   //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   
   //rtc.adjust(DateTime(2021, 1, 1, 6, 59, 50));
 }
}

bool isNextDay(){
  DateTime now = rtc.now();
  Serial.println("Last Watered:");
   Serial.print(lastWateredTime.year(), DEC);
  Serial.print('/');
  Serial.print(lastWateredTime.month(), DEC);
  Serial.print('/');
  Serial.print(lastWateredTime.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[lastWateredTime.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(lastWateredTime.hour(), DEC);
  Serial.print(':');
  Serial.print(lastWateredTime.minute(), DEC);
  Serial.print(':');
  Serial.print(lastWateredTime.second(), DEC);
  Serial.println();
  if(lastWateredTime.year() == 2012){
    return true; //possible a current cut happened and default is running.
  }
  if(lastWateredTime.day()  > now.day() || lastWateredTime.month() > now.month()
      || lastWateredTime.year() > now.year()){
    return true;
  }
  return false;
}
void loop () {
 DateTime now = rtc.now();
  
  Serial.print("Time:");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
 if(now.hour()>=7 && now.hour() <= 10 && !wateringInProgress && isNextDay()){
  wateringInProgress = true;
  lastWateredTime = rtc.now();
  digitalWrite(relay1, LOW); //Low switches on the relay since its in normally open mode
  Serial.println("Good morning, Plants !! Time for water");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  
  DateTime future (now + TimeSpan(0,0,0,30)); //one hour from start watering
  waterStopHour = future.second();
 }else if(now.second() >= waterStopHour && wateringInProgress){
  wateringInProgress = false;
  digitalWrite(relay1, HIGH); //HIGH switches off the relay since relay is operating at normally open mode
  Serial.println("Stopped watering at ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
 }
 delay(3000);
}
