//
// Created by lining on 7/6/21.
//

#include "nfc/NFC_Operate.h"

NFC_Operate::NFC_Operate() {
    bzero(devices, sizeof(devices));
    Init_map_nm();
    vector_ISO14443A_106.clear();
    vector_FELICA_212.clear();
    vector_FELICA_424.clear();
    vector_ISO14443B_106.clear();
    vector_ISO14443BI_106.clear();
    vector_ISO14443B2SR_106.clear();
    vector_ISO14443B2CT_106.clear();
    vector_ISO14443BICLASS_106.clear();
    vector_JEWEL_106.clear();
    vector_BARCODE_106.clear();
}

NFC_Operate::~NFC_Operate() {
    map_nm.clear();
    vector_ISO14443A_106.clear();
    vector_FELICA_212.clear();
    vector_FELICA_424.clear();
    vector_ISO14443B_106.clear();
    vector_ISO14443BI_106.clear();
    vector_ISO14443B2SR_106.clear();
    vector_ISO14443B2CT_106.clear();
    vector_ISO14443BICLASS_106.clear();
    vector_JEWEL_106.clear();
    vector_BARCODE_106.clear();
    Close();
}

int NFC_Operate::Init_map_nm() {

    map_nm.clear();
    int count = 0;
    nfc_modulation nm;

    nm.nmt = NMT_ISO14443A;
    nm.nbr = NBR_106;
    map_nm[ISO14443A_106] = nm;
    count++;

    nm.nmt = NMT_FELICA;
    nm.nbr = NBR_212;
    map_nm[FELICA_212] = nm;
    count++;

    nm.nmt = NMT_FELICA;
    nm.nbr = NBR_424;
    map_nm[FELICA_424] = nm;
    count++;

    nm.nmt = NMT_ISO14443B;
    nm.nbr = NBR_106;
    map_nm[ISO14443B_106] = nm;
    count++;

    nm.nmt = NMT_ISO14443BI;
    nm.nbr = NBR_106;
    map_nm[ISO14443BI_106] = nm;
    count++;

    nm.nmt = NMT_ISO14443B2SR;
    nm.nbr = NBR_106;
    map_nm[ISO14443B2SR_106] = nm;
    count++;

    nm.nmt = NMT_ISO14443B2CT;
    nm.nbr = NBR_106;
    map_nm[ISO14443B2CT_106] = nm;
    count++;

    nm.nmt = NMT_ISO14443BICLASS;
    nm.nbr = NBR_106;
    map_nm[ISO14443BICLASS_106] = nm;
    count++;

    nm.nmt = NMT_JEWEL;
    nm.nbr = NBR_106;
    map_nm[JEWEL_106] = nm;
    count++;

    nm.nmt = NMT_BARCODE;
    nm.nbr = NBR_106;
    map_nm[BARCODE_106] = nm;
    count++;

    return count;
}

int NFC_Operate::Open() {
    //1.初始化nfc
    nfc_init(&context);
    if (context == nullptr) {
        printf("Unable to init libnfc (malloc).\n");
        return -1;
    }

    const char *acLibnfcVersion = nfc_version();
    printf("use libnfc version:%s\n", acLibnfcVersion);


    //2.获取读卡器列表
    device_count = nfc_list_devices(context, devices, sizeof(devices));
    if (device_count == 0) {
        printf("No NFC device found.\n");
        return -1;
    }
    //至此完成读卡器列表读取，等待选择读卡器索引 device_index，默认为0

    return 0;
}

int NFC_Operate::SelectDevice(int index) {
    if (index > device_count) {
        printf("index:%s is too big,max:%d \n", index, device_count - 1);
        printf("set default index:%d\n", device_index);
    } else {
        device_index = index;
    }
    nfcDevice = nfc_open(context, devices[index]);

    if (nfcDevice == nullptr) {
        printf("Unable to open NFC device: %s\n", devices[index]);
        return -1;
    }
    if (nfc_initiator_init(nfcDevice) < 0) {
        printf("nfc_initiator_init fail\n");
        return -1;
    }
    printf("NFC device: %s opened\n", nfc_device_get_name(nfcDevice));

    return 0;
}


int NFC_Operate::ListNfcCard(int mask) {
    int ret = 0;
    int res = 0;
    nfc_target ant[MAX_TARGET_COUNT];
    if (mask & 0x1) {
        nm.nmt = NMT_ISO14443A;
        nm.nbr = NBR_106;
        // List ISO14443A targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443A passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_ISO14443A_106.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x02) {
        nm.nmt = NMT_FELICA;
        nm.nbr = NBR_212;
        // List Felica tags
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d Felica (212 kbps) passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_FELICA_212.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x04) {
        nm.nmt = NMT_FELICA;
        nm.nbr = NBR_424;
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d Felica (424 kbps) passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_FELICA_424.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x08) {
        nm.nmt = NMT_ISO14443B;
        nm.nbr = NBR_106;
        // List ISO14443B targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_ISO14443B_106.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x10) {
        nm.nmt = NMT_ISO14443BI;
        nm.nbr = NBR_106;
        // List ISO14443B' targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B' passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_ISO14443BI_106.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x20) {
        nm.nmt = NMT_ISO14443B2SR;
        nm.nbr = NBR_106;
        // List ISO14443B-2 ST SRx family targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B-2 ST SRx passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_ISO14443B2SR_106.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x40) {
        nm.nmt = NMT_ISO14443B2CT;
        nm.nbr = NBR_106;
        // List ISO14443B-2 ASK CTx family targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B-2 ASK CTx passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_ISO14443B2CT_106.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x80) {
        nm.nmt = NMT_ISO14443BICLASS;
        nm.nbr = NBR_106;
        // List ISO14443B iClass targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B iClass passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_ISO14443BICLASS_106.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x100) {
        nm.nmt = NMT_JEWEL;
        nm.nbr = NBR_106;
        // List Jewel targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443A-3 Jewel passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_JEWEL_106.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    if (mask & 0x200) {
        nm.nmt = NMT_BARCODE;
        nm.nbr = NBR_106;
        // List NFC Barcode targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443A-2 NFC Barcode passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                vector_BARCODE_106.push_back(ant[i]);
                print_nfc_target(&ant[i], verbose);
                printf("\n");
            }
        }
    }
    ret += res;

    return ret;
}

