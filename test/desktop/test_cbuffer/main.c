#include <unity.h>
#include <cbuffer.h>



void setUp(void)
{

}

void tearDown(void)
{
    
}

void Test_BufferEmptyAfterReset(void)
{
    uint8_t memblock[8];
    struct CircularBuffer circularBuffer = {
        .memblock = memblock, .elementSize = 1, .len = 8
    };

    CircularBuffer_Reset(&circularBuffer);
    TEST_ASSERT_TRUE(CircularBuffer_Empty(&circularBuffer));
}

void Test_BufferFirstElementAdded(void)
{
    uint8_t memblock[8];
    struct CircularBuffer circularBuffer = {
        .memblock = memblock, .elementSize = 1, .len = 8
    };

    uint8_t variable = 100;
    int retcode;

    CircularBuffer_Reset(&circularBuffer);
    retcode = CircularBuffer_Add(&circularBuffer, &variable);
    TEST_ASSERT_EQUAL(0, retcode);
    TEST_ASSERT_FALSE(CircularBuffer_Empty(&circularBuffer));
    TEST_ASSERT_FALSE(CircularBuffer_Full(&circularBuffer));
}

void Test_BufferFill(void)
{
    uint8_t memblock[4];
    struct CircularBuffer circularBuffer = {
        .memblock = memblock, .elementSize = 1, .len = 4
    };

    uint8_t variable = 100;
    int retcode;

    CircularBuffer_Reset(&circularBuffer);
    TEST_ASSERT_EQUAL(0, CircularBuffer_Add(&circularBuffer, &variable));
    TEST_ASSERT_EQUAL(0, CircularBuffer_Add(&circularBuffer, &variable));
    TEST_ASSERT_EQUAL(0, CircularBuffer_Add(&circularBuffer, &variable));
    TEST_ASSERT_EQUAL(0, CircularBuffer_Add(&circularBuffer, &variable));
    TEST_ASSERT_FALSE(CircularBuffer_Empty(&circularBuffer));
    TEST_ASSERT_TRUE(CircularBuffer_Full(&circularBuffer));
    TEST_ASSERT_EQUAL(-1, CircularBuffer_Add(&circularBuffer, &variable));
}

void Test_BufferFillThenReadAll(void)
{
    uint8_t memblock[16];
    struct CircularBuffer circularBuffer = {
        .memblock = memblock, .elementSize = 1, .len = 16
    };

    uint8_t data = 0xAA;
    CircularBuffer_Reset(&circularBuffer);
    while (!CircularBuffer_Full(&circularBuffer)) {
        CircularBuffer_Add(&circularBuffer, &data);
    }
    
    int retcode, k = 0;
    while ((retcode = CircularBuffer_Get(&circularBuffer, &data)) >= 0) {
        TEST_ASSERT_EQUAL(0xAA, data);
        k++;
    }
    TEST_ASSERT_EQUAL(16, k);
    TEST_ASSERT_TRUE(CircularBuffer_Empty(&circularBuffer));
    TEST_ASSERT_FALSE(CircularBuffer_Full(&circularBuffer));
}

void Test_BufferFillThenReadAll_32bit(void)
{
    int32_t memblock[8];
    struct CircularBuffer circularBuffer = {
        .memblock = memblock, .elementSize = 4, .len = 8
    };

    int32_t data = 0xDEADC0DE;
    CircularBuffer_Reset(&circularBuffer);
    while (!CircularBuffer_Full(&circularBuffer)) {
        CircularBuffer_Add(&circularBuffer, &data);
    }
    
    int retcode, k = 0;
    while ((retcode = CircularBuffer_Get(&circularBuffer, &data)) >= 0) {
        TEST_ASSERT_EQUAL_INT32(0xDEADC0DE, data);
        k++;
    }
    TEST_ASSERT_EQUAL(8, k);
    TEST_ASSERT_TRUE(CircularBuffer_Empty(&circularBuffer));
    TEST_ASSERT_FALSE(CircularBuffer_Full(&circularBuffer));
}

void Test_BufferPeek_32bit(void)
{
    int32_t memblock[8];
    struct CircularBuffer circularBuffer = {
        .memblock = memblock, .elementSize = 4, .len = 8
    };

    int32_t dataOldest = 0xDEADC0DE;
    int32_t dataNewest = 0xDAD0FBED;
    CircularBuffer_Reset(&circularBuffer);
    
    CircularBuffer_Add(&circularBuffer, &dataOldest);
    CircularBuffer_Add(&circularBuffer, &dataNewest);
    CircularBuffer_Add(&circularBuffer, &dataOldest);
    CircularBuffer_Add(&circularBuffer, &dataNewest);

    CircularBuffer_PeekOldest(&circularBuffer, &dataOldest);
    CircularBuffer_PeekNewest(&circularBuffer, &dataNewest);

    CircularBuffer_PeekOldest(&circularBuffer, &dataOldest);
    CircularBuffer_PeekNewest(&circularBuffer, &dataNewest);

    TEST_ASSERT_EQUAL_INT32(0xDEADC0DE, dataOldest);
    TEST_ASSERT_EQUAL_INT32(0xDAD0FBED, dataNewest);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_BufferEmptyAfterReset);
    RUN_TEST(Test_BufferFirstElementAdded);
    RUN_TEST(Test_BufferFill);
    RUN_TEST(Test_BufferFillThenReadAll);
    RUN_TEST(Test_BufferFillThenReadAll_32bit);
    RUN_TEST(Test_BufferPeek_32bit);
}