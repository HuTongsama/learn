#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL strtod() */
#include<math.h>/* HUGE_VALF, HUGE_VAL, HUGE_VALL */
#include <string.h> /* memcpy() */
#include<stdio.h>


#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')
#define PUTC(c,ch) do{*(char*)lept_context_push(c,sizeof(char))=(ch);}while(0)
#define PUTS(c,s,len)    memcpy(lept_context_push(c,len),s,len)

typedef struct {
    const char* json;
	char* stack;
	size_t size, top;
}lept_context;

#ifndef LEPT_PARSE_STACK_INIT_SIZE
#define LEPT_PARSE_STACK_INIT_SIZE 256
#endif

#ifndef LEPT_PARSE_STRINGIFY_INIT_SIZE
#define LEPT_PARSE_STRINGIFY_INIT_SIZE 256
#endif // !LEPT_PARSE_STRINGIFY_INIT_SIZE

//压入数据栈
static void* lept_context_push(lept_context* c, size_t size)
{
	void* ret;
	assert(size > 0);
	if (c->top + size >= c->size) {
		if (c->size == 0)
			c->size = LEPT_PARSE_STACK_INIT_SIZE;
		while (c->top + size >= c->size)
			c->size += c->size >> 1;
		c->stack = (char*)realloc(c->stack, c->size);
	}
	ret = c->stack + c->top;
	c->top += size;
	return ret;
}
//弹出数据栈
static void* lept_context_pop(lept_context* c, size_t size)
{
	assert(c->top >= size);
	return c->stack + (c->top -= size);
}
//解析空字符
static void lept_parse_whitespace(lept_context* c) 
{
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}
/*
static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

static int lept_parse_true(lept_context* c, lept_value* v) {
	EXPECT(c, 't');
	if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type = LEPT_TRUE;
	return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c, lept_value* v) {
	EXPECT(c, 'f');
	if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
		return LEPT_PARSE_INVALID_VALUE;
	c->json += 4;
	v->type = LEPT_FALSE;
	return LEPT_PARSE_OK;
}
*/
//解析文本
static int lept_parse_literal(lept_context* c, lept_value* v,
	const char* literal, lept_type type)
{
	EXPECT(c, *literal);
	int i;
	for (i = 0; literal[i + 1]; i++)
		if (c->json[i] != literal[i + 1])
			return LEPT_PARSE_INVALID_VALUE;
	c->json += i;
	v->type = type;
	return LEPT_PARSE_OK;
}
//解析数字
static int lept_parse_number(lept_context* c, lept_value* v) 
{
	char* str = c->json;
	if (*str == '-')
		str++;
	if (*str == '0')
		str++;
	else {
		if (!ISDIGIT1TO9(*str))
			return LEPT_PARSE_INVALID_VALUE;
		str++;
		while (ISDIGIT(*str))
			str++;
	}
	if (*str == '.')
	{
		str++;
		if (!ISDIGIT(*str))
			return LEPT_PARSE_INVALID_VALUE;
		str++;
		while (ISDIGIT(*str))
			str++;
	}
	if (*str == 'E' || *str == 'e')
	{
		str++;
		if (*str == '+' || *str == '-'|| ISDIGIT(*str))
		{
			str++;
			while (ISDIGIT(*str))
				str++;
		}
		else
			return LEPT_PARSE_INVALID_VALUE;
	}
	/*if (ISDIGIT(*str) || *str == '-') 
	{
		if (*str == '-')
			str++;
		if (ISDIGIT1TO9(*str))
		{
			str++;
			while (ISDIGIT(*str))
				str++;
			if (*str == '.')
			{
				str++;
				if (!ISDIGIT(*str))
					return LEPT_PARSE_INVALID_VALUE;
			}
		}
		else
		{
			str++;
			if (*str == '.')
			{
				str++;
				if (!ISDIGIT(*str))
					return LEPT_PARSE_INVALID_VALUE;
			}
			else if (*str != '\0')
				return LEPT_PARSE_ROOT_NOT_SINGULAR;
		}
	}
	else
		return LEPT_PARSE_INVALID_VALUE;*/
	v->u.n = strtod(c->json, NULL);
	if (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL)
		return LEPT_PARSE_NUMBER_TOO_BIG;
	c->json = str;
	v->type = LEPT_NUMBER;
	return LEPT_PARSE_OK;
}

