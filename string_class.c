#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define nullString { NULL, }
#define ERROR_NUM 127
#define BUFFER_1K 1024
#define BUFFER_2K 2048
#define BUFFER_4K 4096

typedef struct __string string;
typedef string String;

void initString(String *this);
void destroyString(String *this);

String new_String();
String newString(const char *);
String *new_Ptring();
String *newPtring(const char *);

typedef void (*__set_string)(String*, const char *);
typedef const char *(*__get_string)(String*);
typedef unsigned (*__get_length)(String*) ;
typedef String *(*__shallow_copy)(String*);
typedef void (*__deep_copy)(String*, const String*);
typedef String *(*__realloc_mem)(String*);
typedef String (*__move_string)(String*);
typedef void (*__append_string)(String*, const String*);
typedef void (*__append_const)(String*, const char *);
typedef void (*__swap_string)(String*, String*);
typedef void (*__clear_string)(String*);
typedef int (*__empty_string)(const String*);
typedef int (*__compare_string)(const String*, const String*);
typedef int (*__compare_const)(const String*, const char*);
typedef int (*__equal_string)(const String*, const String*);
typedef int (*__equal_const)(const String*, const char*);
typedef void (*__format_string)(String*, const char*, ...);
typedef void (*__format_const)(String*, const char*, ...);

struct __string {
	char *str;
	unsigned len;

	__set_string set;
	__get_string get;
	__get_length length;
	__shallow_copy addrcpy;
	__deep_copy memcpy;
	__realloc_mem realloc;
	__move_string move;
	__append_string append;
	__append_const appendRow;
	__swap_string swap;
	__clear_string clear;
	__empty_string isEmpty;
	__compare_string compare;
	__compare_const compareRow;
	__equal_string isEqual;
	__equal_const isEqualRow;
	__format_string format;
	__format_const formatRow;
};

void setString(String *this, const char *str) {
	if(!this->isEmpty(this))
		this->clear(this);

	this->len = strlen(str);
	this->str = (char*)malloc(sizeof(char) * this->len);
	strncpy(this->str, str, this->len);
}

const char *getString(String *this) {
	return this->str;
}

unsigned getLength(String *this) {
	return this->len;
}

String *weakCopy(String *other) {
	String *str = NULL;
	if(!other->isEmpty(other))
		str = other;
	else
		fprintf(stderr, "String is empty.\n");
	return str;
}

void strongCopy(String *this, const String *other) {
	if(!other->isEmpty(other))
		this->set(this, other->str);
	else
		fprintf(stderr, "String is empty.\n");
}

String *relocMemory(String *this) {
	String *str = (String*)malloc(sizeof(String));
	initString(str);
	destroyString(str);
	return str;
}

String mvString(String *other) {
	String str = nullString;
	initString(&str);
	destroyString(&str);
	if(!other->isEmpty(other)) {
		str.set(&str, other->str);
		other->clear(other);
	} else
		fprintf(stderr, "String is empty.\n");
	return str;
}

void appendString(String *this, const String *other) {
	if(!other->isEmpty(other)) {
		this->len += other->len;
		this->str = (char*)realloc(this->str, this->len);
		strncat(this->str, other->str, strlen(other->str));
	} else
		fprintf(stderr, "String is empty.\n");
}

void appendConst(String *this, const char *str) {
	if(str && strlen(str) > 0) {
		this->len += strlen(str);
		this->str = (char*)realloc(this->str, this->len);
		strncat(this->str, str, strlen(str));
	} else
		fprintf(stderr, "String is empty.\n");
}

void swapString(String *this, String *other) {
	char *str = this->str;
	unsigned len = this->len;

	this->str = other->str;
	this->len = other->len;

	other->str = str;
	other->len = len;
}

void clearString(String *this) {
	memset(this->str, 0, this->len);
	this->len = 0;
	free(this->str);
}

int emptyString(const String *this) {
	return !this->len;
}

int compareString(const String *this, const String *other) {
	if(this->len <= 0 || other->len <= 0) return ERROR_NUM;
	return strncmp(this->str, other->str, strlen(this->str));
}

int compareConst(const String *this, const char *str) {
	if(this->len <= 0 || strlen(str) <= 0) return ERROR_NUM;
	return strncmp(this->str, str, strlen(this->str));
}

int equalString(const String *this, const String *other) {
	if(this->len == other->len)
		return strncmp(this->str, other->str, strlen(this->str));
	else
		return !0;
}

int equalConst(const String *this, const char *str) {
	if(this->len == strlen(str))
		return strncmp(this->str, str, strlen(this->str));
	else
		return !0;
}

