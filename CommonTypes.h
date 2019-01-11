#ifndef COMMENTYPES_H
#define COMMENTYPES_H

#include <iostream>
#include <vector>
#include <cassert>
#include <memory>

enum BranchType {
    NO_BRANCH = 0, // NOTE: use as Im_not_branch in cb
    TRUNK, // the branch is a trunk layer
    BRANCH  // the branch is really a branch
};

enum PoolType {
    MAX = 0,
    AVERAGE
};

struct Block
{
    unsigned height;
    unsigned width;
    unsigned length;
    Block() = default;
    Block(unsigned h, unsigned w, unsigned l)
        : height(h)
        , width(w)
        , length(l)
    {}

    unsigned volume() const
    {
        return height * width * length;
    }
    friend std::ostream& operator<< ( std::ostream & o, Block const & b);
};

struct Mode
{
    unsigned inParallel;
    unsigned outParallel;
    Mode(unsigned pi, unsigned po)
            : inParallel(pi)
            , outParallel(po)
    {}
    friend std::ostream& operator << ( std::ostream & o, Mode const & m);
};

// redundant information included
// inBlock.height = f( outBlock.height, Layer.k, Layer.stride)
// inBlock.width == Layer.inBlock.width
// outBlock.width == Layer.outBlock.width
struct Partition
{
    Block inBlock;
    Block outBlock;
    Mode mode;
    Partition(Block const & ib, Block const & ob, Mode const & m)
        : inBlock(ib)
        , outBlock(ob)
        , mode(m)
    {}
    friend std::ostream& operator << ( std::ostream & o, Partition const & p);
};

enum GroupStrategy
{
    repeat = 0,
    writeOut = 1
};

std::string stg2Str(GroupStrategy stg);
GroupStrategy stgFromStr(std::string stg);

constexpr unsigned numGroupStrategy = 2;

/// output Group
struct Group
{
    std::vector<std::string> layers;  // whithout pool
    GroupStrategy stg;
    std::vector<Partition> partitions; // length == layers.length
    unsigned fctUR;                     // factor of utilization ratio. {1 2 4}
	int useRate;                       //bic % pi == 0 ->2; bic /pi == 1/2 ->1; bic /pi == 1/4 -> 0?????????????????????????????????????????????????

    
    
};
struct gg { int c; };
struct HardwareConfig
{
    unsigned inDataWidth;
    unsigned outDataWidth;

    float inBitWidth;
    float outBitWidth;

    unsigned inMemCapacity;
    unsigned outMemCapacity;
    unsigned paraMemCapacity;

    float freq;

    std::vector<Mode> modes;
};

struct PoolInfo
{
    std::string name;
    unsigned k;
    unsigned stride;
    unsigned pad;
    unsigned bitSel;
    unsigned shiftWidth;
    unsigned outputShiftWidth;
	unsigned rightShiftWidth;
	unsigned leftShiftWidth;
	unsigned branchShiftWidth;
    unsigned poolSizePara; // Note: 8-bit unsigned representation of 1 / (k * k). From JSON.
    PoolType poolType;
    // there is no need to record inBlock
    Block outBlock;
};

enum LayerType {
    NORMAL = 0,
    CHANNEL_COMBINE,
};

/// ConvLayer
struct Layer
{
    LayerType type;

    unsigned k;
    unsigned stride;
    unsigned pad;
    Block inBlock;
    Block outBlock;
    std::string name;
    bool hasPool;
    bool hasRelu;
    unsigned bitSel;
    unsigned shiftWidth;
    unsigned outputShiftWidth;
	unsigned rightShiftWidth;
	unsigned leftShiftWidth;
	unsigned branchShiftWidth;
    PoolInfo poolInfo;

    std::vector<Layer*> source;
    Layer* branch;
    BranchType branchType;

    friend std::ostream& operator << ( std::ostream & o, Layer const & l);
};

struct FCLayer
{
    std::string name;
    std::string source; // Note: source must exists
    unsigned IC;
    unsigned OC;
    bool hasRelu;

    unsigned bitSel;
    unsigned shiftWidth;
    unsigned outputShiftWidth;
	unsigned rightShiftWidth;//2??有些硬件参数不清楚
	unsigned leftShiftWidth;
	unsigned branchShiftWidth;

    friend std::ostream& operator << ( std::ostream & o, FCLayer const & l);
};

struct FCPartition
{ 
    std::string name;
    Mode mode;
    unsigned bic;
    unsigned boc;
    unsigned miniOC;
    friend std::ostream& operator << ( std::ostream & o, FCPartition const & fp);
};

#endif
