// Прибиваем артефакты, на случай если компилятор 
// сильно пытается впендюрить функции из newlib в бинарник.

int _write()
{
	return -1;
}

int _read()
{
	return -1;
}

int _close()
{
	return -1;
}

int _fstat()
{
	return -1;
}

int _isatty()
{
	return -1;
}

int _lseek()
{
	return -1;
}

int _getpid()
{
	return -1;
}

int _kill()
{
	return -1;
}

int _exit()
{
	return -1;
}

void *_sbrk(int incr)
{
	return 0;
}

