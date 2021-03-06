#if !defined(_MT_H_)
#define _MT_H_

//////////////////////////////////////////////////////////////////////////
// ������ ��������� ���������������
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// ���������� �����
//////////////////////////////////////////////////////////////////////////

#include "Mt_Cfg.h" // ������������ ���������� Multitasking
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

// ������������ ���������� GCC "Labels as Values".
// ����� ����� ����� ����������� ����� ���������� ������ ��������� switch.
#define LC_INCLUDE "lc-addrlabels.h" 

#include "pt\pt.h" // ���������� Protothreads (�����������)

//////////////////////////////////////////////////////////////////////////




#define ID_UNKNOWN		0xFF




//////////////////////////////////////////////////////////////////////////
// �������� ������������ ������������� ��������
//////////////////////////////////////////////////////////////////////////


#if !defined(MT_TASK_COUNT)
#error "MT: MT_TASK_COUNT must be defined!"
#else
#define __MT_TASK_COUNT (MT_TASK_COUNT+0)
#if ((__MT_TASK_COUNT < 1) || (__MT_TASK_COUNT > 32))
#error "MT: MT_TASK_COUNT must be in the range 1...32!"
#endif
#endif

#if !defined(MT_TIMEOUT_COUNT)
#define __MT_TIMEOUT_COUNT	0
#else
#define __MT_TIMEOUT_COUNT (MT_TIMEOUT_COUNT+0)
#if (__MT_TIMEOUT_COUNT > 255)
#error "MT: MT_TIMEOUT_COUNT must be in the range 0...255!"
#endif
#endif

#if !defined(MT_MUTEX_COUNT)
#define __MT_MUTEX_COUNT	0
#else
#define __MT_MUTEX_COUNT (MT_MUTEX_COUNT+0)
#if (__MT_MUTEX_COUNT > 255)
#error "MT: MT_MUTEX_COUNT must be in the range 0...255!"
#endif
#endif

#if !defined(MT_DRV_COUNT)
#define __MT_DRV_COUNT	0
#else
#define __MT_DRV_COUNT (MT_DRV_COUNT+0)
#if (__MT_DRV_COUNT > 255)
#error "MT: MT_DRV_COUNT must be in the range 0...255!"
#endif
#endif




//////////////////////////////////////////////////////////////////////////
// � ����������� �� ���������� ����� � ���������, ���������� �����������
// ���������� (��� MT_TYPE), ������������ ����������� Multitasking
//////////////////////////////////////////////////////////////////////////
#if __MT_TASK_COUNT <= 8
typedef uint8_t MT_TYPE;
#elif __MT_TASK_COUNT <= 16
typedef uint16_t MT_TYPE;
#else
typedef uint32_t MT_TYPE;
#endif
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ��� �������, ������������ � �������� ������
//////////////////////////////////////////////////////////////////////////
typedef char (*MT_TASK_TYPE)(struct pt *Context);
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ������� ��� �������� ������ ���������� ������� � ������������ � �������
// �� ����������� � ��������� �����.
//////////////////////////////////////////////////////////////////////////

// ������ ��� �������������� ����������� � ����� ���������� �������
#define MS_TO_SYSTICK(ms)	((ms) / MT_SYSTIMER_PERIOD_MS)

// ������ ��� �������������� ������ ���������� ������� � ������������
#define SYSTICK_TO_MS(ticks) ((ticks) * MT_SYSTIMER_PERIOD_MS)

//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// ������� ��� ����������� ������
//////////////////////////////////////////////////////////////////////////

// ������ ����������
#define DISABLE_IRQ() cli()

// ���������� ����������
#define ENABLE_IRQ() sei()

// ���� � ����������� ������ � ��������������� ������ ��� ��������
// ��������� ����������
#define ENTER_CRITICAL() uint8_t __StatusIrq = SREG; cli()

// ����� �� ����������� ������
#define EXIT_CRITICAL() SREG = __StatusIrq

