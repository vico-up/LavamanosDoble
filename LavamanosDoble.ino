/*
Version de lavamanos doble funcionando las siguientes funcionalidades

Sensores leyendo
relay activando
boton de antidad de jabon funcioanando
led de activcion funcionando


Tareas
incluir botones de despensdo continuo
*/

#include <protothreads.h>
#include <EEPROM.h>

const int sensorJabon = 2;
const int ledOnJabon = 3;   //color verde led encendido jabon
const int dispensador = 4;  //salida para activar el rele del dispensador

bool estadoSensorJabon = true;    //guarda el estado del sensor jabon(True si el sensor es NPN(logica negativa))

const int sensorAgua = 5;
const int ledOnAgua = 6;   //color verde led encendido jabon
const int bombaAgua = 7;     //salida para activar el rele de la (Bomba, electrovalvula)

bool estadoSensorAgua = true;  //guarda el estado del sensor agua(True si el sensor es NPN(logica negativa))

const int sensorJabon2 = 8;
const int ledOnJabon2 = 9;   //color verde led encendido jabon
const int dispensador2 = 10;  //salida para activar el rele del dispensador

bool estadoSensorJabon2 = true;    //guarda el estado del sensor jabon(True si el sensor es NPN(logica negativa))

const int sensorAgua2 = 11;
const int ledOnAgua2 = 12;   //color verde led encendido jabon
const int bombaAgua2 = 14;     //salida para activar el rele de la (Bomba, electrovalvula)

bool estadoSensorAgua2 = true;  //guarda el estado del sensor agua(True si el sensor es NPN(logica negativa))

const int botonTiempito = 15; //boton para determinar tiempo del dispensador
bool estadoBotonTiempito = false;
bool UltestadoBotonTiempito = true; //ultimo estado del boton tiempito
const int buzzer = 16;

int contador = 0;   //variable se usa en el tiempo de dispensado de jabon
int ultimoContador;

int tiempito = 0;
//variables para guardar el tiempo de dispensado de jabon
int tiempito1 = 1100;
int tiempito2 = 2100; //pareca que este va a ser maximo
//int tiempito3 = 3200;
int ultimoTiempito;

const int botonJabonn = 17; //variable para modo continuo
bool estadoPulsadorJabon;

const int botonJabonn2 = 18;
bool estadoPulsadorJabon2;


bool encenderLedAgua = false;     //si led es anodo comun ennceder con logica negativa
bool apagarLedAgua = true;

bool encenderLedJabon = false;
bool apagarLedJabon = true;

bool encenderLedAgua2 = false;     //si led es anodo comun ennceder con logica negativa
bool apagarLedAgua2 = true;

bool encenderLedJabon2 = false;
bool apagarLedJabon2 = true;

pt ptActivarAgua;
int activarAguaThread(struct pt* pt)
{
    PT_BEGIN(pt);

    for(;;)
    {
        if(estadoSensorAgua == false) //compara con logica negativa por el sensor es NPN(se activa con logica negativa)
        {
            digitalWrite(ledOnAgua, LOW);  //led enciende con LOW
            digitalWrite(bombaAgua, LOW);
            PT_SLEEP(pt, 5000);   //tiempo de espera para evitar falso del sensor de agua y no apague y prenda
        }

        else
        {
            
            digitalWrite(ledOnAgua, HIGH);
            digitalWrite(bombaAgua, HIGH);
            PT_YIELD(pt);
        }
    }
    PT_END(pt);
}

pt ptSensorAgua;
int sensorAguaThread(struct pt* pt) 
{
  PT_BEGIN(pt);

  for(;;) 
  {  
    estadoSensorAgua = digitalRead(sensorAgua);
    PT_YIELD(pt);
  }

  PT_END(pt);
}

pt ptActivarAgua2;
int activarAgua2Thread(struct pt* pt)
{
    PT_BEGIN(pt);

    for(;;)
    {
        if(estadoSensorAgua2 == false) //compara con logica negativa por el sensor es NPN(se activa con logica negativa)
        {
            digitalWrite(ledOnAgua2, LOW);  //led enciende con LOW
            digitalWrite(bombaAgua2, LOW);
            PT_SLEEP(pt, 5000);   //tiempo de espera para evitar falso del sensor de agua y no apague y prenda
        }

        else
        {
            
            digitalWrite(ledOnAgua2, HIGH);
            digitalWrite(bombaAgua2, HIGH);
            PT_YIELD(pt);
        }
    }
    PT_END(pt);
}

