#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "student.h"
#include <inttypes.h>
//#include "commands_key.h"

#define CMD_INCORRECT_COMMAND 99
#define CMD_EXIT_COMMAND 0
#define CMD_WRITE_COMMAND 1
#define CMD_READ_FILE_COMMAND 2
#define CMD_WRITE_ALL_COMMAND 3
#define CMD_WRITE_ALL_KEYS_COMMAND 4
#define CMD_WRITE_SPEC_COMMAND 5
#define CMD_ADD_STUDENT_COMMAND	6
#define CMD_DEL_STUDENT_COMMAND 7
#define CMD_SORT_COMMAND 8
#define CMD_SORT_KEY_COMMAND 9
#define CMD_MEASURE_TIME_COMMAND 10

#define ERROR_COMMANDS 100
#define ERROR_READ_FILE 200

#define RESET   "\033[0m"
#define GREEN	"\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[0;33m"
#define BLUE 	"\033[0;36m"
#define BLACK	"\033[0;30;47m"


// io module commands

void print_title(void)
{
	printf("1.  Вывести все команды.\n");
	printf("2.  Считать данные из файла.\n");
	printf("3.  Вывести таблицу всех студентов.\n");
	printf("4.  Вывести таблицу всех студентов по ключам.\n");
	printf("5.  Поиск студентов по полям.\n");
	printf("6.  Добавить студента.\n");
	printf("7.  Удалить студента.\n");
	printf("8.  Сортировка студентов.\n");
	printf("9.  Сортировка студентов по ключу.\n");
	printf("10. Измерить время сортировок.\n");
	printf("0.  Выйти из программы.\n");
}

void print_mini_title(void)
{
	printf("1. Вывести все команды.\n");
	printf("0. Выйти из программы.\n");
}

