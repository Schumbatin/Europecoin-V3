// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;

#include "chainparamsseeds.h"

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 819678;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 120 * 60; // two hour
        consensus.nPowTargetSpacing = 300; // 300 seconds
        consensus.fPowAllowMinDifficultyBlocks = false;
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
		pchMessageStart[0] = 0x45; 
        pchMessageStart[1] = 0x52; 
        pchMessageStart[2] = 0x4f; 
        pchMessageStart[3] = 0x55;
        vAlertPubKey = ParseHex("045244948ceada9a43a7a5d5f6c56306575a7adfcc07754aec34eea88026d7e7f0acd9089edaac9aa734db5a4eb1aee96b9cedb8260c4ecff4f7db25a5952487aa");
        nDefaultPort = 8881;
        nMinerThreads = 0;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database.
         *
         * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
         *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
         *   vMerkleTree: 4a5e1e
         */
        const char* pszTimestamp = "WHO IS NEXT, Matthias, Steve and Christian";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1555553672; //9-07-2016
        genesis.nBits    = 0x21000FFF;
        genesis.nNonce   = 0;
        genesis.nStartLocation = 189030;
        genesis.nFinalCalculation = 1494081267;


        if(genesis.GetHash() != uint256S("000d0da26987ead011c5d568e627f7e3d4a4f83a0b280b1134d8e7e366377f9a") ){
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 thash;
            char *scratchpad;
            scratchpad=new char[(1<<30)];
            while(true){
                int collisions=0;
                thash = genesis.FindBestPatternHash(collisions,scratchpad,8);
                LogPrintf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(),
                hashTarget.ToString().c_str());
                if (UintToArith256(thash) <= hashTarget)
                    break;
                genesis.nNonce=genesis.nNonce+10000;
                if (genesis.nNonce == 0){
                    LogPrintf("NONCE WRAPPED, incrementing time\n");
                    ++genesis.nTime;
                }
            }
            delete scratchpad;
            LogPrintf("block.nTime = %u \n", genesis.nTime);
            LogPrintf("block.nNonce = %u \n", genesis.nNonce);
            LogPrintf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            LogPrintf("block.nBits = %u \n", genesis.nBits);
            LogPrintf("block.nStartLocation = %u \n", genesis.nStartLocation);
            LogPrintf("block.nFinalCalculation = %u \n", genesis.nFinalCalculation);
            consensus.hashGenesisBlock=genesis.GetHash();
        }

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == genesis.GetHash());

        //assert(consensus.hashGenesisBlock == uint256S("0x001"));
        //assert(genesis.hashMerkleRoot == uint256S("0x001"));


        vSeeds.push_back(CDNSSeedData("", ""));
		
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,33);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,40+128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true; // Default 1
        fMiningRequiresPeers = true; // Default 1
        fDefaultConsistencyChecks = false; // Default 0
        fRequireStandard = true; // Default 1
        fMineBlocksOnDemand = false; // Default 0 
        fTestnetToBeDeprecatedFieldRPC = false; // Default 0
		
		/* Testnet
		fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
		*/

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x001"))
            1482022023, // * UNIX timestamp of last checkpoint block
            48616,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1000.0     // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.fPowAllowMinDifficultyBlocks = true;
		pchMessageStart[0] = 0x45; 
        pchMessageStart[1] = 0x52; 
        pchMessageStart[2] = 0x4f; 
        pchMessageStart[3] = 0x55;
        vAlertPubKey = ParseHex("045244948ceada9a43a7a5d5f6c56306575a7adfcc07754aec34eea88026d7e7f0acd9089edaac9aa734db5a4eb1aee96b9cedb8260c4ecff4f7db25a5952487aa");
        nDefaultPort = 8989;
        nMinerThreads = 0;
        nMaxTipAge = 0x7fffffff;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1555553672; //9-07-2016
        genesis.nNonce = 0;

        genesis.nStartLocation = 187905;
        genesis.nFinalCalculation = 1629771148;


        if(genesis.GetHash() != uint256S("0023e5f7be49f920756146f4bd0498f658f340ba41b0c8f2d2b83d1939703995") ){
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 thash;
            char *scratchpad;
            scratchpad=new char[(1<<30)];
            while(true){
                int collisions=0;
                thash = genesis.FindBestPatternHash(collisions,scratchpad,8);
                LogPrintf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(),
                hashTarget.ToString().c_str());
                if (UintToArith256(thash) <= hashTarget)
                    break;
                genesis.nNonce=genesis.nNonce+10000;
                if (genesis.nNonce == 0){
                    LogPrintf("NONCE WRAPPED, incrementing time\n");
                    ++genesis.nTime;
                }
            }
            delete scratchpad;
            LogPrintf("block.nTime = %u \n", genesis.nTime);
            LogPrintf("block.nNonce = %u \n", genesis.nNonce);
            LogPrintf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            LogPrintf("block.nBits = %u \n", genesis.nBits);
            LogPrintf("block.nStartLocation = %u \n", genesis.nStartLocation);
            LogPrintf("block.nFinalCalculation = %u \n", genesis.nFinalCalculation);
            consensus.hashGenesisBlock=genesis.GetHash();
        }

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == genesis.GetHash());

        //consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x001"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("", ""));
        vSeeds.push_back(CDNSSeedData("", ""));//East Coast


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,33);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,40+128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();


        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x001")),
            1457163389,
            0,
            10
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        pchMessageStart[0] = 0xb5;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xfa;
        pchMessageStart[3] = 0xda;
        nMinerThreads = 1;
        nMaxTipAge = 24 * 60 * 60;
		genesis.nTime = 1555553672; //06-07-2016
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 0;
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        //assert(consensus.hashGenesisBlock == uint256S("0x001"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x001")),
            0,
            0,
            0
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