pt ptSensorAgua2;
int sensorAgua2Thread(struct pt* pt) 
{
  PT_BEGIN(pt);

  for(;;) 
  { 
    estadoSensorAgua2 = digitalRead(sensorAgua2);
  PT_YIELD(pt);
  }

  PT_END(pt);
}

pt ptActivarDispensador;     //para controlar  dispensdor de jabon
int ActivarDispensadorThread(struct pt* pt) 
{
    PT_BEGIN(pt);

    for(;;) 
    {
	    if (estadoSensorJabon == false) 
        {
            digitalWrite(ledOnJabon, LOW);
            digitalWrite(dispensador, LOW);   // Encender dispensador (el module relay activa con LOW)
            PT_SLEEP(pt, tiempito);                 //tiempo de activacion
            digitalWrite(ledOnJabon, HIGH);
		    digitalWrite(dispensador, HIGH);    // apgar dispensador relay no activa con HIGH
		    PT_SLEEP(pt, 5000);               //tiempo para evitar desperdicio de jabon
	    } 
    
        else 
        {
            digitalWrite(dispensador, HIGH);    // mantener apagado sipensador si no hay activacion
            digitalWrite(ledOnJabon, HIGH);
		    PT_YIELD(pt);
        }
    }

    PT_END(pt);
}

pt ptSensorJabon;      //ptButton controla el estado del sensor de jabon
int SensorJabonThread(struct pt* pt) 
{
  PT_BEGIN(pt);

  // Loop forever
  for(;;) 
  {
  	estadoSensorJabon = digitalRead(sensorJabon);
	PT_YIELD(pt);
  }

  PT_END(pt);
}

pt ptActivarDispensador2;     //para controlar  dispensdor de jabon
int ActivarDispensador2Thread(struct pt* pt) 
{
    PT_BEGIN(pt);

    for(;;) 
    {
	    if (estadoSensorJabon2 == false) 
        {
            digitalWrite(ledOnJabon2, LOW);
            digitalWrite(dispensador2, LOW);   // Encender dispensador (el module relay activa con LOW)
            PT_SLEEP(pt, tiempito);                 //tiempo de activacion
            digitalWrite(ledOnJabon2, HIGH);
		    digitalWrite(dispensador2, HIGH);    // apgar dispensador relay no activa con HIGH
		    PT_SLEEP(pt, 5000);               //tiempo para evitar desperdicio de jabon
	    } 
    
        else 
        {
            digitalWrite(dispensador2, HIGH);    // mantener apagado sipensador si no hay activacion
            digitalWrite(ledOnJabon2, HIGH);
		    PT_YIELD(pt);
        }
    }

    PT_END(pt);
}

pt ptSensorJabon2;      //ptButton controla el estado del sensor de jabon
int SensorJabon2Thread(struct pt* pt) 
{
  PT_BEGIN(pt);

  // Loop forever
  for(;;) 
  {
  	estadoSensorJabon2 = digitalRead(sensorJabon2);
	PT_YIELD(pt);
  }

  PT_END(pt);
}

void setup()
{
    Serial.begin(9600);
    tiempito = EEPROM.get(10, ultimoTiempito); //retorna el ultimo dato de contadr y tiempito
    contador = EEPROM.get(0, ultimoContador);

    PT_INIT(&ptActivarAgua);
    PT_INIT(&ptSensorAgua);
    PT_INIT(&ptActivarAgua2);
    PT_INIT(&ptSensorAgua2);

    PT_INIT(&ptActivarDispensador);
    PT_INIT(&ptSensorJabon);
    PT_INIT(&ptActivarDispensador2);
    PT_INIT(&ptSensorJabon2);


    pinMode(sensorJabon, INPUT);
    pinMode(dispensador, OUTPUT);
    pinMode(ledOnJabon, OUTPUT);

    pinMode(sensorAgua, INPUT);
    pinMode(bombaAgua, OUTPUT);
    pinMode(ledOnAgua, OUTPUT);

    pinMode(sensorJabon2, INPUT);
    pinMode(dispensador2, OUTPUT);
    pinMode(ledOnJabon2, OUTPUT);

    pinMode(sensorAgua2, INPUT);
    pinMode(bombaAgua2, OUTPUT);
    pinMode(ledOnAgua2, OUTPUT);

    pinMode(botonTiempito, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);

    pinMode(botonJabonn, INPUT_PULLUP);
    pinMode(botonJabonn2, INPUT_PULLUP);

    digitalWrite(ledOnAgua, HIGH);
    digitalWrite(ledOnAgua2, HIGH);
    digitalWrite(ledOnJabon, HIGH);
    digitalWrite(ledOnJabon2, HIGH);

}