#define ISHEX(ch) ISDIGIT(ch)||((ch)>='A'&&(ch)<='F')||((ch)>='a'&&(ch)<='f')
//检测是否符合4位16进制数字
static const char* lept_parse_hex4(const char* p, unsigned* u)
{
	/* \TODO */
	int count = 0;
	char tmp[5];
	while (ISHEX(*p)&&count<4)
	{
		tmp[count++] = *p++;
	}
	*u = strtoul(tmp, NULL, 16);
	if (count == 4)
		return p;
	else
		return NULL;
}
//编码为utf8
static void lept_encode_utf8(lept_context* c, unsigned u)
{
	/* \TODO */
	//bit:7 byte1:0xxxxxxx
	char ch;
	if (u >= 0x0000 && u <= 0x007f)
	{
		ch = u & 0xff;
		PUTC(c, ch);
	}
	//bit:11 byte1:110xxxxx byte2:10xxxxxx
	else if (u >= 0x0080 && u <= 0x07ff)
	{
		ch = 0xc0 | ((u >> 6) & 0xff);
		PUTC(c, ch);
		ch = 0x80 | (u & 0x3f);
		PUTC(c, ch);		
	}
	//bit:16 byte1:1110xxxx byte2:10xxxxxx byte3:10xxxxxx
	else if (u >= 0x0800 && u <= 0xffff)
	{
		ch = 0xe0 | ((u >> 12) & 0xff);
		PUTC(c, ch);
		ch = 0x80 | ((u >> 6) & 0x3f);
		PUTC(c, ch);
		ch = 0x80 | (u & 0x3f);
		PUTC(c, ch);
	}
	//bit:21 byte1:11110xxx byte2:10xxxxxx byte3:10xxxxxx byte4:10xxxxxx
	else if (u >= 0x10000 && u <= 0x10ffff)
	{
		ch = 0xf0 | ((u >> 18) & 0xff);
		PUTC(c, ch);
		ch = 0x80 | ((u >> 12) & 0x3f);
		PUTC(c, ch);
		ch = 0x80 | ((u >> 6) & 0x3f);
		PUTC(c, ch);
		ch = 0x80 | (u & 0x03f);
		PUTC(c, ch);
	}

}
#define STRING_ERROR(ret) do { c->top = head; return ret; } while(0)
/*解析JSON字符串，把结果写入str和len
str指向c->stack中的元素*/
static int lept_parse_string_raw(lept_context* c, char** str, size_t* len)
{
	size_t head = c->top;
	unsigned u;
	const char* p;
	EXPECT(c, '\"');
	p = c->json;
	for (;;) {
		char ch = *p++;
		switch (ch)
		{
		case '\"':
			*len = c->top - head;
			*str = (const char*)lept_context_pop(c, *len);
			c->json = p;
			return LEPT_PARSE_OK;
		case'\\':
			switch (*p++)
			{
			case 'n':
				PUTC(c, '\n'); break;
			case '/':
				PUTC(c, '\/'); break;
			case '\"':
				PUTC(c, '\"'); break;
			case '\\':
				PUTC(c, '\\'); break;
			case 'b':
				PUTC(c, '\b'); break;
			case 'f':
				PUTC(c, '\f'); break;
			case 'r':
				PUTC(c, '\r'); break;
			case 't':
				PUTC(c, '\t'); break;
			case 'u':
				if (!(p = lept_parse_hex4(p, &u)))
					STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_HEX);
				if (u >= 0xD800 && u <= 0xDBFF)
				{
					if (*p != '\\'&&*(p + 1) != 'u')
						STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
					else
					{
						p += 2;
						unsigned l;
						if (!(p = lept_parse_hex4(p, &l)))
							STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
						if (l >= 0xdc00 && l <= 0xdfff)
							u = 0x10000 + (u - 0xd800) * 0x400 + (l - 0xdc00);
						else
							STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
					}
				}
				/* \TODO surrogate handling */
				lept_encode_utf8(c, u);
				break;
			default:
				STRING_ERROR(LEPT_PARSE_INVALID_STRING_ESCAPE);
			}
			break;
		case '\0':
			STRING_ERROR(LEPT_PARSE_MISS_QUOTATION_MARK);
		default:
			if ((unsigned char)ch < 0x20)
				STRING_ERROR(LEPT_PARSE_INVALID_STRING_CHAR);
			PUTC(c, ch);
		}
	}

}