// ���� � ����������� ������ ��� �������������� ������ ��� ��������
// ��������� ����������
#define ENTER_CRITICAL2() __StatusIrq = SREG; cli()

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ��������, ��������� � �������������� � ������� ������� � ��������
// ����������.
//////////////////////////////////////////////////////////////////////////

#if defined(MT_USE_TASK_SLEEP) || (__MT_TIMEOUT_COUNT > 0) ||\
	defined(MT_USE_GETSYSTIMER) || defined(MT_SYSTIMER_CALLBACK)
	
// ���� ��������� ������������ "������" (������� MT_SleepMs(),
// MT_SetSleep(), MT_TaskSetSleep()) ��� �������� (�������
// MT_TimeoutSet(), MT_TimeoutSetIrq(), MT_TimeoutMs(), MT_TimeoutMsIrq(),
// MT_TimeoutGet()) ��� ������ MT_SYSTIMER_CALLBACK() ��� �������
// MT_GetSysTimer(), �� ���������� ������������ ��������� ������.
#define MT_USE_SYSTIMER

// �������� ������������ ������ �������, ������� ����� �������������� �
// �������� ����������

#if !defined(MT_SYSTIMER)
#error "MT_SYSTIMER must be defined!"
#endif

#if ((MT_SYSTIMER < 0) || (MT_SYSTIMER > 5))

#error "MT: MT_SYSTIMER must be in the range 0...5"

#else

#if (MT_SYSTIMER > 4) && defined(__AVR_ATmega328PB__)
#error "MT: MT_SYSTIMER for ATmega328PB must be in the range 0...4"

#elif (MT_SYSTIMER > 3)\
	&& (defined(__AVR_ATmega64__) || defined(__AVR_ATmega64A__)\
	|| defined(__AVR_ATmega128__) || defined(__AVR_ATmega128A__)\
	|| defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__))
#error "MT: MT_SYSTIMER for this chip must be in the range 0...3"

#elif (MT_SYSTIMER > 2)\
	&& (defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__)\
	|| defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__)\
	|| defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__)\
	|| defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__)\
	|| defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__)\
	|| defined(__AVR_ATmega88PB__)\
	|| defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__)\
	|| defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__)\
	|| defined(__AVR_ATmega164PA__) \
	|| defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__)\
	|| defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__)\
	|| defined(__AVR_ATmega168PB__)\
	|| defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__)\
	|| defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324P__)\
	|| defined(__AVR_ATmega324PA__)\
	|| defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)\
	|| defined(__AVR_ATmega328PB__)\
	|| defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__)\
	|| defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__))
#error "MT: MT_SYSTIMER for this chip must be in the range 0...2"

#elif (MT_SYSTIMER > 1)\
	&& (defined(__AVR_ATtiny2313__) || defined(__AVR_ATtint2313A__)\
	|| defined(__AVR_ATtint4313A__) || defined(__AVR_ATmega8515__)) 
#error "MT: MT_SYSTIMER for this chip must be in the range 0...1"

#endif
#endif
#endif

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ���� ������ ��������� ���������� ������ 8 ���, �� ��������
// ������������� ��������� GPIOR0...GPIOR2
//////////////////////////////////////////////////////////////////////////
#if __MT_TASK_COUNT > 8
#undef MT_USE_GPIOR
#endif
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ��� �����������������, � ������� ����������� �������� GPIOR0...GPIOR2
// �������� �� �������������.
//////////////////////////////////////////////////////////////////////////
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) ||\
	defined(__AVR_ATmega8515__) ||\
	defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) ||\
	defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__) ||\
	defined(__AVR_ATmega64__) || defined(__AVR_ATmega64A__) ||\
	defined(__AVR_ATmega128__) || defined(__AVR_ATmega128A__)
#undef MT_USE_GPIOR
#endif
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ������� ���������� ������
//////////////////////////////////////////////////////////////////////////

// ��������� �� ������
extern MT_TASK_TYPE __MT_Task[__MT_TASK_COUNT];

// ��������� �����
extern struct pt __MT_Context[__MT_TASK_COUNT];

