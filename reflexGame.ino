int player1ButtonPin = 5;
int player2ButtonPin = 6;
int player3ButtonPin = 7;
int resetButtonPin = 11;
int piezoPin = A0;

int player1LedPin = 2;
int player2LedPin = 3;
int player3LedPin = 4;
int gameStatusLedPin1 = 12;
int gameStatusLedPin2 = 13;

int winner = 0;
boolean player1disq = false;
boolean player2disq = false;
boolean player3disq = false;
boolean gameIsPlaying = true;
boolean ballHasHitTable = false;
double voltageThreshold = 0.05;

//============================================================================

void setup() {

  // Because there are no resistors used with the buttons, I use the port's internal pullup resistor with INPUT_PULLUP
  pinMode(player1ButtonPin, INPUT_PULLUP);
  pinMode(player2ButtonPin, INPUT_PULLUP);
  pinMode(player3ButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(piezoPin, INPUT);

  pinMode(gameStatusLedPin1, OUTPUT);
  pinMode(gameStatusLedPin2, OUTPUT);
  pinMode(player1LedPin, OUTPUT);
  pinMode(player2LedPin, OUTPUT);
  pinMode(player3LedPin, OUTPUT);

  Serial.begin(9600);
}

//============================================================================

void loop() {

  if (gameIsPlaying) {

    // Read the value of the Piezo
    int piezoADC = analogRead(piezoPin);
    float piezoV = piezoADC / 1023.0 * 5.0;

    // If the value exceeds the threshold, the ball has hit the table
    if (!ballHasHitTable && piezoV >= voltageThreshold) {

      ballHasHitTable = true;
      String voltage = "Hit detected with voltage:  " + String(piezoV);
      Serial.println(voltage);
    }

    // check if any player's buttons have been pressed yet
    int player1Read = digitalRead(player1ButtonPin);
    int player2Read = digitalRead(player2ButtonPin);
    int player3Read = digitalRead(player3ButtonPin);

    // Determine if any players have won or been disqualified
    if (ballHasHitTable) {
      checkForWinner( player1Read, player2Read, player3Read);
    }
    else {
      checkForEarlyButtonPresses( player1Read, player2Read, player3Read);
    }
  }

  // Display the winning lights if there is a winner
  showWinnerLEDifNeeded();

  // Check if the reset button has been pressed
  int resetButtonRead = digitalRead(resetButtonPin);
  if (resetButtonRead == LOW) {

    resetGame();
  }

  showGameStatusLEDs();

  delay(50);
}

//============================================================================

void showGameStatusLEDs() {

  if (gameIsPlaying) {

    if (ballHasHitTable) {

      digitalWrite(gameStatusLedPin1, HIGH);
      digitalWrite(gameStatusLedPin2, LOW);
    }
    else {
      digitalWrite(gameStatusLedPin1, LOW);
      digitalWrite(gameStatusLedPin2, HIGH);
    }
  }
  else {
    digitalWrite(gameStatusLedPin1, LOW);
    digitalWrite(gameStatusLedPin2, LOW);
  }
}

//============================================================================

void showWinnerLEDifNeeded() {

  if (winner == 1) {
    digitalWrite(player1LedPin, HIGH);
  }
  else if (winner == 2) {
    digitalWrite(player2LedPin, HIGH);
  }
  else if (winner == 3) {
    digitalWrite(player3LedPin, HIGH);
  }
  else {
    digitalWrite(player1LedPin, LOW);
    digitalWrite(player2LedPin, LOW);
    digitalWrite(player3LedPin, LOW);
  }
}

//============================================================================

void checkForEarlyButtonPresses(int player1Read, int player2Read, int player3Read) {

  if (player1Read == LOW && !player1disq) {

    player1disq = true;
    Serial.println("player 1 too early");
  }
  if (player2Read == LOW && !player2disq) {

    player2disq = true;
    Serial.println("player 2 too early");
  }
  if (player3Read == LOW && !player3disq) {

    player3disq = true;
    Serial.println("player 3 too early");
  }
}

//============================================================================

void checkForWinner(int player1Read, int player2Read, int player3Read) {

  if (player1Read == LOW && !player1disq && winner == 0) {

    winner = 1;
    gameIsPlaying = false;
    Serial.println("player 1 wins");
  }
  else if (player2Read == LOW && !player2disq && winner == 0) {

    winner = 2;
    gameIsPlaying = false;
    Serial.println("player 2 wins");
  }
  else if (player3Read == LOW && !player3disq && winner == 0) {

    winner = 3;
    gameIsPlaying = false;
    Serial.println("player 3 wins");
  }
}

//============================================================================

void resetGame() {

  winner = 0;
  gameIsPlaying = true;
  ballHasHitTable = false;
  player1disq = false;
  player2disq = false;
  player3disq = false;

  Serial.println("reset game");
}
