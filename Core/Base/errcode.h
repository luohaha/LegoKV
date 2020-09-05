#ifndef ERRCODE_H
#define ERRCODE_H

// err code

#define ERR_SUCCESS 0
#define ERR_UNEXPECTED 1
#define ERR_NOT_FOUND 2
#define ERR_JSON_PARSER_ERR 3
#define ERR_CONSENSUS_NO_EXIST 4

// err info map
 static const char *err_info_map[5]
 = {"ERR_SUCCESS", "ERR_UNEXPECTED", "ERR_NOT_FOUND", "ERR_JSON_PARSER_ERR",
    "ERR_CONSENSUS_NO_EXIST"};

#endif
