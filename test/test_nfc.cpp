//
// Created by lining on 7/6/21.
//

#include "nfc/NFC_Operate.h"

int main(int argc, const char *argv[]) {
    NFC_Operate *nfcOperate = new NFC_Operate();
    nfcOperate->Open();
    nfcOperate->SelectDevice(1);
    nfcOperate->ListNfcCard(nfcOperate->mask);

    nfcOperate->Close();
    delete nfcOperate;
    return 0;
}