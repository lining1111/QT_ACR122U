//
// Created by lining on 7/6/21.
//

#include "nfc/NFC_Operate.h"

NFC_Operate::NFC_Operate() {
    bzero(devices, sizeof(devices));
}

NFC_Operate::~NFC_Operate() {
    Close();
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
    int res = 0;
    nfc_target ant[MAX_TARGET_COUNT];
    unsigned int ant_num = 0;
    int index = 0;
    if (mask & 0x1) {
        nm.nmt = NMT_ISO14443A;
        nm.nbr = NBR_106;
        // List ISO14443A targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443A passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x02) {
        nm.nmt = NMT_FELICA;
        nm.nbr = NBR_212;
        // List Felica tags
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d Felica (212 kbps) passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x04) {
        nm.nmt = NMT_FELICA;
        nm.nbr = NBR_424;
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d Felica (424 kbps) passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x08) {
        nm.nmt = NMT_ISO14443B;
        nm.nbr = NBR_106;
        // List ISO14443B targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x10) {
        nm.nmt = NMT_ISO14443BI;
        nm.nbr = NBR_106;
        // List ISO14443B' targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B' passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x20) {
        nm.nmt = NMT_ISO14443B2SR;
        nm.nbr = NBR_106;
        // List ISO14443B-2 ST SRx family targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B-2 ST SRx passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x40) {
        nm.nmt = NMT_ISO14443B2CT;
        nm.nbr = NBR_106;
        // List ISO14443B-2 ASK CTx family targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B-2 ASK CTx passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x80) {
        nm.nmt = NMT_ISO14443BICLASS;
        nm.nbr = NBR_106;
        // List ISO14443B iClass targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443B iClass passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x100) {
        nm.nmt = NMT_JEWEL;
        nm.nbr = NBR_106;
        // List Jewel targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443A-3 Jewel passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    if (mask & 0x200) {
        nm.nmt = NMT_BARCODE;
        nm.nbr = NBR_106;
        // List NFC Barcode targets
        if ((res = nfc_initiator_list_passive_targets(nfcDevice, nm, &ant[index], MAX_TARGET_COUNT - index)) >= 0) {
            if (verbose || (res > 0)) {
                printf("%d ISO14443A-2 NFC Barcode passive target(s) found%s\n", res, (res == 0) ? ".\n" : ":");
            }
            for (int i = 0; i < res; i++) {
                print_nfc_target(&ant[index + i], verbose);
                printf("\n");
            }
        }
    }
    index += res;

    ant_num = index;
    return index;
}

int NFC_Operate::GetSelectCardUid(int card_index, uint8_t *uid, int uid_len) {
    if (ant_num == 0) {
        //未获取过卡列表
        ListNfcCard(this->mask);
    }
    if (card_index > (ant_num - 1)) {
        printf("card index %d is big than max:%d\n", card_index, ant_num - 1);
        return -1;
    }
    memcpy(uid, ant[card_index].nti.)

    return 0;
}

int NFC_Operate::GetSelectCardUid(nfc_modulation nm, uint8_t *uid, int uid_len) {
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