#define DIGITS "0123456789"
int is_correct_command(char *str)
{
	for (size_t i = 0; str[i]; i++)
		if (!strchr(DIGITS, str[i]))
			return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int is_correct_name(char *str)
{
	for (size_t i = 0; str[i]; i++)
		if (strchr(DIGITS, str[i]))
			return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

type_t get_command(int flag) // flag: >1 - none title, 0 - mini title, 1 - full title
{
	if (flag == 0)
		print_mini_title();
	else if (flag == 1)
		print_title();

	char str[10];

	printf("%sВведите команду:%s ", BLUE, RESET);
	gets(str);
	printf("\n");

	if (strlen(str) > 5)
		return CMD_INCORRECT_COMMAND;

	if (!is_correct_command(str))
	{
		int cmd = atoi(str);

		switch (cmd)
		{
			case CMD_WRITE_COMMAND:
				return CMD_WRITE_COMMAND;
			case CMD_EXIT_COMMAND:
				return CMD_EXIT_COMMAND;
			case CMD_READ_FILE_COMMAND:
				return CMD_READ_FILE_COMMAND;
			case CMD_WRITE_ALL_COMMAND:
				return CMD_WRITE_ALL_COMMAND;
			case CMD_WRITE_ALL_KEYS_COMMAND:
				return CMD_WRITE_ALL_KEYS_COMMAND;
			case CMD_WRITE_SPEC_COMMAND:
				return CMD_WRITE_SPEC_COMMAND;
			case CMD_ADD_STUDENT_COMMAND:
				return CMD_ADD_STUDENT_COMMAND;
			case CMD_DEL_STUDENT_COMMAND:
				return CMD_DEL_STUDENT_COMMAND;
			case CMD_SORT_COMMAND:
				return CMD_SORT_COMMAND;
			case CMD_SORT_KEY_COMMAND:
				return CMD_SORT_KEY_COMMAND;
			case CMD_MEASURE_TIME_COMMAND:
				return CMD_MEASURE_TIME_COMMAND;
			default:
				return CMD_INCORRECT_COMMAND;
		}
	}
	else
		return CMD_INCORRECT_COMMAND;
}

// io module input students;

int get_students(FILE *f, student_t *students, size_t *count)
{
	int i = 0;
	while (!feof(f))
	{
		char buf_str[200];
		student_t buf_student;
		int buf_int;
		fgets(buf_str, 200, f);
		char *istr;

		istr = strtok(buf_str, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (strlen(istr) >= 50)
			return EXIT_FAILURE;
		if (is_correct_name(istr))
			return EXIT_FAILURE;
		strcpy(buf_student.name.first_name, istr);

		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (strlen(istr) >= 50)
			return EXIT_FAILURE;
		if (is_correct_name(istr))
			return EXIT_FAILURE;
		strcpy(buf_student.name.last_name, istr);

		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (strlen(istr) >= 10)
			return EXIT_FAILURE;
		strcpy(buf_student.group, istr);

		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (is_correct_command(istr))
			return EXIT_FAILURE;
		buf_int = atoi(istr);
		if (buf_int < 0 || buf_int > 1)
			return EXIT_FAILURE;
		buf_student.gender = buf_int;

		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (is_correct_command(istr))
			return EXIT_FAILURE;
		buf_int = atoi(istr);
		if (buf_int <= 0 || buf_int > 100)
			return EXIT_FAILURE;
		buf_student.age = buf_int;


		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		buf_student.rating = atof(istr);

		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (is_correct_command(istr))
			return EXIT_FAILURE;
		buf_int = atoi(istr);
		if (buf_int <= 0 || buf_int > 31)
			return EXIT_FAILURE;
		buf_student.date.day = buf_int;

		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (is_correct_command(istr))
			return EXIT_FAILURE;
		buf_int = atoi(istr);
		if (buf_int <= 0 || buf_int > 12)
			return EXIT_FAILURE;
		buf_student.date.month = buf_int;

		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (is_correct_command(istr))
			return EXIT_FAILURE;
		buf_int = atoi(istr);
		if (buf_int <= 0 || buf_int > 2021)
			return EXIT_FAILURE;
		buf_student.date.year = buf_int;

		istr = strtok(NULL, ";");
		if (!istr)
			return EXIT_FAILURE;
		if (is_correct_command(istr))
			return EXIT_FAILURE;
		buf_int = atoi(istr);
		if (buf_int < 0 || buf_int > 1)
			return EXIT_FAILURE;
		buf_student.type_adress = buf_int;

		if (buf_student.type_adress == 0)
		{
			istr = strtok(NULL, ";");
			if (!istr)
				return EXIT_FAILURE;
			if (is_correct_command(istr))
				return EXIT_FAILURE;
			buf_int = atoi(istr);
			if (buf_int <= 0)
				return EXIT_FAILURE;
			buf_student.adress.dormitory.number = buf_int;

			istr = strtok(NULL, ";");
			if (!istr)
				return EXIT_FAILURE;
			if (is_correct_command(istr))
				return EXIT_FAILURE;
			buf_int = atoi(istr);
			if (buf_int <= 0)
				return EXIT_FAILURE;
			buf_student.adress.dormitory.room = buf_int;
		}
		else if (buf_student.type_adress == 1)
		{
			istr = strtok(NULL, ";");
			if (!istr)
				return EXIT_FAILURE;
			if (strlen(istr) >= 50)
				return EXIT_FAILURE;
			strcpy(buf_student.adress.house.street, istr);

			istr = strtok(NULL, ";");
			if (!istr)
				return EXIT_FAILURE;
			if (is_correct_command(istr))
				return EXIT_FAILURE;
			buf_int = atoi(istr);
			if (buf_int <= 0)
				return EXIT_FAILURE;
			buf_student.adress.house.number = buf_int;

			istr = strtok(NULL, ";");
			if (!istr)
				return EXIT_FAILURE;
			if (is_correct_command(istr))
				return EXIT_FAILURE;
			buf_int = atoi(istr);
			if (buf_int <= 0)
				return EXIT_FAILURE;
			buf_student.adress.house.apart = buf_int;
		}

		students[i] = buf_student;
		i += 1;
	}
	*count = i;
	return EXIT_SUCCESS;
}

// table for all students
void table_all(student_t *students, size_t count)
{
	// 20 20 9 8 5 8 12 6 (5 6) (20 5 11)
	printf("%s+--------------------+--------------------+---------+--------+-----+--------+------------+--------+-----+------+--------------------+-----+-----------+%s\n", BLACK, RESET);
	printf("%s| %-18s | %-18s | %-7s | %6s | %3s | %6s | %-10s | %-6s | %3s | %4s | %-18s | %3s | %9s |%s\n", BLACK, "First name", "Last name", "Group", "Gender", "Age", "Rating", "Date b.", "Type", "Num", "Room", "Street", "Num", "Apartment", RESET);
	printf("%s+--------------------+--------------------+---------+--------+-----+--------+------------+--------+-----+------+--------------------+-----+-----------+%s\n", BLACK, RESET);

	for (size_t i = 0; i < count; i++)
	{
		student_t s_i = students[i];

		printf("%s", BLACK);
		printf("| %-18s ", s_i.name.first_name);
		printf("| %-18s ", s_i.name.last_name);
		printf("| %-7s ", s_i.group);
		if (s_i.gender == 0)
			printf("| %-6s ", "Male");
		else if (s_i.gender == 1)
			printf("| %-6s ", "Female");
		printf("| %-3d ", s_i.age);
		printf("| %-6.1lf ", s_i.rating);
		printf("| %02d.%02d.%4d ", s_i.date.day, s_i.date.month, s_i.date.year);
		if (s_i.type_adress == 0)
		{
			printf("| %-6s ", "Dormi.");
			printf("| %-3d ", s_i.adress.dormitory.number);
			printf("| %-4d ", s_i.adress.dormitory.room);
			printf("| %-18s ", "---");
			printf("| %-3s ", "---");
			printf("| %-9s |", "---");

		}
		else if (s_i.type_adress == 1)
		{
			printf("| %-6s ", "House");
			printf("| %-3s ", "---");
			printf("| %-4s ", "---");
			printf("| %-18s ", s_i.adress.house.street);
			printf("| %-3d ", s_i.adress.house.number);
			printf("| %-9d |", s_i.adress.house.apart);
		}
		printf("%s\n", RESET);
	}

	printf("%s+--------------------+--------------------+---------+--------+-----+--------+------------+--------+-----+------+--------------------+-----+-----------+%s\n\n", BLACK, RESET);
}

void table_all_keys(student_t *students, size_t count, keys_t *keys)
{
	// 20 20 9 8 5 8 12 6 (5 6) (20 5 11)
	printf("%s+--------------------+--------------------+---------+--------+-----+--------+------------+--------+-----+------+--------------------+-----+-----------+%s\n", BLACK, RESET);
	printf("%s| %-18s | %-18s | %-7s | %6s | %3s | %6s | %-10s | %-6s | %3s | %4s | %-18s | %3s | %9s |%s\n", BLACK, "First name", "Last name", "Group", "Gender", "Age", "Rating", "Date b.", "Type", "Num", "Room", "Street", "Num", "Apartment", RESET);
	printf("%s+--------------------+--------------------+---------+--------+-----+--------+------------+--------+-----+------+--------------------+-----+-----------+%s\n", BLACK, RESET);

	for (size_t i = 0; i < count; i++)
	{
		size_t index = keys[i].index;
		student_t s_i = students[index];

		printf("%s", BLACK);
		printf("| %-18s ", s_i.name.first_name);
		printf("| %-18s ", s_i.name.last_name);
		printf("| %-7s ", s_i.group);
		if (s_i.gender == 0)
			printf("| %-6s ", "Male");
		else if (s_i.gender == 1)
			printf("| %-6s ", "Female");
		printf("| %-3d ", s_i.age);
		printf("| %-6.1lf ", s_i.rating);
		printf("| %02d.%02d.%4d ", s_i.date.day, s_i.date.month, s_i.date.year);
		if (s_i.type_adress == 0)
		{
			printf("| %-6s ", "Dormi.");
			printf("| %-3d ", s_i.adress.dormitory.number);
			printf("| %-4d ", s_i.adress.dormitory.room);
			printf("| %-18s ", "---");
			printf("| %-3s ", "---");
			printf("| %-9s |", "---");

		}
		else if (s_i.type_adress == 1)
		{
			printf("| %-6s ", "House");
			printf("| %-3s ", "---");
			printf("| %-4s ", "---");
			printf("| %-18s ", s_i.adress.house.street);
			printf("| %-3d ", s_i.adress.house.number);
			printf("| %-9d |", s_i.adress.house.apart);
		}
		printf("%s\n", RESET);
	}

	printf("%s+--------------------+--------------------+---------+--------+-----+--------+------------+--------+-----+------+--------------------+-----+-----------+%s\n\n", BLACK, RESET);
}

void table_sort(uint64_t *times, size_t count)
{
	printf("%s+--------------------+----------+%s\n", BLACK, RESET);
	printf("%s| %-18s | %-8s |%s\n", BLACK, "Sort type", "Ticks", RESET);
	printf("%s+--------------------+----------+%s\n", BLACK, RESET);
	printf("%s| %-18s | %-8lld |%s\n", BLACK, "Buble sort table", times[0], RESET);
	printf("%s| %-18s | %-8lld |%s\n", BLACK, "Quick sort table", times[1], RESET);
	printf("%s| %-18s | %-8lld |%s\n", BLACK, "Buble sort keys", times[2], RESET);
	printf("%s| %-18s | %-8lld |%s\n", BLACK, "Quick sort keys", times[3], RESET);
	printf("%s+--------------------+----------+%s\n\n", BLACK, RESET);
}

// table for specifiс students
int str_equal(char *str1, char *str2)
{
	if (strlen(str1) != strlen(str2))
		return EXIT_FAILURE;

	for (size_t i = 0; str1[i]; i++)
		if (toupper(str1[i]) != toupper(str2[i]))
			return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

void table_specific(student_t *students, size_t count)
{
	char buf_str[50];
	student_t buf[500];
	size_t buf_count = 0;
	size_t result_count = 0;
	student_t result[500];
	int is_house = -1;

	for (size_t i = 0; i < count; i++)
		buf[i] = students[i];
	buf_count = count;

	printf("%sВведите ключевое слово или нажмите ENTER%s\n", BLUE, RESET);

	printf("%sВведите имя:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (!str_equal(buf[i].name.first_name, buf_str))
			{
				result[result_count] = buf[i];
				result_count += 1;
			}

		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите фамилию:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (!str_equal(buf[i].name.last_name, buf_str))
			{
				result[result_count] = buf[i];
				result_count += 1;
			}

		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите группу:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (!str_equal(buf[i].group, buf_str))
			{
				result[result_count] = buf[i];
				result_count += 1;
			}

		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите пол:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int gender;
		if (!str_equal(buf_str, "male"))
			gender = 0;
		else if (!str_equal(buf_str, "female"))
			gender = 1;
		else
		{
			if(!is_correct_command(buf_str))
				gender = atoi(buf_str);
			else
			{
				printf("%sТакого пола не существует%s\n", YELLOW, RESET);
				return;
			}

			if (gender != 0 && gender != 1)
			{
				printf("%sТакого пола не существует%s\n", YELLOW, RESET);
				return;
			}
		}

		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (buf[i].gender == gender)
			{
				result[result_count] = buf[i];
				result_count += 1;
			}

		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите возраст:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int age;
		if(!is_correct_command(buf_str))
			age = atoi(buf_str);
		else
		{
			printf("%sВозраст введен в неправильном формате%s\n", YELLOW, RESET);
			return;
		}

		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (buf[i].age == age)
			{
				result[result_count] = buf[i];
				result_count += 1;
			}

		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите рейтинг:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		double rating = atof(buf_str);
		if (rating < 0.1)
		{
			printf("%sНекоректный рейтинг%s\n", YELLOW, RESET);
			return;	
		}

		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (buf[i].rating == rating)
			{
				result[result_count] = buf[i];
				result_count += 1;
			}

		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите день рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int day;
		if(!is_correct_command(buf_str))
		{
			day = atoi(buf_str);

			if (day > 31 || day <= 0)
			{
				printf("%sЭто точно день?%s\n", YELLOW, RESET);
				return;
			}
		}
		else
		{
			printf("%sДень введен в неправильном формате%s\n", YELLOW, RESET);
			return;
		}

		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (buf[i].date.day == day)
			{
				result[result_count] = buf[i];
				result_count += 1;
			}
		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите месяц рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int month;
		if(!is_correct_command(buf_str))
		{
			month = atoi(buf_str);

			if (month > 12 || month <= 0)
			{
				printf("%sЭто точно месяц?%s\n", YELLOW, RESET);
				return;
			}
		}
		else
		{
			printf("%sМесяц введен в неправильном формате%s\n", YELLOW, RESET);
			return;
		}

		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (buf[i].date.month == month)
			{
				result[result_count] = buf[i];
				result_count += 1;
			}
		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите год рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int year;
		if(!is_correct_command(buf_str))
		{
			year = atoi(buf_str);

			if (year > 2021 || year <= 0)
			{
				printf("%sЭто точно год?%s\n", YELLOW, RESET);
				return;
			}
		}
		else
		{
			printf("%sГод введен в неправильном формате%s\n", YELLOW, RESET);
			return;
		}

		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (buf[i].date.year == year)
			{
				result[result_count] = buf[i];
				result_count += 1;
			}
		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	printf("%sВведите тип жилья:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int type;
		if (!str_equal(buf_str, "dormi.") || !str_equal(buf_str, "dormitory"))
			type = 0;
		else if (!str_equal(buf_str, "house"))
			type = 1;
		else
		{
			if(!is_correct_command(buf_str))
				type = atoi(buf_str);
			else
			{
				printf("%sТакого типа жилья не существует%s\n", YELLOW, RESET);
				return;
			}

			if (type != 0 && type != 1)
			{
				printf("%sТакого типа жилья не существует%s\n", YELLOW, RESET);
				return;
			}
		}

		is_house = type;
		result_count = 0;
		for (size_t i = 0; i < buf_count; i++)
		{
			if (buf[i].type_adress == type)
			{
				result[result_count] = buf[i];
				result_count += 1;
			}

		}

		for (size_t i = 0; i < result_count; i++)
			buf[i] = result[i];
		buf_count = result_count;
	}
	if (buf_count == 0)
	{
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
		return;
	}

	if (is_house != 1)
	{
		printf("%sВведите номер общежития:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 0;
			int number_dorm;
			if(!is_correct_command(buf_str))
				number_dorm = atoi(buf_str);
			else
			{
				printf("%sНомер общежития введен в неправильном формате%s\n", YELLOW, RESET);
				return;
			}

			result_count = 0;
			for (size_t i = 0; i < buf_count; i++)
			{
				if (buf[i].adress.dormitory.number == number_dorm)
				{
					result[result_count] = buf[i];
					result_count += 1;
				}

			}

			for (size_t i = 0; i < result_count; i++)
				buf[i] = result[i];
			buf_count = result_count;
		}
		if (buf_count == 0)
		{
			printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
			return;
		}
	}

	if (is_house != 1)
	{
		printf("%sВведите комнату:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 0;
			int room;
			if(!is_correct_command(buf_str))
				room = atoi(buf_str);
			else
			{
				printf("%sКомната введен в неправильном формате%s\n", YELLOW, RESET);
				return;
			}

			result_count = 0;
			for (size_t i = 0; i < buf_count; i++)
			{
				if (buf[i].adress.dormitory.room == room)
				{
					result[result_count] = buf[i];
					result_count += 1;
				}

			}

			for (size_t i = 0; i < result_count; i++)
				buf[i] = result[i];
			buf_count = result_count;
		}
		if (buf_count == 0)
		{
			printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
			return;
		}
	}

	if (is_house != 0)
	{
		printf("%sВведите улицу:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 1;
			result_count = 0;
			for (size_t i = 0; i < buf_count; i++)
			{
				if (!str_equal(buf[i].adress.house.street, buf_str))
				{
					result[result_count] = buf[i];
					result_count += 1;
				}

			}

			for (size_t i = 0; i < result_count; i++)
				buf[i] = result[i];
			buf_count = result_count;
		}
		if (buf_count == 0)
		{
			printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
			return;
		}
	}

	if (is_house != 0)
	{
		printf("%sВведите номер дома:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 1;
			int number_house;
			if(!is_correct_command(buf_str))
				number_house = atoi(buf_str);
			else
			{
				printf("%sНомер дома введен в неправильном формате%s\n", YELLOW, RESET);
				return;
			}

			result_count = 0;
			for (size_t i = 0; i < buf_count; i++)
			{
				if (buf[i].adress.house.number == number_house)
				{
					result[result_count] = buf[i];
					result_count += 1;
				}

			}

			for (size_t i = 0; i < result_count; i++)
				buf[i] = result[i];
			buf_count = result_count;
		}
		if (buf_count == 0)
		{
			printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
			return;
		}
	}

	if (is_house != 0)
	{
		printf("%sВведите квартиру:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 1;
			int apart;
			if(!is_correct_command(buf_str))
				apart = atoi(buf_str);
			else
			{
				printf("%sКвартира введен в неправильном формате%s\n", YELLOW, RESET);
				return;
			}

			result_count = 0;
			for (size_t i = 0; i < buf_count; i++)
			{
				if (buf[i].adress.house.apart == apart)
				{
					result[result_count] = buf[i];
					result_count += 1;
				}

			}

			for (size_t i = 0; i < result_count; i++)
				buf[i] = result[i];
			buf_count = result_count;
		}
	}

	if (result_count > 0)
		table_all(result, result_count);
	else
		printf("%sТаких студентов не найдено%s\n\n", YELLOW, RESET);
}

// Считать студента
int day_month(int d, int m)
{
	if (m == 1 && d > 31)
		return EXIT_FAILURE;
	if (m == 2 && d > 29)
		return EXIT_FAILURE;
	if (m == 3 && d > 31)
		return EXIT_FAILURE;
	if (m == 4 && d > 30)
		return EXIT_FAILURE;
	if (m == 5 && d > 31)
		return EXIT_FAILURE;
	if (m == 6 && d > 30)
		return EXIT_FAILURE;
	if (m == 7 && d > 31)
		return EXIT_FAILURE;
	if (m == 8 && d > 31)
		return EXIT_FAILURE;
	if (m == 9 && d > 30)
		return EXIT_FAILURE;
	if (m == 10 && d > 31)
		return EXIT_FAILURE;
	if (m == 11 && d > 30)
		return EXIT_FAILURE;
	if (m == 12 && d > 31)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int day_month_year(int d, int m, int y)
{
	if (y % 4 != 0 && m == 2 && d > 28)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int get_student(student_t *student)
{
	char buf_str[50];
	student_t buf;

	printf("%sВведите имя (не больше 18 символов):%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) > 18 || strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	else
		strcpy(buf.name.first_name, buf_str);

	printf("%sВведите фамилию (не больше 18 символов):%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) > 18 || strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	else
		strcpy(buf.name.last_name, buf_str);

	printf("%sВведите группу (не больше 7 символов):%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) > 7 || strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	else
		strcpy(buf.group, buf_str);

	printf("%sВведите пол (0-male или 1-female):%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	if (!str_equal(buf_str, "male"))
		buf.gender = 0;
	else if (!str_equal(buf_str, "female"))
		buf.gender = 1;
	else if (!is_correct_command(buf_str))
	{
		int gender = atoi(buf_str);
		if (gender != 0 && gender != 1)
			return EXIT_FAILURE;
		else
			buf.gender = gender;
	}
	else
		return EXIT_FAILURE;

	printf("%sВведите возраст:%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	if (!is_correct_command(buf_str))
	{
		int age = atoi(buf_str);
		if (age <= 0 || age > 100)
			return EXIT_FAILURE;
		else
			buf.age = age;
	}
	else
		return EXIT_FAILURE;

	printf("%sВведите рейтинг:%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	double rating = atof(buf_str);
	if (rating < 0.1)
		return EXIT_FAILURE;
	else
		buf.rating = rating;

	printf("%sВведите день рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	if (!is_correct_command(buf_str))
	{
		int day = atoi(buf_str);
		if (day <= 0 || day > 31)
			return EXIT_FAILURE;
		else
			buf.date.day = day;
	}
	else
		return EXIT_FAILURE;

	printf("%sВведите месяц рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	if (!is_correct_command(buf_str))
	{
		int month = atoi(buf_str);
		if (month <= 0 || month > 31)
			return EXIT_FAILURE;
		else if (day_month(buf.date.day, month))
			return EXIT_FAILURE;
		else
			buf.date.month = month;
	}
	else
		return EXIT_FAILURE;

	printf("%sВведите год рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	if (!is_correct_command(buf_str))
	{
		int year = atoi(buf_str);
		if (year <= 0 || year > 2020)
			return EXIT_FAILURE;
		else if (day_month_year(buf.date.day, buf.date.month, year))
			return EXIT_FAILURE;
		else
			buf.date.year = year;
	}
	else
		return EXIT_FAILURE;

	printf("%sВведите тип жилья (0-dormitory или 1-house):%s ", BLUE, RESET);
	gets(buf_str);
	if (strlen(buf_str) == 0)
		return	EXIT_FAILURE;
	if (!str_equal(buf_str, "dormi.") || !str_equal(buf_str, "dormitory"))
		buf.type_adress = 0;
	else if (!str_equal(buf_str, "house"))
		buf.type_adress = 1;
	else if (!is_correct_command(buf_str))
	{
		int type = atoi(buf_str);
		if (type != 0 && type != 1)
			return EXIT_FAILURE;
		else
			buf.type_adress = type;
	}
	else
		return EXIT_FAILURE;

	if (buf.type_adress == 0)
	{
		printf("%sВведите номер общежития:%s ", BLUE, RESET);
		gets(buf_str);
		if (strlen(buf_str) == 0)
			return	EXIT_FAILURE;
		if (!is_correct_command(buf_str))
		{
			int number_dorm = atoi(buf_str);
			buf.adress.dormitory.number = number_dorm;
		}
		else
			return EXIT_FAILURE;

		printf("%sВведите комнату:%s ", BLUE, RESET);
		gets(buf_str);
		if (strlen(buf_str) == 0)
			return	EXIT_FAILURE;
		if (!is_correct_command(buf_str))
		{
			int room = atoi(buf_str);
			buf.adress.dormitory.room = room;
		}
		else
			return EXIT_FAILURE;
	}
	else
	{
		printf("%sВведите улицу (не больше 18 символов):%s ", BLUE, RESET);
		gets(buf_str);
		if (strlen(buf_str) > 18 || strlen(buf_str) == 0)
			return	EXIT_FAILURE;
		else
			strcpy(buf.adress.house.street, buf_str);

		printf("%sВведите номер дома:%s ", BLUE, RESET);
		gets(buf_str);
		if (strlen(buf_str) == 0)
			return	EXIT_FAILURE;
		if (!is_correct_command(buf_str))
		{
			int number_house = atoi(buf_str);
			buf.adress.house.number = number_house;
		}
		else
			return EXIT_FAILURE;

		printf("%sВведите квартиру:%s ", BLUE, RESET);
		gets(buf_str);
		if (strlen(buf_str) == 0)
			return	EXIT_FAILURE;
		if (!is_correct_command(buf_str))
		{
			int apart = atoi(buf_str);
			buf.adress.house.apart = apart;
		}
		else
			return EXIT_FAILURE;
	}

	*student = buf;
	return EXIT_SUCCESS;
}

// delete student
void copy_students(student_t *dst, student_t *src, size_t count)
{
	for (size_t i = 0; i < count; i++)
		dst[i] = src[i];
}

void del_student(student_t *students, size_t *count)
{
	char buf_str[50];
	int index_del[*count];
	int is_house = -1;

	for (size_t i = 0; i < *count; i++)
		index_del[i] = 1;

	printf("%sВведите ключевое слово или нажмите ENTER%s\n", BLUE, RESET);

	printf("%sВведите имя:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
		for (size_t i = 0; i < *count; i++)
		{
			if (!str_equal(students[i].name.first_name, buf_str) && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;
		}

	printf("%sВведите фамилию:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
		for (size_t i = 0; i < *count; i++)
		{
			if (!str_equal(students[i].name.last_name, buf_str) && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;
		}

	printf("%sВведите группу:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
		for (size_t i = 0; i < *count; i++)
		{
			if (!str_equal(students[i].group, buf_str) && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;
		}

	printf("%sВведите пол:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int gender;
		if (!str_equal(buf_str, "male"))
			gender = 0;
		else if (!str_equal(buf_str, "female"))
			gender = 1;
		else
		{
			if(!is_correct_command(buf_str))
				gender = atoi(buf_str);
			else
			{
				printf("%sТакого пола не существует%s\n\n", YELLOW, RESET);
				return;
			}

			if (gender != 0 && gender != 1)
			{
				printf("%sТакого пола не существует%s\n\n", YELLOW, RESET);
				return;
			}
		}

		for (size_t i = 0; i < *count; i++)
		{
			if (students[i].gender == gender && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;
		}
	}

	printf("%sВведите возраст:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int age;
		if(!is_correct_command(buf_str))
			age = atoi(buf_str);
		else
		{
			printf("%sВозраст введен в неправильном формате%s\n\n", YELLOW, RESET);
			return;
		}

		for (size_t i = 0; i < *count; i++)
		{
			if (students[i].age == age && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;

		}
	}

	printf("%sВведите рейтинг:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		double rating = atof(buf_str);
		if (rating < 0.1)
		{
			printf("%sНекоректный рейтинг%s\n\n", YELLOW, RESET);
			return;	
		}

		for (size_t i = 0; i < *count; i++)
		{
			if (students[i].rating == rating && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;
		}
	}

	printf("%sВведите день рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int day;
		if(!is_correct_command(buf_str))
		{
			day = atoi(buf_str);

			if (day > 31 || day <= 0)
			{
				printf("%sЭто точно день?%s\n\n", YELLOW, RESET);
				return;
			}
		}
		else
		{
			printf("%sДень введен в неправильном формате%s\n\n", YELLOW, RESET);
			return;
		}

		for (size_t i = 0; i < *count; i++)
		{
			if (students[i].date.day == day && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;
		}
	}

	printf("%sВведите месяц рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int month;
		if(!is_correct_command(buf_str))
		{
			month = atoi(buf_str);

			if (month > 12 || month <= 0)
			{
				printf("%sЭто точно месяц?%s\n\n", YELLOW, RESET);
				return;
			}
		}
		else
		{
			printf("%sМесяц введен в неправильном формате%s\n\n", YELLOW, RESET);
			return;
		}

		for (size_t i = 0; i < *count; i++)
		{
			if (students[i].date.month == month && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;
		}
	}

	printf("%sВведите год рождения:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int year;
		if(!is_correct_command(buf_str))
		{
			year = atoi(buf_str);

			if (year > 2021 || year <= 0)
			{
				printf("%sЭто точно год?%s\n\n", YELLOW, RESET);
				return;
			}
		}
		else
		{
			printf("%sГод введен в неправильном формате%s\n\n", YELLOW, RESET);
			return;
		}

		for (size_t i = 0; i < *count; i++)
		{
			if (students[i].date.year == year && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;
		}
	}

	printf("%sВведите тип жилья:%s ", BLUE, RESET);
	gets(buf_str);
	if (buf_str[0])
	{
		int type;
		if (!str_equal(buf_str, "dormi.") || !str_equal(buf_str, "dormitory"))
			type = 0;
		else if (!str_equal(buf_str, "house"))
			type = 1;
		else
		{
			if(!is_correct_command(buf_str))
				type = atoi(buf_str);
			else
			{
				printf("%sТакого типа жилья не существует%s\n\n", YELLOW, RESET);
				return;
			}

			if (type != 0 && type != 1)
			{
				printf("%sТакого типа жилья не существует%s\n\n", YELLOW, RESET);
				return;
			}
		}

		is_house = type;
		for (size_t i = 0; i < *count; i++)
		{
			if (students[i].type_adress == type && index_del[i] == 1)
				index_del[i] = 1;
			else if (index_del[i] == 1)
				index_del[i] = 0;

		}
	}

	if (is_house != 1)
	{
		printf("%sВведите номер общежития:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 0;
			int number_dorm;
			if(!is_correct_command(buf_str))
				number_dorm = atoi(buf_str);
			else
			{
				printf("%sНомер общежития введен в неправильном формате%s\n\n", YELLOW, RESET);
				return;
			}

			for (size_t i = 0; i < *count; i++)
			{
				if (students[i].adress.dormitory.number == number_dorm && index_del[i] == 1)
					index_del[i] = 1;
				else if (index_del[i] == 1)
					index_del[i] = 0;
			}
		}
	}

	if (is_house != 1)
	{
		printf("%sВведите комнату:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 0;
			int room;
			if(!is_correct_command(buf_str))
				room = atoi(buf_str);
			else
			{
				printf("%sКомната введен в неправильном формате%s\n\n", YELLOW, RESET);
				return;
			}

			for (size_t i = 0; i < *count; i++)
			{
				if (students[i].adress.dormitory.room == room && index_del[i] == 1)
					index_del[i] = 1;
				else if (index_del[i] == 1)
					index_del[i] = 0;
			}
		}
	}

	if (is_house != 0)
	{
		printf("%sВведите улицу:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 1;
			for (size_t i = 0; i < *count; i++)
			{
				if (!str_equal(students[i].adress.house.street, buf_str) && index_del[i] == 1)
					index_del[i] = 1;
				else if (index_del[i] == 1)
					index_del[i] = 0;
			}
		}
	}

	if (is_house != 0)
	{
		printf("%sВведите номер дома:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 1;
			int number_house;
			if(!is_correct_command(buf_str))
				number_house = atoi(buf_str);
			else
			{
				printf("%sНомер дома введен в неправильном формате%s\n\n", YELLOW, RESET);
				return;
			}

			for (size_t i = 0; i < *count; i++)
			{
				if (students[i].adress.house.number == number_house && index_del[i] == 1)
					index_del[i] = 1;
				else if (index_del[i] == 1)
					index_del[i] = 0;
			}
		}
	}

	if (is_house != 0)
	{
		printf("%sВведите квартиру:%s ", BLUE, RESET);
		gets(buf_str);
		if (buf_str[0])
		{
			is_house = 1;
			int apart;
			if(!is_correct_command(buf_str))
				apart = atoi(buf_str);
			else
			{
				printf("%sКвартира введен в неправильном формате%s\n\n", YELLOW, RESET);
				return;
			}

			for (size_t i = 0; i < *count; i++)
			{
				if (students[i].adress.house.apart == apart && index_del[i] == 1)
					index_del[i] = 1;
				else if (index_del[i] == 1)
					index_del[i] = 0;
			}
		}
	}

	size_t count_del = 0;
	for (size_t i = 0; i < *count; i++)
		if (index_del[i] == 1)
			count_del += 1;

	if (count_del <= 0)
	{
		printf("%sНет студентов для удаления%s\n\n", YELLOW, RESET);
		return;
	}

	student_t buf[500];
	copy_students(buf, students, *count);

	size_t tmp_i = 0;
	for (size_t i = 0; i < *count; i++)
		if (index_del[i] != 1)
			students[tmp_i++] = buf[i];

	printf("%sКоличество удаленных студентов - %zu%s\n\n", GREEN, count_del, RESET);
	*count -= count_del;
}

// buble sort for rating
void buble_sort_rating(student_t *students, size_t count)
{
	for (size_t i = 0; i < count - 1; i++)
		for (size_t j = 0; j < count - i - 1; j++)
		{
			if (students[j].rating > students[j + 1].rating)
			{
				student_t tmp = students[j];
				students[j] = students[j + 1];
				students[j + 1] = tmp;
			}
		}
}

void quick_sort_rating(student_t *students, size_t left, size_t right)
{
	size_t i = left;
	size_t j = right;
	double x = students[(left + right) / 2].rating;

	do
	{
		while (students[i].rating < x)
			i += 1;
		while (students[j].rating > x)
			j -= 1;

		if (i <= j)
		{
			if (students[i].rating > students[j].rating)
			{
				student_t tmp = students[j];
				students[j] = students[i];
				students[i] = tmp;
			}

			i += 1;
			j -= 1;
		}
	} while (i <= j);

	if (i < right)
		quick_sort_rating(students, i, right);
	if (left < j)
		quick_sort_rating(students, left, j);
}

int get_sort(int *sort_key)
{
	char buf_str[50];

	printf("1. Buble sort\n");
	printf("2. Quick sort\n");
	printf("%sВведите тип сортировки:%s ", BLUE, RESET);
	gets(buf_str);

	if (!is_correct_command(buf_str))
	{
		int tmp = atoi(buf_str);
		if (tmp != 1 && tmp != 2)
			return EXIT_FAILURE;

		*sort_key = tmp;
	}
	else
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

void fill_keys(keys_t *keys, student_t *students, size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		keys[i].rating = students[i].rating;
		keys[i].index = i;
	}
}

void buble_sort_keys(keys_t *keys, size_t count)
{
	for (size_t i = 0; i < count - 1; i++)
		for (size_t j = 0; j < count - i - 1; j++)
		{
			if (keys[j].rating > keys[j + 1].rating)
			{
				keys_t tmp = keys[j];
				keys[j] = keys[j + 1];
				keys[j + 1] = tmp;
			}
		}
}

void quick_sort_keys(keys_t *keys, size_t left, size_t right)
{
	size_t i = left;
	size_t j = right;
	double x = keys[(left + right) / 2].rating;

	do
	{
		while (keys[i].rating < x)
			i += 1;
		while (keys[j].rating > x)
			j -= 1;

		if (i <= j)
		{
			if (keys[i].rating > keys[j].rating)
			{
				keys_t tmp = keys[j];
				keys[j] = keys[i];
				keys[i] = tmp;
			}

			i += 1;
			j -= 1;
		}
	} while (i <= j);

	if (i < right)
		quick_sort_keys(keys, i, right);
	if (left < j)
		quick_sort_keys(keys, left, j);
}

uint64_t tick(void)
{
    uint32_t high, low;
    __asm__ __volatile__(
	        "rdtsc\n"
	        "movl %%edx, %0\n"
	        "movl %%eax, %1\n"
        : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}

int main(void)
{
	setbuf(stdout, NULL);

	type_t run_key = 1;
	type_t cmd_key = get_command(1);
	student_t students[500];
	student_t measure_time[500];
	keys_t keys[500];
	keys_t measure_time_keys[500];
	student_t buf_student;
	char file_name[100];
	size_t count = 0;
	int sort_key;
	int is_key_sorted = 0;
	uint64_t times[4];
	uint64_t start, end;

	while(run_key)
	{
		switch (cmd_key)
		{
			case CMD_WRITE_COMMAND:			// вывести на экран все команды
				cmd_key = get_command(1);
				break;
			case CMD_EXIT_COMMAND:			// выйти из программы
				printf("%sВыход из программы...%s\n", GREEN, RESET);
				run_key = 0;
				break;
			case CMD_INCORRECT_COMMAND:		// вывести на экран все команды, если некоректный ввод
				printf("%sВы ввели неправильную команду...%s\n", YELLOW, RESET);
				printf("%sПопробуйте еще раз%s\n\n", YELLOW, RESET);
				cmd_key = get_command(1);
				break;
			case CMD_READ_FILE_COMMAND: 	// считать студентов из файла
				printf("%sВведите название файла:%s ", BLUE, RESET);
				gets(file_name);
				FILE *f = fopen(file_name, "r");

				if (!f)
					printf("%sНеправильное название файла%s\n\n", YELLOW, RESET);
				else
				{
					if (get_students(f, students, &count))
					{
						printf("%sВо время считывания файла произошла ошибка%s\n\n", YELLOW, RESET);
						fclose(f);
						cmd_key = get_command(0);
						break;
					}

					printf("%sКоличество студентов в файле - %zu%s\n\n", GREEN, count, RESET);
					fclose(f);
				}

				cmd_key = get_command(0);
				break;
			case CMD_WRITE_ALL_COMMAND: 	// печать таблицы всех студентов
				if (count <= 0)
				{
					printf("%sНет студентов для печати%s\n\n", YELLOW, RESET);
					cmd_key = get_command(0);
					break;
				}
				
				table_all(students, count);

				cmd_key = get_command(0);
				break;
			case CMD_WRITE_ALL_KEYS_COMMAND: // печать таблицы всех студентов по ключам
				if (count <= 0)
				{
					printf("%sНет студентов для печати%s\n\n", YELLOW, RESET);
					cmd_key = get_command(0);
					break;
				}
				
				if (!is_key_sorted)
					fill_keys(keys, students, count);

				table_all_keys(students, count, keys);


				cmd_key = get_command(0);
				break;
			case CMD_WRITE_SPEC_COMMAND: 	// печать таблицу с определенными полями
				if (count <= 0)
				{
					printf("%sНет студентов для поиска%s\n\n", YELLOW, RESET);
					cmd_key = get_command(0);
					break;
				}

				table_specific(students, count);

				cmd_key = get_command(0);
				break;
			case CMD_ADD_STUDENT_COMMAND: 	// добавить студента в таблицу
				if (!get_student(&buf_student))
				{
					students[count] = buf_student;
					count += 1;
					printf("%sСтудент добавлен%s\n\n", GREEN, RESET);
				}
				else
					printf("%sСтудент не добавлен%s\n\n", YELLOW, RESET);

				cmd_key = get_command(0);
				break;
			case CMD_DEL_STUDENT_COMMAND:
				if (count <= 0)
				{
					printf("%sНет студентов для удаления%s\n\n", YELLOW, RESET);
					cmd_key = get_command(0);
					break;
				}

				del_student(students, &count);

				cmd_key = get_command(0);
				break;
			case CMD_SORT_COMMAND: 			// сортировка таблицы
				if (count <= 0)
				{
					printf("%sНет студентов для сортировки%s\n\n", YELLOW, RESET);
					cmd_key = get_command(0);
					break;
				}

				if (!get_sort(&sort_key))
				{
					if (sort_key == 1)
						buble_sort_rating(students, count);
					else if (sort_key == 2)
						quick_sort_rating(students, 0, count-1);

					printf("%sСтуденты отсортированы%s\n\n", GREEN, RESET);
				}
				else
					printf("%sСортировка не выбрана%s\n\n", YELLOW, RESET);

				cmd_key = get_command(0);
				break;
			case CMD_SORT_KEY_COMMAND:		// сортировка таблицы по ключу
				if (count <= 0)
				{
					printf("%sНет студентов для сортировки%s\n\n", YELLOW, RESET);
					cmd_key = get_command(0);
					break;
				}

				fill_keys(keys, students, count);

				if (!get_sort(&sort_key))
				{
					if (sort_key == 1)
						buble_sort_keys(keys, count);
					else if (sort_key == 2)
					 	quick_sort_keys(keys, 0, count-1);

					printf("%sСтуденты отсортированы%s\n\n", GREEN, RESET);
					is_key_sorted = 1;
				}
				else
					printf("%sСортировка не выбрана%s\n\n", YELLOW, RESET);


				cmd_key = get_command(0);
				break;
			case CMD_MEASURE_TIME_COMMAND: 	// сортировка таблицы по ключу
				if (count <= 0)
				{
					printf("%sНет студентов для измерения времени%s\n\n", YELLOW, RESET);
					cmd_key = get_command(0);
					break;
				}

				copy_students(measure_time, students, count);
				start = tick();
				buble_sort_rating(measure_time, count);
				end = tick();
				times[0] = end - start;

				copy_students(measure_time, students, count);
				start = tick();
				quick_sort_rating(measure_time, 0, count - 1);
				end = tick();
				times[1] = end - start;

				fill_keys(measure_time_keys, students, count);
				start = tick();
				buble_sort_keys(measure_time_keys, count);
				end = tick();
				times[2] = end - start;

				fill_keys(measure_time_keys, students, count);
				start = tick();
				quick_sort_keys(measure_time_keys, 0, count - 1);
				end = tick();
				times[3] = end - start;

				table_sort(times, 4);

				cmd_key = get_command(0);
				break;
			default:						// непредвиденные обстоятельства
				printf("%sЧто-то пошло не так...%s\n", RED, RESET);
				return ERROR_COMMANDS;
		}
	}

	return EXIT_SUCCESS;
}