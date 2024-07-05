//Programa: Modulo Leitor de impressao digital - Leitura
//Alteracoes: Arduino e Cia
//Baseado no exemplo original da biblioteca Adafruit Fingerprint

#include <Adafruit_Fingerprint.h>
#include <Servo.h>

//Cria um objeto para controlar o servo motor
Servo myservo;

//Parametros da serial por software (pinos 2 e 3)
SoftwareSerial mySerial(2, 3);

#define pino_led 7
#define pino_servo 8

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  Serial.begin(9600);
  Serial.println("\n\nModulo Leitor de Impressao Digital - Leitura");
  pinMode(pino_led, OUTPUT);
  digitalWrite(pino_led, LOW);

  // Define que o servo esta ligado a porta 7
  myservo.attach(pino_servo);
  myservo.write(0);

  //Taxa de comunicação com o modulo
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Sensor de impressao digital detectado!");
  } else {
    Serial.println("Sensor de impressao digital nao encontrado:(");
    while (1) {
      delay(1);
    }
  }

  Serial.println(F("Lendo parametros do sensor"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacidade: ")); Serial.println(finger.capacity);
  Serial.print(F("Nivel de Seguranca: ")); Serial.println(finger.security_level);
  Serial.print(F("Endereco do dispositivo: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor nao contem nenhuma digital. Cadastre as digitais primeiro!");
  }
  else {
    Serial.println("Aguardando digital...");
    Serial.print("O sensor contem "); Serial.print(finger.templateCount); Serial.println(" digitais cadastradas");
  }
}

void loop()
{
  getFingerprintID();
  delay(50);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem registrada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Digital nao detectada");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicacao");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imagem incorreta");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }

  // OK success!
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem invalida");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicacao");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Nao foi possivel encontrar recursos de impressao digital");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Nao foi possivel encontrar recursos de impressao digital");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Encontrada digital correspondente!");
    //Aciona o led e o servo motor
    digitalWrite(pino_led, HIGH);
    myservo.write(90);
    //Aguarda 1 segundo e desliga o led e o servo
    delay(1000);
    digitalWrite(pino_led, LOW);
    myservo.write(0);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID encontrado #"); Serial.print(finger.fingerID);
  Serial.print(" com confianca de "); Serial.println(finger.confidence);
  return finger.fingerID;
}

//Retorna -1 em caso de falha, senao retorna o numero do ID
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  Serial.print("ID encontrado #"); Serial.print(finger.fingerID);
  Serial.print(" com confianca de "); Serial.println(finger.confidence);
  return finger.fingerID;
}
