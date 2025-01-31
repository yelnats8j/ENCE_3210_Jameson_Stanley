#define LED_GREEN  4
#define LED_RED    5
#define BUTTON_PIN 2

#define TASK1_TIMER_COMPARE 62500  // 1 Hz: 16MHz / (256 * 1 Hz)
#define TASK2_TIMER_COMPARE 6250   // 10 Hz: 16MHz / (256 * 10 Hz)
#define COUNTER_TIME         3     // 3 seconds for the foreground task

volatile unsigned long gCounter = 0; // Counter for Task 1
volatile unsigned char gTask1Flag = 0;
volatile unsigned char gTask2Flag = 0;

void setup() 
{
    // Initialize communications
    Serial.begin(9600);

    // Initialize IOs
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);

    // Initialize Timer1 for Task 1 (1 Hz)
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = TASK1_TIMER_COMPARE;
    TCCR1B |= (1 << WGM12); // CTC mode
    TCCR1B |= (1 << CS12);  // 256 prescaler
    TIMSK1 |= (1 << OCIE1A);
    interrupts();

    // Initialize Timer2 for Task 2 (10 Hz)
    noInterrupts();
    TCCR2A = 0;
    TCCR2B = 0;
    OCR2A = TASK2_TIMER_COMPARE;
    TCCR2A |= (1 << WGM21); // CTC mode
    TCCR2B |= (1 << CS22) | (1 << CS21); // 256 prescaler
    TIMSK2 |= (1 << OCIE2A);
    interrupts();
}

ISR(TIMER1_COMPA_vect) 
{
    gTask1Flag = 1; // Task 1 Timer Interrupt
    gCounter++;
}

ISR(TIMER2_COMPA_vect) 
{
    gTask2Flag = 1; // Task 2 Timer Interrupt
}

void loop() {
    static unsigned long lastCounterValue = 0;

    if (gTask1Flag) 
    {
        gTask1Flag = 0;

        // Task 1 - Blink Green LED
        static bool greenLEDState = false;
        digitalWrite(LED_GREEN, greenLEDState ? HIGH : LOW);
        greenLEDState = !greenLEDState;
    }

    if (gTask2Flag) 
    {
        gTask2Flag = 0;

        // Task 2 - Read Button Input and control Red LED
        int buttonState = digitalRead(BUTTON_PIN);
        digitalWrite(LED_RED, buttonState);
    }

    // Foreground Task - Print message every 3 seconds
    if (gCounter - lastCounterValue >= COUNTER_TIME) 
    { // Use Task 1 counter as a timer
        lastCounterValue = gCounter;
        Serial.print("Counter value: ");
        Serial.println(gCounter);
        Serial.println("Arduino multitasking is working");
    }
}
