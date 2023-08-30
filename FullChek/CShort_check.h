#pragma once
#include <iostream>
#include <vector>
#include "COutputChannel.h"
#include "CSettingsChannel.h"
using namespace std;
class СShort_check
{
public:
	СShort_check(vector<COutputChannel>& book2, vector<CSettingsChannel>& book)
	{
		char str[8]="";
		for (int i = 0; i < book.size(); i++)
		{
			if (!book[i].type_id)
			{
				for (int j = 0; j < book2.size(); j++)
				{
					if (book[i].id == book2[j].id && book2[j].cheak_id)
					{
						for (int z = 0; z < 8; z++)
						{
							if (book[i].msg[z] != book2[j].msg[z])
							{
								book2[j].cheak_msg = false;
								printf("Ошибка! Полученное сообщение не совпадает с заданным. Заданное сообщение - %u %u %u %u %u %u %u %u, полученное сообщение - %u %u %u %u %u %u %u %u\n", book[i].msg[0],
									book[i].msg[1], book[i].msg[2], book[i].msg[3], book[i].msg[4], book[i].msg[5], book[i].msg[6], book[i].msg[7], book2[j].msg[0],
									book2[j].msg[1], book2[j].msg[2], book2[j].msg[3], book2[j].msg[4], book2[j].msg[5], book2[j].msg[6], book2[j].msg[7]);
								break;
							}
							else
							{
								book2[j].cheak_msg = true;
							}
						}
						if (book2[j].periodicity<int(book[i].periodicity * 1.1) && book2[j].periodicity>int(book[i].periodicity * 0.9))
						{
							book2[j].cheak_periodicity = true;
						}
						else
						{
							printf("Ошибка при получении id- %i, не совпадает время. Заданное значение - %i, полученное - %i\n", book[i].id, book[i].periodicity, book2[j].periodicity);
							book2[j].cheak_periodicity = false;
						}
						break;
					}
					else if (book[i].id == book2[j].id && !book2[j].cheak_id && !book[i].type_id)
					{
						printf("Ошибка! Порт %d получил сообщение c порта %d. Полученный id - %d\n", book2[j].nubmer_port, book[i].nubmer_port, book[i].id);
					}
					else if (book2[j].id == -1)
					{
						printf("Ошибка! Порт - %d не получил id\n", book[i].nubmer_port);
					}
				}
			}
		}
	}
};

