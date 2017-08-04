// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (  0, uint256("0x10cf47de8824d3e4f05877e2447c7665213fa432a4f45b4f21b16fd54ce3c7eb"))
        (  1200, uint256("0x63bf4ff57d7e2e2814fe9e654fdc4de69c31e697d726db35e6535b7fd60f1834"))
        ;
static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1501896077, // * UNIX timestamp of last checkpoint block
        1201,   // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        87.4     // * estimated number of transactions per day after checkpoint
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, uint256("0x3fa994d9991496c8d7b2609fbbabe0e560dca8455db7f56403d4b647c5efe170"))
        ;
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1482424380,
        0,
        0
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("3fa994d9991496c8d7b2609fbbabe0e560dca8455db7f56403d4b647c5efe170"))
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xdb;
        vAlertPubKey = ParseHex("04f27731aa06febf844fa2de9834da9deaba41d03efb2d5bb571edfc0d3c71957611c52fe2d0b7cff9aeb12945194d172ec6a4c2f8d83a54532cfec44de26bd9f3");
        nDefaultPort = 33317;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nSubsidyHalvingInterval = 1000000;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 2 * 24 * 60 * 60; // Outastracoin: 2 days
        nTargetSpacing = 5 * 60; // Outastracoin: 5 minutes
        nMaxTipAge = 24 * 60 * 60;

        const char* pszTimestamp = "Czwartek, 22 gru 2016 17:33:00 OutAstra";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 500 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04f27731aa06febf844fa2de9834da9deaba41d03efb2d5bb571edfc0d3c71957611c52fe2d0b7cff9aeb12945194d172ec6a4c2f8d83a54532cfec44de26bd9f3") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1482424380;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 767505;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x10cf47de8824d3e4f05877e2447c7665213fa432a4f45b4f21b16fd54ce3c7eb"));
        assert(genesis.hashMerkleRoot == uint256("0xc117f2cfa78bb65bc3a88d6cbafe05a96f54b69d9820387238f15bc5b0b07a8a"));

        vSeeds.push_back(CDNSSeedData("dnsseed.outastracoin.info", "dnsseed.outastracoin.pro"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,15);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        // Outastracoin: Mainnet v2 enforced as of block 710k
        nEnforceV2AfterHeight = 710000;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        vAlertPubKey = ParseHex("04f27731aa06febf844fa2de9834da9deaba41d03efb2d5bb571edfc0d3c71957611c52fe2d0b7cff9aeb12945194d172ec6a4c2f8d83a54532cfec44de26bd9f3");
        nDefaultPort = 43317;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 2 * 24 * 60 * 60; // Outastracoin: 2 days
        nTargetSpacing = 5 * 60; // Outastracoin: 5 minutes
        nMaxTipAge = 0x7fffffff;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1482424380;
        genesis.nNonce = 3187270;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x3fa994d9991496c8d7b2609fbbabe0e560dca8455db7f56403d4b647c5efe170"));
        assert(genesis.hashMerkleRoot == uint256("0xc117f2cfa78bb65bc3a88d6cbafe05a96f54b69d9820387238f15bc5b0b07a8a"));
        
        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("dnsseed.outastracoin.pro", "dnsseed.outastracoin.pro"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        // Outastracoin: Testnet v2 enforced as of block 400k
        nEnforceV2AfterHeight = 400000;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 2 * 24 * 60 * 60; // Outastracoin: 2 days
        nTargetSpacing = 5 * 60; // Outastracoin: 5 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nMaxTipAge = 0x7fffffff;
        genesis.nTime = 1482424380;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 3187270;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 19444;
        assert(hashGenesisBlock == uint256("0x3fa994d9991496c8d7b2609fbbabe0e560dca8455db7f56403d4b647c5efe170"));
        assert(genesis.hashMerkleRoot == uint256("0xc117f2cfa78bb65bc3a88d6cbafe05a96f54b69d9820387238f15bc5b0b07a8a"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        // Outastracoin: v2 enforced using Bitcoin's supermajority rule
        nEnforceV2AfterHeight = -1;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18445;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;

        // Litecoin: v2 enforced using Bitcoin's supermajority rule
        nEnforceV2AfterHeight = -1;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval)  { nSubsidyHalvingInterval=anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

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
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
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
