#include "../../ds_common.h"

#include "../php_ds_parameters.h"
#include "../php_ds_arginfo.h"

#include "../iterators/php_ds_map_iterator.h"
#include "../handlers/php_ds_map_handlers.h"

#include "php_ds_collection_ce.h"
#include "php_ds_sequence_ce.h"
#include "php_ds_map_ce.h"
#include "php_ds_set_ce.h"

#define METHOD(name) PHP_METHOD(Map, name)

zend_class_entry *php_ds_map_ce;

METHOD(__construct)
{
    PHP_DS_PARAM_PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        ds_map_put_all(THIS_DS_MAP(), values);
    }
}

METHOD(allocate)
{
    PHP_DS_PARAM_PARSE_LONG(capacity);
    ds_map_allocate(THIS_DS_MAP(), capacity);
}

METHOD(apply)
{
    PHP_DS_PARAM_PARSE_CALLABLE();
    ds_map_apply(THIS_DS_MAP(), FCI_ARGS);
}

METHOD(capacity)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_LONG(ds_map_capacity(THIS_DS_MAP()));
}

METHOD(put)
{
    PHP_DS_PARAM_PARSE_ZVAL_ZVAL(key, value);
    ds_map_put(THIS_DS_MAP(), key, value);
}

METHOD(putAll)
{
    PHP_DS_PARAM_PARSE_ZVAL(values);
    ds_map_put_all(THIS_DS_MAP(), values);
}

METHOD(get)
{
    PHP_DS_PARAM_PARSE_ZVAL_OPTIONAL_ZVAL(key, def);
    RETURN_ZVAL_COPY(ds_map_get(THIS_DS_MAP(), key, def));
}

METHOD(intersect)
{
    PHP_DS_PARAM_PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_intersect(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

METHOD(remove)
{
    PHP_DS_PARAM_PARSE_ZVAL_OPTIONAL_ZVAL(key, def);
    ds_map_remove(THIS_DS_MAP(), key, def, return_value);
}

METHOD(hasKey)
{
    PHP_DS_PARAM_PARSE_ZVAL(key);
    RETURN_BOOL(ds_map_has_key(THIS_DS_MAP(), key));
}

METHOD(hasValue)
{
    PHP_DS_PARAM_PARSE_ZVAL(value);
    RETURN_BOOL(ds_map_has_value(THIS_DS_MAP(), value));
}

METHOD(diff)
{
    PHP_DS_PARAM_PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_diff(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

METHOD(clear)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_map_clear(THIS_DS_MAP());
}

METHOD(sort)
{
    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_COMPARE_CALLABLE();
        ds_map_sort_by_value_callback(THIS_DS_MAP());
    } else {
        ds_map_sort_by_value(THIS_DS_MAP());
    }
}

METHOD(sorted)
{
    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_COMPARE_CALLABLE();
        RETURN_DS_MAP(ds_map_sorted_by_value_callback(THIS_DS_MAP()));
    } else {
        RETURN_DS_MAP(ds_map_sorted_by_value(THIS_DS_MAP()));
    }
}

METHOD(ksort)
{
    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_COMPARE_CALLABLE();
        ds_map_sort_by_key_callback(THIS_DS_MAP());
    } else {
        ds_map_sort_by_key(THIS_DS_MAP());
    }
}

METHOD(ksorted)
{
    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_COMPARE_CALLABLE();
        RETURN_DS_MAP(ds_map_sorted_by_key_callback(THIS_DS_MAP()));
    } else {
        RETURN_DS_MAP(ds_map_sorted_by_key(THIS_DS_MAP()));
    }
}

METHOD(keys)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_DS_SET(ds_set_ex(ds_htable_clone(THIS_DS_MAP()->table)));
}

METHOD(last)
{
    PHP_DS_PARAM_PARSE_NONE;
    // RETURN_DS_PAIR(ds_map_last(THIS_DS_MAP()));
}

METHOD(merge)
{
    PHP_DS_PARAM_PARSE_ZVAL(values);
    RETURN_DS_MAP(ds_map_merge(THIS_DS_MAP(), values));
}

METHOD(pairs)
{
    ds_map_t *map = THIS_DS_MAP();
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_DS_SEQUENCE(
        ds_deque_from_buffer(ds_map_pairs(map), DS_MAP_SIZE(map)));
}

METHOD(toArray)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_map_to_array(THIS_DS_MAP(), return_value);
}

METHOD(count)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_LONG(DS_MAP_SIZE(THIS_DS_MAP()));
}

METHOD(isEmpty)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_BOOL(DS_MAP_IS_EMPTY(THIS_DS_MAP()));
}

