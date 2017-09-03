#pragma once

void vglTextOut(double x, double y, double size, double thickness, char const *text);
void vglTextOutF(double x, double y, double size, double thickness, char const *text, ...);

void vglBeginPrint(double x, double y, double size, double thickness);
void vglEndPrint();
void vglWriteLn(char const *text, ...);
