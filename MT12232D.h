#if !defined(_MT12232D_H_)
#define _MT12232D_H_


#define LCD_USE_RAM_BUF // ������������ ����� � ��� � �������� ����� ������

#if defined(LCD_ROTATE) && !defined(LCD_USE_RAM_BUF)
// ���� ��������� ���������� "����� ������", �� ������ �������������� ����� �
// ���, � �� ����� ����������
#define LCD_USE_RAM_BUF
#endif


#define	LCD_X_RES	122	// ���������� �� �����������
#define	LCD_Y_RES	32	// ���������� �� ���������

// ������ ������ ������
#define LCD_TWICE_WIDTH		0x1 // �������� ������ ��������
#define LCD_TWICE_HEIGHT	0x2 // �������� ������ ��������
#define LCD_INVERSION		0x4 // �s�������� ����������� ��������

// ������ ������ �������
#define LCD_OR   0 // ������� �������������� �������� ���������
#define LCD_AND  1 // ������� �������������� ��������� ��������
#define LCD_XOR  2 // ������� �������������� ��������� ��������

// ����� ���������������� ��� ���������� �����������
#define _A0_	C,0,H	// 0-�������, 1-������
#define _RW_	C,1,H	// 0-������, 1-������
#define _E_		C,2,H	// ������������� (0->1)
#define _RES_	C,3,H	// 0-�����
#define _CS_	B,2,H	// 0-����� ������� �����������,
						// 1-����� ������ �����������



// ������������� ����������
extern void LCD_Init(void);

#if defined(LCD_USE_RAM_BUF)
// ������� ���������� ���������� ������� ������ � ���������
extern void LCD_Update(void);
#endif

#if defined(LCD_USE_RAM_BUF)
// ������� ������� ������ � ���������� ������������
extern void LCD_ClearBuf(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
#endif

// ������� ������� ���������� � ���������� ������������
extern void LCD_Clear(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);

#if defined(LCD_USE_RAM_BUF)
// ��������� ����������� ������ � ������, ������������� � ���
extern void LCD_StrBuf(uint8_t X, uint8_t Y, uint8_t idFont, char *Str,
	uint8_t Settings);
#endif

// ������� ����������� ������ �� ���������
extern void LCD_Str(uint8_t X, uint8_t Y, uint8_t idFont, char *Str,
	uint8_t Settings);

#if defined(LCD_USE_RAM_BUF)
// ��������� ����������� ������ � ������, ������������� � ���.
// �������� ������ ��� ���� ������������� �� FLASH.
extern void LCD_StrBuf_P(uint8_t X, uint8_t Y, uint8_t idFont,
	const char *Str_P, uint8_t Settings);
#endif

// ������� ����������� ������ � ���������.
// �������� ������ ��� ���� ������������� �� FLASH.
extern void LCD_Str_P(uint8_t X, uint8_t Y, uint8_t idFont, const char *Str_P,
	uint8_t Settings);

#if defined(LCD_USE_RAM_BUF)
// ��������� ����������� ������� � ������, ������������� � ���
extern void LCD_ChrBuf(uint8_t X, uint8_t Y, uint8_t idFont, char Chr,
	uint8_t Settings);
#endif

// ������� ����������� ������� �� ���������
extern void LCD_Chr(uint8_t X, uint8_t Y, uint8_t idFont, char Chr,
	uint8_t Settings);


// �������
///////////////////////////////////////////////////////////////////////////////

// ��������� ������ ��������� �������
extern void LCD_DrawMode(uint8_t Mode);

// ������� ���� ������ �� ���������
extern void LCD_Pixel(uint8_t X, uint8_t Y);

// ������ ����� ����� ����� ������� �� ����������
extern void LCD_Line(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);

// ������ ����������
extern void LCD_Circle(uint8_t X, uint8_t Y, uint8_t R);

// ��������� ��������������
extern void LCD_Rect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2,
	uint8_t Fill);

#endif
