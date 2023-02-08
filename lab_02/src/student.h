#ifndef _STUDENT_H_
#define _STUDENT_H_

typedef int type_t;

typedef struct
{
	char first_name[50];
	char last_name[50];
} name_t;

typedef struct 
{
	int day;
	int month;
	int year;
} date_t;

typedef struct
{
	int number;
	int room;
} dormitory_t;

typedef struct
{
	char street[50];
	int number;
	int apart;
} house_t;

typedef union
{
	dormitory_t dormitory;
	house_t house;
} adress_t;

typedef struct 
{
	name_t name;
	char group[10];
	type_t gender; // 0 - муж, 1 - жен
	int age;
	double rating;
	date_t date;
	type_t type_adress; // 0 - общага, 1 - квартира
	adress_t adress;
} student_t;

typedef struct
{
	double rating;
	size_t index;
} keys_t;

#endif // _STUDENT_H_