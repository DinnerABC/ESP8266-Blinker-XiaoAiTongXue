#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>

uint8_t wsMode = BLINKER_CMD_MIOT_DAY;

char auth[] = ""; //key
char ssid[] = "wifi";
char pswd[] = "password";

#define DEBUG //是否开启debug功能

#ifdef DEBUG
#define DebugPrintln(message)    Serial.println(message)
#else
#define DebugPrintln(message)
#endif

#ifdef DEBUG
#define DebugPrint(message)    Serial.print(message)
#else
#define DebugPrint(message)
#endif

int GPIO0 = 0; //定义继电器输入引脚为GPIO0/0
int GPIO5 = 4;
// 新建组件对象
BlinkerButton Button1("btn-abc");//注意：要和APP组件’数据键名’一致
BlinkerButton Button2("btn-123");
// 按下BlinkerAPP按键即会执行该函数
void button1_callback(const String & state) {
  BLINKER_LOG("get button state: ", state);
  digitalWrite(GPIO0, !digitalRead(GPIO0));
  digitalWrite(GPIO5, !digitalRead(GPIO5));
  Blinker.vibrate();
}

void button2_callback(const String & state) {
  BLINKER_LOG("get button state: ", state);
  digitalWrite(GPIO5, !digitalRead(GPIO5));
  Blinker.vibrate();
}

//小爱电源类操作的回调函数:
//当小爱同学向设备发起控制, 设备端需要有对应控制处理函数
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);
  if (state == BLINKER_CMD_ON) {
    digitalWrite(GPIO0, LOW);
    digitalWrite(GPIO5, LOW);
    BlinkerMIOT.powerState("OFF");
    BlinkerMIOT.print();
  }
  else if (state == BLINKER_CMD_OFF) {
    digitalWrite(GPIO0, HIGH);
    digitalWrite(GPIO5, HIGH);
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
  }
}

void miotMode(uint8_t mode)
{
  BLINKER_LOG("need set mode: ", mode);

  if (mode == BLINKER_CMD_MIOT_DAY) { //日光模式
    digitalWrite(GPIO0, HIGH);
    digitalWrite(GPIO5, LOW);
    BlinkerMIOT.print();
  }
  else if (mode == BLINKER_CMD_MIOT_NIGHT) { //月光模式
    digitalWrite(GPIO0, LOW);
    digitalWrite(GPIO5, HIGH);
    BlinkerMIOT.print();
  }
  /*else if (mode == BLINKER_CMD_MIOT_COLOR) {
      digitalWrite(GPIO0, LOW);
      digitalWrite(GPIO5, LOW);
      BlinkerMIOT.print();
    }*/
  else if (mode == BLINKER_CMD_MIOT_WARMTH) { //温馨模式
    digitalWrite(GPIO0, LOW);
    digitalWrite(GPIO5, LOW);
    BlinkerMIOT.print();
  }
  /*  else if (mode == BLINKER_CMD_MIOT_TV) {
        digitalWrite(GPIO0, LOW);
        digitalWrite(GPIO5, LOW);
        BlinkerMIOT.print();
    }
    else if (mode == BLINKER_CMD_MIOT_READING) {
        digitalWrite(GPIO0, LOW);
        digitalWrite(GPIO5, LOW);
        BlinkerMIOT.print();
    }
    else if (mode == BLINKER_CMD_MIOT_COMPUTER) {
        digitalWrite(GPIO0, LOW);
        digitalWrite(GPIO5, LOW);
        BlinkerMIOT.print();
    }*/

  wsMode = mode;

  BlinkerMIOT.mode(mode);
  BlinkerMIOT.print();
}
void setup() {
  // 初始化串口，并开启调试信息，调试用可以删除
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  // 初始化IO
  pinMode(GPIO0, OUTPUT);
  pinMode(GPIO5, OUTPUT);
  digitalWrite(GPIO0, HIGH);
  digitalWrite(GPIO5, HIGH);
  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);

  Button1.attach(button1_callback);
  DebugPrintln("AutoConfig Success");
  DebugPrint("SSID:");
  DebugPrintln(WiFi.SSID().c_str());
  DebugPrint("PSW:");
  DebugPrintln(WiFi.psk().c_str());
  //小爱同学务必在回调函数中反馈该控制状态
  BlinkerMIOT.attachPowerState(miotPowerState);//注册回调函数
  BlinkerMIOT.attachMode(miotMode);
}
void loop() {
  Blinker.run();
}
