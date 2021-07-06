//
// Created by lining on 7/6/21.
//

#ifndef _NFC_OPERATE_H
#define _NFC_OPERATE_H

#include <nfc/nfc.h>
#include "nfc/nfc-utils.h"
#include <string>
#include <vector>

using namespace std;

class NFC_Operate {
public:
#define MAX_DEVICE_COUNT 16
#define MAX_TARGET_COUNT 16
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

    bool verbose = false;//是否开启信息打印


    //卡面信息集合
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_FELICA_212;
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_ISO14443A_106;
    vector<nfc_target> vector_ISO14443A_106;



    string tag_uid;//curr cardid

public:
    NFC_Operate();

    ~NFC_Operate();

public:
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
     * @param card_index in 卡索引
     * @param uid out 卡id数据
     * @param uid_len 卡id数组最大长度
     * @return 卡id数据长度
     * @brief 在执行过ListNfcCard函数后，获取了卡列表(nfc_target ant)后可以根据索引值获取，否则需要执行ListNfcCard
     */
    int GetSelectCardUid(int card_index, uint8_t *uid, int uid_len);

    /**
    * 获取卡id
    * @param nm in 卡类型属性
    * @param uid out 卡id数据
    * @param uid_len 卡id数组最大长度
    * @return 卡id数据长度
    */
    int GetSelectCardUid(nfc_modulation nm, uint8_t *uid, int uid_len);


    /**
     * 是否是扇区内的第一个块区 (第1扇区的第1块，是存放uid的地方，写入时要慎重)
     * @param blockNum in 块id 从0开始
     * @return
     */
    bool IsFirstBlock(uint32_t blockNum);

    /**
     * 是否是扇区尾部 (扇区尾部一般不读写，主要存放密钥和控制信息)
     * @param blockNum in 块id 从0开始
     * @return
     */
    bool IsTrailerBlock(uint32_t blockNum);

    /**
     * 获取扇区尾部
     * @param blockNum in 块id 从0开始
     * @return 块id所在扇区尾部的块id
     */
    uint32_t GetTailerBlock(uint32_t blockNum);

    /**
     * 解锁卡
     * @param isWrite in 是否是读操作
     * @return true：解锁成功 false：解锁失败
     */
    bool Unlock_Card(bool isWrite);

    /**
     * 认证卡的某一块
     * @param blockNum in 块id 从0开始
     * @return true：认证成功 false：认证失败
     */
    bool Authenticate_Card(uint32_t blockNum);

    /**
     * 读取卡上指定块的数据
     * @param blockNum in 块id 从0开始
     * @param data out 读取块数据
     * @param data_size in 存放数据的最大长度
     * @return 块数据的长度
     */
    int ReadData(uint32_t blockNum, uint8_t *data, int data_size);

    /**
     * 向卡上指定块写入数据
     * @param blockNum in 块id 从0开始
     * @param data in 写入块数据
     * @param data_size in 块数据长度
     * @return 写入的数据长度
     */
    int WriteData(uint32_t blockNum, uint8_t *data, int data_size);

    /**
     * 关闭nfc
     * @return 0：success -1:fail
     */
    int Close();

};


#endif //_NFC_OPERATE_H
