// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2018 The VITAE developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPORK_H
#define SPORK_H

#include "base58.h"
#include "key.h"
#include "main.h"
#include "net.h"
#include "sync.h"
#include "util.h"

#include "obfuscation.h"
#include "protocol.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what

    Sporks 11,12, and 16 to be removed with 1st zerocoin release
*/
#define SPORK_START 10001
#define SPORK_END 10022

#define SPORK_2_SWIFTTX 10001
#define SPORK_3_SWIFTTX_BLOCK_FILTERING 10002
#define SPORK_5_MAX_VALUE 10004
#define SPORK_7_FUNDAMENTALNODE_SCANNING 10006
#define SPORK_8_FUNDAMENTALNODE_PAYMENT_ENFORCEMENT 10007
#define SPORK_9_FUNDAMENTALNODE_BUDGET_ENFORCEMENT 10008
//#define SPORK_10_FUNDAMENTALNODE_PAY_UPDATED_NODES 10009
//#define SPORK_11_LOCK_INVALID_UTXO 10010
//#define SPORK_12_RECONSIDER_BLOCKS 10011
#define SPORK_13_ENABLE_SUPERBLOCKS 10012
#define SPORK_14_NEW_PROTOCOL_ENFORCEMENT 10013
#define SPORK_15_NEW_PROTOCOL_ENFORCEMENT_2 10014
#define SPORK_16_NEW_PROTOCOL_ENFORCEMENT_3 10015
#define SPORK_17_NEW_PROTOCOL_ENFORCEMENT_4 10016
#define SPORK_18_NEW_PROTOCOL_ENFORCEMENT_5 10017
#define SPORK_19_FUNDAMENTALNODE_PAY_UPDATED_NODES 10018
#define SPORK_21_MASTERNODE_PAY_UPDATED_NODES 10020
#define SPORK_22_REMOVE_SEESAW_BLOCK 10021
#define SPORK_23_DISABLE_NEW_FUNDAMENTALNODE 10022
#define SPORK_24_MASTERNODE_SCANNING 10023
#define SPORK_25_MASTERNODE_PAYMENTS_ENFORCEMENT 10024

#define SPORK_2_SWIFTTX_DEFAULT 978307200                         //2001-1-1
#define SPORK_3_SWIFTTX_BLOCK_FILTERING_DEFAULT 1424217600        //2015-2-18
#define SPORK_5_MAX_VALUE_DEFAULT 1000                            //1000 VITAE
#define SPORK_7_FUNDAMENTALNODE_SCANNING_DEFAULT 978307200             //2001-1-1
#define SPORK_8_FUNDAMENTALNODE_PAYMENT_ENFORCEMENT_DEFAULT 4070908800 //OFF
#define SPORK_9_FUNDAMENTALNODE_BUDGET_ENFORCEMENT_DEFAULT 4070908800  //OFF
//#define SPORK_10_FUNDAMENTALNODE_PAY_UPDATED_NODES_DEFAULT 4070908800  //OFF
//#define SPORK_11_LOCK_INVALID_UTXO_DEFAULT 4070908800             //OFF - NOTE: this is block height not time!
#define SPORK_13_ENABLE_SUPERBLOCKS_DEFAULT 4070908800            //OFF
#define SPORK_14_NEW_PROTOCOL_ENFORCEMENT_DEFAULT 4070908800      //OFF
#define SPORK_15_NEW_PROTOCOL_ENFORCEMENT_2_DEFAULT 4070908800   //ON
#define SPORK_16_NEW_PROTOCOL_ENFORCEMENT_3_DEFAULT 4070908800    //ON
#define SPORK_17_NEW_PROTOCOL_ENFORCEMENT_4_DEFAULT 4070908800   //ON
#define SPORK_18_NEW_PROTOCOL_ENFORCEMENT_5_DEFAULT 4070908800   //OFF
#define SPORK_19_FUNDAMENTALNODE_PAY_UPDATED_NODES_DEFAULT 4070908800  //OFF
#define SPORK_21_MASTERNODE_PAY_UPDATED_NODES_DEFAULT 4070908800  //OFF
#define SPORK_22_REMOVE_SEESAW_BLOCK_DEFAULT 9999999999  //OFF
#define SPORK_23_DISABLE_NEW_FUNDAMENTALNODE_DEFAULT 0  //OFF, 0 if off, other value is the block height
#define SPORK_24_MASTERNODE_SCANNING_DEFAULT 0  //OFF, 0 if off, other value is the block height
#define SPORK_25_MASTERNODE_PAYMENTS_ENFORCEMENT_DEFAULT 4070908800  //OFF, 0 if off, other value is the block height

class CSporkMessage;
class CSporkManager;

extern std::map<uint256, CSporkMessage> mapSporks;
extern std::map<int, CSporkMessage> mapSporksActive;
extern CSporkManager sporkManager;

void LoadSporksFromDB();
void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
int64_t GetSporkValue(int nSporkID);
bool IsSporkActive(int nSporkID);
void ReprocessBlocks(int nBlocks);

//
// Spork Class
// Keeps track of all of the network spork settings
//

class CSporkMessage
{
public:
    std::vector<unsigned char> vchSig;
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    uint256 GetHash()
    {
        uint256 n = Hash(BEGIN(nSporkID), END(nTimeSigned));
        return n;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    }
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;

public:
    CSporkManager()
    {
    }

    std::string GetSporkNameByID(int id);
    int GetSporkIDByName(std::string strName);
    bool UpdateSpork(int nSporkID, int64_t nValue);
    bool SetPrivKey(std::string strPrivKey);
    bool CheckSignature(CSporkMessage& spork, bool fCheckSigner = false);
    bool Sign(CSporkMessage& spork);
    void Relay(CSporkMessage& msg);
};

#endif