static int lept_parse_string(lept_context* c, lept_value* v) 
{
	int ret;
	char* s;
	size_t len;
	if ((ret = lept_parse_string_raw(c, &s, &len)) == LEPT_PARSE_OK)
		lept_set_string(v, s, len);
	return ret;
}
//解析值的前置声明
static int lept_parse_value(lept_context* c, lept_value* v);
//解析数组
static int lept_parse_array(lept_context* c, lept_value* v)
{
	size_t size = 0;
	int ret;
	size_t head = c->top;
	EXPECT(c, '[');
	lept_parse_whitespace(c);
	if (*c->json == ']')
	{
		c->json++;
		v->type = LEPT_ARRAY;
		v->u.a.size = 0;
		v->u.a.e = NULL;
		return LEPT_PARSE_OK;
	}
	for (;;)
	{
		lept_parse_whitespace(c);
		lept_value e;
		lept_init(&e);
		if ((ret = lept_parse_value(c, &e)) != LEPT_PARSE_OK)
			break;
		memcpy(lept_context_push(c, sizeof(lept_value)), &e, sizeof(lept_value));
		size++;
		lept_parse_whitespace(c);
		if (*c->json == ',')
			c->json++;
		else if (*c->json == ']')
		{
			c->json++;
			lept_set_array(v, size);
			v->u.a.size = size;
			size *= sizeof(lept_value);
			memcpy(v->u.a.e, lept_context_pop(c, size), size);
			return LEPT_PARSE_OK;
		}
		else
		{
			ret = LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
			break;
		}
	}

	for (int i = 0; i < size; i++)
		lept_free((lept_value*)lept_context_pop(c, sizeof(lept_value)));
	return ret;

}

static int lept_parse_object(lept_context* c, lept_value* v)
{
	size_t size;
	lept_member m;
	int ret;
	EXPECT(c, '{');
	lept_parse_whitespace(c);
	if (*c->json == '}')
	{
		c->json++;
		v->type = LEPT_OBJECT;
		v->u.o.m = 0;
		v->u.o.size = 0;
		return LEPT_PARSE_OK;
	}
	m.k = NULL;
	size = 0;
	for (;;)
	{
		lept_init(&m.v);

		char* str;
		size_t len;
		lept_parse_whitespace(c);
		if (*c->json == '\"')
		{	
			/* \todo parse key to m.k, m.klen */
			lept_parse_string_raw(c, &str, &len);
			m.k = (char*)malloc(len + 1);
			memcpy(m.k, str, len);
			m.k[len] = '\0';
			m.klen = len;
			/* parse ws colon ws */
			lept_parse_whitespace(c);
			if (*c->json == ':')
			{
				c->json++;
				lept_parse_whitespace(c);
				/* parse value */
				if ((ret = lept_parse_value(c, &m.v)) != LEPT_PARSE_OK)
					break;
			}
			else
			{
				ret = LEPT_PARSE_MISS_COLON;
				break;
			}
		}
		else
		{
			ret = LEPT_PARSE_MISS_KEY;
			break;
		}
		memcpy(lept_context_push(c, sizeof(lept_member)), &m, sizeof(lept_member));
		size++;
		m.k = NULL;	/* ownership is transferred to member on stack */
		/*parse ws [comma | right-curly-brace] ws */
		lept_parse_whitespace(c);
		if (*c->json == ',')
		{
			c->json++;
		}
		else if (*c->json == '}')
		{
			c->json++;
			/*v->type = LEPT_OBJECT;
			v->u.o.size = size;
			size *= sizeof(lept_member);
			v->u.o.m = (lept_member*)malloc(size);*/
			lept_set_object(v, size);
			v->u.o.size = size;
			memcpy(v->u.o.m, lept_context_pop(c, size*sizeof(lept_member)), size*sizeof(lept_member));
			return LEPT_PARSE_OK;
		}
		else
		{
			ret = LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
			break;
		}
	}
	/* \todo Pop and free members on the stack */
	lept_member* tmp;
	if (0 == size && m.k != NULL)
		free(m.k);
	for (int i = 0; i < size; i++)
	{
		tmp = (lept_member*)lept_context_pop(c, sizeof(lept_member));
		lept_free(&tmp->v);
		free(tmp->k);
	}
	return ret;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
		case 'n':  return lept_parse_literal(c, v, "null", LEPT_NULL);
		case 't':  return lept_parse_literal(c, v, "true", LEPT_TRUE);
		case 'f':  return lept_parse_literal(c, v, "false", LEPT_FALSE);
		default:   return lept_parse_number(c, v);
		case '"':  return lept_parse_string(c, v);
		case '[':  return lept_parse_array(c, v);
		case '{':  return lept_parse_object(c, v);
		case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    assert(v != NULL);
    c.json = json;
	c.stack = NULL;
	c.size = c.top = 0;
	lept_init(v);
	lept_parse_whitespace(&c);
    int result=lept_parse_value(&c, v);
	lept_parse_whitespace(&c);
	if (*c.json != '\0'&& result == 0)
	{
		v->type = LEPT_NULL;
		return LEPT_PARSE_ROOT_NOT_SINGULAR;
	}
	else
	{	
		assert(c.top == 0);
		free(c.stack);
		return result;
	}
}

