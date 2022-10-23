#include <unity.h>
#include <protocol.h>

void Test_UndefinedRequest(void)
{
    struct Request request;
    char line1[16] = "abcdrf";
    char line2[16] = "60000,40000";

    Protocol_ParseRequest(line1, &request);
    TEST_ASSERT_EQUAL(RequestType_Undefined, request.type);

    Protocol_ParseRequest(line2, &request);
    TEST_ASSERT_EQUAL(RequestType_Undefined, request.type);
}

void Test_VolumeRequest(void)
{
    struct Request request;
    char line1[16] = "v";
    char line2[16] = "V";

    Protocol_ParseRequest(line1, &request);
    TEST_ASSERT_EQUAL(RequestType_SetVolume, request.type);
    TEST_ASSERT_FALSE(request.content.volumeRaised);

    Protocol_ParseRequest(line2, &request);
    TEST_ASSERT_EQUAL(RequestType_SetVolume, request.type);
    TEST_ASSERT_TRUE(request.content.volumeRaised);
}

void Test_FiniteToneRequest(void)
{
    struct Request request;
    char line1[16] = "1500,300";
    char line2[16] = "0,400";
    
    Protocol_ParseRequest(line1, &request);
    TEST_ASSERT_EQUAL(RequestType_PlayFiniteTone, request.type);
    TEST_ASSERT_EQUAL(1500, request.content.frequency);
    TEST_ASSERT_EQUAL(300, request.content.duration);

    Protocol_ParseRequest(line2, &request);
    TEST_ASSERT_EQUAL(RequestType_PlayEmptyFiniteTone, request.type);
    TEST_ASSERT_EQUAL(0, request.content.frequency);
    TEST_ASSERT_EQUAL(400, request.content.duration);
}

void Test_InfiniteToneRequest(void)
{
    struct Request request;;
    char line1[16] = "500,0";
    char line2[16] = "700";

    Protocol_ParseRequest(line1, &request);
    TEST_ASSERT_EQUAL(RequestType_PlayInfiniteTone, request.type);
    TEST_ASSERT_EQUAL(500, request.content.frequency);
    TEST_ASSERT_EQUAL(0, request.content.duration);

    Protocol_ParseRequest(line2, &request);
    TEST_ASSERT_EQUAL(RequestType_PlayInfiniteTone, request.type);
    TEST_ASSERT_EQUAL(700, request.content.frequency);
    TEST_ASSERT_EQUAL(0, request.content.duration);
}

void Test_StopToneRequest(void)
{
    struct Request request;
    char line1[16] = "0,0";
    char line2[16] = "0";

    Protocol_ParseRequest(line1, &request);
    TEST_ASSERT_EQUAL(RequestType_StopPlaying, request.type);
    TEST_ASSERT_EQUAL(0, request.content.frequency);
    TEST_ASSERT_EQUAL(0, request.content.duration);

    Protocol_ParseRequest(line2, &request);
    TEST_ASSERT_EQUAL(RequestType_StopPlaying, request.type);
    TEST_ASSERT_EQUAL(0, request.content.frequency);
    TEST_ASSERT_EQUAL(0, request.content.duration);
}

void Test_ResponseBuilding(void)
{
    struct Response response;
    char buff[16];

    response.type = ResponseType_BadRequest;
    Protocol_BuildResponse(&response, buff);
    TEST_ASSERT_EQUAL_STRING("1\n", buff);

    response.type = ResponseType_Acknowledge;
    Protocol_BuildResponse(&response, buff);
    TEST_ASSERT_EQUAL_STRING("0\n", buff);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_UndefinedRequest);
    RUN_TEST(Test_VolumeRequest);
    RUN_TEST(Test_FiniteToneRequest);
    RUN_TEST(Test_InfiniteToneRequest);
    RUN_TEST(Test_StopToneRequest);
    RUN_TEST(Test_ResponseBuilding);
    return UNITY_END();
}