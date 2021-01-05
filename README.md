# Вывод текста и графики на индикатор МЭЛТ MT-12232D


## Введение

Проект разработан для демонстрации вывода текстовых данных с использованием
разных шрифтов, а также графических примитивов на индикатор МЭЛТ MT-12232D с
разрешением 122x32. Для сборки проекта использована интегрированная среда
разработки Atmel Studio 6.2 (build 1563).


## Описание

Для проекта было использовано устройство, состоящее из платы Arduino NANO и
индикатора МЭЛТ MT-12232D, которые должны быть соединены друг с другом в
соответствии со следующей таблицей:

|MT-12232D-2YLG|Arduino Nano|Название сигнала            |
|--------------|------------|----------------------------|
|01 (DB4)      |D4  (PD4)   |Шина данных 4               |
|02 (DB5)      |D5  (PD5)   |Шина данных 5               |
|03 (DB6)      |D6  (PD6)   |Шина данных 6               |
|04 (DB7)      |D7  (PD7)   |Шина данных 7               |
|05 (A0)       |A0  (PC0)   |Команда(0)/Данные(1)        |
|06 (R/W)      |A1  (PC1)   |Запись(0)/Чтение(1)         |
|07 (E)        |A2  (PC2)   |Стробирование               |
|08 (DB3)      |D3  (PD3)   |Шина данных 3               |
|09 (DB2)      |D2  (PD2)   |Шина данных 2               |
|10 (DB1)      |D9  (PB1)   |Шина данных 1               |
|11 (DB0)      |D8  (PB0)   |Шина данных 0               |
|12 (GND)      |GND (GND)   |Общий                       |
|13 (VO)       |GND (GND)   |Контрастность               |
|14 (VCC)      |5V  (5V)    |Питание 5В                  |
|15 (K)        |-           |Подсветка - (Катод)         |
|16 (A)        |-           |Подсветка + (Анод)          |
|17 (RES)      |A3  (PC3)   |Сброс индикатора (0)        |
|18 (CS)       |D10 (PB2)   |Выбор левого контроллера(1)/|
|              |            |Выбор правого контроллера(0)|

Схема устройства также представлена в файле MT12232D-ARDUINO_NANO.pdf в
подкаталоге Doc проекта.

Описание индикатора MT-12232D представлено в файле MT-12232D.pdf в подкаталоге
Doc проекта.

Управление индикатором производится по параллельному интерфейсу.

Для загрузки HEX-файла в микроконтроллер можно использовать загрузчик,
записанный в Arduino NANO. Можно также использовать программатор AVRISP mkII.

Для загрузки HEX-файла в микроконтроллер с помощью загрузчика, прошитого в
Arduino NANO необходимо:
- С помощью кабеля USB поключить Arduino NANO к компьютеру.
- Установить драйвер для преобразователя USB-COM, расположенного на
  Arduino NANO. На китайских платах Arduino NANO чаще всего установлена
  микросхема CH340.
- В каталоге проекта открыть в текстовом редакторе файл Demo_MT12232.bat и в
  строке "@set PORT=COMn" установить номер COM-порта, который появится в
  системе после установки драйвера для преобразователя USB-COM.
- Запустить на выполнение командный файл Demo_MT12232.bat.

Для организации работы программы в режиме кооперативной многозадачности в
проекте использована свободная библиотека макросов Protothreads автора Adam
Dunkels для работы с протопотоками, а также вспомогательная библиотека функций
и макросов Multitasking собственной разработки, предназначенная для работы
совместно с библиотекой Protothreads и обеспечивающей дополнительный сервис:

- Организация последовательного выполнения задач.
- Реализация общесистемного таймера.
- Реализация мьютексов для синхронизации доступа к ресурсам.
- Реализация таймаутов для отслеживания различных временных интервалов.
- Поддержка внеочередного выполнения приоритетной задачи.
- Поддержка драйверов периферийных устройств.

файл Config.h представляет собой конфигурационный файл программы.
Содержит объявления констант и макросов, используемых программными модулями,
входящими в проект.

