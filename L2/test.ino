// C++ code
//
void setup()
{
    pinMode(9, OUTPUT);
    pinMode(6, OUTPUT);
}

int state = 0;
int val_old = 0;

void loop()
{
    int val_new;
    val_new = digitalRead(6);
    if ((val_old == 1) && (val_new == 0))
    {
        state = 1 - state;
        digitalWrite(9, state);
    }
    val_old = val_new;
}