//
// Created by lining on 7/6/21.
//

#include "nfc/NFC_Operate.h"

NFC_Operate::NFC_Operate() {

}

NFC_Operate::~NFC_Operate() {
    Close();
}

int NFC_Operate::Open() {
    return 0;
}

void NFC_Operate::SelectDevice(int index) {

}

int NFC_Operate::GetSelectCardUid(uint8_t *uid, int uid_len) {
    return 0;
}

bool NFC_Operate::IsFirstBlock(uint32_t blockNum) {
    return false;
}

bool NFC_Operate::IsTrailerBlock(uint32_t blockNum) {
    return false;
}

uint32_t NFC_Operate::GetTailerBlock(uint32_t blockNum) {
    return 0;
}

bool NFC_Operate::Unlock_Card(bool isWrite) {
    return false;
}

bool NFC_Operate::Authenticate_Card(uint32_t blockNum) {
    return false;
}

int NFC_Operate::ReadData(uint32_t blockNum, uint8_t *data, int data_size) {
    return 0;
}

int NFC_Operate::WriteData(uint32_t blockNum, uint8_t *data, int data_size) {
    return 0;
}

int NFC_Operate::Close() {

    if (nfcDevice){
        nfc_close(nfcDevice);
    }

    if (context){
        nfc_exit(context);
    }

    return 0;
}
