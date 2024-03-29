#define BUZZER 26
#define BUTTON 0

hw_timer_t *buzzer_timer = NULL;
hw_timer_t *state_timer = NULL;

// C C# D Eb E F F# G G# A Bb B A
// 0 1  2 3  4 5 6  7 8  9 10 11 12
int key_note[13] = {1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 880, 1865, 1976, 1760};

int note[] = {9, 0, 2, 2, 2, 4, 5, 5, 5, 7, 4, 4, 2, 0, 2, 2, 9, 0, 2, 2, 2, 4, 5, 5, 5, 7, 4, 4, 2, 0, 2, 2, 
              9, 0, 2, 2, 2, 5, 7, 7, 7, 12, 10, 10, 12, 7, 12, 2, 2, 2, 4, 4, 5, 5, 7, 7, 12, 12, 2, 2, 2, 5, 4, 4, 4, 5, 2, 4};

int state = 0;
int delay_note_time = 250000;
int delay_note_time2 = 250000;
int note_time = 1000000;
int speed = 0;

void IRAM_ATTR onBuzzerTimer()
{
  digitalWrite(BUZZER, !digitalRead(BUZZER));
}
void IRAM_ATTR onStateTimer()
{
  if (speed == 0)
  {
    delay_note_time = delay_note_time2 * 1;
  }
  else if (speed == 1)
  {
    delay_note_time = delay_note_time2 * 0.75;
  }
  else if (speed == 2)
  {
    delay_note_time = delay_note_time2 * 0.5;
  }
  else if (speed == 3)
  {
    delay_note_time = delay_note_time2 * 1.5;
  }
  else if (speed == 4)
  {
    delay_note_time = delay_note_time2 * 1.25;
  }
  note_time = (note[state] == -1) ? 10 : int((1.0 / (key_note[note[state]] * 1.0)) * 1e6);
  timerAlarmWrite(buzzer_timer, note_time, true);
  timerAlarmWrite(state_timer, delay_note_time, true);
  state++;
}
void IRAM_ATTR IO_INT_ISR()
{
  if (!digitalRead(BUTTON) == 1)
  {
    speed = (speed > 3) ? 0 : speed + 1;
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(BUTTON, IO_INT_ISR, FALLING);
  buzzer_timer = timerBegin(0, 80, true);  
  timerAttachInterrupt(buzzer_timer, &onBuzzerTimer, true); 
  timerAlarmWrite(buzzer_timer, note_time, true);    
  timerAlarmEnable(buzzer_timer);

  state_timer = timerBegin(1, 80, true);
  timerAttachInterrupt(state_timer, &onStateTimer, true);
  timerAlarmWrite(state_timer, 225000, true);
  timerAlarmEnable(state_timer);
}
void loop()
{
  Serial.println(speed);
}