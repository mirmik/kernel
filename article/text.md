Как известно, каждый кодер втайне мечтает написать операционную систему.
На хабре полно статей на тему того, как написать ОС. Это одна из них. Идея статьи, впрочем не в том, чтобы написать еще один загрузчик, сколько в том, чтобы логически вывести необходимость тех или иных компонентов ОС, дать определённую ретроспективу того как вся эта мошна кода совместно работает и зачем она вообще нужна.

Большая проблема с которой сталкивается новоявленный автор очередной революционной ОС, когда начинает изучать код написанный сотней и тысячей его предшественников состоит в том, что изучая код операционных систем невозможно понять, где у них начало. Объекты сильно связаны и часто циклически зависимы друг от друга. В "Дорожной карте разработчика ОС" я хотел бы пройти этот путь последовательно таким образом, чтобы избежать блужданий в хороводах объектов ядра.

В качестве целевой платформы для разработки мы возьмём в достаточной степени абстрактный контроллер stm32 и иногда x86, но только в одноядерном режиме. В статьях не будет большого количества низкоуровневых деталей и практически не будет подробностей настройки системы сборки или программатора. Код, который будет приводится в качестве примера будет сильно упрощен по сравнению с реальными боевыми системами.

Автор предполагает, что вы знаете как работать с железом, представляете работу архитектуры фон неймана, аки папа умеете в си, знаете такие слова как диспетчер, драйвер, переферия процессора, прерывание, представляете как организацию памяти и не пугаетесь страшного слова "регистр". 

Данная статья не про то, как писать ОС, а про то, в каком порядке её возможно писать.

Поскольку на хабре есть довольно много неоконченных статей по теме разработки ОС, дабы не продолжать сию печальную традицию, я написал практически весь текст сразу, но поскольку его много, я планирую выкладывать его постепенно.

### Зачем процессору ОС?

Исторически сложившееся понимание архитектуры програмного обеспечения такого, что мы стараемся оформить программы в некоторые модули ограниченной функциональности. Принцип декомпозиции применяется как на уровне отдельных программ при дроблении их на объекты в рамках объектной парадигмы, так и на уровне всей вычислительной системы в целом. Операционные системы появились именно тогда, когда понадобилось разделять вычислительное время между независимыми или слабо зависящими друг от друга задачами.

Здесь требуется решить ряд задач. Очевидно необходимо научиться распределять вычислительное время и уметь передавать управление гостевому коду. Однако вычислительная система не заканчивается на АЛУ. Вычислительная система это также переферийное оборудование с которым несколько пользователей могут захотеть работать одномоментно и которое способно генерировать прерывания, обращающиеся к определённому набору програмных ресурсов в неопределённые моменты времени. Для того, чтобы недопустить конфликтов доступа, операционная система должна уметь правильно управлять програмными ресурсами и обеспечивать некоторые средства синхронизации доступа. Эти два пункта, а именно управление гостевым кодом и синхронизация - по сути являются главными задачами ОС.

Для того, чтобы корректно обработать вопрос синхронизации аппаратных ресурсов и програмных ресурсов самой ОС, разработчики операционных систем предпочитают давать пользователям достаточно узкий, ограниченный АПИ, работу с которым можно легко контролировать. Это выражается в наличии в ОС некоторого количества обобщенных интерфейсов к ресурсам системы, с которыми разрешено или же рекомендуется работать гостевому коду. Сам же гостевой код оформляется в виде независимых программ, обращающихся к этому АПИ. Помимо управления ресурсами, процессам также часто бывает необходимо взаимодействовать друг с другом, что также ложится на плечи модерирующей этот процесс ОС, что выражается в виде появления механизмов OPC, таких как пайпы (pipe), очереди сообщений и прочие, зависящие от организации системы методы.

Одним из крайних методов решения вопроса синхронизации в ядре ОС является архитектура микроядра, когда все взаимодействие внутри системы оформляется в виде сообщений, доставляемых через системную шину. В такой системе управление ресурсами зачастую вынесено вне ядра ОС и оформленно как отдельный процесс, что, впрочем, не умаляет его системной значимости. Фактически в микроядрерной архитектуре реализуются отдельные системные процессы, отвечающие за реализацию того или иного системного АПИ. 

