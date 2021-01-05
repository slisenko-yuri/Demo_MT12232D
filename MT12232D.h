#if !defined(_MT12232D_H_)
#define _MT12232D_H_


#define LCD_USE_RAM_BUF // Использовать буфер в ОЗУ в качестве копии экрана

#if defined(LCD_ROTATE) && !defined(LCD_USE_RAM_BUF)
// Если индикатор перевернут "вверх ногами", то должен использоваться буфер в
// ОЗУ, а не буфер индикатора
#define LCD_USE_RAM_BUF
#endif


#define	LCD_X_RES	122	// разрешение по горизонтали
#define	LCD_Y_RES	32	// разрешение по вертикали

// Режимы вывода текста
#define LCD_TWICE_WIDTH		0x1 // Удвоение ширины символов
#define LCD_TWICE_HEIGHT	0x2 // Удвоение высоты символов
#define LCD_INVERSION		0x4 // Иsнверсное отображение символов

// Режимы вывода графики
#define LCD_OR   0 // Графика отрисовывается обычными пикселами
#define LCD_AND  1 // Графика отрисовывается стиранием пикселов
#define LCD_XOR  2 // Графика отрисовывается инверсией пикселов

// Порты микроконтроллера для управления индикатором
#define _A0_	C,0,H	// 0-Команда, 1-Данные
#define _RW_	C,1,H	// 0-Запись, 1-Чтение
#define _E_		C,2,H	// Стробирование (0->1)
#define _RES_	C,3,H	// 0-Сброс
#define _CS_	B,2,H	// 0-Выбор правого контроллера,
						// 1-Выбор левого контроллера



// Инициализация индикатора
extern void LCD_Init(void);

#if defined(LCD_USE_RAM_BUF)
// Выводит содержимое измененной области буфера в индикатор
extern void LCD_Update(void);
#endif

#if defined(LCD_USE_RAM_BUF)
// Очищает область буфера с указанными координатами
extern void LCD_ClearBuf(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
#endif

// Очищает область индикатора с указанными координатами
extern void LCD_Clear(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);

#if defined(LCD_USE_RAM_BUF)
// Формирует изображение строки в буфере, расположенном в ОЗУ
extern void LCD_StrBuf(uint8_t X, uint8_t Y, uint8_t idFont, char *Str,
	uint8_t Settings);
#endif

// Выводит изображение строки на индикатор
extern void LCD_Str(uint8_t X, uint8_t Y, uint8_t idFont, char *Str,
	uint8_t Settings);

#if defined(LCD_USE_RAM_BUF)
// Формирует изображение строки в буфере, расположенном в ОЗУ.
// Исходная строка при этом располагается во FLASH.
extern void LCD_StrBuf_P(uint8_t X, uint8_t Y, uint8_t idFont,
	const char *Str_P, uint8_t Settings);
#endif

// Выводит изображение строки в индикатор.
// Исходная строка при этом располагается во FLASH.
extern void LCD_Str_P(uint8_t X, uint8_t Y, uint8_t idFont, const char *Str_P,
	uint8_t Settings);

#if defined(LCD_USE_RAM_BUF)
// Формирует изображение символа в буфере, расположенном в ОЗУ
extern void LCD_ChrBuf(uint8_t X, uint8_t Y, uint8_t idFont, char Chr,
	uint8_t Settings);
#endif

// Выводит изображение символа на индикатор
extern void LCD_Chr(uint8_t X, uint8_t Y, uint8_t idFont, char Chr,
	uint8_t Settings);


// ГРАФИКА
///////////////////////////////////////////////////////////////////////////////

// Установка режима отрисовки графики
extern void LCD_DrawMode(uint8_t Mode);

// Выводит один пиксел на индикатор
extern void LCD_Pixel(uint8_t X, uint8_t Y);

// Рисует линию между двумя точками на индикаторе
extern void LCD_Line(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);

// Рисует окружность
extern void LCD_Circle(uint8_t X, uint8_t Y, uint8_t R);

// Рисование прямоугольника
extern void LCD_Rect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2,
	uint8_t Fill);

#endif
