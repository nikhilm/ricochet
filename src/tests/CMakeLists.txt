ADD_EXECUTABLE(test_hs ${RICOCHET_SRCS} test_hs.cpp)
ADD_EXECUTABLE(t_resrc ${RICOCHET_SRCS} t_resrc.cpp)
ADD_EXECUTABLE(t_blocks ${RICOCHET_SRCS} t_blocks.cpp)
ADD_EXECUTABLE(t_lparser ${RICOCHET_SRCS} t_lparser.cpp)

ADD_TEST (test_hs test_hs)
ADD_TEST (t_resrc t_resrc)
ADD_TEST (t_blocks t_blocks)
ADD_TEST (t_lparser t_lparser)
