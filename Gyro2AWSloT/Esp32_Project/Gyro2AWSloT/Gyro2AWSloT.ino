#include "sea_esp32_qspi.h"
#include "SimpleDHT.h"
#include "string.h"

#include "AWS_IOT.h"
#include "WiFi.h"
#include <spartan-edge-esp32-boot.h>
#include "ESP32IniFile.h"

#define SHAKE_LIMIT 25000
#define COUNT_DELAY 5
#define COUNT_END 60000
// initialize the spartan_edge_esp32_boot library
spartan_edge_esp32_boot esp32Cla;

const size_t bufferLen = 80;
char buffer[bufferLen];
char buffer1[bufferLen];
AWS_IOT hornbill;

char WIFI_SSID[]="502";
char WIFI_PASSWORD[]="00000000";
char HOST_ADDRESS[]="a2uo9ix2irhly4-ats.iot.us-east-1.amazonaws.com";
char CLIENT_ID[]= "MQTT_FX_Client";
char TOPIC_NAME[]= "$aws/things/tomatoes/shadow/update";


int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0,tick1=0;
char payload[512];
char rcvdPayload[512];
uint8_t data2[2];

void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

int16_t read_data(){
    uint8_t data_x[2];
    int16_t *x;
    SeaTrans.read(2,data_x,2);
    x = (int16_t*)data_x;
    return *x;
}

int8_t shake(){
    int16_t temp = read_data();
    if(temp > SHAKE_LIMIT)
       return 1;
    else if(temp < -SHAKE_LIMIT)
       return -1;
    else
       return 0; 
}

void setup()
{
  //read .bit file
  // initialization 
  esp32Cla.begin();

  // check the .ini file exist or not
  const char *filename = "/board_config.ini";
  IniFile ini(filename);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(filename);
    Serial.println(" does not exist");
    return;
  }
  Serial.println("Ini file exists");

  // check the .ini file valid or not
  if (!ini.validate(buffer, bufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    return;
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Overlay_List_Info", "Overlay_Dir", buffer, bufferLen)) {
    Serial.print("section 'Overlay_List_Info' has an entry 'Overlay_Dir' with value ");
    Serial.println(buffer);
  }
  else {
    Serial.print("Could not read 'Overlay_List_Info' from section 'Overlay_Dir', error was ");
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Board_Setup", "overlay_on_boot", buffer1, bufferLen)) {
    Serial.print("section 'Board_Setup' has an entry 'overlay_on_boot' with value ");
    Serial.println(buffer1);
  }
  else {
    Serial.print("Could not read 'Board_Setup' from section 'overlay_on_boot', error was ");
  }

  // Splicing characters
  strcat(buffer,buffer1);
  
  // XFPGA pin Initialize
  esp32Cla.xfpgaGPIOInit();

  // loading the bitstream
  esp32Cla.xlibsSstream(buffer);


  //connect to WIFI and AWS_IOT
  Serial.begin(115200);
  SeaTrans.begin();
  delay(2000);

    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // wait 5 seconds for connection:
        delay(5000);
    }

    Serial.println("Connected to wifi");

    if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==hornbill.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfully");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }

    delay(2000);
  
}

void loop()
{
    Serial.println("现在开始计数：");
    unsigned long time_end = millis()+COUNT_END;
    int count = -1;
    int8_t pre = 0;
    int8_t next = 0;
    while(millis()<time_end){
        delay(COUNT_DELAY);
        next = shake();
        if((1 == next) && (1!=pre))
        {
            pre = next;
            ++count;  
        }
        else if((-1 == next) && (-1!=pre))
        {
            pre = next;
            ++count;  
        }
        else
        ;
    }
    Serial.println(count);
    sprintf(payload,"在过去60秒钟内，摇摆次数为%d",count);
    if(hornbill.publish(TOPIC_NAME,payload) == 0)
        {        
            Serial.println("Publish successfully!");
            Serial.println(payload);
        }
        else
        {
            Serial.println("Publish failed!");
        }
    Serial.println("------------------------------------");
}
