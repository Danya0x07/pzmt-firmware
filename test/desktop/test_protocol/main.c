#include <unity.h>
#include <protocol.h>

void Test_UnrecognizedComand(void)
{
    struct Command cmd;
    char line1[16] = "abcdrf";
    char line2[16] = "60000,40000";

    Protocol_ParseCommand(line1, &cmd);
    TEST_ASSERT_EQUAL(CommandType_UNRECOGNIZABLE, cmd.type);

    Protocol_ParseCommand(line2, &cmd);
    TEST_ASSERT_EQUAL(CommandType_UNRECOGNIZABLE, cmd.type);
}

void Test_VolumeCommand(void)
{
    struct Command cmd;
    char line1[16] = "v";
    char line2[16] = "V";

    Protocol_ParseCommand(line1, &cmd);
    TEST_ASSERT_EQUAL(CommandType_SET_VOLUME, cmd.type);
    TEST_ASSERT_FALSE(cmd.params.volumeRaised);

    Protocol_ParseCommand(line2, &cmd);
    TEST_ASSERT_EQUAL(CommandType_SET_VOLUME, cmd.type);
    TEST_ASSERT_TRUE(cmd.params.volumeRaised);
}

void Test_FiniteToneCommand(void)
{
    struct Command cmd;
    char line1[16] = "1500,300";
    char line2[16] = "0,400";
    
    Protocol_ParseCommand(line1, &cmd);
    TEST_ASSERT_EQUAL(CommandType_PLAY_FINITE_TONE, cmd.type);
    TEST_ASSERT_EQUAL(1500, cmd.params.frequency);
    TEST_ASSERT_EQUAL(300, cmd.params.duration);

    Protocol_ParseCommand(line2, &cmd);
    TEST_ASSERT_EQUAL(CommandType_PLAY_FINITE_TONE, cmd.type);
    TEST_ASSERT_EQUAL(0, cmd.params.frequency);
    TEST_ASSERT_EQUAL(400, cmd.params.duration);
}

void Test_InfiniteToneCommand(void)
{
    struct Command cmd;;
    char line1[16] = "500,0";
    char line2[16] = "700";

    Protocol_ParseCommand(line1, &cmd);
    TEST_ASSERT_EQUAL(CommandType_PLAY_INFINITE_TONE, cmd.type);
    TEST_ASSERT_EQUAL(500, cmd.params.frequency);
    TEST_ASSERT_EQUAL(0, cmd.params.duration);

    Protocol_ParseCommand(line2, &cmd);
    TEST_ASSERT_EQUAL(CommandType_PLAY_INFINITE_TONE, cmd.type);
    TEST_ASSERT_EQUAL(700, cmd.params.frequency);
    TEST_ASSERT_EQUAL(0, cmd.params.duration);
}

void Test_StopToneCommand(void)
{
    struct Command cmd;
    char line1[16] = "0,0";
    char line2[16] = "0";

    Protocol_ParseCommand(line1, &cmd);
    TEST_ASSERT_EQUAL(CommandType_STOP_PLAYING, cmd.type);
    TEST_ASSERT_EQUAL(0, cmd.params.frequency);
    TEST_ASSERT_EQUAL(0, cmd.params.duration);

    Protocol_ParseCommand(line2, &cmd);
    TEST_ASSERT_EQUAL(CommandType_STOP_PLAYING, cmd.type);
    TEST_ASSERT_EQUAL(0, cmd.params.frequency);
    TEST_ASSERT_EQUAL(0, cmd.params.duration);
}

void Test_ReplyBuilding(void)
{
    struct Reply reply;
    char buff[16];

    reply.code = ReplyCode_WRONG_CMD;
    Protocol_BuildReply(&reply, buff);
    TEST_ASSERT_EQUAL_STRING("0\n", buff);

    reply.code = ReplyCode_OK;
    Protocol_BuildReply(&reply, buff);
    TEST_ASSERT_EQUAL_STRING("1\n", buff);

    reply.code = ReplyCode_READY;
    Protocol_BuildReply(&reply, buff);
    TEST_ASSERT_EQUAL_STRING("2\n", buff);

    reply.code = ReplyCode_BUSY;
    Protocol_BuildReply(&reply, buff);
    TEST_ASSERT_EQUAL_STRING("3\n", buff);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_UnrecognizedComand);
    RUN_TEST(Test_VolumeCommand);
    RUN_TEST(Test_FiniteToneCommand);
    RUN_TEST(Test_InfiniteToneCommand);
    RUN_TEST(Test_StopToneCommand);
    RUN_TEST(Test_ReplyBuilding);
    return UNITY_END();
}