static void lept_stringify_string(lept_context* c, const char* s, size_t len)
{
	PUTC(c, '\"');
	char* p = s;
	for (int i = 0; i != len; i++)
	{
		char ch = *p++;
		switch (ch)
		{
		case '\n':
			PUTS(c, "\\n", 2);
			break;
		case '\"':
			PUTS(c, "\\\"", 2);
			break;
		case '\\':
			PUTS(c, "\\\\", 2);
			break;
		case '\b':
			PUTS(c, "\\b", 2);
			break;
		case '\f':
			PUTS(c, "\\f", 2);
			break;
		case '\r':
			PUTS(c, "\\r", 2);
			break;
		case '\t':
			PUTS(c, "\\t", 2);
			break;
		default:
			if (ch < 0x20)
			{
				PUTS(c, "\\u00", 4);
				char tmp = ch / 16;
				tmp += 48;
				PUTC(c, tmp);
				tmp = ch % 16;
				tmp += 48;
				PUTC(c, tmp);
				break;
			}

			PUTC(c, ch);
		}
		
	}
	PUTC(c, '\"');
}

static void lept_stringify_value(lept_context* c, const lept_value* v)
{
	switch (v->type)
	{
	case LEPT_NULL:	  PUTS(c, "null", 4); break;
	case LEPT_FALSE:  PUTS(c, "false", 5); break;
	case LEPT_TRUE:   PUTS(c, "true", 4); break;
	case LEPT_NUMBER: c->top -= 32 - sprintf(lept_context_push(c, 32), "%.17g", v->u.n); break;
	case LEPT_STRING: lept_stringify_string(c, v->u.s.s, v->u.s.len); break;
	case LEPT_ARRAY:
		/**/
		PUTC(c, '[');
		for (int i = 0; i < v->u.a.size; i++)
		{
			lept_stringify_value(c, v->u.a.e + i);
			if (i != v->u.a.size - 1)
				PUTC(c, ',');
		}
		PUTC(c, ']');
		break;
	case LEPT_OBJECT:
		/**/
		PUTC(c, '{');
		for (int i = 0; i < v->u.o.size; i++)
		{
			lept_stringify_string(c, (v->u.o.m + i)->k, (v->u.o.m + i)->klen);
			PUTC(c, ':');
			lept_stringify_value(c, &(v->u.o.m + i)->v);
			if (i != v->u.o.size - 1)
				PUTC(c, ',');
		}
		PUTC(c, '}');
		break;
	default:
		assert(0 && "invalid type");
	}
}

char* lept_stringify(const lept_value* v, size_t* length)
{
	lept_context c;
	assert(v != NULL);
	c.stack = (char*)malloc(c.size = LEPT_PARSE_STRINGIFY_INIT_SIZE);
	c.top = 0;
	lept_stringify_value(&c, v);
	if (length)
		*length = c.top;
	PUTC(&c, '\0');
	return c.stack;
}