void loop()
{
    PT_SCHEDULE(activarAguaThread(&ptActivarAgua));
    PT_SCHEDULE(sensorAguaThread(&ptSensorAgua));

    PT_SCHEDULE(activarAgua2Thread(&ptActivarAgua2));
    PT_SCHEDULE(sensorAgua2Thread(&ptSensorAgua2));

    PT_SCHEDULE(ActivarDispensadorThread(&ptActivarDispensador));
    PT_SCHEDULE(SensorJabonThread(&ptSensorJabon));

    PT_SCHEDULE(ActivarDispensador2Thread(&ptActivarDispensador2));
    PT_SCHEDULE(SensorJabon2Thread(&ptSensorJabon2));

    

    estadoBotonTiempito = digitalRead(botonTiempito); //Lee pulsador de cantidad de dispensado de jabon
    if (estadoBotonTiempito != UltestadoBotonTiempito)
    {
        if (estadoBotonTiempito == true)
        {
            llamarBuzzer();
            contador++;
            delay(600);
            Serial.print("El contador es: ");
            Serial.println(contador);
        }
    }
    UltestadoBotonTiempito = estadoBotonTiempito;
    
    if (contador == 1)
    {
        modo1();
    }
    else if (contador == 3)
    {
        modo2();
    }
      /*
      else if (contador == 5)
      {
        modo3();
      }*/
    
}

void llamarBuzzer()
{
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
}

void modo2()
{
    tiempito = tiempito2;
    ultimoTiempito = tiempito;
    EEPROM.put(10, ultimoTiempito);

    digitalWrite(ledOnJabon, LOW);
    digitalWrite(ledOnJabon2, LOW);
    delay(500);
    digitalWrite(ledOnJabon, HIGH);
    digitalWrite(ledOnJabon2, HIGH);
    delay(250);
    digitalWrite(ledOnJabon, LOW);
    digitalWrite(ledOnJabon2, LOW);
    delay(500);
    digitalWrite(ledOnJabon, HIGH);
    digitalWrite(ledOnJabon2, HIGH);

    
    contador = 0;
    ultimoContador = contador;
    EEPROM.put(0, ultimoContador);
    /*
    Serial.println("Ultimo tiempo es: "); 
    Serial.println(EEPROM.get(10, ultimoTiempito));
    
    Serial.println("Ultimo contador es: ");
    Serial.println(EEPROM.get(0, ultimoContador));
    */
}
/*
void modo3()
{
  
    tiempito = tiempito3;
    ultimoTiempito = tiempito;
    EEPROM.put(10, ultimoTiempito);
    digitalWrite(ledOffJabon, LOW);
    digitalWrite(ledOnJabon, HIGH);
    delay(500);
    digitalWrite(ledOnJabon, LOW);
    delay(250);
    digitalWrite(ledOnJabon, HIGH);
    delay(500);
    digitalWrite(ledOnJabon, LOW);
    delay(250);
    digitalWrite(ledOnJabon, HIGH);
    delay(500);
    digitalWrite(ledOnJabon, LOW);
    contador = 0;
    ultimoContador = contador;
    EEPROM.put(0, ultimoContador);
    
    Serial.println("Ultimo tiempo es: "); 
    Serial.println(EEPROM.get(10, ultimoTiempito));
    
    Serial.println("Ultimo contador es: ");
    Serial.println(EEPROM.get(0, ultimoContador));
    
}
*/

void modo1()
{
  
    tiempito = tiempito1;
    ultimoTiempito = tiempito;
    EEPROM.put(10, ultimoTiempito);

    digitalWrite(ledOnJabon, LOW);
    digitalWrite(ledOnJabon2, LOW);
    delay(500);
    digitalWrite(ledOnJabon, HIGH);
    digitalWrite(ledOnJabon2, HIGH);
    
    contador = contador + 1;
    ultimoContador = contador;
    EEPROM.put(0, ultimoContador);
    /*
    Serial.println("Ultimo tiempo es: "); 
    Serial.println(EEPROM.get(10, ultimoTiempito));
    
    Serial.println("Ultimo contador es: ");
    Serial.println(EEPROM.get(0, ultimoContador));
    */
}       

void jabonContinuo()
{
  digitalWrite(ledOnJabon, LOW);
  digitalWrite(dispensador, LOW);
}

void jabonContinuoOff()
{
    digitalWrite(ledOnJabon, HIGH);
    digitalWrite(dispensador, HIGH);
}

void jabonContinuo2()
{
  digitalWrite(ledOnJabon2, LOW);
  digitalWrite(dispensador2, LOW);
}

void jabonContinuoOff2()
{
    digitalWrite(ledOnJabon2, HIGH);
    digitalWrite(dispensador2, HIGH);
}