#include "seg.h"

int main(int argc, char ** argv)
{
    MixSegment seg(JIEBA_DICT_FILE, HMM_DICT_FILE, USER_DICT_FILE);
    std::vector<std::string> word;
    seg.cut(argv[1],word);
    std::vector<std::string>::iterator iter=word.begin();
    for(iter;iter!=word.end();iter++)
    {
        std::cout<<*iter<<std::endl;
    }
    return EXIT_SUCCESS;
}