void lept_copy(lept_value * dst, const lept_value * src)
{
	assert(src != NULL && dst != NULL && src != dst);
	size_t size;
	switch (src->type)
	{
	case LEPT_STRING:
		lept_set_string(dst, src->u.s.s, src->u.s.len);
		break;
	case LEPT_ARRAY:
		lept_set_array(dst, src->u.a.capacity);
		for (int i = 0; i < dst->u.a.size; i++)
		{
			lept_copy(&dst->u.a.e[i], &src->u.a.e[i]);
		}
		break;
	case LEPT_OBJECT:
	/*	lept_free(dst);
		dst->type = LEPT_OBJECT;
		dst->u.o.size = src->u.o.size;
		size = dst->u.o.size * sizeof(lept_member);
		dst->u.o.m = (lept_member*)malloc(size);*/
		lept_set_object(dst, src->u.o.size);
		dst->u.o.size = src->u.o.size;
		for (int i = 0; i < dst->u.o.size; i++)
		{
			dst->u.o.m[i].k = (char*)malloc(src->u.o.m[i].klen);
			memcpy(dst->u.o.m[i].k, src->u.o.m[i].k, src->u.o.m[i].klen);
			lept_copy(&dst->u.o.m[i].v, &src->u.o.m[i].v);
		}

		break;
	default:
		lept_free(dst);
		memcpy(dst, src, sizeof(lept_value));
		break;
	}
}

void lept_move(lept_value * dst, lept_value * src)
{
	assert(dst != NULL && src != NULL && src != dst);
	lept_free(dst);
	memcpy(dst, src, sizeof(lept_value));
	lept_init(src);
}

void lept_swap(lept_value * lhs, lept_value * rhs)
{
	assert(lhs != NULL && rhs != NULL);
	if (lhs != rhs)
	{
		lept_value tmp;
		memcpy(&tmp, lhs, sizeof(lept_value));
		memcpy(lhs, rhs, sizeof(lept_value));
		memcpy(rhs, &tmp, sizeof(lept_value));
	}
}

void lept_free(lept_value* v) {
	assert(v != NULL);
	if (v->type == LEPT_STRING)
		free(v->u.s.s);
	else if (v->type == LEPT_ARRAY)
	{
		for (int i = 0; i < v->u.a.size; i++)
			lept_free(lept_get_array_element(v, i));
		free(v->u.a.e);
	}
	else if (v->type == LEPT_OBJECT)
	{
		for (int i = 0; i < v->u.o.size; i++)
		{
			free(v->u.o.m[i].k);
			lept_free(&v->u.o.m[i].v);
		}
		free(v->u.o.m);
	}
	v->type = LEPT_NULL;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

int lept_is_equal(const lept_value * lhs, const lept_value * rhs)
{
	size_t i;
	assert(lhs != NULL && rhs != NULL);
	if (lhs->type != rhs->type)
		return 0;

	switch (lhs->type)
	{
	case LEPT_STRING:
		return lhs->u.s.len == rhs->u.s.len&&
			memcmp(lhs->u.s.s, rhs->u.s.s, lhs->u.s.len) == 0;
	case LEPT_NUMBER:
		return lhs->u.n == rhs->u.n;
	case LEPT_ARRAY:
		if (lhs->u.a.size != rhs->u.a.size)
			return 0;
		for (i = 0; i < lhs->u.a.size; i++)
			if (!lept_is_equal(&lhs->u.a.e[i], &rhs->u.a.e[i]))
				return 0;
		return 1;
	case LEPT_OBJECT:
		if (lhs->u.o.size != rhs->u.o.size)
			return 0;
		for (i = 0; i < lhs->u.o.size; i++)
		{
			size_t index = lept_find_object_index(rhs, lhs->u.o.m[i].k, lhs->u.o.m->klen);
			if (index != LEPT_KEY_NOT_EXIST)
				if (!lept_is_equal(&lhs->u.o.m[i].v, &rhs->u.o.m[index].v))
					return 0;
		}
		return 1;
	default:
		return 1;
	}
}

int lept_get_boolean(const lept_value* v) {
	assert(v != NULL&&v->type == LEPT_FALSE || v->type == LEPT_TRUE);
	if (v->type == LEPT_FALSE)
		return 0;
	else
		return 1;
}

void lept_set_boolean(lept_value* v, int b)
{
	assert(v != NULL);
	lept_free(v);
	if (b == 0)
		v->type = LEPT_FALSE;
	else
		v->type = LEPT_TRUE;
}

double lept_get_number(const lept_value* v) 
{
	assert(v != NULL&&v->type == LEPT_NUMBER);
	return v->u.n;
}

void lept_set_number(lept_value* v, double n) 
{
	assert(v != NULL);
	lept_free(v);
	v->u.n = n;
	v->type = LEPT_NUMBER;
}

const char* lept_get_string(const lept_value* v) 
{
	assert(v != NULL && v->type == LEPT_STRING);
	return v->u.s.s;
}

size_t lept_get_string_length(const lept_value* v) 
{
	assert(v != NULL && v->type == LEPT_STRING);
	return v->u.s.len;
}

void lept_set_string(lept_value* v, const char* s, size_t len)
{
	assert(v != NULL && (s != NULL || len == 0));
	lept_free(v);
	v->u.s.s = (char*)malloc(len + 1);
	memcpy(v->u.s.s, s, len);
	v->u.s.s[len] = '\0';
	v->u.s.len = len;
	v->type = LEPT_STRING;
}

void lept_set_array(lept_value * v, size_t capacity)
{
	assert(v != NULL);
	lept_free(v);
	v->type = LEPT_ARRAY;
	v->u.a.size = 0;
	v->u.a.capacity = capacity;
	v->u.a.e = capacity > 0 ? (lept_value*)malloc(capacity * sizeof(lept_value)) : NULL;
}

size_t lept_get_array_size(const lept_value * v)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	return v->u.a.size;
}

