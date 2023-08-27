//핀 번호 지정
#define TRIG1 33
#define TRIG2 35
#define TRIG3 37
#define ECHO1 32
#define ECHO2 34
#define ECHO3 36

//전역변수 설정

int trig[3] = {TRIG1, TRIG2, TRIG3};
int echo[3] = {ECHO1, ECHO2, ECHO3};
double distance[3];
int LED[3][3];
int adj[3][3];

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i<=2; i++){
    pinMode(echo[i], INPUT);
  }

  for (int i = 0; i<=2; i++){
    pinMode(trig[i], OUTPUT);
  }
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      LED[i][j] = i * 3 + j + 2;
      pinMode(LED[i][j], OUTPUT);
      digitalWrite(LED[i][j], HIGH);
    }
  }
}

bool chk[3][3];
bool found = false;

bool dfs(int x, int y, int end) {
  chk[x][y] = true;
  if (found) return false;
  if (x * 3 + y == end) {
    found = true;
    digitalWrite(LED[x][y], HIGH);
    return true;
  }
  int dx[4] = { 1, 0, -1, 0 };
  int dy[4] = { 0, -1, 0, 1 };
  for (int i = 0; i < 4; i++) {
    int nxtx = x + dx[i];
    int nxty = y + dy[i];
    if (nxtx >= 3 || nxtx < 0 || nxty >= 3 || nxty < 0) continue;
    if (chk[nxtx][nxty] || adj[nxtx][nxty] == 1) continue;
    bool res = dfs(nxtx, nxty, end);
    if (res) {
      digitalWrite(LED[x][y], HIGH);
      return true;
    }
  }
  return false;
}

double dur;

void loop() 
{
  for (int i = 0; i < 3; i++){

    digitalWrite(trig[i], LOW);
    delay(2);
    digitalWrite(trig[i], HIGH);
    delay(10);
    digitalWrite(trig[i], LOW);
    dur = pulseIn(echo[i], HIGH);

    distance[i] = (340.0 * dur / 10000.0) / 2.0;
    Serial.println(distance[i]);
  }

  for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) {
    adj[i][j] = 0;
    digitalWrite(LED[i][j], LOW);
  }
  for (int i = 0; i < 3; i++) {
    
    int dist = distance[i];
    if (dist >= 0.0 && dist < 7.0) { // 1
      adj[i][0] = 1;
    } else if (dist >= 7.0 && dist < 14.0) {
      adj[i][1] = 1;
    } else if (dist >= 14.0 && dist < 21.0) {
      adj[i][2] = 1;
    }

  }

  int end = 7;
  for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) chk[i][j] = false;
  found = false;
  dfs(0, 1, end); // start is 1; (0, 1)

  delay(1000);

}