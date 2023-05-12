#include "psql_masc.h"
#include "bitvector.cpp"

#define MAX_BITMAP_RANGE_END UINT64_C(0x100000000)
#define INT4_MIN -2147483648
#define INT4_MAX 2147483647

//fastbit-masc from array

Datum fstb_masc(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(fstb_masc);

Datum
fstb_masc(PG_FUNCTION_ARGS) {
    ArrayType  *array;

    if (PG_ARGISNULL(0))
		PG_RETURN_NULL();
        
    array = PG_GETARG_ARRAYTYPE_P(0);
    ibis::bitvector *bv;

    bv = new ibis::bitvector(VARDATA(array));
    if (!bv)
        ereport(ERROR,
                (errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED),
                 errmsg("bitmap format is error")));

    (*bv).compress_masc();
    PG_RETURN_ARRAYTYPE_P(bv);
}