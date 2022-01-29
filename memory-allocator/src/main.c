#include <stdio.h>
#include "tests.h"

int main(){
    void* heap = heap_init(10000);
    debug_heap(stdout, heap);

    // Обычное успешное выделение памяти.
    test_1(heap);

    // Освобождение одного блока из нескольких выделенных.
    test_2(heap);

    // Освобождение двух блоков из нескольких выделенных.
    test_3(heap);

    // Память закончилась, новый регион памяти расширяет старый.
    test_4(heap);

    // Память закончилась, старый регион памяти не расширить из-за другого выделенного диапазона адресов, новый регион выделяется в другом месте.
    test_5(heap);

    return 0;
}
