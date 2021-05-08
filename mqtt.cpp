#include "mqtt.h"
#include <SoftwareSerial.h>
#include <Arduino_JSON.h>

bool ping = false;
int count = 0;

bool pub = false;
int count2 = 0;
//oftwareSerial mySerial(10,11);
SoftwareSerial mySerial(14, 12);
Mqtt::Mqtt(bool displayMsg) {

}

void Mqtt::begin(int baudRate) {
  mySerial.begin(baudRate);
  Serial.begin(baudRate);  

  Serial.println("ok");
 initAt();
 http();  /*
  cli();                     
  TCCR1A = 0;               
  TCCR1B = 0;                
  TCCR1B |= B00000101;  
  TIMSK1 |= B00000010;       
  OCR1A = 15624;            
  sei();   */
}
/*
ISR(TIMER1_COMPA_vect){
  TCNT1  = 0; 
  count++;
    count2++;
  if(count >= 50){
    ping = true;
    count = 0;
  }
  if(count2 >=70){
    pub = true;
    count2 = 0;
  }
}*/

void Mqtt::connect(String server,String server1,String port,String s,bool auth,String user,String pswd) {
    serverr =server;
    portt =port;
    clientId = s;
    withauth = auth;
    userr = user;
    pwdd = pswd;

   mySerial.println("AT+CIPSHUT\r");
  delay(2000);
  byte co[] = {0x00, 0x04, 0x4d, 0x51, 0x54,
               0x54, 0x04, 0xc2, 0x00, 0x3c, 0x00};
  byte pwd[] = {0x00};

  int length = s.length()+user.length()+pswd.length()+16;
  if(!auth){
    co[7] = 0x02;
    length-= 4;

  }
  mySerial.print("AT+CIPSTART=\"TCP\",\"");
  delay(1000);

  mySerial.print(server);
  delay(1000);
  rr();
    mySerial.print(server1);
  delay(1000);
  rr();
   mySerial.print("\",\"");
  delay(1000);
 rr();
  mySerial.print(port);
  delay(1000);
 rr();
  mySerial.println("\"\r");

  delay(5000);
 rr();

  mySerial.print("AT+CIPSEND\r");
  delay(3000);

 rr();
  mySerial.write(0x10);
  mySerial.write(length);
  mySerial.write(co, 11);

  mySerial.write(s.length());
  mySerial.print(s);
 
  if(auth){
    mySerial.write(pwd, 1);
    mySerial.write(user.length());
    mySerial.print(user);
    mySerial.write(pwd, 1);  

    mySerial.write(pswd.length());
    mySerial.print(pswd);
  }


  delay(1000);
 rr();
  // mySerial.write(0x1a);
  mySerial.println("\r");
  delay(1000);
 rr();
}
void Mqtt::publish(String topic,  String msg) {
  int length = 5 + topic.length() + msg.length();
  byte pu[] = {0x00};
  mySerial.print("AT+CIPSEND=");
  mySerial.print(length-1);
  mySerial.print("\r");
  delay(3000);
   rr();
  mySerial.write(0x31);
  mySerial.write(length - 3);
  mySerial.write(pu, 1);
  mySerial.write(topic.length());
  mySerial.print(topic);
  mySerial.print(msg);
  delay(1000);
   rr();
  mySerial.write(0x1a);
  delay(1000);
   rr();
}
void Mqtt::subscribe(String topic) {
  topicc =topic;
  int length = 7 + topic.length();
  byte su[] = {0x9b, 0x9c, 0x00};
  byte s[] = {0x00};
  mySerial.print("AT+CIPSEND=");
  mySerial.print(length);
  mySerial.print("\r");
  delay(1000);
   rr();
  mySerial.write(0x82);
  mySerial.write(length - 2);
  mySerial.write(su, 3);
  mySerial.write(topic.length());
  mySerial.print(topic);
  mySerial.write(s, 1);
  mySerial.write(0X1a);
  delay(3000);
   rr();
   connected = true;
}
void Mqtt::initAt() {
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r");

  delay(1000);
   rr();
  mySerial.println("AT+CIPSHUT\r");

  delay(1000);
   rr();
  mySerial.println("AT+CIPSTATUS\r");
  delay(1000);
   rr();
  mySerial.println("AT+CIPMUX=0\r");
  delay(1000);
   rr();
  mySerial.println("AT+CSTT=\"internet.ooredoo.tn\"\r");
  delay(1000);
   rr();
  mySerial.println("AT+CIICR\r");
  delay(5000);
   rr();
  mySerial.println("AT+CIFSR\r");
  delay(1000);
   rr();
  mySerial.println("AT+CIPSPRT=0\r");
  delay(1000);
   rr();
}
void Mqtt::rr() {
  while (mySerial.available() >= 1) {
    Serial.write(mySerial.read());
  }
}
void Mqtt::http(){
     mySerial.println("AT+SAPBR=2,1\r");
  delay(5000);
   rr();
  mySerial.println("AT+SAPBR=1,1\r");
  delay(5000);
   rr();
    mySerial.println("AT+HTTPTERM\r");
  delay(1000);
  rr();
  mySerial.println("AT+HTTPINIT\r");
  delay(1000);
  rr();
  mySerial.println("AT+HTTPPARA=\"URL\",\"http://safta-iot.000webhostapp.com/espitems/4\"\r");
  delay(1000);
  rr();
  mySerial.println("AT+HTTPPARA=\"CID\",1\r");
  delay(1000);
  rr();
  mySerial.println("AT+HTTPACTION=0\r");
  delay(2000);
  rr();
  while (mySerial.available()<=0) {
  Serial.print(".");
  delay(1000);
  }
  rr();
  mySerial.println("AT+HTTPREAD\r");

  String s = mySerial.readString();
  s= s.substring(s.indexOf("["),s.indexOf("]")+1);
  Serial.print("response : => ");
  Serial.println(s);
  mySerial.println("AT+HTTPTERM\r");
  delay(1000);
  rr();

  JSONVar myObject = JSON.parse(s);
  if (JSON.typeof(myObject) == "undefined") {
  Serial.println("Parsing input failed!");
  return;
  }
 Serial.println(myObject.length());
   for (int i=0; i<myObject.length(); i++) {
  Serial.println((int)myObject[i]["value"]);

    if((String)((const char*)myObject[i]["action"])=="D"){
      pinMode((int)myObject[i]["pin"],OUTPUT);
      digitalWrite((int)myObject[i]["pin"],!(int)myObject[i]["value"]);
    }

   }

}
void Mqtt::getData(){
 while (true) {
      if (available()) {
         line[ lineIndex ] = '\0';                   
        lineIndex = 0;
        String s(line);
        Serial.println(line);
        if(s.indexOf("ok")>=0|| s.indexOf("ERROR")>=0|| s.indexOf("FAIL")>=0){
          break;
        }
      }
  }
}
void Mqtt::sendPing(){

  if(connected){
  byte co[] = {0xc0,0x00,0x1a};
  mySerial.print("AT+CIPSEND\r");
  delay(2000);
  mySerial.write(co,3);
  Serial.println("ping");
  }
}
bool Mqtt::available(){
    if(ping){
      ping = false;
      sendPing();
      count = 0;
    }

        if(pub){
      pub = false;
      publish("iot-2/4561", "{\"action\":\"D\",\"pin\":13,\"value\":1,\"options\":50}");
      count2 = 0;
    }
     while ( mySerial.available()>0 ) {
     char  c = mySerial.read();
     //Serial.print(c);
      if (/*( c == 0x0d) ||*/ ( c == 0x0a)) {  
        if ( lineIndex > 2 ) {    
          return true; 
        }else {
            line[ 0 ] = '\0';              
            lineIndex = 0;
         return false;
        }      

          //checkResponse();
        }else if ( c == '}'){
          line[ lineIndex++ ] = c;      
          return true;                                   
          //checkResponse(true);

        } else {
          if ( lineIndex >= LINE_BUFFER_LENGTH-1 ) {
            Serial.println("overflow");
             lineIndex = 0;
          } 
          else if ( c >= 'a' && c <= 'z' ) {        
            line[ lineIndex++ ] = c-'a'+'A';
          } 
          else if ( (c >= '0' && c <= '9') ||(c >= 'A' && c <= 'Z') || c==':'|| c=='{'|| c==0x22|| c==',') {
            line[ lineIndex++ ] = c;
          }
        return false; 
      }
    }
    return false; 
}



String Mqtt::readString(){
    if ( lineIndex > 0 ) {                        
        line[ lineIndex ] = '\0';                   
        lineIndex = 0;
        String s(line);
        if(s.indexOf("IOT")>=0){
          return s.substring(s.indexOf("{"));
        }else if(s.indexOf("OK")>=0){
          return "OK";
        }else if(s.indexOf("CLOSED")>=0 || s.indexOf("ERROR")>=0|| s.indexOf("FAIL")>=0){
              //reconnect
                 connected = false;
              connect(serverr, "ernetofthings.ibmcloud.com",portt,clientId,withauth,userr,pwdd);
              delay(2000);
              subscribe(topicc);
              line[ 0 ] = '\0';
              return "ERROR";
        }
    }else {
        line[ 0 ] = '\0';                   
        lineIndex = 0;
    }
  return "";
}