Одним из типичных, но необязательных способов построения интерфеса к ресурсам является файловая система и апи файловых дескрипторов. Концепция файла, тоесть представления ресурсов в виде именнованного потока данных, оказалось поистине мощной и позволила построить действительно универсальные вычислительные системы, способные адаптироваться к широкому классу задач без необходимости регулярной перепрошивки. Это достигается благодаря тому, что файловое апи является универсальным програмным интерфейсом и объединяет не только отдельные программы и библиотеки, но и вычислительные машины в глобальной сети. Если система умеет работать с файлами, она умеет работать со всем, что может быть представлено в виде файлов.

Вопросы сетевого взаимодействия также полезно увязать с возможностями ОС, поскольку обработка сетевых пакетов требует установки того, какой гостевой процесс ответственнен за то или иное сетевое взаимодействие, что требует некоторой информации о процессах и умения доставлять им информацию в рамках того или иного системного АПИ.


### Как следует думать об ОС её разработчику. 

Теперь, когда вы прочитали типовое вступление из предыдущего параграфа, вы можете сразу его забыть. Поскольку ОС внутри - совсем не то, чем она кажется снаружи.

Если вы полагаете, что ядро операционной системы - это некая программа, которая выполняется на голом железе, переодически запускает пользовательский код и модерирует аппаратные прерывания, вы скорее всего не понимаете, как работает ОС. 

Правда в том, что ОС это не программа, а библиотека, которой программы-задачи пользуются для организации взаимодействия между собой. Операционная система не имеет собственного вычислительного времени и не может его иметь. Сама по себе операционная система не исполняется никогда. Всё вычислительное время системы распределено между процессами-задачами и обработчиками аппаратных прерываний. Нет такой ситуации, когда операционная система "берёт управления на себя" и выполняет какой-то ОС специфический алгоритм. Ближе всего к ситуации "исполнения кода ОС" мы подходим в двух случаях. 

Первый - начальная инициализация системы. Это момент, когда ОС еще только разворачивается и фактически еще не работает.

Второй - это диспетчеризация, когда некоторый модуль ядра ОС, именуемый диспетчером, каким-либо образом получая управления от пользовательского процесса или аппаратного прерывания решает, какую задачу ему следует пнуть следующей. Хотя можно говорить, что это тот самый момент, когда ОС получает управление, нам, как разработчикам ОС, не следует рассматривать эту ситуацию таким образом. Если мы учтём, каким образом диспетчер получил управление и какого состояние аппаратуры в этот момент, то обнаружим, что диспетчеризация более напоминает бесконечную перепасовку мяча между задачами, чем иерархически выстроенное командное взаимодействие. Царь и бог в системе тот, кому принадлежит вычислительное время и этим кем-то никогда не является ОС.

Поскольку ОС не может ничем управлять, чтобы ограничить произвол кода пользовательских процессов и обработчиков аппаратных прерываний, волюнтаристски захватывающих управление в непредсказуемые моменты времени, ОС устанавливает жёсткие правила о том, что можно а чего нельзя делать в системе. Правила эти могут оформляться просто как соглашения или, что более типично, контролироваться теми или иными аппаратными и програмными средствами. Когда процесс ОС Linux или Windows выполняет тот или иной системный вызов, он не передаёт никаким магическим образом управление ядру, которое, как мы помним, вообще не имеет собственного вычислительного времени. При использовании системного вызова, процесс остаётся в своём потоке исполнения, своём стеке и сохраняет свою идентичность. Однако, пройдя через бутылочное горлышко интерфейса системного вызова он оказывается сильно урезанным в своих деструктивных возможностях. В этот момент его права в системе повышаются и ему разрешается доступ к структурам данных ядра и регистрам аппаратуры, до тех пор, пока в процессе возврата он не покинет системный вызов. Это принцип инкапсуляции всеми любимого ООП. Вы можете работать с системой до тех пор, пока соблюдаете интерфейс.

Часто бывает, что процесс на долгое время останавливается, находясь в недрах системного вызова. Это означает, что процесс каким-либо образом, подписался на ожидание какого-то события, самопроизвольно перешёл в состояние остановки и добровольно передал управление, воспользовавшись установленными ядром средствами. В дальнейшем, когда какой-то иной процесс или обработчик используя установленные способы и правила будет обрабатывать возникновение события, он снимет ожидающий процесс из очереди и снова отметит его на исполнение. 

Понимание того, что ядро ничего не выполняет само, но только даёт возможность тем или иным задачам производить те или иные действия, важно для понимания того, как организовывать код ОС.

## Изоляция ядра. Архитектура с точки зрения ОС