// ������� ���������� ������������������ �����.
// (������������ ������ ��� ������������� �����)
extern uint8_t __MT_TaskCount;




#if !defined(MT_USE_GPIOR)

// ������� ����� �������� ����� (������� � ����������)
extern MT_TYPE __MT_TaskActiveFlags;

// ID ������� ������
extern uint8_t __MT_TaskCur;

// ������� ����� ������� ������ (������������� ��� � �������, ������ ID
// ������� ������).
extern MT_TYPE __MT_TaskCurMask;

#else // #if !defined(MT_USE_GPIOR)

// ������� ����� �������� ����� (������� � ����������)
#define __MT_TaskActiveFlags	GPIOR0

// ID ������� ������
#define __MT_TaskCur			GPIOR1

// ������� ����� ������� ������ (������������� ��� � �������, ������ ID
// ������� ������).
#define __MT_TaskCurMask		GPIOR2

#endif // #if !defined(MT_USE_GPIOR)




#if defined(MT_USE_TASK_SLEEP)

// �������� ������ ���������� ������� ��� "������" �����
extern uint16_t __MT_TaskSysTimer[__MT_TASK_COUNT];

#endif




#if (__MT_TIMEOUT_COUNT > 0)

#if defined(MT_USE_LONG_TIMEOUT)

// ���, ������������ ��� ��������� (49.7 ����� ��� ���������� ����� 1��)
typedef uint32_t MT_TIMEOUT_TYPE;

#else

// ���, ������������ ��� ��������� (65.5 ������ ��� ���������� ����� 1��)
typedef uint16_t MT_TIMEOUT_TYPE;

#endif

// �������� ��� ���������
extern MT_TIMEOUT_TYPE __MT_Timeout[__MT_TIMEOUT_COUNT];

#endif




#if (__MT_MUTEX_COUNT > 0)

// ��������� ��������� (>0 - ��������, 0 - ��������)
extern uint8_t __MT_Mutex[__MT_MUTEX_COUNT];

// ID �����, ����������� ��������������� �������
extern uint8_t __MT_MutexOwner[__MT_MUTEX_COUNT];

// ������� ����� �����, ������� ���� ������������ ����������������
// ��������
extern MT_TYPE __MT_MutexWaitFlags[__MT_MUTEX_COUNT];

#endif




#if defined(MT_USE_HIGH_PRIORITY_TASK)

// ID ������������ ������
extern uint8_t __MT_TaskPriority;

// ������� ����� ������������ ������
extern MT_TYPE __MT_TaskPriorityMask;

#endif




#if (__MT_DRV_COUNT > 0)

// ������� ����� �����, ������ ���������� ������ ��������������� ���������
extern MT_TYPE __MT_DrvWaitFlags[__MT_DRV_COUNT];

#endif

//////////////////////////////////////////////////////////////////////////





#if defined(MT_USE_SYSTIMER)
//////////////////////////////////////////////////////////////////////////
// ��������� ������������ ��� �������� 0,1,3,4,5
//////////////////////////////////////////////////////////////////////////
#define TIM_DIV_1		0x01
#define TIM_DIV_8		0x02
#define TIM_DIV_64		0x03
#define TIM_DIV_256		0x04
#define TIM_DIV_1024	0x05
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ��������� ������������ ��� ������� 2
//////////////////////////////////////////////////////////////////////////
#define TIM2_DIV_1		0x01
#define TIM2_DIV_8		0x02
#define TIM2_DIV_32		0x03
#define TIM2_DIV_64		0x04
#define TIM2_DIV_128	0x05
#define TIM2_DIV_256	0x06
#define TIM2_DIV_1024	0x07
//////////////////////////////////////////////////////////////////////////
#endif





//////////////////////////////////////////////////////////////////////////
// ��������� ������
//////////////////////////////////////////////////////////////////////////

#define MT_TaskCur()	__MT_TaskCur	// ID ������� ������

//////////////////////////////////////////////////////////////////////////
// ������������� ������ MT
//////////////////////////////////////////////////////////////////////////
extern void MT_Init(void);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ������������� ������
//////////////////////////////////////////////////////////////////////////
extern uint8_t MT_TaskInit(MT_TASK_TYPE Task, uint8_t fActive);
//////////////////////////////////////////////////////////////////////////

#if defined(MT_USE_GETSYSTIMER) || (__MT_TIMEOUT_COUNT > 0)
//////////////////////////////////////////////////////////////////////////
// ������� ���������� ���������� ������ ���������� �������, ��������� �
// ������� ��� �������.
//////////////////////////////////////////////////////////////////////////
extern uint32_t MT_GetSysTimer(void);
//////////////////////////////////////////////////////////////////////////
#endif



//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////

#if defined(MT_USE_TASK_SLEEP)

//////////////////////////////////////////////////////////////////////////
// ������� MT_SetSleep � MT_TaskSetSleep ������������� �����, � �������
// �������� ������ ����� ���������� � ���������� ���������. �����
// ����������� � ������ ���������� �������. ��� �������� ������� �
// ������������� ���������� ������������ ������ MS_TO_SYSTICK. � �������
// ������� MT_SetSleep ������ ����� ��������� � ��������� �������� ������
// ���� ����, � � ������� ������� MT_TaskSetSleep ����� ������ ������.
//////////////////////////////////////////////////////////////////////////
extern void MT_SetSleep(uint16_t Takts);
extern void MT_TaskSetSleep(uint8_t idTask, uint16_t Takts);
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ������������ ���������� ������� ������ �� �������� ����������
// �����������. ����� ���������� ������� �������� ������ ��������� ����
// ���������� � ���������, ���������� ����� ����� ������� MT_SleepMs.
//////////////////////////////////////////////////////////////////////////
#define MT_SleepMs(Context, TimeMs) \
do {\
	MT_SetSleep(MS_TO_SYSTICK(TimeMs)); /*��������� ������� ��������*/\
	PT_YIELD(Context); /*�������� ���������� � �����������*/\
} while(0)
//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////

#if (__MT_TIMEOUT_COUNT > 0)

//////////////////////////////////////////////////////////////////////////
// ��������� �������� � ������ ���������� �������.
//////////////////////////////////////////////////////////////////////////
#define MT_TimeoutSet(idTimeout, Takts) \
do {\
	ENTER_CRITICAL();\
	__MT_Timeout[idTimeout] = Takts;\
	EXIT_CRITICAL();\
} while(0)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ��������� �������� � ������ ���������� ������� ��� �����������
// �����������.
//////////////////////////////////////////////////////////////////////////
#define MT_TimeoutSetIrq(idTimeout, Takts) \
do {\
	__MT_Timeout[idTimeout] = Takts;\
} while(0)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ��������� �������� � �������������.
//////////////////////////////////////////////////////////////////////////
#define MT_TimeoutMs(idTimeout, Ms) \
MT_TimeoutSet(idTimeout, MS_TO_SYSTICK(Ms))
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ��������� �������� � ������������� ��� ����������� �����������
//////////////////////////////////////////////////////////////////////////
#define MT_TimeoutMsIrq(idTimeout, Ms) \
MT_TimeoutSetIrq(idTimeout, MS_TO_SYSTICK(Ms))
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ���������� ���������� ����� �������� � ������ ���������� �������.
// ���� ���������� 0, �� ��� ��������, ��� ������� ����������.
//////////////////////////////////////////////////////////////////////////
#define MT_TimeoutGet(idTimeout) __MT_Timeout[idTimeout]
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ��������� ���������
//////////////////////////////////////////////////////////////////////////

