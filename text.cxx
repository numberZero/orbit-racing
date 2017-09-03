#include "text.hxx"
#include <cstdarg>
#include <cstdio>
#include <GL/gl.h>
#include <GL/glut.h>

void vglTextOut(double x, double y, double size, double thickness, char const *text)
{
	glLineWidth(thickness);
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glScaled(size/150.0, size/150.0, 0.0);
	for(; *text; ++text)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *text);
	glPopMatrix();
}

void vglTextOutF(double x, double y, double size, double thickness, char const *text, ...)
{
	static int const BUF_SIZE = 1024;
	char buf[BUF_SIZE];
	va_list list;
	va_start(list, text);
	vsnprintf(buf, BUF_SIZE, text, list);
	va_end(list);
	vglTextOut(x, y, size, thickness, buf);
}

void vglBeginPrint(double x, double y, double size, double thickness)
{
	glLineWidth(thickness);
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glScaled(size/150.0, size/150.0, 0.0);
	glTranslated(50.0, -150.0, 0.0);
	glPushMatrix();
}

void vglEndPrint()
{
	glPopMatrix();
	glPopMatrix();
}

void vglWrite(const char* text)
{
	for(; *text; ++text)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *text);
}

void vglWrite(const char* text, va_list list)
{
	static int const BUF_SIZE = 1024;
	char buf[BUF_SIZE];
	vsnprintf(buf, BUF_SIZE, text, list);
	vglWrite(buf);
}

void vglWrite(const char* text, ...)
{
	va_list list;
	va_start(list, text);
	vglWrite(text, list);
	va_end(list);
}

void vglWriteLn(const char* text, ...)
{
	va_list list;
	va_start(list, text);
	vglWrite(text, list);
	va_end(list);
	glPopMatrix();
	glTranslated(0.0, -150.0, 0.0);
	glPushMatrix();
}