С целью минимизации трудозатрат на поддержку кода операционной системы и увеличения её потециальной применимости операционные системы стараются делать максимально независимыми от конкретной вычислительной архитектуры. Разумеется, как и всегда в биологии, некоторые подвиды операционных систем грубо нарушают это правило. На слуху, например, ОС Колибри полностью написанная на ассемблере. И всё же, подавляющее большинство ограничивают архитектурно зависимый код тонким слоем аппаратной абстракции. Реализация этого слоя абстракции выполняется интерфейсами драйверов аппаратных устройств. С этой точки зрения драйвер устройства является адаптером конкретного аппаратного интерфейса к програмному интерфейсу, определенному в коде ядра. Однако, код драйвера зачастую несколько толще, чем непосредственно уровень абстракции над операциями с регистрами устройств, поскольку интерфейс ОС подчиняется в большей степени тому, чтобы быть удобным для гостевого кода, нежели для того, чтобы передать максимум функциональности аппаратного уровня. Драйвера в юникс подобных ОС зачастую используют довольно сложное поведение и используют примитивы синхронизации ядра с тем, чтобы управлять выполнением обращающихся к ним гостевых процессов с тем, чтобы вести себя в духе интерфейса драйвера.

Поскольку код драйверов отделён интерфейсами взаимодействия, нетрудно провести границу между ними и кодом ядра ОС.

Еще один важный участок, где ОС могла бы взаимодействовать с архитектурой лежит в области векторов прерываний. С точки зрения виртуальной вычислительной машины, создаваемой ОС, прерывания есть элемент хаоса. В коде программ, реализующих управления микроконтроллерами и связанными с ними системами обработчики прерываний часто реализуют непосредственно в обход каких либо механизмов ОС, что значительно повышает привязку системы к архитектуре. Разработчики операционных систем, стараясь по озвученным выше причинам изолировать ядро и прикладной код от архитектуры, предпочитают сводить активность векторов прерываний к работе некой централизованной системы, представленной неким системным АПИ. Это позволяет гибко и аппаратно-независимо устанавливать обработчики прерываний и централизованно контролировать все происходящие в системе события.

### Системные утилиты нулевого уровня.

Дорожную карту разработчика ОС мы начнём с реализации некоторых независимых и вспомогательных утилит ядра, без которых никакая жизнь не возможна. 

Как должно было стать понятно из лирического отступления в параграфе "Как следует думать об ОС её разработчику", разработка ядра значительно больше похожа на разработку библиотеки, чем на разработку программы, а потому не стоит удивляться, что те части ОС которые мы будем рассматривать не будут выглядеть так уж операционно системно. На самом деле все эти несложные утилиты и есть суть и содержание ОС.

### Глобальная блокировка прерываний.
Глобальная блокировка прерываний есть с одной стороны очень грубый, а с другой самый действенный и простой способ управления синхронизацией в ядре. Поскольку глобальная блокировка должна вызываться на минимально возможное время, чтобы не нарушать отзывчивости системы, предпочтительнее реализовывать её в виде инлайн кода.

```c
// kernel/irq.h
#ifndef KERNEL_IRQ_H
#define KERNEL_IRQ_H

#include <sys/cdefs.h>
#include <asm/irq.h>

__BEGIN_DECLS
// Глобальное запрещение прерываний
void irqs_enable();

// Глобальное разрешение прерываний
void irqs_disable();

// Глобальное запрещение прерываний с запоминанием текущего состояния
irqsave_t irqs_save();

// Востановление ранее сохранённого состояния глобального разрешения прерываний
void irqs_restore(irqsave_t save);

__END_DECLS

#endif
```

```c
// arch/arm/armv7-e/asm/irq.h
#ifndef ARCH_ARM_V7E_IRQ_H
#define ARCH_ARM_V7E_IRQ_H

// реализация kernel/irq.h для arm_v7e процессоров

#include <inttypes.h>

typedef unsigned int irqsave_t;

static inline void irqs_enable(void) {
	__asm__ __volatile__ (
		"cpsie i \n\t");

}

static inline irqsave_t irqs_save(void) {
	uint32_t r;
	__asm__ __volatile__ (
		"mrs %0, PRIMASK;\n\t"
		"cpsid i \n\t"
		: "=r"(r));
	return r;
}

static inline void irqs_restore(irqsave_t state) {
	__asm__ __volatile__ (
		"msr PRIMASK, %0;\n\t"
		:
		: "r"(state));
}

static inline void irqs_disable(void) {
	(void) irqs_save();
}

#endif
```

