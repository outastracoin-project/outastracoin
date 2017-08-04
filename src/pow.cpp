// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

unsigned int GetNextWorkRequired_V1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // Outastracoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int static DarkGravityWave3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params) {
     /* current difficulty formula, darkcoin - DarkGravity v3, written by Evan Duffield - evan@darkcoin.io */
     const CBlockIndex *BlockLastSolved = pindexLast;
     const CBlockIndex *BlockReading = pindexLast;
     const CBlockHeader *BlockCreating = pblock;
     BlockCreating = BlockCreating;
     int64_t nActualTimespan = 0;
     int64_t LastBlockTime = 0;
     int64_t PastBlocksMin = 24;
     int64_t PastBlocksMax = 24;
     int64_t CountBlocks = 0;
     arith_uint256 PastDifficultyAverage;
     arith_uint256 PastDifficultyAveragePrev;
 
     if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMin) {
         // This is the first block or the height is < PastBlocksMin
         // Return minimal required work. (1e0fffff)
         return UintToArith256(params.powLimit).GetCompact();
     }
     
     // loop over the past n blocks, where n == PastBlocksMax
     for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
         if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
         CountBlocks++;
 
         // Calculate average difficulty based on the blocks we iterate over in this for loop
         if(CountBlocks <= PastBlocksMin) {
             if (CountBlocks == 1) { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
             else { PastDifficultyAverage = ((PastDifficultyAveragePrev * CountBlocks)+(arith_uint256().SetCompact(BlockReading->nBits))) / (CountBlocks+1); }
             PastDifficultyAveragePrev = PastDifficultyAverage;
         }
 
         // If this is the second iteration (LastBlockTime was set)
         if(LastBlockTime > 0){
             // Calculate time difference between previous block and current block
             int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());
             // Increment the actual timespan
             nActualTimespan += Diff;
         }
         // Set LasBlockTime to the block time for the block in current iteration
         LastBlockTime = BlockReading->GetBlockTime();      
 
         if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
         BlockReading = BlockReading->pprev;
     }
     
     // bnNew is the difficulty
     arith_uint256 bnNew(PastDifficultyAverage);
 
     // nTargetTimespan is the time that the CountBlocks should have taken to be generated.
     int64_t nTargetTimespan = CountBlocks*params.nPowTargetSpacing;
 
     // Limit the re-adjustment to 3x or 0.33x
     // We don't want to increase/decrease diff too much.
     if (nActualTimespan < nTargetTimespan/3)
         nActualTimespan = nTargetTimespan/3;
     if (nActualTimespan > nTargetTimespan*3)
         nActualTimespan = nTargetTimespan*3;
 
     // Calculate the new difficulty based on actual and target timespan.
     bnNew *= nActualTimespan;
     bnNew /= nTargetTimespan;
 
     // If calculated difficulty is lower than the minimal diff, set the new difficulty to be the minimal diff.
     if (bnNew > UintToArith256(params.powLimit)){
         bnNew = UintToArith256(params.powLimit);
     }
     
     // Some logging.
     // TODO: only display these log messages for a certain debug option.
     LogPrintf("Difficulty Retarget - Dark Gravity Wave 3\n");
     LogPrintf("Before: %08x %s\n", BlockLastSolved->nBits, arith_uint256().SetCompact(BlockLastSolved->nBits).ToString().c_str());
     LogPrintf("After: %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
 
     // Return the new diff.
     return bnNew.GetCompact();
 }
 
 unsigned int static GetNextWorkRequired_V3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&params)
 {
 return DarkGravityWave3(pindexLast, pblock, params);
 }
 
 unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
 {
 int DiffMode = 1;
 	if(/*fTestnet*/false)
 	{
 		if (pindexLast->nHeight+1 <=2) { DiffMode = 1; }
 		else { DiffMode = 2;}
 
 		if	(DiffMode == 1) { return GetNextWorkRequired_V1(pindexLast, pblock, params); }
 		else if	(DiffMode == 2) { return GetNextWorkRequired_V3(pindexLast, pblock, params); }
 	}
 	else
 	{
 		if(pindexLast->nHeight >= 1092600){ return GetNextWorkRequired_V3(pindexLast, pblock, params); }
 		else{ return GetNextWorkRequired_V1(pindexLast, pblock, params);}
 	}
 }
 

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // Outastracoin: intermediate uint256 can overflow by 1 bit
    bool fShift = bnNew.bits() > 235;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;
    if (fShift)
        bnNew <<= 1;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