void formatString(String *this, const char *format, ...) {
	char buf[BUFFER_2K] = {0,};

	va_list args;
	va_start(args, format);

	vsprintf(buf, format, args);
	this->set(this, buf);

	va_end(args);
}

void formatConst(String *this, const char *format, ...) {
	char buf[BUFFER_2K] = {0,};

	va_list args;
	va_start(args, format);

	vsprintf(buf, format, args);
	this->appendRow(this, buf);

	va_end(args);
}

String new_String() {
	String str = nullString;
	initString(&str);
	destroyString(&str);
	return str;
}

String newString(const char *s) {
	String str = nullString;
	initString(&str);
	str.set(&str, s);
	destroyString(&str);
	return str;
}

String *new_Ptring() {
	String *str = (String*)malloc(sizeof(String));
	initString(str);
	destroyString(str);
	return str;
}

String *newPtring(const char *s) {
	String *str = (String*)malloc(sizeof(String));
	initString(str);
	str->set(str, s);
	destroyString(str);
	return str;
}

void initString(String *this) {
	this->set = setString;
	this->get = getString;
	this->length = getLength;
	this->addrcpy = weakCopy;
	this->memcpy = strongCopy;
	this->realloc = relocMemory;
	this->move = mvString;
	this->append = appendString;
	this->appendRow = appendConst;
	this->swap = swapString;
	this->clear = clearString;
	this->isEmpty = emptyString;
	this->compare = compareString;
	this->compareRow = compareConst;
	this->isEqual = equalString;
	this->isEqualRow = equalConst;
	this->format = formatString;
	this->formatRow = formatConst;
}

void destroyString(String *this) {
}