#define MT_TIMEOUT_CALC() \
do {\
	static MT_TIMEOUT_TYPE old_time = 0;\
	MT_TIMEOUT_TYPE time, interval;\
	uint8_t i;\
	\
	time = MT_GetSysTimer();\
	\
	/* ������� �������, ���������� � ������� ��������� ��������� */\
	/* �������� */\
	interval = time - old_time;\
	if (interval)\
	{\
		old_time = time;\
		for (i = 0; i < __MT_TIMEOUT_COUNT; i++)\
		{\
			DISABLE_IRQ();\
			/* ��������� �� ���������� �������, ���������� � ������� */\
			/* ��������� ��������� */\
			time = __MT_Timeout[i];\
			if (interval >= time) time = 0;\
			else time -= interval;\
			__MT_Timeout[i] = time;\
			ENABLE_IRQ();\
		}\
	}\
} while (0)

//////////////////////////////////////////////////////////////////////////

#else // #if __MT_TIMEOUT_COUNT > 0)

#define MT_TIMEOUT_CALC() // �����, ���� �������� �� ������������

#endif // (__MT_TIMEOUT_COUNT > 0)

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//���� ����� �������� �������, ��������� ����� MT_MutexWait ��� ������ �
//���� �� �������� �� �������� � ���������� ������. ����� ����, � �������
//������� ��� � �������, ��� ��� MT_MutexFree ������ ���� ������� ������� ��
//���, ������� ���� ������� MT_MutexWait. ����� �������, �� ����� �����
//�������� ������ ������� ����, ���������� � ������ �������, �����
//MT_MutexWait - MT_MutexFree, �� �������� � ���, ��� ���� ��� ����� ����
//������ ����������.
//////////////////////////////////////////////////////////////////////////

extern uint8_t __MT_MutexWait(uint8_t Mutex);

//////////////////////////////////////////////////////////////////////////
// �������� � ������ ��������.
// ����������: ������ ������ ������ �������� ������� ���
// �������������� ������ MT_MutexFree ����� 255 ��� (�� ������ �����������
// ��������)
//////////////////////////////////////////////////////////////////////////
#define MT_MutexWait(pt, Mutex) \
do {\
	PT_WAIT_UNTIL(pt, __MT_MutexWait(Mutex));\
} while (0)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ������������ ��������
//////////////////////////////////////////////////////////////////////////
extern void MT_MutexFree(uint8_t Mutex);
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ������� ����� � ��������/��������� ���������.
//////////////////////////////////////////////////////////////////////////

#if defined(MT_USE_TASK_RUN_STOP)

// ������� ������ � �������� ���������.
extern void MT_TaskSetActive(uint8_t idTask);

// ������� ������ � �������� ��������� ��� ����������� �����������.
extern void MT_TaskSetActiveIrq(uint8_t idTask);

// ������� ������ � ��������� ���������.
extern uint8_t MT_TaskSetPassive(void);

#endif

//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ������� ������ ��������� � ������ ���������.
// ��������� ������� ����� ������� ������ � ��������.
// ����������: ������������ ������ � ���������.
//////////////////////////////////////////////////////////////////////////
#define MT_DrvTaskWait(idDrv) \
do {\
	MT_TYPE mask;\
	ENTER_CRITICAL();\
	mask = __MT_TaskCurMask;\
	__MT_DrvWaitFlags[idDrv] |= mask;\
	__MT_TaskActiveFlags &= ~mask;\
	EXIT_CRITICAL();\
} while (0)
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ������� ������ ��������� � ������ ���������.
// ������������ ������ ��� ����������� �����������.
//////////////////////////////////////////////////////////////////////////
#define MT_DrvTaskWaitIrq(idDrv) \
do {\
	MT_TYPE mask;\
	mask = __MT_TaskCurMask;\
	__MT_DrvWaitFlags[idDrv] |= mask;\
	__MT_TaskActiveFlags &= ~mask;\
} while (0)
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ������� ��������� ������ ����� � �������� ���������
//////////////////////////////////////////////////////////////////////////
#define MT_DrvTaskRun(idDrv) \
do {\
	ENTER_CRITICAL();\
	/*���������� ������, ������� ���� ������������ ������� ��������*/\
	__MT_TaskActiveFlags |= __MT_DrvWaitFlags[idDrv];\
	__MT_DrvWaitFlags[idDrv] = 0;\
	EXIT_CRITICAL();\
} while (0)
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ������ ����������� ������� ��� ������������� � ����������
// ��� ��� ����������� �����������
//////////////////////////////////////////////////////////////////////////
#define MT_DrvTaskRunIrq(idDrv) \
do {\
	/*���������� ������, ������� ���� ������������ ������� ��������*/\
	__MT_TaskActiveFlags |= __MT_DrvWaitFlags[idDrv];\
	__MT_DrvWaitFlags[idDrv] = 0;\
} while (0)
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ���������
//////////////////////////////////////////////////////////////////////////




