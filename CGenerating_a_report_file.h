#pragma once
#include <fstream>
#include <string>
#include "COutputChannel.h"
#include <vector>
#include "CStruk.h"
#include "CStruk_input.h"
#include <stdlib.h>
using namespace std;
class CGenerating_a_report_file
{
public:
	string str;
	CGenerating_a_report_file(vector < COutputChannel>& Output_book, vector<CStruk>& Struk_book, vector<CStruk_input>& book_Struk_input, string s)
	{
		ofstream file("D:\\" + s + ".txt");
		for (int i = 0; i < Output_book.size(); i++)
		{
			str += "���� " + to_string(Output_book[i].nubmer_port) + "\n";
			if (Output_book[i].cheak_periodicity && Output_book[i].cheak_id && Output_book[i].cheak_msg)
			{
				str += "���� " + to_string(Output_book[i].nubmer_port) + " ������� ������� ID " + to_string(Output_book[i].id) + "\n";
			}
			else
			{
				if (!Output_book[i].cheak_periodicity)
				{
					str += "������! ������� ��������� ��������� �� ����� " + to_string(Output_book[i].nubmer_port) + " �� ��������� � ��������. ID " + to_string(Output_book[i].id) + "\n";
					for (int z = 0; z < book_Struk_input.size(); z++)
					{
						if (book_Struk_input[z].id == Output_book[i].id)
						{

							str += "�������� ������ - " + to_string(book_Struk_input[i].periodicity) + " ���������� - " + to_string(Output_book[i].periodicity) + "\n";
							break;
						}
					}
				}
				if (!Output_book[i].cheak_msg)
				{
					str += "������! ��������� ��������� �� ����� " + to_string(Output_book[i].nubmer_port) + " �� ��������� � ��������. ID " + to_string(Output_book[i].id) + "\n";
					str += "������� ��������� - ";
					for (int z = 0; z < book_Struk_input.size(); z++)
					{
						if (book_Struk_input[z].id == Output_book[i].id)
						{

							for (int j = 0; j < 8; j++)
							{
								str += book_Struk_input[z].msg[j] + " ";
							}
							break;
						}
					}
					str += "; ���������� - ";
					for (int j = 0; j < 8; j++)
					{
						str += Output_book[i].msg[j] + " ";
					}
					str += "\n";
				}
				if (!Output_book[i].cheak_id)
				{
					str += "������! ���� " + to_string(Output_book[i].nubmer_port) + " �� ����� ID\n";
				}
			}
		}
		for (int i = 0; i < Struk_book.size(); i++)
		{
			if (Struk_book[i].msg_ind && Struk_book[i].periodicity_ind && Struk_book[i].port_ind)
			{
				str += "���� " + to_string(Struk_book[i].port) + " ������� ������� ID " + to_string(Struk_book[i].id) + "\n";
			}
			else if (Struk_book[i].port == -1)
			{
				str += "������! ID " + to_string(Struk_book[i].id) + " �� ������\n";
			}
			else
			{
				if (!Struk_book[i].msg_ind)
				{
					str += "������! ���������� ��������� �� ���� " + to_string(Struk_book[i].port) + " �� ��������� � ��������\n";
					str += "������� ��������� - ";
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
					str += "; ���������� - ";
					for (int j = 0; j < 8; j++)
					{
						str += Struk_book[i].msg[j] + " ";
					}
					str += "\n";
				}
				if (!Struk_book[i].periodicity_ind)
				{
					str += "������! ������� ��������� ��������� �� ����� " + to_string(Struk_book[i].port) + " �� ��������� � ��������\n";
					for (int z = 0; z < book_Struk_input.size(); z++)
					{
						if (book_Struk_input[z].id == Struk_book[i].id)
						{
							str += "�������� ������ - " + to_string(book_Struk_input[z].periodicity) + " ���������� - " + to_string(Struk_book[i].periodicity) + "\n";
							break;
						}
					}
				}
			}
		}
	}
};