size_t lept_get_array_capacity(const lept_value * v)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	return v->u.a.capacity;
}

void lept_reserve_array(lept_value* v, size_t capacity)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	if (v->u.a.capacity < capacity)
	{
		v->u.a.capacity = capacity;
		v->u.a.e = (lept_value*)realloc(v->u.a.e, capacity * sizeof(lept_value));
	}
}

void lept_shrink_array(lept_value * v)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	if (v->u.a.capacity > v->u.a.size)
	{
		v->u.a.capacity = v->u.a.size;
		v->u.a.e = (lept_value*)realloc(v->u.a.e, v->u.a.capacity * sizeof(lept_value));
	}
}

void lept_clear_array(lept_value * v)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	lept_erase_array_element(v, 0, v->u.a.size);
}

lept_value * lept_get_array_element(const lept_value * v, size_t index)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	assert(index < v->u.a.size);
	return &v->u.a.e[index];
	
}

lept_value * lept_pushback_array_element(lept_value * v)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	if (v->u.a.size == v->u.a.capacity)
	{
		lept_reserve_array(v, v->u.a.capacity == 0 ? 1 : v->u.a.capacity * 2);
	}
	lept_init(&v->u.a.e[v->u.a.size]);
	return &v->u.a.e[v->u.a.size++];
}

void lept_popback_array_element(lept_value * v)
{
	assert(v != NULL && v->type == LEPT_ARRAY && v->u.a.size > 0);
	lept_free(&v->u.a.e[--v->u.a.size]);
}

lept_value * lept_insert_array_element(lept_value * v, size_t index)
{
	assert(v != NULL && v->type == LEPT_ARRAY && v->u.a.size > 0);
	assert(index >= 0 && index < v->u.a.size);
	if (v->u.a.size + 1 > v->u.a.capacity)
		lept_reserve_array(v, 2 * v->u.a.capacity);
	int pos = v->u.a.size;
	lept_init(&v->u.a.e[pos]);
	while (pos != index)
	{
		lept_move(&v->u.a.e[pos], &v->u.a.e[pos - 1]);
		pos--;
	}
	lept_init(&v->u.a.e[pos]);
	v->u.a.size++;
	return &v->u.a.e[pos];
}

void lept_erase_array_element(lept_value * v, size_t index, size_t count)
{
	assert(v != NULL && v->type == LEPT_ARRAY && index + count <= v->u.a.size);
	int pos = 0;
	while (pos < count)
	{
		lept_free(&v->u.a.e[index + pos]);
		pos++;
	}
	int dst = index;
	while (pos != 0 && dst + pos < v->u.a.size)
	{
		lept_move(&v->u.a.e[dst], &v->u.a.e[dst + pos]);
		dst++;
	}
	v->u.a.size -= pos;
}

void lept_set_object(lept_value * v, size_t capacity)
{
	assert(v != NULL);
	lept_free(v);
	v->type = LEPT_OBJECT;
	v->u.o.size = 0;
	v->u.o.capacity = capacity;
	v->u.o.m = capacity > 0 ? (lept_member*)malloc(capacity * sizeof(lept_member)) : NULL;
}

