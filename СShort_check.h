#pragma once
#include <iostream>
#include <vector>
#include "CStruk.h"
#include "CStruk_input.h"
using namespace std;
class СShort_check
{
public:
	СShort_check(vector<CStruk>& book2, vector<CStruk_input>& book)
	{
		bool idi;
		CStruk a;
		for (int i = 0; i < book.size(); i++)
		{
			idi = false;
			for (int j = 0; j < book2.size(); j++)
			{
				if (book[i].id == book2[j].id && book2[j].port_ind && !book[i].otpravka_ind)
				{
					idi = true;
					for (int z = 0; z < 8; z++)
					{
						if (book[i].msg[z] != book2[j].msg[z])
						{
							book2[j].msg_ind = false;
							printf("Ошибка! Полученное сообщение не совпадает с заданным. Заданное сообщение - %u %u %u %u %u %u %u %u %u, полученное сообщение - %u %u %u %u %u %u %u %u %u\n", book[i].msg[0],
								book[i].msg[1], book[i].msg[2], book[i].msg[3], book[i].msg[4], book[i].msg[5], book[i].msg[6], book[i].msg[7], book2[i].msg[0],
								book2[i].msg[1], book2[i].msg[2], book2[i].msg[3], book2[i].msg[4], book2[i].msg[5], book2[i].msg[6], book2[i].msg[7]);
							break;
						}
						else
						{
							book2[j].msg_ind = true;
						}
					}
					if (book2[j].periodicity<book[i].periodicity + 10 && book2[j].periodicity>book[i].periodicity - 10)
					{
						book2[j].periodicity_ind = true;
					}
					else
					{
						printf("Ошибка при получении id- %d, не совпадает время. Заданное значение- %f, полученное- %f\n", book[i].id, book[i].periodicity, book2[j].periodicity);
					}
				}
				else if (book[i].id == book2[j].id && !book2[j].port_ind && !book[i].otpravka_ind)
				{
					printf("Ошибка! Порт %d получил сообщение c порта %d. Полученный id- %d\n", book2[j].port, book[i].port, book[i].id);
				}
			}
			if (!idi)
			{
				printf("Ошибка! id- %d не найден\n", book[i].id);
				a.id = book[i].id;
				a.port = -1;
				a.periodicity = -1;
				for (int j = 0; j < 8; j++)
				{
					a.msg[i] = '-1';
				}
				a.msg_ind = false;
				a.periodicity_ind = false;
				a.port_ind = false;
				book2.push_back(a);
			}
		}
	}
};

