/**
 * @file Error.h
 * @brief error number
 * @version 0.1
 * @author SYS Lab
 * @date 2022.11.01
 */

#ifndef ERROR_H
#define ERROR_H

namespace LightTable {

enum ErrorCode {
  SUCCESS = 0,
  ERROR_TABLE = 1,
  SEGMENT_TYPE_ERROR = 2,
  SEGMENT_STATUS_ERROR = 3,
  SEGMENT_IS_EXHAUSTED = 4,
  BUCKET_ITEM_NOT_FOUND = 5,
  ADD_BUCKET_ITEM_ERROR = 6,
  PREFETCH_BLOCK_NOT_FOUND = 7,
  GET_AVAILABLE_BLOCK_ERROR = 8,
  BLOCK_OCCUPIED = 9,
  ADD_ROW_MAP_ENTRY_ERROR = 10,
  ROW_MAP_ENTRY_NOT_FOUND = 11,
  ADD_SCHEMA_ENTRY_ERROR = 12,
  SCHEMA_ENTRY_NOT_FOUND = 13,
  COLUMN_ITEM_NOT_FOUND = 14,
  COLUMN_ID_NOT_CONTINUOUS = 15,
  DELETE_PARIMARY_KEY_COLUMN = 16,
  TABLE_NOT_FOUND = 17,
  TABLE_TUPLE_NOT_FOUND = 18,
  ADD_TABLE_TUPLE_ERROR = 19,
  BUFFER_EMPTY = 20,
  TYPE_INVALID = 21,
  ROWLOCATION_INVALID = 22,
  CREATE_SOCKET_ERROR = 23,
  INET_PTON_ERROR = 24,
  BIND_SOCKET_ERROR = 25,
  LISTEN_SOCKET_ERROR = 26,
  ACCEPT_SOCKET_ERROR = 27,
  CONNECT_SOCKET_ERROR = 28,
  SEND_MESSAGE_ERROR = 29,
  EVENT_TYPE_NOT_DEFINED = 30,
  SCHEMA_NOT_EXIST = 31,
  OPEN_META_FILE_ERROR = 32,
  INVALID_META_FILE = 33,
  INVALID_MEMORY_BLOCK_ID = 34,
  UPDATE_TUPLE_ERROR = 35,
  INVALID_PAGE_ID = 36,
  ADD_QUERY_CACHE_ERROR = 37,
  TRIE_LEAF_EXIST = 38,
  TRIE_LEAF_NOT_EXIST = 39,
  TRIE_LEAF_NOT_FOUND = 40,
  INVALID_SCAN_RANGE = 41,
  INVALID_PRELOAD_TASK = 42,
};

}  // namespace LightTable

#endif  // ERROR_H
