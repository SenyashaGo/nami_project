#pragma once
#include <fstream>
#include <string>
#include "COutputChannel.h"
#include <vector>
#include "CStruk.h"
#include "CStruk_input.h"
#include <stdlib.h>
#include "CSettingsChannel.h"
using namespace std;
class CGenerating_a_report_file
{
public:
	string str;
	CGenerating_a_report_file(vector < COutputChannel>& Struk_book, vector<CSettingsChannel>& book_Struk_input, string s)
	{
		ofstream file("D:\\" + s + ".txt");
		for (int i = 0; i < Struk_book.size(); i++)
		{
			if (Struk_book[i].cheak_msg && Struk_book[i].cheak_periodicity && Struk_book[i].cheak_id)
			{
				str += "Порт " + to_string(Struk_book[i].nubmer_port) + " успешно получил ID " + to_string(Struk_book[i].id) + "\n";
			}
			else if (Struk_book[i].id==-1)
			{
				str += "Ошибка! Порт " + to_string(Struk_book[i].nubmer_port) + " не получил id\n";
			}
			else
			{
				if (!Struk_book[i].cheak_msg)
				{
					str += "Ошибка! Полученное сообщение на порт " + to_string(Struk_book[i].nubmer_port) + " не совпадает с заданным\n";
					str += "Заданое сообщение - ";
					for (int z = 0; z < book_Struk_input.size(); z++)
					{
						if (book_Struk_input[z].id == Struk_book[i].id)
						{

							for (int j = 0; j < 8; j++)
							{
								str += book_Struk_input[z].msg[j] + " ";
							}
							break;
						}
					}
					str += "; полученное - ";
					for (int j = 0; j < 8; j++)
					{
						str += Struk_book[i].msg[j] + " ";
					}
					str += "\n";
				}
				if (!Struk_book[i].cheak_periodicity)
				{
					str += "Ошибка! Частота получения сообщения на порту " + to_string(Struk_book[i].nubmer_port) + " не совпадает с заданным\n";
					for (int z = 0; z < book_Struk_input.size(); z++)
					{
						if (book_Struk_input[z].id == Struk_book[i].id)
						{
							str += "Заданный периуд - " + to_string(book_Struk_input[z].periodicity) + " полученный - " + to_string(Struk_book[i].periodicity) + "\n";
							break;
						}
					}
				}
			}
			for (int z = 0; z < 2; z++)
			{
				str += "\n";
			}
		}
		file << str;
		file.close();
	}
};