int NFC_Operate::GetSelectCardUid(CardType cardType, int index, uint8_t *uid, int uid_len) {
    int ret = 0;

    nfc_modulation nm;

    switch (cardType) {
        case ISO14443A_106: {
            nm.nmt = NMT_ISO14443A;
            nm.nbr = NBR_106;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= ant[index].nti.nai.szUidLen) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nai.abtUid, ant[index].nti.nai.szUidLen);
                        ret = ant[index].nti.nai.szUidLen;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        case FELICA_212: {
            nm.nmt = NMT_FELICA;
            nm.nbr = NBR_212;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= 8) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nfi.abtId, 8);
                        ret = 8;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        case FELICA_424: {
            nm.nmt = NMT_FELICA;
            nm.nbr = NBR_424;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= 8) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nfi.abtId, 8);
                        ret = 8;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        case ISO14443B_106: {
            nm.nmt = NMT_ISO14443B;
            nm.nbr = NBR_106;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    //copy uid
                    memcpy(uid, &ant[index].nti.nbi.ui8CardIdentifier, 1);
                    ret = 1;
                }
            }
        }
            break;
        case ISO14443BI_106: {
            nm.nmt = NMT_ISO14443BI;
            nm.nbr = NBR_106;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= 4) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nii.abtDIV, 4);
                        ret = 4;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        case ISO14443B2SR_106: {
            nm.nmt = NMT_ISO14443B2SR;
            nm.nbr = NBR_106;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= 8) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nsi.abtUID, 8);
                        ret = 8;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        case ISO14443B2CT_106: {
            nm.nmt = NMT_ISO14443B2CT;
            nm.nbr = NBR_106;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= 4) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nci.abtUID, 4);
                        ret = 4;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        case ISO14443BICLASS_106: {
            nm.nmt = NMT_ISO14443BICLASS;
            nm.nbr = NBR_106;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= 8) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nhi.abtUID, 8);
                        ret = 8;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        case JEWEL_106: {
            nm.nmt = NMT_JEWEL;
            nm.nbr = NBR_106;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= 4) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nji.btId, 4);
                        ret = 4;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        case BARCODE_106: {
            nm.nmt = NMT_BARCODE;
            nm.nbr = NBR_106;
            int res = 0;
            nfc_target ant[MAX_TARGET_COUNT];
            res = nfc_initiator_list_passive_targets(nfcDevice, nm, ant, MAX_TARGET_COUNT);
            if (res <= 0) {
                printf("no target.\n");
                ret = -1;
            } else {
                if (index > (res - 1)) {
                    printf("index:%d big than max:%d.\n", index, (res - 1));
                    ret = -1;
                } else {
                    if (uid_len >= ant[index].nti.nti.szDataLen) {
                        //copy uid
                        memcpy(uid, &ant[index].nti.nti.abtData, ant[index].nti.nti.szDataLen);
                        ret = ant[index].nti.nti.szDataLen;
                    } else {
                        ret = 0;
                    }
                }
            }
        }
            break;
        default:
            break;
    }

    return ret;
}

bool NFC_Operate::mfclassic_IsFirstBlock(uint32_t blockNum) {
    // Test if we are in the small or big sectors
    if (blockNum < 128)
        return ((blockNum) % 4 == 0);
    else
        return ((blockNum) % 16 == 0);
}

bool NFC_Operate::mfclassic_IsTrailerBlock(uint32_t blockNum) {
    // Test if we are in the small or big sectors
    if (blockNum < 128)
        return ((blockNum + 1) % 4 == 0);
    else
        return ((blockNum + 1) % 16 == 0);
}

uint32_t NFC_Operate::mfclassic_GetTailerBlock(uint32_t blockNum) {
    // Test if we are in the small or big sectors
    uint32_t trailer_block = 0;
    if (blockNum < 128) {
        trailer_block = blockNum + (3 - (blockNum % 4));
    } else {
        trailer_block = blockNum + (15 - (blockNum % 16));
    }
    return trailer_block;
}

bool NFC_Operate::mfclassic_transmit_bits(uint8_t *tx, size_t tx_len, uint8_t *rx, size_t *rx_len) {



    return false;
}

bool NFC_Operate::mfclassic_transmit_bytes(uint8_t *tx, size_t tx_len, uint8_t *rx, size_t *rx_len) {
    return false;
}

bool NFC_Operate::mfclassic_UnlockCard(bool isWrite) {
    return false;
}

bool NFC_Operate::mfclassic_AuthenticateCard(uint32_t blockNum) {
    return false;
}

int NFC_Operate::mfclassic_ReadData(uint32_t blockNum, uint8_t *data, int data_size) {
    return 0;
}

int NFC_Operate::mfclassic_WriteData(uint32_t blockNum, uint8_t *data, int data_size) {
    return 0;
}

int NFC_Operate::Close() {

    if (nfcDevice) {
        nfc_close(nfcDevice);
        nfcDevice = nullptr;
    }

    if (context) {
        nfc_exit(context);
        context = nullptr;
    }

    return 0;
}