Здесь мы видем один из способов организации абстракции от архитектуры. Файл `kernel/irq.h`, являющийся частью ядра подключает файл `asm/irq.h`. Для каждой архитектуры может существовать свой `asm/irq.h`. Выбор того или иного файла реализации в данном случае будет осуществляться при передаче флага -I (путь к заголовочным файлам) компилятору при сборке ядра.

Файл `sys/cdefs.h` и макросы `__BEGIN_DECLS` и `__END_DECLS` являются частью стандарта posix и нужны на случай использования заголовочного файла в с++ коде.

```c
// sys/cdefs.h
#ifndef SYS_CDEFS_H
#define SYS_CDEFS_H

#ifndef __BEGIN_DECLS
#	ifdef __cplusplus
#		define __BEGIN_DECLS extern "C" {
#	else
#		define __BEGIN_DECLS
#	endif
#endif

#ifndef __END_DECLS
#	ifdef __cplusplus
#		define __END_DECLS }
#	else
#		define __END_DECLS
#	endif
#endif

#endif
```

Система управления глобальными прерываниями, вероятно, является самой простой, но тем не менее очень важной частью АПИ ядра. Эти методы будут часто применяться в нашей дальнейшей работе для защиты системных ресурсов от состояния гонки.

### Система диагностического вывода

// TODO: отладочный интерфейс для тавера.

Программирование невозможно без отладочных инструментов и одним из самых ценных инструментов разработчика, а в особенности системного разработчика, является отладочная печать. Поэтому, первое, что необходимо сделать - реализовать средства отладочной печати.

Данное АПИ мы позволим себе сразу же реализовать в виде достаточно мощной и способного к изменению во времени абстракции. Идея этой абстракции, как и многое другое в данной серии статей нагло упёрта из кода ОС embox. 

```c
#ifndef KERNEL_DIAG_H
#define KERNEL_DIAG_H

// интерфейс вывода отладочной информации

#include <sys/cdefs.h>

__BEGIN_DECLS

struct diag_ops 
{
	int (*putc)(void*, char);
	int (*write)(void*, const char*, int);
};

typedef int(*diag_putc_t)(void*, char);
typedef int(*diag_write_t)(void*, const char*, int);

// Установка реализации интерфейса отладочного вывода
// @ops - таблица функций
// @priv - приватная переменная
void diag_setup(const struct diag_ops* ops, void* priv);

// Операции с текущим установленным интерфейсом отладочного вывода.
int diag_putc(char c);
int diag_write(const char* buf, int len);

int diag_write_common(void* priv, const char* buf, int len);

__END_DECLS

#endif
```

```c
#include <diag/diag.h>
#include <stdlib.h> // for NULL

struct diag_ops* current_diag_ops  = NULL;
void*            current_diag_priv = NULL;

void diag_setup(struct diag_ops* ndiag, void* priv)
{
	current_diag_ops = ndiag;
	current_diag_priv = priv;
}

int diag_putc(char c)
{
	if (current_diag_ops == NULL) 
		return 0;
	
	return current_diag_ops->putc(current_diag_priv, c);
}

int diag_write(const char* buf, int len)
{
	if (current_diag_ops == NULL) 
		return 0;
	
	return current_diag_ops->write(current_diag_priv, buf, len);
}

int diag_write_common(void* priv, const char* buf, int len)
{
	int ret = 0;

	if (current_diag_ops == NULL) 
		return 0;
	
	while(len--) 
		ret += diag_putc((int)*buf++);
	
	return ret;
}
```

```c
#include <diag/diag.h>
#include <periph/map.h>

#include <asm/stm32_usart.h>

int stm32_diag_putchar(void* priv_usart, char c) 
{
	USART_TypeDef * usart = (USART_TypeDef *) priv_usart;
	
	stm32_usart_wait_cantx(usart);
	stm32_usart_putc(usart, c);
	stm32_usart_wait_cantx(usart);

	return 1;
}

static struct diag_ops __diag_operations = 
{
	.putc = stm32_diag_putchar,
	.write = diag_write_common
}

void stm32_diag_init(USART_TypeDef * regs) 
{
	diag_setup(&__diag_operations, regs);
}
```