#if defined(MT_USE_HIGH_PRIORITY_TASK)

//////////////////////////////////////////////////////////////////////////
// ����������� ������ ������ ������������
//////////////////////////////////////////////////////////////////////////
#define MT_TaskSetPriority(idTask) \
do {\
	__MT_TaskPriority = idTask;\
	__MT_TaskPriorityMask = (MT_TYPE)1 << idTask;\
} while (0)
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// ���������� ������ ������������ ������
//////////////////////////////////////////////////////////////////////////
#define MT_TaskPriorityClr() \
do {\
	__MT_TaskPriorityMask = 0;\
} while (0)
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// �����������
//////////////////////////////////////////////////////////////////////////

#define MT_DISPATCH() \
do {\
	static uint8_t saved_task;\
	uint8_t task;\
	MT_TYPE mask;\
	\
	/*�������� ������� ����� ������������ ������ � ��������� ����������*/\
	mask = __MT_TaskPriorityMask;\
	\
	/*��������� ��� ���������� ������������ ������*/\
	if (__MT_TaskActiveFlags & mask)\
	{\
		task = __MT_TaskPriority;\
		if (__MT_Task[task] != NULL)\
		{\
			saved_task = __MT_TaskCur;\
			__MT_TaskCur = task;\
			__MT_TaskCurMask = mask;\
			\
			/*������ ������������ ������*/\
			__MT_Task[task](&__MT_Context[task]);\
			\
			__MT_TaskCur = saved_task;\
			__MT_TaskCurMask = (MT_TYPE)1 << __MT_TaskCur;\
		}\
	}\
	else\
	{\
		task = __MT_TaskCur;\
		mask = __MT_TaskCurMask;\
		\
		if (__MT_TaskActiveFlags & mask)\
		{\
			/*������ ��������� ������*/\
			if (__MT_Task[task] != NULL)\
				__MT_Task[task](&__MT_Context[task]);\
		}\
		task++;\
		mask <<= 1;\
		/*if (task >= __MT_TASK_COUNT) {task = 0; mask = (MT_TYPE)1 << 0;}*/\
		if (task >= __MT_TASK_COUNT) {task = 0; mask = 1;}\
		\
		__MT_TaskCur = task;\
		__MT_TaskCurMask = mask;\
	}\
	\
	MT_TIMEOUT_CALC(); /*������� ���������*/\
	\
} while (0)
//////////////////////////////////////////////////////////////////////////

#else // #if defined(MT_USE_HIGH_PRIORITY_TASK)

#define MT_DISPATCH() \
do {\
	uint8_t task;\
	MT_TYPE mask;\
	\
	task = __MT_TaskCur;\
	mask = __MT_TaskCurMask;\
	\
	if (__MT_TaskActiveFlags & mask)\
	{\
		if (__MT_Task[task] != NULL) __MT_Task[task](&__MT_Context[task]);\
	}\
	task++;\
	mask <<= 1;\
	\
	if (task >= __MT_TASK_COUNT) {task = 0; mask = 1;}\
	\
	__MT_TaskCur = task;\
	__MT_TaskCurMask = mask;\
	\
	MT_TIMEOUT_CALC(); /*������� ���������*/\
	\
} while (0)

#endif // #if defined(MT_USE_HIGH_PRIORITY_TASK)
//////////////////////////////////////////////////////////////////////////

#endif
