#include "vec.h"

#include <stdlib.h>

struct vec *vec_create(void)
{
	struct vec *v = malloc(sizeof(struct vec));
	if (v == NULL)
		return NULL;

	v->size = 0;
	v->cap = VEC_DEFAULT_CAP;

	v->items = malloc(v->cap * sizeof(void *));
	if (v->items == NULL)
		return NULL;

	return v;
}

void vec_destroy(struct vec *v)
{
	free(v->items);
	free(v);
}

static void extend_vec(struct vec *v)
{
	v->cap *= 2;
	v->items = realloc(v->items, v->cap * sizeof(void *));
}

void vec_push_back(struct vec *v, void *el)
{
	if (v->size + 1 >= v->cap)
		extend_vec(v);

	v->items[v->size++] = el;
}

void *vec_pop_back(struct vec *v)
{
	if (vec_size(v) == 0)
		return NULL;

	void *last = vec_get(v, vec_size(v) - 1);
	v->size--;
	return last;
}

void *vec_get(struct vec *v, size_t i)
{
	if (i >= v->size)
		return NULL;

	return v->items[i];
}