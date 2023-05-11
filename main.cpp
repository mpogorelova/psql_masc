#include "bitvector.h"
#include </home/maria/fastbit/src/fileManager.h>
#include <fstream>

int main() {
    std::ifstream fdes("x.idx", std::ios::binary);
    //	int fdes=_open("x_test3.cmx",O_RDONLY | O_BINARY);
    std::ofstream ofdes("x_test3.cmx", std::ios::binary);
    //	int ofdes=_open("x_test2.cmx",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,_S_IREAD|_S_IWRITE);
    uint32_t *offset = NULL;
    char head[8];

    //read header
    fdes.read(head, 8);

    uint32_t len[2];

    //read length
    fdes.read(reinterpret_cast<char *>(len), 8);
    offset = (uint32_t *) malloc(sizeof(uint32_t) * (len[1] + 1));

    //read offset
    fdes.read(reinterpret_cast<char *>(offset), sizeof(uint32_t) * (len[1] + 1));
    ofdes.write(head, 8);
    ofdes.write(reinterpret_cast<char *>(len), 8);
    ofdes.write(reinterpret_cast<char *>(offset), (len[1] + 1) * 4);


    //read bitmap
    int i = 0;
    for (i = 0; i < 5; i++) {
        ibis::array_t<ibis::bitvector::word_t>
                a0("x.idx", offset[i], offset[i + 1]);
        ibis::bitvector *bv = new ibis::bitvector(a0);//we only read one bitvector as test
        bv->sloppySize(len[0]);
        //bv->COMPAX2WAH();

        (*bv).compress_masc();
        //do some querry
		bv->decompress_masc();

        //bv->WAH2COMPAX();

        bv->write("x_test3.cmx");//need to be replaced by a for loop
    }
    return 0;
}