int main() {
	String str1 = new_String();
	String str2 = newString("wowow");
	String *str3 = new_Ptring();
	String *str4 = newPtring("whataaa");

	printf("##### Hello! This is String Class implements test!!\n");
	printf("##### set(), length() Test...\n");

	str1.set(&str1, "hello");
	printf("[str1: %p]String: %s, Length: %d\n", &str1, str1.get(&str1), str1.length(&str1));
	printf("[str2: %p]String: %s, Length: %d\n", &str2, str2.get(&str2), str2.length(&str2));

	str3->set(str3, "nonono");
	printf("[str3: %p]Ptring: %s, Length: %d\n", &str3, str3->get(str3), str3->length(str3));
	printf("[str4: %p]Ptring: %s, Length: %d\n", &str4, str4->get(str4), str4->length(str4));

	printf("\n##### addrcpy() Test...\n");

	str3 = str3->addrcpy(str4);
	printf("[str3: %p]Ptring: %s, Length: %d\n", &str3, str3->get(str3), str3->length(str3));
	printf("\t\t-->String Address: %p\n\t\t-->Length Address: %p\n", &(str3->str), &(str3->len));
	printf("[str4: %p]Ptring: %s, Length: %d\n", &str4, str4->get(str4), str4->length(str4));
	printf("\t\t-->String Address: %p\n\t\t-->Length Address: %p\n", &(str4->str), &(str4->len));

	printf("\n... Memory realloc ...\n");

	str3 = str3->realloc(str3);
	printf("[str3: %p]Ptring: %s, Length: %d\n", &str3, str3->get(str3), str3->length(str3));
	printf("\t\t-->String Address: %p\n\t\t-->Length Address: %p\n", &(str3->str), &(str3->len));
	printf("[str4: %p]Ptring: %s, Length: %d\n", &str4, str4->get(str4), str4->length(str4));
	printf("\t\t-->String Address: %p\n\t\t-->Length Address: %p\n", &(str4->str), &(str4->len));

	printf("\n##### memcpy() Test...\n");

	str1.memcpy(&str1, &str2);
	printf("[str1: %p]String: %s, Length: %d\n", &str1, str1.get(&str1), str1.length(&str1));
	printf("\t\t-->String Address: %p\n\t\t-->Length Address: %p\n", &(str1.str), &(str1.len));
	printf("[str2: %p]String: %s, Length: %d\n", &str2, str2.get(&str2), str2.length(&str2));
	printf("\t\t-->String Address: %p\n\t\t-->Length Address: %p\n", &(str2.str), &(str2.len));

	printf("\n##### move() Test...\n");

	str1 = str1.move(&str2);
	printf("[str1: %p]String: %s, Length: %d\n", &str1, str1.get(&str1), str1.length(&str1));
	printf("[str2: %p]String: %s, Length: %d\n", &str2, str2.get(&str2), str2.length(&str2));

	printf("\n##### append() Test...\n");

	str2.append(&str2, &str1);
	str1.append(&str1, &str1);
	printf("[str1: %p]String: %s, Length: %d\n", &str1, str1.get(&str1), str1.length(&str1));
	printf("[str2: %p]String: %s, Length: %d\n", &str2, str2.get(&str2), str2.length(&str2));

	printf("\n##### appendRow() Test...\n");

	str4->appendRow(str4, " Append");
	printf("[str3: %p]Ptring: %s, Length: %d\n", &str3, str3->get(str3), str3->length(str3));
	printf("[str4: %p]Ptring: %s, Length: %d\n", &str4, str4->get(str4), str4->length(str4));

	printf("\n##### swap() Test...\n");

	str3->swap(str3, str4);
	printf("[str3: %p]Ptring: %s, Length: %d\n", &str3, str3->get(str3), str3->length(str3));
	printf("[str4: %p]Ptring: %s, Length: %d\n", &str4, str4->get(str4), str4->length(str4));

	printf("\n##### clear() Test...\n");
	
	str1.clear(&str1);
	printf("[str1: %p]String: %s, Length: %d\n", &str1, str1.isEmpty(&str1) == 1 ? "(null)" : str1.get(&str1), str1.length(&str1));
	printf("[str2: %p]String: %s, Length: %d\n", &str2, str2.get(&str2), str2.length(&str2));

	printf("\n##### isEmpty() Test...\n");

	printf("str1 is ... %s\n", str1.isEmpty(&str1) == 1 ? "Empty!!" : "Not Empty!!");
	printf("str2 is ... %s\n", str2.isEmpty(&str2) == 1 ? "Empty!!" : "Not Empty!!");
	printf("str3 is ... %s\n", str3->isEmpty(str3) == 1 ? "Empty!!" : "Not Empty!!");
	printf("str4 is ... %s\n", str4->isEmpty(str4) == 1 ? "Empty!!" : "Not Empty!!");
	
	printf("\n##### compare() Test...\n");

	str1.set(&str1, str2.get(&str2));
	printf("[str1: %p]String: %s, Length: %d\n", &str1, str1.get(&str1), str1.length(&str1));
	printf("[str2: %p]String: %s, Length: %d\n", &str2, str2.get(&str2), str2.length(&str2));
	printf(">>> comparing.. %s (with %d size)\n", str1.compare(&str1, &str2) == 0 ? "Equal!!" : "Not Eqaul!!", str1.length(&str1));
	printf("[str3: %p]Ptring: %s, Length: %d\n", &str3, str3->get(str3), str3->length(str3));
	printf("[str4: %p]Ptring: %s, Length: %d\n", &str4, str4->get(str4), str4->length(str4));
	printf(">>> comparing.. %s (with %d size)\n", str3->compare(str3, str4) == 0 ? "Equal!!" : "Not Eqaul!!", str3->length(str3));
	
	printf("\n##### compareRow() Test...\n");

	printf("str1: %s\nexample string: %s\n", str1.get(&str1), "wowowabc");
	printf(">>> comparing.. %s (with %d size)\n", str1.compareRow(&str1, "wowowabc") == 0 ? "Equal!!" : "Not Eqaul!!", str1.length(&str1));
	
	printf("\n##### isEqual() Test...\n");

	printf("[str1: %p]String: %s, Length: %d\n", &str1, str1.get(&str1), str1.length(&str1));
	printf("[str2: %p]String: %s, Length: %d\n", &str2, str2.get(&str2), str2.length(&str2));
	printf(">>> comparing.. %s (with %d size)\n", str1.isEqual(&str1, &str2) == 0 ? "Equal!!" : "Not Eqaul!!", str1.length(&str1));

	printf("\n##### isEqualRow() Test...\n");

	printf("str3: %s\nexample string: %s\n", str3->get(str3), "whataaa Appends");
	printf(">>> comparing.. %s (with %d size)\n", str3->isEqualRow(str3, "whataaa Appends") == 0 ? "Equal!!" : "Not Eqaul!!", str3->length(str3));

	printf("\n##### format() Test...\n");

	str4->format(str4, "%s %d %s", "hello", 777, "nice");
	printf("[str4: %p]Ptring: %s, Length: %d\n", &str4, str4->get(str4), str4->length(str4));

	printf("\n##### formatRow() Test...\n");
	str4->formatRow(str4, "%s %d %s", "append", 111, "good");
	printf("[str4: %p]Ptring: %s, Length: %d\n", &str4, str4->get(str4), str4->length(str4));

	return 0;
}