METHOD(copy)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_OBJ(php_ds_map_create_clone(THIS_DS_MAP()));
}

METHOD(jsonSerialize)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_map_to_array(THIS_DS_MAP(), return_value);
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_CALLABLE();
        RETURN_DS_MAP(ds_map_filter_callback(THIS_DS_MAP(), FCI_ARGS));
    } else {
        RETURN_DS_MAP(ds_map_filter(THIS_DS_MAP()));
    }
}

METHOD(first)
{
    PHP_DS_PARAM_PARSE_NONE;
    // RETURN_DS_PAIR(ds_map_first(THIS_DS_MAP()));
}

METHOD(reduce)
{
    PHP_DS_PARAM_PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_map_reduce(THIS_DS_MAP(), FCI_ARGS, initial, return_value);
}

METHOD(reverse)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_map_reverse(THIS_DS_MAP());
}

METHOD(reversed)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_DS_MAP(ds_map_reversed(THIS_DS_MAP()));
}

METHOD(skip)
{
    PHP_DS_PARAM_PARSE_LONG(position);
    // RETURN_DS_PAIR(ds_map_skip(THIS_DS_MAP(), position));
}

METHOD(map)
{
    PHP_DS_PARAM_PARSE_CALLABLE();
    RETURN_DS_MAP(ds_map_map(THIS_DS_MAP(), FCI_ARGS));
}

METHOD(slice)
{
    ds_map_t *map = THIS_DS_MAP();

    if (ZEND_NUM_ARGS() > 1) {
        PHP_DS_PARAM_PARSE_LONG_AND_LONG(index, length);
        RETURN_DS_MAP(ds_map_slice(map, index, length));
    } else {
        PHP_DS_PARAM_PARSE_LONG(index);
        RETURN_DS_MAP(ds_map_slice(map, index, DS_MAP_SIZE(map)));
    }
}

METHOD(sum)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_map_sum(THIS_DS_MAP(), return_value);
}

METHOD(union)
{
    PHP_DS_PARAM_PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_union(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

METHOD(values)
{
    ds_map_t *map = THIS_DS_MAP();
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_DS_SEQUENCE(
        ds_deque_from_buffer(ds_map_values(map), DS_MAP_SIZE(map)));
}

METHOD(xor)
{
    PHP_DS_PARAM_PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_xor(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

PHP_DS_ARRAY_ACCESS_FORWARDING_METHODS(php_ds_map);

void php_ds_register_map()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Map, __construct)
        PHP_DS_ME(Map, allocate)
        PHP_DS_ME(Map, apply)
        PHP_DS_ME(Map, capacity)
        PHP_DS_ME(Map, diff)
        PHP_DS_ME(Map, filter)
        PHP_DS_ME(Map, first)
        PHP_DS_ME(Map, get)
        PHP_DS_ME(Map, hasKey)
        PHP_DS_ME(Map, hasValue)
        PHP_DS_ME(Map, intersect)
        PHP_DS_ME(Map, keys)
        PHP_DS_ME(Map, ksort)
        PHP_DS_ME(Map, ksorted)
        PHP_DS_ME(Map, last)
        PHP_DS_ME(Map, map)
        PHP_DS_ME(Map, merge)
        PHP_DS_ME(Map, pairs)
        PHP_DS_ME(Map, put)
        PHP_DS_ME(Map, putAll)
        PHP_DS_ME(Map, reduce)
        PHP_DS_ME(Map, remove)
        PHP_DS_ME(Map, reverse)
        PHP_DS_ME(Map, reversed)
        PHP_DS_ME(Map, skip)
        PHP_DS_ME(Map, slice)
        PHP_DS_ME(Map, sort)
        PHP_DS_ME(Map, sorted)
        PHP_DS_ME(Map, sum)
        PHP_DS_ME(Map, union)
        PHP_DS_ME(Map, values)
        PHP_DS_ME(Map, xor)

        PHP_DS_COLLECTION_ME_LIST(Map)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Map), methods);

    php_ds_map_ce = zend_register_internal_class(&ce);
    php_ds_map_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_map_ce->create_object  = php_ds_map_create_object;
    php_ds_map_ce->get_iterator   = php_ds_map_get_iterator;
    php_ds_map_ce->serialize      = php_ds_map_serialize;
    php_ds_map_ce->unserialize    = php_ds_map_unserialize;

    zend_declare_class_constant_long(
        php_ds_map_ce,
        STR_AND_LEN("MIN_CAPACITY"),
        DS_HTABLE_MIN_CAPACITY
    );

    zend_class_implements(php_ds_map_ce, 1, php_ds_collection_ce);
    php_ds_register_map_handlers();
}