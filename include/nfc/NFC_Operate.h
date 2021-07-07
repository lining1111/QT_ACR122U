//
// Created by lining on 7/6/21.
//

#ifndef _NFC_OPERATE_H
#define _NFC_OPERATE_H

#include <nfc/nfc.h>
#include "nfc/nfc-utils.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class NFC_Operate {
public:
#define MAX_FRAME_LEN 264
#define MAX_DEVICE_COUNT 16
#define MAX_TARGET_COUNT 16
    enum CardType {
        ISO14443A_106 = 0,
        FELICA_212,
        FELICA_424,
        ISO14443B_106,
        ISO14443BI_106,
        ISO14443B2SR_106,
        ISO14443B2CT_106,
        ISO14443BICLASS_106,
        JEWEL_106,
        BARCODE_106,
    };

public:
    nfc_context *context = nullptr;
    nfc_connstring devices[MAX_DEVICE_COUNT];
    size_t device_count = 0;
    int device_index = 0;
    nfc_device *nfcDevice = nullptr;
    /**
     * 支持卡面的种类,会影响nfc_modulation的设置
     * 0x01:NMT_ISO14443A
     * 0x02:NMT_FELICA(212)
     * 0x04:NMT_FELICA(424)
     * 0x08:NMT_ISO14443B
     * 0x10:NMT_ISO14443BI
     * 0x20:NMT_ISO14443B2SR
     * 0x40:NMT_ISO14443B2CT
     * 0x80:NMT_ISO14443BICLASS
     * 0x100:NMT_JEWEL
     * 0x200:NMT_BARCODE
     */
    int mask = 0x3ff;
    nfc_modulation nm;

    map<CardType, nfc_modulation> map_nm;

    bool verbose = false;//是否开启信息打印

    //卡面信息集合
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_FELICA_212;
    vector<nfc_target> vector_FELICA_424;
    vector<nfc_target> vector_ISO14443B_106;
    vector<nfc_target> vector_ISO14443BI_106;
    vector<nfc_target> vector_ISO14443B2SR_106;
    vector<nfc_target> vector_ISO14443B2CT_106;
    vector<nfc_target> vector_ISO14443BICLASS_106;
    vector<nfc_target> vector_JEWEL_106;
    vector<nfc_target> vector_BARCODE_106;

    string tag_uid;//curr cardid

public:
    NFC_Operate();

    ~NFC_Operate();

public:

    /**
     * 初始化卡类型和nm
     * @return
     */
    int Init_map_nm();

    /**
     * 打开 nfc
     * @return 0:success -1:fail
     */
    int Open();

    /**
     * 选择一个读卡器
     * @param index in 读卡器索引
     * @return 0:success -1:fail
     */
    int SelectDevice(int index);

    /**
     * 列出所有连接的卡
     * @param mask in 支持的卡种类的掩码
     * @return 连接的卡的数量
     */
    int ListNfcCard(int mask);

    /**
    * 获取卡id
    * @param cardType in 卡类型属性
    * @param index in 卡索引
    * @param uid out 卡id数据
    * @param uid_len 卡id数组最大长度
    * @return 卡id数据长度
    */
    int GetSelectCardUid(CardType cardType, int index, uint8_t *uid, int uid_len);


    /**
     * 是否是扇区内的第一个块区 (第1扇区的第1块，是存放uid的地方，写入时要慎重)
     * @param blockNum in 块id 从0开始
     * @return
     */
    bool mfclassic_IsFirstBlock(uint32_t blockNum);

    /**
     * 是否是扇区尾部 (扇区尾部一般不读写，主要存放密钥和控制信息)
     * @param blockNum in 块id 从0开始
     * @return
     */
    bool mfclassic_IsTrailerBlock(uint32_t blockNum);

    /**
     * 获取扇区尾部
     * @param blockNum in 块id 从0开始
     * @return 块id所在扇区尾部的块id
     */
    uint32_t mfclassic_GetTailerBlock(uint32_t blockNum);

    /**
     * mifare 传输bits
     * @param tx in 输入数据
     * @param tx_len in 输入数据长度
     * @param rx out 输出数据 预留MAX_FRAME_LEN长度
     * @param rx_len out 输出数据长度
     * @return
     */
    static bool mfclassic_transmit_bits(uint8_t *tx, size_t tx_len, uint8_t *rx, size_t *rx_len);

    /**
    * mifare 传输byts
    * @param tx in 输入数据
    * @param tx_len in 输入数据长度
    * @param rx out 输出数据 预留MAX_FRAME_LEN长度
    * @param rx_len out 输出数据长度
    * @return
    */
    static bool mfclassic_transmit_bytes(uint8_t *tx, size_t tx_len, uint8_t *rx, size_t *rx_len);

    /**
     * 解锁卡
     * @param isWrite in 是否是读操作
     * @return true：解锁成功 false：解锁失败
     */
    bool mfclassic_UnlockCard(bool isWrite);

    /**
     * 认证卡的某一块
     * @param blockNum in 块id 从0开始
     * @return true：认证成功 false：认证失败
     */
    bool mfclassic_AuthenticateCard(uint32_t blockNum);

    /**
     * 读取卡上指定块的数据
     * @param blockNum in 块id 从0开始
     * @param data out 读取块数据
     * @param data_size in 存放数据的最大长度
     * @return 块数据的长度
     */
    int mfclassic_ReadData(uint32_t blockNum, uint8_t *data, int data_size);

    /**
     * 向卡上指定块写入数据
     * @param blockNum in 块id 从0开始
     * @param data in 写入块数据
     * @param data_size in 块数据长度
     * @return 写入的数据长度
     */
    int mfclassic_WriteData(uint32_t blockNum, uint8_t *data, int data_size);

    /**
     * 关闭nfc
     * @return 0：success -1:fail
     */
    int Close();

};


#endif //_NFC_OPERATE_H
