#include <math.h>
#define START_MSG "\n" + String(__FILE__) + " start."
#define IR_PIN 2
 
void setup()
{
  delay(100);
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
  Serial.println(START_MSG);
}
 
typedef struct {
  long  high;   // ↓遷移検出タイミング
  long  low;   // ↑遷移検出タイミング
  int high_T; // H持続時間（usec)
  int low_T; // L持続時間（usec)
  int value;  // 論理値 : 0 or 1
} irdata_t;
 
void decode_data(int t, int count);
 
#define AEHA_T  425
#define NEC_T 560
 
#define DATA_COUNT  500
irdata_t irdata[DATA_COUNT];
 
void loop()
{
  long first_time;
  int t_length;
  int data_count = 0;
  static int last_state  = 1;
  
  int state = digitalRead(IR_PIN);
  if (state == LOW) { // 最初の立下り検出
    last_state = state;
    data_count = 0;
    first_time = micros();
    irdata[data_count].high = first_time;
    while(data_count < DATA_COUNT) { 
      long now = micros();
      if (now < first_time) {
        data_count = 0;
        break;
      }
      if (now - first_time > 1000000L)
        break;  // 1000msec経過したらやめる。
      state = digitalRead(IR_PIN);
      if (state == last_state)
        continue;
      last_state = state;
      if (state == LOW) // active lowなので。
        irdata[data_count].high = now;
      else
        irdata[data_count++].low = now;
    }
// 検出終了。データ解析
    if (data_count > 20) {
      int i;
      int m = 0;
      int nec = 0;
      int aeha = 0;
      for(i = 0; i < data_count - 1; i++) {
        irdata[i].high = irdata[i].low - irdata[i].high;
        irdata[i].low = irdata[i + 1].high - irdata[i].low;
        if (i < 20) {
          int high = irdata[i].high;
          if (high > 800)
              continue;
          if (abs(high - AEHA_T) <= AEHA_T * 0.15)
            aeha++;
          if (abs(high - NEC_T) <= NEC_T * 0.15)
            nec++;        
        }
      }
      if (aeha > 10 && aeha > nec)
        t_length = AEHA_T;
      else if (nec > 10 && nec > aeha)
        t_length = NEC_T;
      else
        t_length = 500; // どちらでもない。適当な値
      Serial.println("use T = " + String(t_length) + " usec.");
      // 論理値に変換。
      for(i = 0; i < data_count; i++) {
        irdata[i].high_T = (int) round((float)irdata[i].high / t_length);
        irdata[i].low_T = (int) round((float)irdata[i].low / t_length);
        irdata[i].value = 0;
        if (irdata[i].high_T == 1 && irdata[i].low_T == 1)
          irdata[i].value = 0;
        else if (irdata[i].high_T == 1 && irdata[i].low_T == 3)
          irdata[i].value = 1;
#if 1
      // dump raw data
        char tmp[255];
        sprintf(tmp, "%d\tH = %d, L= %d", (i + 1), irdata[i].high, irdata[i].low);
        Serial.print(tmp);
        sprintf(tmp, "  H : %d %% of T", irdata[i].high * 100 / t_length);
        Serial.print(tmp);
        sprintf(tmp, "  ON=%d OFF=%d value=%d", irdata[i].high_T, irdata[i].low_T, irdata[i].value);
        Serial.println("\t" + String(tmp));  
#endif
        delay(1);
      }
      decode_data(t_length, data_count);
      data_count = 0;
      first_time = 0;
      last_state = 1;
    }
    delay(1);
  }
}
 
// decode data from leader.
void decode_data(int t_length, int total) {
    bool in_frame = false;
    int data[300];
    int count = 0;
    int index = 0;
    if (total == 0)
      return;
    Serial.println("\nRx Data ----"); 
    while (index < total) {
      delay(1);        
      if ((t_length == AEHA_T && irdata[index].high_T == 8 && irdata[index].low_T == 4) ||
          (t_length == NEC_T && irdata[index].high_T == 16 && irdata[index].low_T == 8)) {  // リーダー検出
        index++;
        in_frame = true;
        continue;
      }
      if (!in_frame) {
        index++;
        continue;
      }
      if (irdata[index].low > 8000) { // トレーラー
        index++;
        in_frame = false;
        Serial.println("\n-----");  
        continue;
      }
      if (total - index < 8)
        break;
      int cur = 0;
      for(int i = 0; i < 8; i++)
        cur = cur >> 1 | (irdata[index++].value ? 0x80 : 0);       
      data[count++] = cur;
      char tmp[255];
      sprintf(tmp, "%02x ", cur);
      Serial.print(tmp);  
    }
    Serial.println("\n-----");  
}