```c
// arch/arm/stm32/asm/stm32_usart.h
//...
static inline
void stm32_usart_wait_cantx(USART_TypeDef * usart) 
{
	while (!(usart->ISR & (1 << 7)));
}

static inline
int stm32_usart_putc(USART_TypeDef * usart, char c)
{
	usart->TDR = c;
	return 1;
}
//...
```

Такая организация кода - типичный пример реализации интерфейса в языке си.
Функция `stm32_diag_init` инициализирует систему отладочного вывода, устанавливая таблицу операций и приватную переменную, являющуюся указателем на регистры аппаратного usart интерфейса. После её вызова функции diag_putc и diag_write начнут работать через указанный USART интерфейс. Разумеется, USART канал перед этим должен быть правильно инициализирован и настроен.

При необходимости метод вывода данных всегда можно изменить повторным вызовом `diag_setup`.

Интерфейс `diag` сам по себе не сильно функционален. Максимум, что он может - вывести строку на печать. Для работы с числами, указателями, дампами памяти хорошо разработать библиотеку вывода, использующую функции интерфейса diag (см. kernel/dprint.h).

### макросы BUG(), BUG_ON().

```c
#ifndef KERNEL_BUG_H
#define KERNEL_BUG_H

#include <kernel/dprint.h>
#include <kernel/irq.h>

#define BUG()					                                                      \
{                                                                                     \
	irqs_disable();                                                                   \
	debug_print("file: "); debug_print(location.file); debug_line();                  \
	debug_print("func: "); debug_print(location.func); debug_line();                  \
	debug_print("line: "); debug_printdec_unsigned_int(location.line); debug_line();  \
	while(1);                                                                         \
}

#define BUG_ON(eq)					                                                     \
{									                                                     \
	if (eq) {						                                                     \
	    irqs_disable();                                                                  \
		debug_print("file: "); debug_print(location.file); debug_line();                 \
		debug_print("func: "); debug_print(location.func); debug_line();                 \
		debug_print("line: "); debug_printdec_unsigned_int(location.line); debug_line(); \
		debug_print(#eq);			                                                     \
	    while(1);                                                                        \
	}								                                                     \
}

#endif
```

Такие макросы как BUG и BUG_ON очень полезны для отладки кода ядра. Их задача состоит в том,
чтобы при возникновении некоторых условий остановить ядро и вывести точку возниконвения ошибки на отладочную консоль. Любопытно то, что реализация такого простого поведения потребовала применения двух ранее созданных подсистем ядра. 
 
### Код инициализации архитектуры. API инициализации.

Хотя я уже несколько раз повторил, что ОС - это библиотека, от необходимости перваначльной настройки архитектуры нас никто не избавит.

Код базовой инициализации среды виртуальной машины, реализуемой языком c/c++, зависит от архитектуры. Для arm/stm32 этот код может быть реализован. прямо на самом си:

```c
#include <asm/irq.h>
#include <string.h>

typedef void (*func_ptr)(void); 

extern unsigned char _sdata;
extern unsigned char _edata;
extern unsigned char _sidata;

extern unsigned char _sbss;
extern unsigned char _ebss;

extern int main();

extern func_ptr __init_array_start[0], __init_array_end[0];
extern func_ptr __fini_array_start[0], __fini_array_end[0];

void cxx_invoke_init_array(void) {
	for ( func_ptr* func = __init_array_start; func != __init_array_end; func++ )	{
		(*func)();
	}
}

void cxx_invoke_fini_array(void) {
	for ( func_ptr* func = __fini_array_start; func != __fini_array_end; func++ )	{
		(*func)();
	}
}

void __start(void) 
{
	irqs_disable();

	/*data section copy*/
	memcpy(&_sdata, &_sidata, (unsigned int) (&_edata - &_sdata));
	
	/*bss section init*/
	unsigned int bss_size = &_ebss - &_sbss;
	memset(&_sbss, '\0', bss_size);

	/*global ctors invoke*/
	cxx_invoke_init_array();
	
	/*main programm start*/
	main();
	
	/*global dtors invoke*/
	cxx_invoke_fini_array();

	/*programm end stub*/
	irqs_disable();
	for( ; ; );
}
```  

После выполнения кода инициализации вызывается функция main (можно выбрать другое название), где необходимо произвести настройку переферии, руководствуясь особенностями архитектуры и отладочной платы.

```c
// main.c
#include <kernel/sysinit.h>

int main() 
{
	system_init();

	irqs_enable();
	
	while(1) 
	{
		// superloop
	}
}
```

