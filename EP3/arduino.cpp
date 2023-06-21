#define BLUE_LED 10
#define GREEN_LED 11
#define YELLOW_LED 12
#define RED_LED 13
#define PC 0
#define W 1
#define X 2
#define Y 3
#define MEMORY_SIZE 100

uint8_t lastIdx = 4;

String memory[MEMORY_SIZE];
String input = "";

void setup()
{
  Serial.begin(9600); // abre a porta serial a 9600 bps
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  memory[PC] = "0";
}

void loop()
{
  if (Serial.available())
  {
    input = Serial.readString();

    // Preenchendo memória
    for (int i = 0; i < input.length() && lastIdx < MEMORY_SIZE; i++)
    {
      if (input.charAt(i) == ' ')
      {
        memory[lastIdx++] = input.substring(0, i);
        input = input.substring(i + 1);
        i = 0;
      }
    }

    memory[PC] = "4";
  }

  // Executando código
  uint8_t pc = memory[PC].toInt();

  if (pc == 4)
    imprimirMemoria();

  if (pc > 0 && pc < lastIdx)
  {

    memory[X] = String(memory[pc].charAt(0));
    memory[Y] = String(memory[pc].charAt(1));
    uint8_t op = hexToInt(memory[pc].charAt(2));

    memory[W] = intToHex(operation(hexToInt(memory[X].charAt(0)), hexToInt(memory[Y].charAt(0)), op));
    memory[PC] = String(pc + 1);

    // Exibindo memória
    imprimirMemoria();

    // Exibindo resultado
    String result = intToBinary(memory[W].toInt());
    if (result.charAt(0) == '1')
      digitalWrite(RED_LED, HIGH);
    if (result.charAt(1) == '1')
      digitalWrite(YELLOW_LED, HIGH);
    if (result.charAt(2) == '1')
      digitalWrite(GREEN_LED, HIGH);
    if (result.charAt(3) == '1')
      digitalWrite(BLUE_LED, HIGH);

    delay(2000);
  }

  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

uint8_t hexToInt(char hex)
{
  uint8_t val = 0;

  if (hex >= '0' && hex <= '9')
  {
    val = hex - '0';
  }
  else if (hex >= 'A' && hex <= 'F')
  {
    val = hex - 'A' + 10;
  }
  else if (hex >= 'a' && hex <= 'f')
  {
    val = hex - 'a' + 10;
  }

  return val;
}

char intToHex(uint8_t val)
{
  char hex = '0';

  if (val >= 0 && val <= 9)
  {
    hex = val + '0';
  }
  else if (val >= 10 && val <= 15)
  {
    hex = val + 'A' - 10;
  }

  return hex;
}

String intToBinary(int num)
{
  String binary = "";
  int count = 0;
  while (num > 0)
  {
    binary = (num % 2) + binary;
    num /= 2;
    count++;
  }
  return ("0000" + binary).substring(count);
}

uint8_t binaryToInt(String binary)
{
  uint8_t num = 0;
  for (int i = 0; i < binary.length(); i++)
  {
    num += (binary.charAt(i) - '0') * pow(2, binary.length() - i - 1);
  }
  return num;
}

uint8_t operation(uint8_t x, uint8_t y, uint8_t opcode)
{
  switch (opcode)
  {
  case 0:
    return Not(x);
  case 1:
    return Not(Or(x, y));
  case 2:
    return And(Not(x), y);
  case 3:
    return Zero();
  case 4:
    return Not(And(x, y));
  case 5:
    return Not(y);
  case 6:
    return Xor(x, y);
  case 7:
    return And(x, Not(y));
  case 8:
    return Or(Not(x), y);
  case 9:
    return Not(Xor(x, y));
  case 10:
    return Copy(y);
  case 11:
    return And(x, y);
  case 12:
    return One();
  case 13:
    return Or(x, Not(y));
  case 14:
    return Or(x, y);
  case 15:
    return Copy(x);
  }
}

uint8_t Not(uint8_t x)
{
  String binary = intToBinary(x);

  for (int i = 0; i < binary.length(); i++)
  {
    if (binary.charAt(i) == '0')
    {
      binary.setCharAt(i, '1');
    }
    else
    {
      binary.setCharAt(i, '0');
    }
  }

  return binaryToInt(binary);
}

uint8_t And(uint8_t x, uint8_t y)
{
  return x & y;
}

uint8_t Or(uint8_t x, uint8_t y)
{
  return x | y;
}

uint8_t Xor(uint8_t x, uint8_t y)
{
  return x ^ y;
}

uint8_t Zero()
{
  return 0;
}

uint8_t One()
{
  return 15;
}

uint8_t Copy(uint8_t x)
{
  return x;
}

void imprimirMemoria()
{
  uint8_t pc = memory[PC].toInt() - 2;
  // Valores de operação
  for (int i = 0; i < 4; i++)
  {
    String format = "   ";
    for (int j = 0; j < memory[i].length(); j++)
    {
      format.setCharAt(j, memory[i].charAt(j));
    }
    Serial.print("|  " + format);
  }
  Serial.print("|    ");
  for (int i = 4; i < lastIdx; i++)
  {
    String format = "   ";

    for (int j = 0; j < memory[i].length(); j++)
    {
      format.setCharAt(j, memory[i].charAt(j));
    }
    Serial.print(" | " + format);
  }
  Serial.println("|");
  Serial.print("|  PC |  W  |  X  |  Y  |");

  for (int i = 0; i < pc; i++)
  {
    if (i == pc - 1)
      Serial.print("  ^  ");
    else
      Serial.print("      ");
  }

  Serial.println("\n\n");
}