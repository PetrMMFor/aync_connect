#include <gtest/gtest.h>
#include "Functions.h"


struct TestSerializationTwo : public testing::Test
{
    std::vector<std::string> rates = {"AED",
                                            "BOB",
                                            "EGP",
                                            "COP",
                                            "FJD",
                                            "EUR",
                                            "CUC",
                                            "CUP",
                                            "BLA"};

    std::vector<std::string> url = {{"data.fixer.io/api/latest?access_key=*****&format=1",
                                    "http://data.fixer.io/api/latest?access_key=*****&format=1",
                                    "https://data.fixer.io/api/latest?access_key=*****&format=1",
                                    "https://data.fixer.io:80/api/latest?access_key=*****&format=1"}};

};

TEST_F(TestSerializationTwo,test_parsing_cfg)
{
    std::pair<std::vector<std::string>, std::vector<std::string>> json_cfg = get_config("/home/petr/CLionProjects/Finans/cfg.json");


    EXPECT_EQ(rates.size(),json_cfg.first.size());
    EXPECT_EQ(url.size(),json_cfg.second.size());

    EXPECT_TRUE(std::equal(rates.begin(), rates.end(), json_cfg.first.begin()));
    EXPECT_TRUE(std::equal(url.begin(), url.end(), json_cfg.second.begin()));
}