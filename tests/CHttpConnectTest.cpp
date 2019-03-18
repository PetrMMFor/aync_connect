#include <gtest/gtest.h>
#include "CHttpConnect.h"

struct TestSerialization : public testing::Test
{
    std::string etalon_url = "data.fixer.io/api/latest?access_key=*****&format=1";
    std::string url_with_http = "http://data.fixer.io/api/latest?access_key=*****&format=1";
    std::string url_with_https = "https://data.fixer.io/api/latest?access_key=*****&format=1";
    std::string empty_url = "";
    std::string badurl = "dfg dfgdfg dfgdfgdfgdfgdf /fdsg dfgdf/dfg dfgdg df";
    std::string url_whit_port = "https://data.fixer.io:80/api/latest?access_key=*****&format=1";
    std::string url_whitout_taget = "https://data.fixer.io:443";
    std::string url_whitout_port_and_taget = "https://data.fixer.io";
};


TEST_F(TestSerialization,test_url_et)
{
    CHttpConnect url(etalon_url);
    ASSERT_EQ("data.fixer.io",url.get_host());
    ASSERT_EQ("80",url.get_port());
    ASSERT_EQ("/api/latest?access_key=*****&format=1",url.get_target());
}

TEST_F(TestSerialization,test_url_http)
{
    CHttpConnect url(url_with_http);
    ASSERT_EQ("data.fixer.io",url.get_host());
    ASSERT_EQ("80",url.get_port());
    ASSERT_EQ("/api/latest?access_key=*****&format=1",url.get_target());
}

TEST_F(TestSerialization,test_url_https)
{
    CHttpConnect url(url_with_https);
    ASSERT_EQ("data.fixer.io",url.get_host());
    ASSERT_EQ("80",url.get_port());
    ASSERT_EQ("/api/latest?access_key=*****&format=1",url.get_target());
}

TEST_F(TestSerialization,test_url_empty)
{
    CHttpConnect url(empty_url);
    ASSERT_TRUE(url.get_host().empty());
    ASSERT_TRUE(url.get_port().empty());
    ASSERT_TRUE(url.get_target().empty());
}

TEST_F(TestSerialization,test_url_port)
{
    CHttpConnect url(url_whit_port);
    ASSERT_EQ("data.fixer.io",url.get_host());
    ASSERT_EQ("80",url.get_port());
    ASSERT_EQ("/api/latest?access_key=*****&format=1",url.get_target());
}

TEST_F(TestSerialization,test_bad_url)
{
    CHttpConnect url(badurl);
    EXPECT_TRUE(url.get_host().empty());
    EXPECT_TRUE(url.get_port().empty());
    EXPECT_TRUE(url.get_target().empty());
}

TEST_F(TestSerialization,test_url_whitout_taget)
{
    CHttpConnect url(url_whitout_taget);
    ASSERT_EQ("data.fixer.io",url.get_host());
    ASSERT_EQ("443",url.get_port());
    ASSERT_EQ("/",url.get_target());
}

TEST_F(TestSerialization,test_url_whitout_port_and_taget)
{
    CHttpConnect url(url_whitout_port_and_taget);
    ASSERT_EQ("data.fixer.io",url.get_host());
    ASSERT_EQ("80",url.get_port());
    ASSERT_EQ("/",url.get_target());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}