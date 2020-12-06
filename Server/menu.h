#ifndef MENU_H
#define MENU_H

#include <cstdint>

using std::uint32_t;

void menu(void (*ptrMenu)(uint32_t), uint32_t RangeDown, uint32_t& count);
void getServerMenu(uint32_t count);
void getAccountMenu(uint32_t count);
void getTechBaseMenu(uint32_t count);
void getTechMenu(uint32_t count);
void getTypeMenu(uint32_t count);

#endif // MENU_H
