// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int main(){
	int serial_port = open("/dev/ttyUSB0", O_RDWR);// открываем сериал порт для чтения и записи
	
	struct termios tty; // настройки терминала

	// проверям открылся ли порт, запращиваем настройик устройства
	if(tcgetattr(serial_port, &tty) != 0) {
    		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
	
	// настройка работы терминала
	tty.c_cflag &= ~PARENB; // Отключаем бит четности(для проверки правильности сигнала)
  	tty.c_cflag &= ~CSTOPB; // устанавливаем один стоповый бит(по умполчанию)
  	tty.c_cflag &= ~CSIZE; // Очищаем маску установливающую рамзер символа
  	tty.c_cflag |= CS8; // устанавливаем маску в восемь бит
  	tty.c_cflag &= ~CRTSCTS; // отключаем управление потоком данных автоматически (RTS/CTS)
  	tty.c_cflag |= CREAD | CLOCAL; // включаем прием данных и игнорирование управлющих строк

  	tty.c_lflag &= ~ICANON; // отключаем использование спец символов
  	tty.c_lflag &= ~ECHO; // отключаем отображение вводимых символов
 	tty.c_lflag &= ~ECHOE; // тоже отключаем
 	tty.c_lflag &= ~ECHONL; // выключаем отображение новой строки
  	tty.c_lflag &= ~ISIG; // отключаем поддержку INTR, QUIT and SUSP
  	
  	//отключаем все флаги привводе
  	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	//выключаем флаги при выводе
 	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
 	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  	tty.c_cc[VTIME] = 10;    // время ожидания ввода
 	 tty.c_cc[VMIN] = 1; // минимальное количество символов для ввода
 	 
 	   // устанавливаемтактовую частоту
 	cfsetispeed(&tty, B9600);
  	cfsetospeed(&tty, B9600);
	
	  // меняем параметры для терминала и ловим ошибку
  	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      		return 1;
  	}
  	int num_bytes;
  	char read_buf[256];
  	while(1){
  		num_bytes = read(serial_port, &read_buf,sizeof(int));
  		if(num_bytes > 0){
  			printf("%s",read_buf);
		}else{
			break;
		}
	}
	printf("Stop reading data\n");
  	close(serial_port);
  	return 0;
}
