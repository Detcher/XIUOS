/**
 * @file CommonTable.cc
 * @brief CommonTable
 * @version 0.1
 * @author SYS Lab
 * @date 2022.11.01
 */

#include "CommonTable.h"

namespace LightTable {

CommonTable::CommonTable(uint64_t tableID, const char *tableName,
                         uint64_t firstBlockID, uint64_t segmentID,
                         SegmentType segmentType,
                         std::vector<Schema::SchemaEntry> schemaEntrys,
                         bool usePrimaryKeyIndex)
    : Table(tableID, tableName, firstBlockID, segmentID, segmentType,
            schemaEntrys, usePrimaryKeyIndex)

{}

CommonTable::CommonTable(uint64_t tableID, const char *tableName,
                         uint64_t firstBlockID, uint64_t segmentID,
                         SegmentType segmentType, bool usePrimaryKeyIndex)
    : Table(tableID, tableName, firstBlockID, segmentID, segmentType,
            usePrimaryKeyIndex)

{}

}  // namespace LightTable
