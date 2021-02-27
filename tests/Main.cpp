#include "gtest/gtest.h"
#include <Configuration.h>
#include <utils/LogDefines.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    Configuration(argc, argv);

    return RUN_ALL_TESTS();
}