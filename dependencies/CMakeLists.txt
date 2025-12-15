include(FetchContent)

# https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG        "v3.11.0"
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)