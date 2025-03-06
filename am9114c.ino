// تعریف پین‌های آدرس
const int addressPins[10] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

// تعریف پین‌های داده
const int dataPins[4] = {32, 33, 34, 35};

// تعریف پین‌های کنترلی
const int pinCS = 36; // Chip Select
const int pinWE = 37; // Write Enable

void setup() {
  // تنظیم پین‌های آدرس به عنوان خروجی
  for (int i = 0; i < 10; i++) {
    pinMode(addressPins[i], OUTPUT);
    digitalWrite(addressPins[i], LOW);
  }

  // تنظیم پین‌های داده به عنوان خروجی
  for (int i = 0; i < 4; i++) {
    pinMode(dataPins[i], OUTPUT);
    digitalWrite(dataPins[i], LOW);
  }

  // تنظیم پین‌های کنترلی به عنوان خروجی
  pinMode(pinCS, OUTPUT);
  pinMode(pinWE, OUTPUT);

  // غیرفعال کردن سیگنال‌های کنترلی در حالت اولیه
  digitalWrite(pinCS, HIGH);
  digitalWrite(pinWE, HIGH);

  // شروع ارتباط سریال برای نمایش داده‌ها
  Serial.begin(9600);
  // مثال: نوشتن داده 0xA (1010) در آدرس 0x1F (31)
  for (int a = 0; a < 1024; a++) {
    writeDataToRAM(a, 0xa);
    delay(1);
  }
}

void loop() {
  // خواندن داده از آدرس 0x1F
  for (int a = 0; a < 1024; a++) {
    byte data = readDataFromRAM(a);
    Serial.print("Data read from address\t");
    Serial.print(a, HEX);
    Serial.print(" :\t");
    Serial.println(data, BIN);
    delay(10);
  }
}

void writeDataToRAM(unsigned int address, byte data) {
  // تنظیم آدرس
  for (int i = 0; i < 10; i++) {
    digitalWrite(addressPins[i], (address >> i) & 0x01);
  }

  // تنظیم داده
  for (int i = 0; i < 4; i++) {
    digitalWrite(dataPins[i], (data >> i) & 0x01);
  }

  // فعال‌سازی نوشتن
  digitalWrite(pinCS, LOW);
  digitalWrite(pinWE, LOW);
  delayMicroseconds(1); // تاخیر کوتاه برای اطمینان از نوشتن
  digitalWrite(pinWE, HIGH);
  digitalWrite(pinCS, HIGH);
}

byte readDataFromRAM(unsigned int address) {
  byte data = 0;

  // تنظیم آدرس
  for (int i = 0; i < 10; i++) {
    digitalWrite(addressPins[i], (address >> i) & 0x01);
  }

  // تنظیم پین‌های داده به حالت ورودی
  for (int i = 0; i < 4; i++) {
    pinMode(dataPins[i], INPUT);
  }

  // فعال‌سازی خواندن (فقط /CS فعال شود)
  digitalWrite(pinCS, LOW);
  delayMicroseconds(1); // تاخیر کوتاه برای اطمینان از خواندن

 //read data
  for (int i = 0; i < 4; i++) {
    data |= (digitalRead(dataPins[i]) << i);
  }

  // غیرفعال‌سازی خواندن
  digitalWrite(pinCS, HIGH);

  // بازگرداندن پین‌های داده به حالت خروجی
  for (int i = 0; i < 4; i++) {
    pinMode(dataPins[i], OUTPUT);
    digitalWrite(dataPins[i], LOW);
  }

  return data;
}
