unsigned long addr = 0x7da0;
unsigned long ucode_patch[][4] = {
    // U7da0: tmp10:= ZEROEXT_DSZ32(0x00000000); tmp11:= ADD_DSZ32(tmp0, 0x00000001); tmp12:= ADD_DSZ32(tmp0, 0x00000002)
    {0xc0080003a008, 0x80000103b230, 0xc0000203c230, 0x300000c0},
    // U7da4: tmp9:= MOVEFROMCREG_DSZ64( , 0x38c); MOVETOCREG_DSZ64(tmp10, 0x38c) !m2; tmp13:= MOVEFROMCREG_DSZ64( , tmp11) !m2
    {0x40628c0f9200, 0xa0428c0c023a, 0x20620003dec0, 0x300000c0},
    // U7da8: MOVETOCREG_DSZ64(tmp1, tmp11) !m2; MOVETOCREG_DSZ64(tmp2, tmp0) !m2; rax:= MOVEFROMCREG_DSZ64( , tmp12) !m2
    {0x604200000ef1, 0xe04200000c32, 0x606200020f00, 0x300000c0},
    // U7dac: MOVETOCREG_DSZ64(tmp10, tmp11) !m2; MOVETOCREG_DSZ64(tmp9, 0x38c) !m2; unk_256() !m1 SEQW LFNCEWAIT, UEND0
    {0x204200000efa, 0x60428c0c0239, 0x125600000000, 0x130000f2},
};
