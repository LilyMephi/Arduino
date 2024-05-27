//#define _BV(bit) (1 << (bit))
//#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) //присвоить ноль биту
//#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit)) //присвоить единицу биту
#include <avr/interrupt.h>
volatile int result = 0;
volatile bool flag = true;
volatile float voltage = 0;
unsigned volatile long int count = 0;
unsigned volatile long int countall = 0;

ISR(ADC_vect) {
  //...
  // Порядок записи результирующих битов был изменен из-за пренебрежения последними битами
  // ADLAR=1
  result = (int)ADCH;  // Считываем только значимые 8 бит - значение из диапазона 0...255
  // Если нужны все 10 бит (полная 10-битная точность), то если ADLAR=0:
  //result = (int) ADCL | (ADCH << 8);
  // Если ADLAR=1:
  //result = (ADCL>> 6) | (ADCH << 2);
  if (result > 1) {
    //avarege += result;
    voltage = (float)(result * 1.1) / 255.0;
    ++countall;
    if(voltage > 0.01 && voltage < 0.5){
      ++count;
    }
  }
}
void setup() {
  Serial.begin(9600);
  DDRD |= B11111100;
  PORTB &= B00000011;
  ADCSRA = 0;  // Сбрасываем регистр ADCSRA
  ADCSRB = 0;

  ADMUX |= (0 & 0x07);  // Выбираем пин A0 для преобразования

  ADMUX |= (1 << REFS0) | (1 << REFS1);  //устанавливаем внутреннее напряжение как опорное
  //ADMUX |= (1 << REFS0);
  ADMUX |= (1 << ADLAR);  // Меняем порядок записи бит, чтобы можно было читать только 8 бит регистра ADCH
                          // Таким образом отсекаются 2 последних "шумных" бита, результат 8-битный (0...255)

  // ADPS = 111 оцифровка до 128 бит
  ADCSRA |= (1 << ADPS2);
  ADCSRA &= ~((1 << ADPS1) | (1 << ADPS0));

  ADCSRA |= (1 << ADATE);  // Включаем автоматическое преобразование
  ADCSRA |= (1 << ADIE);   // Разрешаем прерывания по завершении преобразования
  ADCSRA |= (1 << ADEN);   // Включаем АЦП
  ADCSRA |= (1 << ADSC);   // Запускаем преобразование*/

}

unsigned long int tmp = 0;
void loop() {
  //Serial.println(voltage, 3);
  Serial.println(count);
  if(count > 100000 || count == tmp){
    Serial.println((float)count/countall,6);
    delay(1000);
    countall = 0;
    count = 0;
  }
  
  tmp =count;
  delay(1000);
}