```c
// board/nucleo/asm/sysinit.c
#include <asm/stm32_arch.h>


	#include <hal/board.h>
#include "curboard.h"

#include <drivers/gpio/gpio.h>
#include <systime/systime.h>

#include <periph/map.h>
#include <util/cpu_delay.h>

#include <asm/stm32_usart.h>
#include <asm/stm32_gpio.h>
#include <asm/stm32_diag.h>
#include <asm/stm32_pll.h>
#include <asm/stm32_systick.h>
#include <asm/stm32_clockbus.h>

gpio_pin board::sysled(SYSLED_GPIO, SYSLED_MASK);
genos::stm32_usart board::sysuart(SYSUART, SYSUART_IRQ);

void system_init() 
{
	arch_init();

	// sysled
	stm32_rcc_enable_gpio(SYSLED_GPIO);
	stm32_gpio_set_output(SYSLED_GPIO, SYSLED_MASK );
	stm32_gpio_set_level(SYSLED_GPIO, SYSLED_MASK, 1);

	// freq
	stm32_clockbus_freq[CLOCKBUS_HSE] = BOARD_HSE_FREQ;
	switch (freqmode)
	{
		case STM32_FREQMODE_HSI: //HSI mode
			stm32_clockbus_hsi_mode();
			break;

		case STM32_FREQMODE_HSE: 
			stm32_clockbus_hse_mode();
			break;
	}
	stm32_clockbus_reevaluate();
	stm32_clockbus_systime_setup();

	/// sysuart
	stm32_rcc_enable_gpio(SYSUART_RX_GPIO);
	stm32_rcc_enable_gpio(SYSUART_TX_GPIO);
	stm32_rcc_enable_usart(SYSUART);

	gpio_settings(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_alternate(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), SYSUART_RX_AF);
	
	gpio_settings(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_alternate(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), SYSUART_TX_AF);

	board::sysuart.setup(115200, 'n', 8, 1);
	stm32_diag_init(SYSUART);
}
}
```


### Как отлаживаться, если нет ни одного отладочного инструмента.

### irqtable Таблица прерываний ядра ОС.

Теперь мы наконец-то можем посмотреть в сторону таблицы прерываний.

```c
#ifndef KERNEL_IRQTABLE_H
#define KERNEL_IRQTABLE_H

/*
	Комутатор прерываний. 
	Если архитектура использует irqtable, все вызовы прерываний перенаправляются в эту подсистему.
	Целью irqtable является унификация работы с прерываниями и упрощение их отслеживания и отладки.
*/

#include <sys/cdefs.h>

typedef void(*irq_handler_t)(void*);

/// Каждая запись таблицы хранит информацию об одном из прерываний.
struct irq_record {
	irq_handler_t handler;   // указатель на обработчик
	void* handler_arg;       // приватный аргумент обработчика
	volatile uint16_t count; // счётчик вызовов
};
	
__BEGIN_DECLS

// Возвращает 1, если управление внутри обработчика irqtable, иначе 0.
unsigned char is_interrupt_context();

// Инициализировать систему (необходимо выполнить до возникновения первого прерывания). 
void irqtable_init();

// Установить обработчик. Номера прерываний должны подключаться в asm/irqdefs.h
void irqtable_set_handler(int irqno, irq_handler_t handler, void* arg);

// Обработчик заглушка для отлова неинициализированных прерываний. 
void irqtable_stub(void* irqno);

// Вывод на отладочную консоль информации о состоянии таблицы прерываний.
void irqtable_debug_print();

__END_DECLS

#endif
```

```c
#include <kernel/irqtable.h>
#include <kernel/dprint.h>

#include <asm/irqdefs.h>

// счётчик входов в контекст прерывания
volatile unsigned char __interrupt_context = 0;

// Длина таблицы определяется в asm/irqdefs.h
struct irq_record __irqtable[NR_IRQS];

void do_irq(uint8_t irq)
{
	__interrupt_context++;
	__irqtable[irq].count++;
	__irqtable[irq].handler(__irqtable[irq].handler_arg);
	__interrupt_context--;
}

unsigned char is_interrupt_context()
{
	return __interrupt_context;
}

void irqtable_stub(void* irqno)
{
	irqs_disable();
	dpr("IRQ_STUB_");
	dprln((uintptr_t)irqno);
	
	while (1);
}

void irqtable_init()
{
	for (int i = 0; i < NR_IRQS; ++i)
	{
		__irqtable[i].handler = &irqtable_stub;
		__irqtable[i].handler_arg = (void*)i;
		__irqtable[i].count = 0;
	}
}

void irqtable_set_handler(int irqno, irq_handler_t handler, void* handler_arg)
{
	irqno = IRQTABLE_IRQNO(irqno);
	__irqtable[irqno].handler = handler;
	__irqtable[irqno].handler_arg = handler_arg;
}

void irqtable_debug_print() 
{
	dprln("__irqtable:");
	for (int i = 0; i < NR_IRQS; ++i)
	{
		dpr(i); 
		dpr(": "); 
		dprptr((void*)__irqtable[i].handler);
		dpr(" ");
		dprptr(__irqtable[i].handler_arg);
		dpr(" ");
		dprln(__irqtable[i].count);		
	}	
}
```