В программе использовано 3 задачи. Все задачи реализованы с помощью
протопотоков. В функции main() реализован общеизвестный бесконечный цикл,
который является аналогом планировщика ОС. В этом цикле с помощью макроса
MT_DISPATCH() в каждом проходе производится поочередный вызов активных задач,
реализованных в виде протопотоков и  зарегистрированных с помощью функции
MT_TaskInit().

Задача Task_Info выводит бегущую строку в верхней части дисплея шрифтом 6x8.

Задача Task_Fonts выводит символы различными шрифтами, а также демонстрирует
работу графических примитивов.

Задача Task_Blink в бесконечном цикле моргает светодиодом. Для формирования
пауз между переключениями светодиода используется вызов MT_SleepMs, который
на интервал времени, уазанный параметром TimeMs переводит задачу в состояние
ожидания, при котором задача не вызывается планировщиком. По истечении
указанного интервала времени обработчик системного таймера переводит задачу в
активное состояние и она продолжает свое выполнение.




## Компоненты, используемые в проекте


### 1. Свободная библиотека макросов Protothreads для работы с функциями-протопотоками.

Содержит включаемые Файлы с описанием макросов и констант для поддержки
протопотоков. Библиотека предназначена для многозадачного программирования с
малыми накладными расходами. Протопотоки позволяют разделить выполнение
программы на несколько независимых задач. В данном проекте протопотоки
используются для реализации кооперативной многозадачности, не требующей больших
накладных расходов. Основным свойством функций-протопотоков является
возможность продолжения исполнения кода с того места, в котором остановилось
выполнение при предыдущем вызове.

Ссылки для ознакомления с библиотекой Protothreads:
- http://dunkels.com/adam/pt/
- http://bsvi.ru/protopotoki-protothreads/
- https://habr.com/ru/company/embox/blog/244361/
- https://habr.com/ru/post/143318/

