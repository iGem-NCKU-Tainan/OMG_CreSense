#include <ESP8266WiFi.h>                                                // esp8266 library
#include <FirebaseArduino.h>    
#include <SoftwareSerial.h>   
#include <time.h> 

#define WIFI_SSID "abc123"
#define WIFI_PASSWORD "123123123"
#define FIREBASE_HOST "igem-ncku-ohmygut.firebaseio.com"
#define FIREBASE_AUTH "KsQ7MzFDS0wcUqIH7ediCogo4FukHmlswARj4I9A"
  
int timezone = (-4) * 3600;//設定時區因為boston是-5所以->時區*3600
int dst = 0;
  
void setup() {
  Serial.begin(115200);  // Open serial communications and wait for port to open:
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);               //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                  //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");
  
  while(!time(nullptr)){
    Serial.print("*");
    delay(1000);
  }
  Serial.println("\nTime response....OK"); 
}
  
int n=0;
float val;
  
void loop() { // run over and over
  while(Serial.available()>0){
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    Serial.print(p_tm->tm_mday);
    Serial.print("/");
    Serial.print(p_tm->tm_mon + 1);
    Serial.print("/");
    Serial.print(p_tm->tm_year + 1900);
  
    Serial.print(" ");
  
    Serial.print(p_tm->tm_hour);
    Serial.print(":");
    Serial.print(p_tm->tm_min);
    Serial.print(":");
    Serial.println(p_tm->tm_sec); 
  
    float val=Serial.parseFloat();
    if (Serial.read()=='\n') {
      Serial.println(val);
    }
  
    /*****send data to firebase*********/
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& valueObject = jsonBuffer.createObject();
  
    valueObject["brightness"] = val;
    
    String mon;
    String day;
    String hour;
    String minu;
    String sec;
    if(p_tm->tm_mon==0||p_tm->tm_mon==1||p_tm->tm_mon==2||p_tm->tm_mon==3||p_tm->tm_mon==4||p_tm->tm_mon==5||p_tm->tm_mon==6||p_tm->tm_mon==7||p_tm->tm_mon==8)
    {mon="0"+String(p_tm->tm_mon+1);}
    else{mon=String(p_tm->tm_mon+1);}
   
    if(p_tm->tm_mday==0||p_tm->tm_mday==1||p_tm->tm_mday==2||p_tm->tm_mday==3||p_tm->tm_mday==4||p_tm->tm_mday==5||p_tm->tm_mday==6||p_tm->tm_mday==7||p_tm->tm_mday==8||p_tm->tm_mday==9)
    {day="0"+String(p_tm->tm_mday);}
    else{day=String(p_tm->tm_mday);}
    
    if(p_tm->tm_hour==0||p_tm->tm_hour==1||p_tm->tm_hour==2||p_tm->tm_hour==3||p_tm->tm_hour==4||p_tm->tm_hour==5||p_tm->tm_hour==6||p_tm->tm_hour==7||p_tm->tm_hour==8||p_tm->tm_hour==9)
    {hour="0"+String(p_tm->tm_hour);}
    else{hour=String(p_tm->tm_hour);}
    
    if(p_tm->tm_min==0||p_tm->tm_min==1||p_tm->tm_min==2||p_tm->tm_min==3||p_tm->tm_min==4||p_tm->tm_min==5||p_tm->tm_min==6||p_tm->tm_min==7||p_tm->tm_min==8||p_tm->tm_min==9)
    {minu="0"+String(p_tm->tm_min);}
    else{minu=String(p_tm->tm_min);}
    
    if(p_tm->tm_sec==0||p_tm->tm_sec==1||p_tm->tm_sec==2||p_tm->tm_sec==3||p_tm->tm_sec==4||p_tm->tm_sec==5||p_tm->tm_sec==6||p_tm->tm_sec==7||p_tm->tm_sec==8||p_tm->tm_sec==9)
    {sec="0"+String(p_tm->tm_sec);}
    else{sec=String(p_tm->tm_sec);}

    
    Firebase.setFloat("6/"+mon + "/"+day+"/"+hour+":"+minu+":"+sec, val);
    
    /*******send time to arduino*********/
    
    int b[6] = {255,0,0,0,0,254};
    b[1] =p_tm->tm_mon;
    b[2] = p_tm->tm_mday;
    b[3] = p_tm->tm_hour;
    b[4] = p_tm->tm_min;
      
    for(int i=1;i<5;i++)
    {
      Serial.write(b[i]);
    }
  
    // handle error
    if (Firebase.failed()) {
      Serial.print("pushing failed:");
      Serial.println(Firebase.error());  
      return;
    }
  }
}
  
   