Магия таблицы прерываний состоит в функции do_irq. do_irq вызывается из ассемблерного кода настоящих (системных) обработчиков прерываний.
Если каждое прерывание в системе будет вызывать эту функцию с заранее определённым индексом, то коммутатор корректно перенаправит прерывание на необходимый обработчик.

Реализацию подобного перевода стрелок для stm32 можно посмотреть в файле `arch/arm/stm32/asm/stm32_vectors.S`. Разумеется, секция .vectors должна располагаться на правильном месте в скомпилированном бинарном файле.

### А почему собственно си? 



### Система времени ядра.

Одной из важных функций ядра является учёт времени для диспетчеризации процессов и донесения этой информации до гостевого кода.

Обычно счетчик системного времени осуществляется на основе прерывания системных часов.

```c
// kernel/systime.h
#ifndef KERNEL_SYSTIME_H
#define KERNEL_SYSTIME_H

#include <inttypes.h>
#include <sys/cdefs.h>

#if __NOARCH
#else
#include <asm/delay.h> // Аппаратная поддержка малотактных задержек.
#endif

typedef int64_t jiffies_t;

// счётчик количества аппаратных тиков системного таймера
extern volatile jiffies_t __jiffies;

__BEGIN_DECLS

// Частота прерывания таймер
void systime_set_frequency(uint32_t freq);
uint32_t systime_frequency();

// Данная функция должна вызываться в прерывании системного таймера
static inline void system_tick()
{
	++__jiffies;
}

jiffies_t jiffies();
jiffies_t millis();
jiffies_t micros();

void delay(double d);
void delayMicroseconds(uint32_t us);

__END_DECLS

#endif
```

Интерфейс этой подсистемы реализовани на основе кода библиотеки Ардуино. Реализацию функций можно увидеть в "".

Подсистема systime позволяет вычислять текущее время, что в последствии позволит нам реализовывать работу с таймерами ядра.


### Структуры данных ядра. Связный список.

Ядро операционной системы должно следить за довольно большим множеством объектов, которые необходимо как-то организовывать в пространстве памяти.

Пожалуй, самая главная структура данных в ядре типовой ОС - это связный список. 
Связные списки ядра linux, embox и прочих подобных систем выполнены по intrusive технологии.

Это значит, что ноды списка содержатся непосредственно в связываемых объектах. 

Выглядит работа с такими списками примерно следующим образом:

```c
// голова списка
struct dlist_head list_of_A_objects = DLIST_HEAD_INIT(list_of_A_objects);

struct A 
{
	struct dlist_head lnk; // поле списка за которое объекты цепляются к голове
	                      // и друг другу.
	int data;
};

int foo() 
{
	A a, b;
	a.data = 1;
	b.data = 1;

	dlist_add(&a, &list_of_A_objects); // добавление объекта в список
	dlist_add(&b, &list_of_A_objects); // добавление объекта в список

	A * it;
	dlist_for_each_entry(it, &list_of_A_objects, lnk) // макрос, реализующий обход списка
	{
		dprln(it->data);
	}
}
```

Подробно ознакомится кодом связных списков можно в файле `kernel/dlist.h`.

### Таймер.
Маленьким, но очень важным объектом является структура, отвечающая за сравнение временных меток.
В дальнейшем на базе этой структуры будет выстроена система таймеров ядра.

### Для чего нужны диспетчер задач и диспетчер таймеров

### Прото ОС. Флаговый автомат с таймерами.
В классической статье "Архитектура Программ" (за авторством камрада @dihalt) введены некоторые типы организации кода, на уровне предшествующем ОС. 