Состоит из нескольких файлов с расширением .h, расположенных в подкаталоге Pt
проекта. Более подробное описание используемых сервисов библиотеки Protothreads
приведено в проекте [Demo_MT](https://github.com/slisenko-yuri/Demo_MT).


### 2. Вспомогательная библиотека MultiTasking (библиотека MT).

Разработана для использования совместно с библиотекой Protothreads.
Предоставляет дополнительный сервис, используемый для кооперативной
многозадачности.  
Библиотека Multitasking состоит из файлов MT_Cfg.h, MT.h, MT.c.
Конфигурирование библиотеки Multitasking производится с помощью директив
препроцессора, расположенных в файле MT_Cfg.h. Более подробное описание
библиотеки Multitasking приведено в проекте [Demo_MT](https://github.com/slisenko-yuri/Demo_MT).


### 3. Библиотека MT12232D для работы с индикатором

Содержит сервис для вывода текстовых и графических данных на индикатор
MT-12232D.

Если в файле MT12232D.h с помощью директивы #define определен идентификатор
LCD_USE_RAM_BUF, то для работы с индикатором будет использован буфер,
расположенный в ОЗУ микроконтроллера. Этот буфер представляет собой копию
экрана индикатора и представляет собой двумерный массив Buf[4][122].  
Точка с координатами Y=0,X=0 располагается в разряде 0 элемента массива
Buf[0][0], а точка с координатами Y=31,X=121 располагается в разряде 7 элемента
массива Buf[3][121]. С использованием буфера в ОЗУ скорость вывода
информации на индикатор выше, чем без буфера. Однако буфер в памяти требует
дополнительного расхода ОЗУ (488 байт для буфера плюс переменные для работы с
буфером). Для некоторых приложений это может оказаться критическим.

Если в файле Config.h с помощью директивы #define определен идентификатор
LCD_ROTATE, то вывод информации на индикатор будет производится "вверх ногами".
Это необходимо в случае, если по конструктивным соображениям индикатор повернут
на 180 градусов. В данном проекте такой вывод изображения возможен только при
использовании экранного буфера в ОЗУ микроконтроллера, при этом точка с
координатами Y=0,X=0 располагается в разряде 7 элемента массива Buf[3][121], а
точка с координатами Y=31,X=121 располагается в разряде 0 элемента массива
Buf[0][0], т.е. для перевернутого индикатора изображение в буфере формируется
ПЕРЕВЕРНУТЫМ!

Библиотека MT12232D состоит из файлов MT12232D.h и MT12232D.c.

#### Сервисы, предоставляемые библиотекой MT12232D
-------------------------------------------------------------------------------

- void LCD_Init(void)

  Инициализация индикатора.  
  Производит начальную инициализацию переменных, используемых для работы с
  индикатором, настройку портов микроконтроллера, используемых для управления и
  обмена данными с индикатором, а также настройку режима работы индикатора.


- void LCD_Update(void);

  Выводит содержимое измененной области экранного буфера на индикатор.  
  Для использования этой функции в проекте необходимо в файле MT12232D.h
  раскомментировать следующую строку:  
  `#define LCD_USE_RAM_BUF`


- void LCD_ClearBuf(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)

  Очищает область экранного буфера с указанными координатами. Параметры X1 и Y1
  указывают на верхний левый угол, а X2 и Y2 на нижний правый угол
  прямоугольной области.  
  С помощью функции LCD_Update() измененную область экранного буфера затем
  можно вывести на индикатор.  
  Для использования функции LCD_ClearBuf в проекте необходимо в файле MT12232D.h
  раскомментировать следующую строку:  
  `#define LCD_USE_RAM_BUF`
 
  ПРИМЕР:
  ```
  LCD_ClearBuf(20, 10, 100, 25); // Очистить прямоугольную область в экранном
                                 // буфере по координатам 20:10 и 100:25
  LCD_Update(); // Вывод измененной части буфера в индикатор
  ```
 
 
- void LCD_Clear(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);

  Очищает область индикатора с указанными координатами. Параметры X1 и Y1
  указывают на верхний левый угол, а X2 и Y2 на нижний правый угол
  прямоугольной области.  


- void LCD_StrBuf(uint8_t X, uint8_t Y, uint8_t idFont, char *Str, uint8_t Settings)

  Формирует изображение строки Str в буфере по координатам, указанным
  параметрами X, Y и шрифтом, идентификатор которого указан параметром idFont.
  Идентификаторы шрифтов объявлены в файле Font.h.
  С помощью параметра Settings можно указать дополнительные режимы отображения
  текста (удвоение ширины, высоты символов, инверсное изображение).
  Эти режимы отображения текста представляют собой константы, объявленные в
  файле MT12232D.h, которые складываются с помощью операции ИЛИ:  
  LCD_TWICE_WIDTH - удвоение ширины символов,  
  LCD_TWICE_HEIGHT - удвоение высоты символов,  
  LCD_INVERSION - инверсное отображение символов.  
  С помощью функции LCD_Update() измененную область экранного буфера затем
  можно вывести на индикатор.  
  Для использования функции LCD_StrBuf в проекте необходимо в файле MT12232D.h
  раскомментировать следующую строку:  
  `#define LCD_USE_RAM_BUF`

  ПРИМЕР:
  ```
  char Msg1[] = "0123456789";
  char Msg2[] = "АБВГДЕЁЖЗИЙК";
  ...
  // Формирование в буфере изображения строки Msg1 шрифтом FONT_6x8 с
  // удвоением символов по ширине и высоте
  LCD_StrBuf(42, 2, FONT_6x8, Msg1, LCD_TWICE_WIDTH | LCD_TWICE_HEIGHT);

  // Формирование в буфере изображения строки Msg2 шрифтом FONT_8x8t
  LCD_StrBuf(10, 22, FONT_8x8t, Msg2, 0);
    
  // Вывод измененной части буфера в индикатор
  LCD_Update();
  ```


- void LCD_StrBuf_P(uint8_t X, uint8_t Y, uint8_t Font, const char *Str, uint8_t Settings);

  Функция работает аналогично функции LCD_StrBuf() с тем исключением, что
  используется для строк, расположенных в программной памяти микроконтроллера
  (во FLASH).  
  Для использования функции LCD_StrBuf в проекте необходимо в файле MT12232D.h
  раскомментировать следующую строку:  
  `#define LCD_USE_RAM_BUF`
    
  ПРИМЕР:  
  ```
  const char Str[] PROGMEM = "Message1"; // Объявление строки во FLASH
  ...
  // Формирование в буфере изображения строки c удвоенной шириной и высотой.
  LCD_StrBuf_P(0, 0, FONT_6x8, Str, LCD_TWICE_WIDTH | LCD_TWICE_HEIGHT);

  // Формирование в буфере изображения строки "Message2" шрифтом FONT_8x16
  LCD_StrBuf_P(3, 16, FONT_8x16, PSTR("Message2"), 0);

  // Вывод измененной части буфера в индикатор
  LCD_Update();  
  ```


- void LCD_Str(uint8_t X, uint8_t Y, uint8_t idFont, char *Str, uint8_t Settings);

  Выводит строку Str на индикатор по координатам, указанным
  параметрами X, Y и шрифтом, идентификатор которого указан параметром idFont.
  Идентификаторы шрифтов объявлены в файле Font.h.
  С помощью параметра Settings можно указать дополнительные режимы отображения
  текста (удвоение ширины, высоты символов, инверсное изображение).
  Эти режимы отображения текста представляют собой константы, объявленные в
  файле MT12232D.h, которые складываются с помощью операции ИЛИ:  
  LCD_TWICE_WIDTH - удвоение ширины символов,  
  LCD_TWICE_HEIGHT - удвоение высоты символов,  
  LCD_INVERSION - инверсное отображение символов.  


- void LCD_Str_P(uint8_t X, uint8_t Y, uint8_t Font, const char *Str, uint8_t Settings);

  Функция работает аналогично функции LCD_Str() с тем исключением, что
  используется для строк, расположенных в программной памяти микроконтроллера
  (во FLASH).  


- void LCD_ChrBuf(uint8_t X, uint8_t Y, uint8_t Font, char Chr, uint8_t Settings);

  Формирует изображение символа Chr в экранном буфере по координатам, указанным
  параметрами X, Y и шрифтом, идентификатор которого указан параметром idFont.
  Идентификаторы шрифтов объявлены в файле Font.h.
  С помощью параметра Settings можно указать дополнительные режимы отображения
  символа (удвоение ширины, высоты символов, инверсное изображение).
  Эти режимы отображения представляют собой константы, объявленные в
  файле MT12232D.h, которые складываются с помощью операции ИЛИ:  
  LCD_TWICE_WIDTH - удвоение ширины символа,  
  LCD_TWICE_HEIGHT - удвоение высоты символа,  
  LCD_INVERSION - инверсное отображение символа.  
  С помощью функции LCD_Update() измененную область экранного буфера затем
  можно вывести на индикатор.  
  Для использования функции LCD_ChrBuf в проекте необходимо в файле MЕ12232.h
  раскомментировать следующую строку:  
  `#define LCD_USE_RAM_BUF`


- void LCD_Chr(uint8_t X, uint8_t Y, uint8_t Font, char Chr, uint8_t Settings);

  Выводит символ Chr на индикатор по координатам, указанным
  параметрами X, Y и шрифтом, идентификатор которого указан параметром idFont.
  Идентификаторы шрифтов объявлены в файле Font.h.
  С помощью параметра Settings можно указать дополнительные режимы отображения
  символа (удвоение ширины, высоты символов, инверсное изображение).
  Эти режимы отображения представляют собой константы, объявленные в
  файле MT12232D.h, которые складываются с помощью операции ИЛИ:  
  LCD_TWICE_WIDTH - удвоение ширины символа,  
  LCD_TWICE_HEIGHT - удвоение высоты символа,  
  LCD_INVERSION - инверсное отображение символа.  


- LCD_DrawMode(uint8_t Mode);

  Функция устанавливает режим отрисовки графики.  
  Этот режим представляют собой одну констант, объявленных в
  файле MT12232D.h и может принимать одно из следующих значений:
  LCD_OR - графика отрисовывается обычными пикселами,  
  LCD_AND - графика отрисовывается стиранием пикселов,  
  LCD_XOR - графика отрисовывается инверсией пикселов.  


- void LCD_Pixel(uint8_t X, uint8_t Y);

  Функция выводит один пиксел на индикатор по указанной координате.  
  Параметр X представляет собой координату по горизонтали и может принимать
  значения от 0 до 121, а параметр Y представляет собой координату по вертикали
  и может принимать значения от 0 до 31.


- void LCD_Line(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);

  Функция рисует линию между двумя точками на индикаторе (алгоритм Брезенхэма).  
  Параметры X1 и Y1 представляют собой горизонтальную и вертикальную координату
  начала линии, а параметры X2 и Y2 горизонтальную и вертикальную координату
  конца линии. Параметры X1 и X2 могут принимать значения от 0 до 121, а
  параметры Y1 и Y2 - от 0 до 31.

- void LCD_Circle(uint8_t X, uint8_t Y, uint8_t R);

  Функция рисует окружность с центром, координаты которого указаны параметрами
  X и Y с радиусом R. Параметры X и R могут принимать значения от 0 до 121, а
  параметр Y - от 0 до 31.

- void LCD_Rect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t Fill);

  Функция рисует прямоугольник.  
  Параметры X1, Y1 и X2, Y2 представляют собой координаты противоположных углов
  прямоугольника. Если параметр Fill не равен 0, то прямоугольник отображается
  закрашенным. Параметры X1 и X2 могут принимать значения от 0 до 121, а
  параметры Y1 и Y2 - от 0 до 31.


### 4. Библиотека шрифтов.

Реализованные шрифты находятся в подкаталоге Fonts, а функции для работы со
шрифтами и идентификаторы шрифтов расположены в файлах Font.h и Font.c.
Для того, чтобы неиспользуемый шрифт не занимал программную память
микроконтроллера следует закомментировать определение его идентификатора в
файле Font.h.  
В подкаталоге FontExamples находятся файлы, с помощью которых можно
ознакомиться с графическим представлением символов реализованных шрифтов.


### 5. Библиотека макросов, объявленных в файле pin_macros.h.

Используется для управления портами микроконтроллера.  
Например, в файле Config.h для управления выводом порта микроконтроллера в
качестве выхода, который управляет светодиодом сделано следующее объявление:

`#define HL		B,5,H`

Здесь символ "B" означает принадлежность вывода к порту B микроконтроллера,
цифра "5" обозначает номер разряда порта, а символ "H" обозначает активный
логический уровень (лог.1) сигнала HL.  
После этого объявления управлять выводом порта можно с помощью
следующих макросов:  
```
DRIVER(HL, OUT); // Настройка вывода порта HL в качестве выхода
ON(HL); // Установить активный уровень сигнала HL (т.е. уровень, определенный
        // символом H при объявлении сигнала HL)
OFF(HL); // Установить пассивный уровень сигнала HL (т.е. уровень,
         // противоположный активному)
CLR(HL); // Установить сигнал HL в состояние лог.0
SET(HL); // Установить сигнал HL в состояние лог.1
CPL(HL); // Инвертировать сигнал HL
TOGGLE(HL); // Смотри CPL(HL)
```

Настройка порта в качестве входа производится следующим образом:
```
#define SW		C,5,L // Порт C, Разряд 5, активный уровень - лог.0
DRIVER(SW, IN);  // Настройка вывода порта SW в качестве входа
```
После этого работать с портом можно с помощью следующих макросов:  
```
if (ACTIVE(SW)) {...} // Если на входе SW активный уровень сигнала, то
                      // выполнить код в фигурных скобках
if (PIN_H(SW)) {...} // Если на входе SW высокий уровень сигнала, то
                     // выполнить код в фигурных скобках
if (PIN_L(SW)) {...} // Если на входе SW низкий уровень сигнала, то
                     // выполнить код в фигурных скобках
```
