#include <unity.h>
#include <playback.h>

void setUp(void)
{
    Playback_ResetQueue();
}

void Test_PlaybackQueueInitialState(void)
{
    TEST_ASSERT_TRUE(Playback_QueueEmpty());
    TEST_ASSERT_FALSE(Playback_QueueFull());
}

void Test_PlaybackEnqueueing(void)
{
    struct FiniteTone ft1 = {.frequency = 400, .duration = 500};
    struct FiniteTone ft2 = {.frequency = 800, .duration = 400};

    Playback_Enqueue(ft1);
    TEST_ASSERT_FALSE(Playback_QueueEmpty());
    TEST_ASSERT_FALSE(Playback_QueueFull());

    Playback_Enqueue(ft2);
    TEST_ASSERT_FALSE(Playback_QueueEmpty());
    TEST_ASSERT_TRUE(Playback_QueueFull());
}

void Test_PlaybackDequeueing(void)
{
    struct FiniteTone ft1 = {.frequency = 400, .duration = 500};
    struct FiniteTone ft2 = {.frequency = 800, .duration = 400};
    struct FiniteTone ft3;

    Playback_Enqueue(ft1);
    Playback_Enqueue(ft2);

    ft3 = Playback_Dequeue();
    TEST_ASSERT_FALSE(Playback_QueueEmpty());
    TEST_ASSERT_FALSE(Playback_QueueFull());

    TEST_ASSERT_EQUAL(400, ft3.frequency);
    TEST_ASSERT_EQUAL(500, ft3.duration);

    ft3 = Playback_Dequeue();
    TEST_ASSERT_TRUE(Playback_QueueEmpty());
    TEST_ASSERT_FALSE(Playback_QueueFull());

    TEST_ASSERT_EQUAL(800, ft3.frequency);
    TEST_ASSERT_EQUAL(400, ft3.duration);

    ft3 = Playback_Dequeue();
    TEST_ASSERT_TRUE(Playback_QueueEmpty());
    TEST_ASSERT_FALSE(Playback_QueueFull());

    TEST_ASSERT_EQUAL(0, ft3.frequency);
    TEST_ASSERT_EQUAL(0, ft3.duration);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_PlaybackQueueInitialState);
    RUN_TEST(Test_PlaybackEnqueueing);
    RUN_TEST(Test_PlaybackDequeueing);
    return UNITY_END();
}