Пропустив в этой классификации суперцикл и суперцикл с прерываниями, перейдём сразу к флаговому автомату как вырожденному примеру диспетчера задач. Идея флагового автомата.

###

Для того, чтобы не связываться со сложными ОС, но получить при этом преимущества диспетчеризации кода микроконтроллерщики любят применять структуру алгоритма, именуемую "флаговый автомат".

Флаговый автомат представляет собой


### Очереди на связных списках. Системный таймер.

Недостаток флагового автомата состоит в том, что его поведение жёстко вшито в код, что ограничивает его гибкость и масштабируемость. 

Нетрудно догадаться, что если мы начнём оформлять задачи в виде объекта данных, а не ветки кода, то наши возможности наши существенно увеличаться. Известно, что подавляющее большинство времени задачи ждут некоторых событий, прихода определённого времени или событий ввода-вывода. Паттерн объекта ожидающего исполнения настолько распространён, что для нашего маленького очень удобно создать такой объект в коде в явном виде. 

```c
// kernel/ctrobj.h

struct ctrobj 
{
	struct dlist_head * lnk;
	void(* exec)(void* obj);
};

static inline 
void ctrobj_execute(struct ctrobj * obj) 
{
	obj->exec(obj);
}
``` 

Объект состоит из ноды списка за которую он будет подцепляться в любую интересующую его очередь и ожидать там своего исполнения. Сам по себе этот объект бесполезен и интересен только как часть чего-то большего, например системного таймера ядра ОС.

```c
struct ktimer_base
{
	struct ctrobj ctr;
	struct timer_head tim; 
};

struct ktimer
{ 
	struct ktimer_base ktim;
	void(* func)(void* obj);
	void * arg;
}

void ctrobj_ktimer_execute(void* obj) 
{
	struct ktimer * ktim = (struct ktimer *) obj;

	dlist_del_init(&ktim->ktim->ctr->lnk);
	ktim->func(arg); 	
}

static inline 
void ktimer_base_init() 
{
	ctrobj_init(&ktim->ctr, exec);
}
```

Здесь определено два типа таймеров.

### Диспетчер таймеров


```c
void ktimer_manager_step()
{
	clock_t now;

	now = jiffies();

	system_lock();

	while (!dlist_empty(&ktimer_list))
	{
		struct ktimer_base * it = dlist_first_entry(&ktimer_list, struct ktimer_base, ctr.lnk);
		system_unlock();

		if (ktimer_check(it, now))
		{
			ktimer_execute(it);
		}
		else return;

		system_lock();
	}

	system_unlock();
}
```

### Пример работы с диспетчером таймеров
```

```

### Единица исполнения. Диспетчер процессов.

### Атомарный процесс. Тасклет.

### Контекст. Кооперативный процесс.

### Вытесняемый процесс.

### API sleep.

### API waiter.

### Примитивы блокировки ядра.
	примитивы без очереди, спинлок
	примитивы с очередью, семафор

### С чем едят микроядро?

### Реальное и нереальное время.

### Драйвер uartring. Драйвера устройств.

### Таблица файловых дескрипторов. Система ресурсов.

### Терминал. Системная консоль. Системные утилиты.

### pipe

### Конвеер процессов.

### Очереди сообщений.

### Управление памятью в ядре. Объектный пул.

### Файловая система. Пространства имён.

### Драйвера шин. regmap.

### Общие замечания касательно сетевого стека.

### Оконный диспетчер.

Здесь мы завершаем нашу дорожную карту разработчика ОС. Несложно догадаться, что современные операционные системы значительно разнообразнее и масштабнее, чем тот небольшой срез, который мы рассмотрели в этом обзоре. Задачей статьи было показать примерный порядок действий и те границы, по которым следует дробить код ОС, чтобы получить систему, которую можно конструировать последовательно и постепенно без необходимости постоянно переписывать заного какие-либо компоненты ядра.

Надеюсь мне удалось убедить вас, что самое главное в дизайне ОС - это её декомпозиция. Если декомпозиция ОС выполнена верно, работа с ней будет доставлять одно удовольствие. В противном случае вы будете погребены под клубками системных объектов.

Конкретная форма объектов и отдельные особенности организации кода в приведенных примерах не важны. На самом деле, хотя и кажется, что многообразие возможных вариантов невообразимо велико, на практике, вид типовой ОС продиктован конкретными инженерными соображениями и логически выводится из архитектуры фон неймана. Многообразие вариаций есть лишь формы одного явления.   
