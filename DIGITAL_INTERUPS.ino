//#define _BV(bit) (1 << (bit))
//#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) //присвоить ноль биту
//#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit)) //присвоить единицу биту
#include <avr/interrupt.h>
#include <avr/io.h>
#define N 1000
#define Vmax 4.0
#define Vmin 3.9
volatile int result = 0;
unsigned volatile int tmp = 0;
unsigned volatile int alltmp = 0;
volatile float voltage = 0;
volatile bool flag = false;
unsigned volatile long int count = 0;
unsigned volatile long int allcount = 0;
float effect = 0.0;



ISR(INT0_vect){
  ADCSRA |= (1 << ADEN);   // Включаем АЦП
  ADCSRA |= (0 << ADATE);  // Включаем РАЗОВОЕ преобразование
  //ADCSRB |= (0<<ADTS0) | (1<<ADTS1) | (0<<ADTS2); // ставим прерывания по тригерру
  ADCSRA |= (1 << ADIE);   // Разрешаем прерывания по завершении преобразования
  ADCSRA |= (1 << ADSC);   // Запускаем преобразование

  
  ++alltmp;
  if(alltmp >= N){
    allcount += 1;
    alltmp = 0;
  }
  if(allcount > N*11){
    allcount = 0;
  }
}

ISR(ADC_vect) {
  //...
  // Порядок записи результирующих битов был изменен из-за пренебрежения последними битами
  // ADLAR=1
  result = (int)ADCH;  // Считываем только значимые 8 бит - значение из диапазона 0...255
  // Если нужны все 10 бит (полная 10-битная точность), то если ADLAR=0:
 // result = (int) ADCL | (ADCH << 8);
  // Если ADLAR=0:
  //result = (ADCL>> 6) | (ADCH << 2);

  
  //if (result > 9) {
    voltage = (float)(result * 5.0) / 255.0;
    if(voltage >= Vmin && voltage <= Vmax){
      ++tmp;
      if( tmp >= N){
        ++count;
        tmp = 0;
      }
      
      if(count > N*11){
         count = 0;
      }
    }
 // }
}
void setup() {
  Serial.begin(9600);
  DDRD |= B11111000;
  PORTB &= B00000011;
  ADCSRA = 0;  // Сбрасываем регистр ADCSRA
  ADCSRB = 0;

  ADCSRB |= (1 << ACME);

  //Добовлаем прерывание на повышение сигнала
  EICRA |= (1<<ISC01)|(1<<ISC00);
  EIMSK |= (1 << INT0);

  ADCSRA |= (1 << ADEN);   // Включаем АЦП
  ADCSRA |= (1 << ADIE);   // Разрешаем прерывания по завершении преобразования
  ADCSRA |= (1 << ADSC);   // Запускаем преобразование
  ADCSRA |= (0 << ADATE);  // Включаем постоянное прерывание преобразование
  //ADCSRB |= (0 << ADTS2) | (1 << ADTS1) | (0 << ADTS0); // ставим прерывания по тригерру
  /*нужно обнулить adcsra in aden
  ACSR |= 
  (0 << ACD  )| //включаем компоратор(сравнение амплитуды)
  (1 << ACBG )| //положительный вход на компаратор
  (0 << ACO  )| //устанавливаем флаг в ноль чтобы смотреть изменения
                // если 0 то ain0 < ain1 , ain0 > ain1
  (1 << ACI  )| // флаг устанавливается в 1 когда срабатывает прерывание
  (0 << ACIE )| // выключаем аналоговое прерывание сравнения
  (0 << ACIC) | //выклчаем ахвата входа аналогового компаратора(по таймру)
  (1 << ACIS1) | (0 << ACIS0); //на падение
  */
  ADMUX |= (0 & 0x07);  // Выбираем пин A0 для преобразования

  ADMUX |= (0 << REFS1) | (1 << REFS0);  //устанавливаем внутреннее напряжение как опорное
  //ADMUX |= (1 << REFS0);
  ADMUX |= (1 << ADLAR);  // Меняем порядок записи бит, чтобы можно было читать только 8 бит регистра ADCH
                          // Таким образом отсекаются 2 последних "шумных" бита, результат 8-битный (0...255)

  // ADPS = 111 оцифровка до 128 бит
  ADCSRA |= (1 << ADPS2)| (1 << ADPS1) | (1 << ADPS0);
  
  //sei();
}


void loop() {
  Serial.println(voltage, 3);
  Serial.println(count);
  Serial.println(allcount);
  if( allcount >= 100 ){
    effect =(float) (count*N+tmp)/(allcount*N+alltmp);
    Serial.println(effect,4);
    delay(10000);
    allcount = 0;
    alltmp = 0;
    tmp = 0;
    count = 0;
  }
  //if(flag){
   // Serial.print("Ops");
   // flag = false;
 // }

  delay(500);
}
