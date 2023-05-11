int ls = A1, l1 = A0, l2 = A2, r1 = A3, r2 = A5, rs = A4;
int c1 = 6, c2 = 5, c3 = 4, c4 = 3, c5 = 2;
int sensor1, sensor2, sensor3, sensor4, sensor5 ;
int node = 0, nodeerr, p = 0;
bool v1, v2, v3, v4, v5;

//PID constants
const float Kp = 5, Kd = 3, Ki = 4; // Integral constant

//define variables for PID
float error = 0, previous_error = 0, integral = 0, derivative = 0, pid_value = 0;
char cmd, color;


//char getCmd(char state, int h) {
//  if ( h ) Serial.print(state);
//  char cmd;
//  while (true) {
//    if (Serial.available()) {
//      cmd = Serial.read();
//      if (!(cmd == 'n' || cmd == 'a' || cmd == 'b' || cmd == 'c' || cmd == 'd'))
//        return getCmd(state, 0);
//      //      while (Serial.available()) {
//      //        Serial.read();
//      //      }
//      return cmd;
//    }
//  }
//}

String path[9];
char cmds;

void setup() {

  Serial.begin(115200);
  pinMode(c1, INPUT);
  pinMode(c2, INPUT);
  pinMode(c3, INPUT);
  pinMode(c4, INPUT);
  pinMode(c5, INPUT);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(ls, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(rs, OUTPUT);
  digitalWrite(l1, 0);
  digitalWrite(l2, 1);
  digitalWrite(r1, 0);
  digitalWrite(r2, 1);
  analogWrite(rs, 0);
  analogWrite(ls, 0);
  Serial.println("actd");
  while (true) {
    if (Serial.available()) {
      cmds = Serial.read();
      if ((cmds == '1' || cmds == '2')) {
        cmd = cmds;
        break;
      }

    }
  }
  if (cmds == '1') {
    path[0] = "left";
    path[1] = "right";
    path[2] = "left";
    path[3] = "left";
  } else {
    path[0] = "right";
    path[1] = "left";
    path[2] = "right";
    path[3] = "right";
  }


  Serial.println("hospital");
  while (true) {
    if (Serial.available()) {
      cmds = Serial.read();
      if ((cmds == '1' || cmds == '2')) {
        cmd = cmds;
        break;
      }

    }
  }
  if (cmds == '1') {
    path[4] = "left";
  } else {
    path[4] = "right";
  }


  Serial.println("rue");
  while (true) {
    if (Serial.available()) {
      cmds = Serial.read();
      if ((cmds == '1' || cmds == '2')) {
        cmd = cmds;
        break;
      }

    }
  }
  if (cmds == '1') {
    if (path[4] == "right") {
      path[5] = "forward";
      path[6] = "right";
      path[7] = "left";
      path[8] = "stop";
    } else {
      path[5] = "forward";
      path[6] = "left";
      path[7] = "right";
      path[8] = "stop";
    }
  } else {
    if (path[4] == "right") {
      path[5] = "right";
      path[6] = "left";
      path[7] = "forward";
      path[8] = "stop";
    } else {
      path[5] = "left";
      path[6] = "right";
      path[7] = "forward";
      path[8] = "stop";
    }
  }
  delay(5000);
}


void readSensors() {
  sensor1 = digitalRead(c1);
  sensor2 = digitalRead(c2);
  sensor3 = digitalRead(c3);
  sensor4 = digitalRead(c4);
  sensor5 = digitalRead(c5);
  //  Serial.print(sensor1);Serial.print(sensor2);Serial.print(sensor3);Serial.print(sensor4);Serial.println(sensor5);
}


bool ss = 0;
void go() {
//  if(analogRead(9)<10){
//    
//  }
Serial.println(analogRead(A5));
  //  readSensors();
  error = (!sensor1 * -30) + (!sensor2 * -30) + (!sensor3 * 0) + (!sensor4 * 30) + (!sensor5 * 30);//}
  if (sensor1 && sensor2 && sensor3 && sensor4 && sensor5 && ss) {
    error = previous_error;
  }
  //calculate PID
  integral = integral + error;
  derivative = error - previous_error;
  pid_value = (Kp * error) + (Ki * integral) + (Kd * derivative);

  //update previous error
  previous_error = error;

  if ( error == 0 ) {
    integral = 0 ;
  }

  if (pid_value < 0) {
    analogWrite(rs, 255);
    analogWrite(ls, (255 + pid_value) >= 0 ? (255 + pid_value) : 0);
  } else {
    analogWrite(rs, (255 - 2 * pid_value) >= 0 ? (255 - 2 * pid_value) : 0);
    analogWrite(ls, 255);
  }
  if ((!sensor1 || !sensor5) && (sensor2 && sensor3 && sensor4)) {
    ss = 1;
  } else if (sensor1 && sensor5 && (!sensor2 || !sensor3 || !sensor4)) {
    ss = 0;
  }
}

void forward() {
  //  do {
  //    go();
  //    readSensors();
  //  } while (!(sensor1 && sensor2 && !sensor3 && sensor4 && sensor5));
  digitalWrite(l1, 0);
  digitalWrite(l2, 1);
  digitalWrite(r1, 0);
  digitalWrite(r2, 1);
  analogWrite(rs, 255);
  analogWrite(ls, 255);
  delay(200);
}

void left() {
  analogWrite(rs, 255);
  analogWrite(ls, 0);
  delay(200);
  do {
    digitalWrite(l1, 1);
    digitalWrite(l2, 0);
    digitalWrite(r1, 0);
    digitalWrite(r2, 1);
    analogWrite(rs, 255);
    analogWrite(ls, 0);
    //    analogWrite(ls, 50);
    readSensors();
  } while (!(sensor1 && (sensor2 && !sensor3) && sensor4 && sensor5));//while (!(sensor1 && (!sensor2 || !sensor3) && sensor4 && sensor5));
  digitalWrite(l1, 0);
  digitalWrite(l2, 1);
  digitalWrite(r1, 0);
  digitalWrite(r2, 1);
  analogWrite(rs, 0);
  analogWrite(ls, 0);
}

void right() {
  digitalWrite(l1, 0);
  digitalWrite(l2, 1);
  digitalWrite(r1, 1);
  digitalWrite(r2, 0);
  analogWrite(rs, 0);
  analogWrite(ls, 255);
  delay(200);
  do {
    digitalWrite(l1, 0);
    digitalWrite(l2, 1);
    digitalWrite(r1, 1);
    digitalWrite(r2, 0);
    //    analogWrite(rs, 50);
    analogWrite(rs, 0);
    analogWrite(ls, 255);
    readSensors();
  } while (!(sensor1 && (sensor4 && !sensor3) && sensor2 && sensor5));//while (!(sensor1 && (!sensor2 || !sensor3) && sensor4 && sensor5));
  digitalWrite(l1, 0);
  digitalWrite(l2, 1);
  digitalWrite(r1, 0);
  digitalWrite(r2, 1);
  analogWrite(rs, 0);
  analogWrite(ls, 0);
}

void loop() {
  
  //read sensor values
  readSensors();
  nodeerr = !sensor1 + !sensor2 + !sensor3 + !sensor4 + !sensor5;
  //  Serial.println(nodeerr);
  if (nodeerr > 2) {
    if (path[node] == "forward") {
      //      Serial.println("forward1");
      forward();
      node++;
    } else if (path[node] == "left") {
      //      Serial.println("left");
      left();
      node++;
    } else if (path[node] == "right") {
    //      Serial.println("right");
    right();
      node++;
    } else if (path[node] == "stop") {
    //      Serial.println("right");
    digitalWrite(l1, 0);
      digitalWrite(l2, 1);
      digitalWrite(r1, 0);
      digitalWrite(r2, 1);
      analogWrite(rs, 0);
      analogWrite(ls, 0);
      delay(5000);
    }
  } else {
    //    Serial.println("forward2");
    go();
  }
}
