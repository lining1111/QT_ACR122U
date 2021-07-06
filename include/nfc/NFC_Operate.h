//
// Created by lining on 7/6/21.
//

#ifndef _NFC_OPERATE_H
#define _NFC_OPERATE_H

#include <nfc/nfc.h>
#include <string>

using namespace std;

class NFC_Operate {
public:
    nfc_context *context = nullptr;
    nfc_connstring devices[8];
    size_t device_count = 0;
    int device_index = 0;
    nfc_device *nfcDevice = nullptr;

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
     */
    void SelectDevice(int index);

    /**
     * 获取卡id
     * @param uid out 卡id数据
     * @param uid_len 卡id数组最大长度
     * @return 卡id数据长度
     */
    int GetSelectCardUid(uint8_t *uid, int uid_len);

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
