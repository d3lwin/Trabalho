//Programa: Modulo Leitor de impressao digital - Registro
//Alteracoes: Arduino e Cia
//Baseado no exemplo original da biblioteca Adafruit Fingerprint

#include <Adafruit_Fingerprint.h>

//Parametros da serial por software (pinos 2 e 3)
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup()
{
  Serial.begin(9600);
  Serial.println("\n\nModulo Leitor de Impressao Digital - Registro");

  //Taxa de comunicação com o modulo
  finger.begin(57600);

  //Checagem do hardware
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
}

uint8_t readnumber(void)
{
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()
{
  Serial.println("Pronto para registrar uma digital!");
  Serial.println("Digite o numero do ID (de 1 a 240) onde voce deseja salvar a digital...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    return;
  }
  Serial.print("Registrando ID #");
  Serial.println(id);

  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll()
{
  int p = -1;
  Serial.print("Aguardando a digital para registrar no ID numero "); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Imagem capturada");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Erro de comunicacao");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Erro de imagem");
        break;
      default:
        Serial.println("Erro desconhecido");
        break;
    }
  }

  // OK success!
  p = finger.image2Tz(1);
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.println("Imagem convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem incorreta");
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

  Serial.println("Remova o dedo do sensor");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Coloque o mesmo dedo novamente");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Imagem capturada");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Erro de comunicacao");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imagem incorreta");
        break;
      default:
        Serial.println("Erro desconhecido");
        break;
    }
  }

  // OK success!
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem incorreta");
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
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Digitais correspondem!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro de comunicacao");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Digitais nao correspondem");
    return p;
  } else {
    Serial.println("Erro desconhecido");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Impressao Digital Armazenada!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro de comunicacao");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Nao foi possivel armazenar nesta localizacao");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Erro gravando na memoria flash");
    return p;
  } else {
    Serial.println("Erro desconhecido");
    return p;
  }
  return true;
}