size_t lept_get_object_size(const lept_value * v)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	return v->u.o.size;
}

size_t lept_get_object_capacity(const lept_value * v)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	return v->u.o.capacity;
}

void lept_reverse_object(lept_value * v, size_t capacity)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	if (capacity > v->u.o.capacity)
	{
		v->u.o.capacity = capacity;
		v->u.o.m = (lept_member*)realloc(v->u.o.m, capacity * sizeof(lept_member));
	}
}

void lept_shrink_object(lept_value * v)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	if (v->u.o.size < v->u.o.capacity)
	{
		v->u.o.capacity = v->u.o.size;
		v->u.o.m = (lept_member*)realloc(v->u.o.m, v->u.o.capacity * sizeof(lept_member));
	}
}

void lept_clear_object(lept_value * v)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	size_t size = v->u.o.size;
	while (size > 0)
	{
		lept_remove_object_value(v, size - 1);
		size = v->u.o.size;
	}
}

const char * lept_get_object_key(const lept_value * v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	assert(index < v->u.o.size&&index >= 0);
	return v->u.o.m[index].k;
}

size_t lept_get_object_key_length(const lept_value * v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	assert(index < v->u.o.size&&index >= 0);
	return v->u.o.m[index].klen;
}

lept_value * lept_get_object_value(const lept_value * v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	assert(index < v->u.o.size&&index >= 0);
	return &v->u.o.m[index].v;
}

size_t lept_find_object_index(const lept_value * v, const char * key, size_t klen)
{
	size_t i;
	assert(v != NULL && v->type == LEPT_OBJECT && key != NULL);
	for (i = 0; i < v->u.o.size; i++)
		if (v->u.o.m[i].klen == klen && memcmp(v->u.o.m[i].k, key, klen) == 0)
			return i;
	return LEPT_KEY_NOT_EXIST;
}

lept_value * lept_find_object_value(lept_value * v, const char * key, size_t klen)
{
	assert(v != NULL && v->type == LEPT_OBJECT && key != NULL);
	size_t index = lept_find_object_index(v, key, klen);
	return index != LEPT_KEY_NOT_EXIST ? &v->u.o.m[index].v : NULL;
}

lept_value * lept_set_object_value(lept_value * v, const char * key, size_t klen)
{
	assert(v != NULL && v->type == LEPT_OBJECT && key != NULL);
	size_t index = lept_find_object_index(v, key, klen);
	if (index != LEPT_KEY_NOT_EXIST)
	{
		return &v->u.o.m[index].v;
	}
	else
	{
		if (v->u.o.size > 0)
		{
			if (v->u.o.size == v->u.o.capacity)
				lept_reverse_object(v, 2 * v->u.o.capacity);
		}
		else
		{
			lept_reverse_object(v, 1);
		}

		v->u.o.m[v->u.o.size].k = (char*)malloc(klen + 1);
		memcpy(v->u.o.m[v->u.o.size].k, key, klen);
		v->u.o.m[v->u.o.size].klen = klen;
		lept_init(&v->u.o.m[v->u.o.size].v);
		return &v->u.o.m[v->u.o.size++].v;
		
	}
}

void lept_remove_object_value(lept_value * v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	assert(index >= 0 && index < v->u.o.size);
	int pos = index;
	free(v->u.o.m[pos].k);
	v->u.o.m[pos].klen = 0;
	if (pos == v->u.o.size - 1)
	{
		lept_free(&v->u.o.m[pos].v);
		int a = 0;
	}
	else
	{
		while (pos + 1 < v->u.o.size)
		{
			lept_move(&v->u.o.m[pos].v, &v->u.o.m[pos + 1].v);
			v->u.o.m[pos].klen = v->u.o.m[pos + 1].klen;
			v->u.o.m[pos].k = (char*)malloc(v->u.o.m[pos].klen + 1);
			memcpy(v->u.o.m[pos].k, v->u.o.m[pos + 1].k, v->u.o.m[pos].klen);
			v->u.o.m[pos].k[v->u.o.m[pos].klen] = '\0';
			pos++;
			free(v->u.o.m[pos].k);
			v->u.o.m[pos].klen = 0;
		}
	}
	v->u.o.size--;
}
